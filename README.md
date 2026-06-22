# Edge-AI-Vision-Pipeline-Optimization
Optimizing NVIDIA-based vision pipelines to achieve ultra-low latency (17ms) for real-time industrial and medical AI applications.

---

## Requirements

- [CMake 3.16+](https://cmake.org/download/)
- [GStreamer 1.0](https://gstreamer.freedesktop.org/download/)
- pkg-config

---

## Installation

### Linux (Arch Linux)

```bash
sudo pacman -S cmake gstreamer gst-plugins-base gst-plugins-good
```

### Linux (Ubuntu/Debian)

```bash
sudo apt install cmake \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev
```

### Windows

#### 1. Install CMake

Download and install from [cmake.org/download](https://cmake.org/download/).
During installation, select **"Add CMake to the system PATH"**.

#### 2. Install GStreamer

Download **both** installers for your architecture (x86_64) from [gstreamer.freedesktop.org/download](https://gstreamer.freedesktop.org/download/):

- `gstreamer-1.0-msvc-x86_64-*.msi` (runtime)
- `gstreamer-1.0-devel-msvc-x86_64-*.msi` (development)

> ⚠️ When prompted, select **"Complete"** installation type — not "Typical".

Default install path: `C:\Program Files\gstreamer\1.0\msvc_x86_64\`

#### 3. Install pkg-config via MSYS2

If you don't have `pkg-config` installed, install it using [MSYS2](https://www.msys2.org/).

Download and install MSYS2 from [msys2.org](https://www.msys2.org/), then open the **MSYS2 MinGW x64** shell and run:

```bash
pacman -S mingw-w64-x86_64-pkg-config
```

Then add the MSYS2 MinGW bin folder to your PATH in PowerShell:

```powershell
[System.Environment]::SetEnvironmentVariable(
    "Path",
    [System.Environment]::GetEnvironmentVariable("Path", "User") + ";C:\msys64\mingw64\bin",
    "User"
)
```

> ⚠️ GStreamer also bundles its own `pkg-config.exe` in its `bin` folder, so this step is only needed if GStreamer's `bin` is not on your PATH yet.

#### 4. Set Environment Variables

Open PowerShell and run the following:

```powershell
# Add GStreamer bin to PATH (for pkg-config.exe and DLLs)
[System.Environment]::SetEnvironmentVariable(
    "Path",
    [System.Environment]::GetEnvironmentVariable("Path", "User") + ";C:\Program Files\gstreamer\1.0\msvc_x86_64\bin",
    "User"
)

# Set GStreamer root
[System.Environment]::SetEnvironmentVariable(
    "GSTREAMER_1_0_ROOT_MSVC_X86_64",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64",
    "User"
)

# Set pkg-config path to GStreamer's .pc files
[System.Environment]::SetEnvironmentVariable(
    "PKG_CONFIG_PATH",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64\lib\pkgconfig",
    "User"
)

# Set GStreamer plugin path so it can find plugins at runtime
[System.Environment]::SetEnvironmentVariable(
    "GST_PLUGIN_PATH",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64\lib\gstreamer-1.0",
    "User"
)
```

> ⚠️ Close and reopen PowerShell after setting environment variables.

#### 5. Verify Setup

```powershell
pkg-config --version
pkg-config --modversion gstreamer-1.0
```

Both commands should return a version number. If `pkg-config` is not found, make sure the GStreamer `bin` folder is on your PATH.

---

## Build

```bash
cmake -S . -B build
cmake --build build
```

---

## Run

**Linux**
```bash
./build/myapp
```

**Windows**
```powershell
.\build\Debug\myapp.exe
```

---

## Troubleshooting (Windows)

**CMake can't find GStreamer packages**
Make sure `PKG_CONFIG_PATH` points to the correct `.pc` files directory and that you opened a new terminal after setting environment variables.

**`myapp.exe` crashes or fails to start**
GStreamer DLLs must be accessible at runtime. Ensure `C:\Program Files\gstreamer\1.0\msvc_x86_64\bin` is in your `PATH`.

**`pkg-config` not found**
It is bundled with GStreamer — confirm the GStreamer `bin` folder is on your `PATH` and reopen your terminal. Alternatively, install it via MSYS2 (see Step 3).