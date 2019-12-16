#include "dlclass.hpp"
#include "include/AI.hpp"
#include "StateManager.hpp"
#include "watcher.h"
#include <iostream>
#include <memory>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <sys/param.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

//
// Prototypes
//

int compile_and_reload();
static void initialize_observers(settings_t *settings);
std::string exec(const char* cmd);
static void fsevents_callback(FSEventStreamRef streamRef, void *clientCallBackInfo, int numEvents, 
                const char *const eventPaths[], const FSEventStreamEventFlags *eventFlags, const uint64_t *eventIDs);
static void runLoopObserverCallBack(CFRunLoopObserverRef, CFRunLoopActivity, void* context);
static void initialize_observers(settings_t *settings);

//
// Variables
// Global declarations because of convenience
// 

const int gameInterval = 1.0f; // CoreFoundation will loop at this interval
std::shared_ptr<AI> bot; // Runtime Class Ref used as test case

auto stateManager = std::make_unique<StateManager>(StateManager());

// Change to this for writing serialized data to file
// auto stateManager = std::make_unique<StateManager>(StateManager(true));



// 
// Main
// 

int main(void) {
    compile_and_reload();
    settings_t _settings, *settings = &_settings;
    memset(settings, 0, sizeof(settings_t));
    settings->since_when = kFSEventStreamEventIdSinceNow;
    settings->latency = 0.5;
    settings->fullpath = "./RuntimeSource/";
    initialize_observers(settings);
    return 0;
}

//
// The FSEventsStreamCallback
//
static void
fsevents_callback(FSEventStreamRef streamRef, void *clientCallBackInfo,
                  int numEvents,
                  const char *const eventPaths[],
                  const FSEventStreamEventFlags *eventFlags,
                  const uint64_t *eventIDs)
{
    std::cout << "FSEvent received" << std::endl;
    compile_and_reload();
}

//
// Sends the shell command to compile again and reload the class into program
// Also, serializes before deletion and reloading the serialized data after loading the new code
//

int compile_and_reload(){
    if(bot)
        bot->serialize(stateManager.get());

    bot = std::dynamic_pointer_cast<AI>(std::shared_ptr<GameObject>(NULL));
    auto result = exec("make runtime");
    std::cout << result << std::endl;

    auto dlBot = new DLClass<GameObject>("./bin/BotA.so");
    
    const int speed = 1;
    bot = std::dynamic_pointer_cast<AI>(dlBot->make_obj(speed));
    delete dlBot;
    bot->unserialize(stateManager.get());
    bot->updateValues();
    
    if(!bot) {
        std::cerr << "Failed to instantiate a bot." << std::endl;
        return 1;
    }
    return 0;
}

//
// Calls the shell to compile runtime updatable classes
// TODO: Only update the class editted and not the entire directory
//
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

//
// Commented code to receive callback from CoreFoundation Run Loop
//
/*
static void 
runLoopObserverCallBack(CFRunLoopObserverRef, CFRunLoopActivity, void* context)
{
    //Custom Loop
}
*/

void SafeExit(FSEventStreamRef stream_ref,settings_t *settings){
    //
    // Save out information about the last event id and uuid for the
    // the device we're watching.
    //
    save_stream_info(FSEventStreamGetLatestEventId(stream_ref), settings->dev_uuid);
    //
    // Invalidation and final shutdown of the stream
    //
    FSEventStreamInvalidate(stream_ref);
    FSEventStreamRelease(stream_ref);

    CFRelease(settings->dev_uuid);
    settings->dev_uuid = NULL;

    cleanup_run_loop_signal_handler(CFRunLoopGetCurrent());

    bot = std::shared_ptr<AI>(NULL);
}

// 
//    This is the main loop callback from CoreFoundation
//    To be used for running main game loop 
//    Gets called with interval gameInterval variable
//
static void
runLoopCallBack(CFRunLoopTimerRef timer, void *info){


    //POC has only 1 bot, ideally we should traverse all game objects on screen to update them.
    if(bot)
    {
        bot->update();

        // As we don't have game window for display, we are using print to get updated info for the game objects
        bot->printInfo();
    }
}

//
// Static function taken from Apple Souce Code 
//
static void 
initialize_observers(settings_t *settings)
{
    FSEventStreamContext  context = {0, NULL, NULL, NULL, NULL};
    FSEventStreamRef      stream_ref = NULL;
    CFMutableArrayRef     cfarray_of_paths;
    CFUUIDRef             uuid_ref;

    //
    // Figure out the device of the path we're watching and
    // get its FSEventStream UUID.
    //
    if (get_dev_info(settings) != 0) {
	    return;
    }

    cfarray_of_paths = create_cfarray_from_path(settings->fullpath);
    
    if (cfarray_of_paths == NULL) {
	    printf("failed to create the array for: %s\n", settings->fullpath);
	    CFRelease(settings->dev_uuid);
        settings->dev_uuid = NULL;
        return;
    }

    context.info = (void *)settings;
    stream_ref = FSEventStreamCreate(kCFAllocatorDefault,
	                            (FSEventStreamCallback)&fsevents_callback,
	                            &context,
	                            cfarray_of_paths,
	                            settings->since_when,
	                            settings->latency,
	                            kFSEventStreamCreateFlagNone);

    CFRelease(cfarray_of_paths);


    if (stream_ref == NULL) {
        printf("failed to create the stream for: %s\n", settings->fullpath);
        CFRelease(settings->dev_uuid);
        settings->dev_uuid = NULL;
        return;
    }

    setup_run_loop_signal_handler(CFRunLoopGetCurrent());

    FSEventStreamScheduleWithRunLoop(stream_ref, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    if (!FSEventStreamStart(stream_ref)) {
        fprintf(stderr, "failed to start the FSEventStream\n");
        SafeExit(stream_ref,settings);
        return;
    }

    //
    //Adding Custom Observer 
    //
    // CFRunLoopObserverContext c = { 0, CFRunLoopGetCurrent(), 0, 0, 0 };
    // auto v = CFRunLoopObserverCreate(kCFAllocatorDefault,kCFRunLoopBeforeTimers,true,0, (CFRunLoopObserverCallBack)&runLoopObserverCallBack,&c);
    // CFRunLoopAddObserver(CFRunLoopGetCurrent(),v,kCFRunLoopDefaultMode);

    CFRunLoopTimerContext tc = { 0, CFRunLoopGetCurrent(), 0, 0, 0 };

    auto CFtimer = CFRunLoopTimerCreate(kCFAllocatorDefault,CFAbsoluteTimeGetCurrent(),gameInterval,0,0,(CFRunLoopTimerCallBack)&runLoopCallBack,&tc);
    CFRunLoopAddTimer(CFRunLoopGetCurrent(),CFtimer,kCFRunLoopDefaultMode);


    //
    // Run
    //
    CFRunLoopRun();

    // Although it's not strictly necessary, make sure we see any pending events... 
    FSEventStreamFlushSync(stream_ref);
    FSEventStreamStop(stream_ref);
    SafeExit(stream_ref,settings);

    return;
}
