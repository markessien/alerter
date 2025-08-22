## Project Setup Instructions
This guide explains how to set up the development environment to compile and run this wxWidgets project on Windows using Visual Studio Code and the MSVC compiler.

## 1. Prerequisites
Before you begin, make sure you have the following software installed:

Visual Studio Code: The code editor for this project.

VS Code C/C++ Extension Pack: Open VS Code, go to the Extensions view (Ctrl+Shift+X), and install the C/C++ Extension Pack from Microsoft.

Visual Studio Build Tools: This provides the necessary C++ compiler (MSVC).

Download it from the Visual Studio website.

During installation, select the Desktop development with C++ workload.

### 2 Use vcpkg
The below instructions are not valid, we use vcpkg now. Be sure to update to latest.

- You can run vcpkg from the x86 command line tools
- it has what we need - wxwidgets, etc
- however, this default vcpkg needs an update, so get latest from github and put it in vcpkg folder
- install and register it, then set it on the path
- test with --version to ensure it is the correct one
- note how the tasks.json references it (relative to workspace root)
- afterwards, cmake should build all those dependencies
