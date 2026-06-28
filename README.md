# Edge-AI-Vision-Pipeline-Optimization

Optimizing NVIDIA-based vision pipelines to achieve ultra-low latency (17ms) for real-time industrial and medical AI applications.

---

## Requirements

- [CMake 3.16+](https://cmake.org/download/)
- [GStreamer 1.0](https://gstreamer.freedesktop.org/download/)
- pkg-config

---

## Installation

### Linux

**Arch Linux**
```bash
sudo pacman -S cmake gstreamer gst-plugins-base gst-plugins-good
```

**Ubuntu/Debian**
```bash
sudo apt install cmake libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
```

### Windows

| Option | Best For | Benefit |
|--------|----------|---------|
| **A – Native** | Maximum performance | No Docker required |
| **B – Docker + VcXsrv** | Isolated environment | Cross-platform, reproducible |

---

#### Option A: Native Windows

##### 1. Install CMake

Download from [cmake.org/download](https://cmake.org/download/) and select **"Add CMake to the system PATH"** during installation.

##### 2. Install GStreamer

Download **both** x86_64 installers from [gstreamer.freedesktop.org/download](https://gstreamer.freedesktop.org/download/):

- `gstreamer-1.0-msvc-x86_64-*.msi` — runtime
- `gstreamer-1.0-devel-msvc-x86_64-*.msi` — development headers

> ⚠️ Select **"Complete"** installation type — "Typical" omits required plugins and `.pc` files.

Default install path: `C:\Program Files\gstreamer\1.0\msvc_x86_64\`

##### 3. Install pkg-config (optional)

GStreamer bundles `pkg-config.exe` in its `bin` folder. Skip this step if you set PATH in step 4.

To install separately via [MSYS2](https://www.msys2.org/), open an **MSYS2 MinGW x64** shell:

```bash
pacman -S mingw-w64-x86_64-pkg-config
```

Then add MSYS2 to PATH:

```powershell
[System.Environment]::SetEnvironmentVariable(
    "Path",
    [System.Environment]::GetEnvironmentVariable("Path", "User") + ";C:\msys64\mingw64\bin",
    "User"
)
```

##### 4. Set Environment Variables

Run each block in PowerShell:

```powershell
# GStreamer binaries (pkg-config.exe + DLLs)
[System.Environment]::SetEnvironmentVariable(
    "Path",
    [System.Environment]::GetEnvironmentVariable("Path", "User") + ";C:\Program Files\gstreamer\1.0\msvc_x86_64\bin",
    "User"
)

# GStreamer root directory
[System.Environment]::SetEnvironmentVariable(
    "GSTREAMER_1_0_ROOT_MSVC_X86_64",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64",
    "User"
)

# .pc files for pkg-config
[System.Environment]::SetEnvironmentVariable(
    "PKG_CONFIG_PATH",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64\lib\pkgconfig",
    "User"
)

# GStreamer plugins for runtime
[System.Environment]::SetEnvironmentVariable(
    "GST_PLUGIN_PATH",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64\lib\gstreamer-1.0",
    "User"
)
```

> ⚠️ Restart PowerShell after setting environment variables.

##### 5. Verify Installation

```powershell
pkg-config --version
pkg-config --modversion gstreamer-1.0
```

Both commands should return a version number. If `pkg-config` is not found, confirm the GStreamer `bin` folder is in PATH.

---

#### Option B: Docker + VcXsrv

##### Prerequisites

- [Docker Desktop for Windows](https://www.docker.com/products/docker-desktop/)
- [VcXsrv / XLaunch](https://sourceforge.net/projects/vcxsrv/) — for displaying video output from the container

##### Step 1: Configure VcXsrv (required before each container start)

1. Open **XLaunch**
2. Display Settings → `Multiple windows` → Display number `-1` → Next
3. Client Type → `Start no client` → Next
4. Extra Settings:
   - ✅ `Clipboard & Primary Selection`
   - ✅ `Native opengl`
   - ✅ `Disable access control` — **required**, or Windows will block video output
5. Click **Finish**

> 💡 Hover over the tray icon to see your Display ID (e.g. `:0.0`). Ensure `docker-compose.yml` has `DISPLAY=host.docker.internal:0.0`.

##### Step 2: Build and Start the Container

```powershell
docker compose up -d --build
```

##### Step 3: Enter the Container

```powershell
docker compose exec gstreamer-env /bin/bash
```

##### Step 4: Build and Run

```bash
cd /workspace
cmake -S . -B build
cmake --build build

# Test display forwarding
gst-launch-1.0 videotestsrc ! ximagesink sync=false

# Run application
./build/myapp
```

---

## Run

| Platform | Command |
|----------|---------|
| Linux | `./build/myapp` |
| Windows (Native) | `.\build\Debug\myapp.exe` |
| Windows (Docker) | `./build/myapp` (inside container) |

---

## Troubleshooting

### Native (Windows)

**CMake can't find GStreamer packages**
Verify `PKG_CONFIG_PATH` points to the correct `.pc` files directory and restart your terminal after setting environment variables.

**`myapp.exe` crashes or won't start**
GStreamer DLLs must be accessible at runtime. Confirm `C:\Program Files\gstreamer\1.0\msvc_x86_64\bin` is in `PATH`.

**`pkg-config` not found**
Confirm the GStreamer `bin` folder is in `PATH` and restart your terminal. Alternatively, install via MSYS2 (see step 3).

### Docker + VcXsrv

**`Could not open display` / `Connection Refused`**
- Confirm VcXsrv is running in the system tray
- Confirm `Disable access control` was checked in XLaunch
- Check Windows Defender Firewall — VcXsrv must be allowed on both Private and Public networks

**Segmentation fault on startup**
Occurs when GStreamer tries to initialize a video sink (`ximagesink` or `autovideosink`) without an X Server connection. Fix the display connection first, or switch to `fakesink` / `filesink` for headless operation.