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

NGIN stands tall on the robust foundation of CMake.

> 📘 **Quick Note**: This guide assumes a basic understanding of CMake and won't delve deep into its intricacies.

- **Windows**:
  - Get started by downloading the Windows installer from the [official CMake website](https://cmake.org/download/). During the setup, make sure to integrate CMake with your system's PATH.
  
- **Ubuntu**:
  - For our Ubuntu aficionados, installation is just a command away:
  
    ```bash
    sudo apt-get install cmake
    ```

### 3️⃣ Graphics Libraries

NGIN is made to support diffrent kinds of graphic APIs, here are all the steps to get them all working.

#### 🎮 Vulkan SDK

- **Windows**:
  - Immerse yourself in Vulkan by visiting the [official Vulkan SDK website](https://vulkan.lunarg.com/sdk/home) and fetching the installer for Windows. Simply follow the on-screen cues, and you're set!
  
- **Ubuntu**:
  - For Ubuntu, the world of Vulkan is just a couple of commands away:

    ```bash
    sudo apt update
    sudo apt install vulkan-sdk
    ```

#### 🖼 OpenGL

- **Windows/Linux**:
  - OpenGL drivers typically come bundled with your GPU drivers. So, all you need to do is to ensure your GPU drivers (from NVIDIA, AMD, or Intel) are up-to-date.
