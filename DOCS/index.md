# Welcome to official documentation for NGIN 🚀

## 🛠 Prerequisites

This part of the guide will show you all steps needed to get NGIN up and compiling on your system

### 1️⃣ C++ Compiler

NGIN is grounded in C++ and is only tested for C++20 or newer.

- **Windows**:
  - Dive into C++ with [Microsoft Visual Studio](https://visualstudio.microsoft.com/downloads/). Opt for the Community edition, which is not only free but also packed with all the essential tools for C++ development. Don't forget to choose the "Desktop development with C++" workload during the installation phase.
  
- **Ubuntu**:
  - Prepare your Linux environment with a robust C++ compiler. For our Ubuntu users, here's a quick setup with GCC:
  
    ```bash
    sudo apt-get update
    sudo apt-get install g++
    ```

### 2️⃣ CMake

NGIN uses CMake as it's build system.

> 📘 **Quick Note**: This guide assumes a basic understanding of CMake and won't delve deep into its intricacies.

- **Windows**:
  - Get started by downloading the Windows installer from the [official CMake website](https://cmake.org/download/). During the setup, make sure to integrate CMake with your system's PATH.
  
- **Ubuntu**:
  - For our Ubuntu aficionados, installation is just a command away:
  
    ```bash
    sudo apt-get install cmake
    ```
  - CMake can also be installed using Snap or the built-in software application

### 3️⃣ Graphics Libraries

NGIN is made to support diffrent kinds of graphic APIs, here are all the steps to get them all working.

#### 🎮 Vulkan SDK

By default, NGIN in configured to support Vulkan. This can be disabled in the cmake configuration with the NGIN_BUILD_VULKAN flag.

- **Windows/Ubuntu**:
  - Immerse yourself in Vulkan by visiting the [official Vulkan SDK website](https://vulkan.lunarg.com/sdk/home) Simply follow the on-screen cues, and you're set!
  


#### 🖼 OpenGL

- **Windows/Ubuntu**:
  - OpenGL drivers typically come bundled with your GPU drivers. So, all you need to do is to ensure your GPU drivers (from NVIDIA, AMD, or Intel) are up-to-date.
  
### 4 Dependencies

#### No need

- NGIN handles dependencies automatically using CMake Fetch. The only requirement is a solid internet connection.
