# Welcome to official documentation for NGIN 🚀



## 🛠 Dependency Guide

Before diving into the vibrant world of NGIN, it's crucial to ensure your environment is primed with the necessary dependencies. This guide provides a streamlined walkthrough to equip you with everything you need to harness the full power of NGIN.

---

## Prerequisites

### 1️⃣ C++ Compiler

Built with cutting-edge performance in mind, NGIN is grounded in C++ and is optimized for C++20 or newer.

- **Windows**:
  - Dive into C++ with [Microsoft Visual Studio](https://visualstudio.microsoft.com/downloads/). Opt for the Community edition, which is not only free but also packed with all the essential tools for C++ development. Don't forget to choose the "Desktop development with C++" workload during the installation phase.
  
- **Linux**:
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
  
- **Linux**:
  - For our Ubuntu aficionados, installation is just a command away:
  
    ```bash
    sudo apt-get install cmake
    ```

### 3️⃣ Graphics Libraries

Depending on your vision and the immersive experiences you want to craft, certain libraries or SDKs become indispensable.

#### 🎮 Vulkan SDK:

- **Windows**:
  - Immerse yourself in Vulkan by visiting the [official Vulkan SDK website](https://vulkan.lunarg.com/sdk/home) and fetching the installer for Windows. Simply follow the on-screen cues, and you're set!
  
- **Linux**:
  - For Ubuntu, the world of Vulkan is just a couple of commands away:

    ```bash
    sudo apt-get install vulkan-tools
    ```

#### 🖼 OpenGL:

- **Windows/Linux**:
  - OpenGL drivers typically come bundled with your GPU drivers. So, to soar in the OpenGL skies, ensure your GPU drivers (from NVIDIA, AMD, or Intel) are always updated.
