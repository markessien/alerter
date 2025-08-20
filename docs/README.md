# Telex Start App
This is a C++ app that sits in the system tray and displays notifications. It can start and stop the Telex flutter app. It serves as a lightweight way to run Telex in the background without needing a lot of memory.


## Developing

To use this app, you must use VSCode Developer Power Shell - it does not work in the normal console except you do some config you probably will not do.

- Open Developer Powershell for VS2022
- Go to source code
- Type code . This opens up VS code with the correct variables set


## Instructions

- Setup and compile wxWidgets
- Run it from the command line as described above


## Compiling wxWidgets on windows
Take note - we compile it statically.

- Download wxWidgets source code into the wxWidgets folder
- Open 64 Native Tools Command Prompt for VS
- Navigate to \TelexStart\wxWidgets\build\msw>
- Clean any previous build with nmake -f makefile.vc clean BUILD=release
- Run nmake on it in this way nmake -f makefile.vc RUNTIME_LIBS=static SHARED=0 MONOLITHIC=0 BUILD=release TARGET_CPU=X64. This creates a release version that is statically linked. This will take a while to build. Build the debug version too with nmake -f makefile.vc RUNTIME_LIBS=static SHARED=0 MONOLITHIC=0 BUILD=debug TARGET_CPU=X64. you need the debug version for step through debugging. 
- Once done, all the libs for linking will be in \lib\vc_x64_lib. Take note that this is referenced in tasks.json

## Building wxrc

Go to the wxrc folder TelexStart\third_party\wxWidgets\utils\wxrc

nmake -f makefile.vc clean BUILD=release RUNTIME_LIBS=static SHARED=0 MONOLITHIC=0 TARGET_CPU=X64
nmake -f makefile.vc BUILD=release RUNTIME_LIBS=static SHARED=0 MONOLITHIC=0 TARGET_CPU=X64

the exe will be in vc_x64_mswu