# GO-KERNEL

A kernel-level software solution designed for reading and writing memory in CS:2.  
This project leverages a custom kernel driver to interact with the game process securely and efficiently from an external user-mode application.  
It is intended for **educational and research purposes**.

---

## ⚠️ Disclaimer
This software is provided for **educational use only**.  
Using software like this in online multiplayer games is a violation of the terms of service for most games and will likely result in a **permanent ban**.  
The author of this repository is not responsible for any damage or consequences that may result from the use of this software. **Use it at your own risk.**

---

## ✨ Features
- **Kernel-Mode Memory Access**: All memory read/write operations are performed by a kernel driver for enhanced security and to bypass common anti-cheat mechanisms.  
- **External User-Mode Application**: A clean separation between the low-level driver and the high-level application logic and user interface.  
- **System Call Hooking**: Communication between the user-mode application and the kernel driver is established by hooking a Windows system call.  
- **ImGui Overlay**: A simple graphical user interface for interacting with the software, built with Dear ImGui.  

---

## 🔧 Architecture Overview

The project is split into two primary components that work together:  
a **user-mode application** and a **kernel-mode driver**.

### `csgo-legit-external` (User-Mode Application)
- This is the main executable that you run.  
- Handles rendering the GUI, managing features, and gathering the necessary information (e.g., memory addresses to read).  
- Cannot directly access the game's memory. Instead, it sends requests to the kernel driver.  

### `kernel-driver` (Kernel-Mode Driver)
- Operates at the highest privilege level (**ring 0**).  
- Sole responsibility is to securely execute memory operations on behalf of the user-mode application.  
- Directly reads or writes to the target process's memory using kernel functions like `MmCopyVirtualMemory`.  

---

## Communication Bridge: How It Works
1. **Driver Loading & Hooking**  
   - When `kernel-driver.sys` is loaded into kernel space, its `DriverEntry` function executes.  
   - This function hooks the `NtOpenCompositionSurfaceSectionInfo` system call by overwriting its initial bytes with a jump to our custom `hook_handler`.  

2. **Sending a Request**  
   - The user-mode application populates a `COPY_MEMORY` struct with the details (process ID, address, data buffer, size, type of operation).  
   - It then calls the original `NtOpenCompositionSurfaceSectionInfo` function.  

3. **Handling the Request**  
   - Since the system call is hooked, the driver’s `hook_handler` is executed.  
   - The handler performs the requested kernel-level memory operation and updates the `COPY_MEMORY` struct with results.  

4. **Receiving the Result**  
   - Control returns to the user-mode application, which reads the results from the struct.  

👉 This method avoids obvious communication channels (like `DeviceIoControl`) that anti-cheat systems monitor.  

---

## 🚀 Getting Started

### Prerequisites
- Windows 10/11 (x64)  
- Microsoft Visual Studio (with C++ Desktop development workload)  
- Windows Driver Kit (WDK)  

### Build Instructions
1. Open `csgo-legit-external.sln` in Visual Studio.  
2. Set the build configuration to **Release** and platform to **x64**.  
3. Build the **kernel-driver** project first → produces `kernel-driver.sys`.  
4. Build the **csgo-legit-external** project → produces `csgo-legit-external.exe`.  

### Running the Software
1. **Update Offsets**: Must be updated manually in source code.  
2. **Map the Driver**: Use a driver mapper (e.g., `kdmapper.exe`) to load `kernel-driver.sys`. Requires administrator privileges.  
3. **Launch CS:2**.  
4. **Run the Executable**: Start `csgo-legit-external.exe` as administrator.  

---

## ❗ Important Notes
- This project is **unfinished** and **no longer actively maintained**.  
- Some features may be incomplete or buggy.  
- **No support will be provided**. You are expected to know how to use and modify the code.  
- Signature scanning has been removed → you must keep game offsets up to date manually.  
