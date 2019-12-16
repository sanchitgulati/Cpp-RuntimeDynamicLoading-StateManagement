# C++ Dynamic Class Loading 
## Runtime Compilation and State Management 


### Scope - Proof Of Concept
----
 - Program compiles and loads classes marked for runtime update. 
 -- Kept in a seprate folder ( **_/DynamicLoading/_** ) for this POC.
 - Sets up observer callbacks for the events from the File System Event API.
 - On received change in file event, recompiles the dynamic classes.
 - Serializes the class data before **_unloading_**.
 - Unserializes the class data after **_loading_** the new binary.
 -- **StateManager** provides options for storing serialized data in _text file_ or _hashmap_ 

> Runs On Mac OS X only (Code is based on Core Foundation APIs)

> QT framework provides same File System Events in cross-compatibile format but was out of scope of this Proof-Of-Concept
## Installation
----
`Make`
  - **all** : compiles everything.
  - **all-static** : compiles everything but statically-linked libs.
  - **runtime** : compiles all runtime compatible libs.
  - **main** : gets called from all: (compiles the main())
  - **main-static** : compiles the same for static linking
  - **clean** : cleans the binary files
  - **watcher-standalone** : compiles the Apple's open-source code as a stand-alone binary
  - **watcher** : compiles the same as a dynamic lib
  - **watcher-static** : compiles the same for static linking
  
```sh
$ make all
./dl
```


`DLClass` allows you to load shared objects at runtime and create instances of the classes inside them.
`StateManager` abstracts the management layer (storing and retreiving) data from the Game Object classes.
`GameObject` virtual class setting the requirements for the Game Objects
`BotA` exmaple bot that gets compiled at runtime
`Watcher`Apple's example source code refactored for modern C++ standard [SOURCE](https://developer.apple.com/library/archive/samplecode/Watcher/Listings/Watcher_c.html) 


### Todo
----
 - Generic way to serialize and unserialize data
 - Cross-Platform support
 - Actual use case implementation
 -- Game Editor to update assets and logic of the fly

### Credits
----
Used Github project by  [**knusbaum**/CP-Dynamic-Class-Loading](https://github.com/knusbaum/CPP-Dynamic-Class-Loading) as a starting point.
Inspired by **_Guerrilla Games_** talk at **_GDC on Creating a Tools Pipeline_** [here](https://www.youtube.com/watch?v=KRJkBxKv1VM)

### License
----
This is free and unencumbered software released into the public domain.
**Free Software**