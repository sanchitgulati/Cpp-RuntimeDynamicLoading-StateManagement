#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/event.h>
#include <dirent.h>
#include <assert.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <pthread.h>

#pragma once

typedef struct _settings_t {
    dev_t                dev;
    FSEventStreamEventId since_when;
    CFAbsoluteTime       latency;
    const char          *fullpath;
    CFUUIDRef            dev_uuid;
    char                 mount_point[MAXPATHLEN];
} settings_t;


void  scan_directory(const char *path, int add, int recursive, int depth);
int   save_dir_items(const char *name);
int   load_dir_items(const char *name);
void  discard_all_dir_items(void);
int   remove_dir_and_children(const char *name);
int   check_children_of_dir(const char *dirname);
off_t get_total_size(void);

void  save_stream_info(uint64_t last_id, CFUUIDRef dev_uuid);
int   load_stream_info(uint64_t *since_when, CFUUIDRef *uuid_ref_ptr);

int   setup_run_loop_signal_handler(CFRunLoopRef loop);
void  cleanup_run_loop_signal_handler(CFRunLoopRef loop);

int   get_dev_info(settings_t *settings);
void  usage(const char *progname);
void  parse_settings(int argc, const char *argv[], settings_t *settings);
CFMutableArrayRef create_cfarray_from_path(const char *path);



static void
watch_dir_hierarchy(settings_t *settings);



static void
fsevents_callback(FSEventStreamRef streamRef, void *clientCallBackInfo,
                  int numEvents,
                  const char *const eventPaths[],
                  const FSEventStreamEventFlags *eventFlags,
                  const uint64_t *eventIDs);