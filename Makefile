mkfile_path = $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir = $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
full_path_dir := $(patsubst %/,%,$(dir $(mkfile_path)))
LDFLAGS = '-Wl,-rpath,$$ORIGIN/'
LIB = -lwatcher -ldl 
FRAMEWORK = -framework CoreFoundation
FRAMEWORK := $(FRAMEWORK) -framework CoreServices
RUNTIME_DIR = RuntimeSource

all: runtime watcher main
all-static: runtime watcher-static main-static

runtime:	
	$(foreach file, $(wildcard $(RUNTIME_DIR)/*.cpp), g++ -shared -fPIC $(file) -o ./bin/$(subst $(RUNTIME_DIR)/,,$(basename $(file))).so;)

main:
	g++ -std=c++14 main.cpp -I$(full_path_dir) -L$(full_path_dir) $(LDFLAGS) $(LIB) -o dl $(FRAMEWORK)

main-static:
	g++ -std=c++14 watcher.o main.cpp -o dl-static $(FRAMEWORK)

clean:
	-@rm *~ *.o dltest *.so *.a

watcher-standalone:
	g++ -std=c++14 watcher.cpp -D STANDALONE -o watcher_standalone.so $(FRAMEWORK)

watcher:
	g++ -dynamiclib -shared -fPIC -std=c++14 watcher.cpp -o libwatcher.dylib $(FRAMEWORK)

watcher-static:
	g++ -shared -fPIC -std=c++14 watcher.cpp -o watcher.o $(FRAMEWORK)