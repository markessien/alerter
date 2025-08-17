## Project Setup Instructions
This guide explains how to set up the development environment to compile and run this wxWidgets project on Windows using Visual Studio Code and the MSVC compiler.

## 1. Prerequisites
Before you begin, make sure you have the following software installed:

Visual Studio Code: The code editor for this project.

VS Code C/C++ Extension Pack: Open VS Code, go to the Extensions view (Ctrl+Shift+X), and install the C/C++ Extension Pack from Microsoft.

Visual Studio Build Tools: This provides the necessary C++ compiler (MSVC).

Download it from the Visual Studio website.

During installation, select the Desktop development with C++ workload.

## 2. Build wxWidgets
The wxWidgets library must be compiled from source first.

Download: Get the wxWidgets source code (e.g., wxWidgets-3.2.x.zip) from the official download page.

Extract: Unzip the source code to a simple path, such as C:\dev\wxWidgets-3.2.x.

Compile:

Open the Developer PowerShell for VS from your Start Menu. This is crucial as it sets up the compiler environment.

Navigate to the wxWidgets build directory:

PowerShell

cd C:\dev\wxWidgets-3.2.x\build\msw
Run the nmake command to build a 64-bit static monolithic library. This will take some time.

PowerShell

nmake /f makefile.vc TARGET_CPU=X64 BUILD=release SHARED=0 MONOLITHIC=1
## 3. Configure the Project
This project includes a .vscode directory with pre-configured files. You just need to ensure the paths inside them match your system.

Open c_cpp_properties.json:

Verify that the includePath points to your wxWidgets installation (e.g., C:/dev/wxWidgets-3.2.x/include).

Open tasks.json:

Verify that the args section correctly points to your wxWidgets include and library folders (e.g., C:/dev/wxWidgets-3.2.x/...).

Note: The current configuration assumes wxWidgets is located at C:\dev\wxWidgets-3.2.x. If you've placed it elsewhere, you must update the paths in both JSON files.

## 4. How to Build and Run
Open the Project: Launch the Developer PowerShell for VS and navigate to this project's root folder. Open VS Code from there by typing:

PowerShell

code .
Select Source File: In VS Code, make sure your main C++ source file (e.g., main.cpp) is the active, open tab.

Build: Press Ctrl+Shift+B to run the default build task. This will compile your code and create an .exe file in the project directory.

Run: Execute the program from the VS Code terminal:

PowerShell

.\main.exe