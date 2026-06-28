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

Windows รองรับ 2 วิธี เลือกตามความเหมาะสม:

| วิธี | เหมาะกับ | ข้อดี |
|------|----------|-------|
| **Option A – Native (ไม่ใช้ Docker)** | ผู้ที่ต้องการ performance สูงสุด | ตรงไปตรงมา ไม่ต้องติดตั้ง Docker |
| **Option B – Docker + VcXsrv** | ผู้ที่ต้องการ environment ที่แยกออกจากระบบ | Cross-platform, reproducible |

---

#### Option A: Native Windows Development

##### 1. ติดตั้ง CMake

ดาวน์โหลดและติดตั้งจาก [cmake.org/download](https://cmake.org/download/)
ระหว่างติดตั้ง ให้เลือก **"Add CMake to the system PATH"**

##### 2. ติดตั้ง GStreamer

ดาวน์โหลด **ทั้งสอง** installer สำหรับ architecture x86_64 จาก [gstreamer.freedesktop.org/download](https://gstreamer.freedesktop.org/download/):

- `gstreamer-1.0-msvc-x86_64-*.msi` — runtime
- `gstreamer-1.0-devel-msvc-x86_64-*.msi` — development headers

> ⚠️ เมื่อถูกถามให้เลือก Installation Type ให้เลือก **"Complete"** เท่านั้น ห้ามเลือก "Typical" เพราะจะขาด plugin และ `.pc` files ที่จำเป็น

Default install path: `C:\Program Files\gstreamer\1.0\msvc_x86_64\`

##### 3. ติดตั้ง pkg-config (ถ้าจำเป็น)

GStreamer มี `pkg-config.exe` bundle มาให้แล้วในโฟลเดอร์ `bin` ดังนั้น **ข้ามขั้นตอนนี้ได้** ถ้าคุณตั้งค่า PATH ในขั้นตอนถัดไปแล้ว

หากต้องการติดตั้งแยกผ่าน [MSYS2](https://www.msys2.org/) ให้เปิด **MSYS2 MinGW x64** shell แล้วรัน:

```bash
pacman -S mingw-w64-x86_64-pkg-config
```

จากนั้นเพิ่ม MSYS2 MinGW bin เข้า PATH ใน PowerShell:

```powershell
[System.Environment]::SetEnvironmentVariable(
    "Path",
    [System.Environment]::GetEnvironmentVariable("Path", "User") + ";C:\msys64\mingw64\bin",
    "User"
)
```

##### 4. ตั้งค่า Environment Variables

เปิด PowerShell แล้วรันคำสั่งต่อไปนี้ทีละบรรทัด:

```powershell
# เพิ่ม GStreamer bin เข้า PATH (สำหรับ pkg-config.exe และ DLLs)
[System.Environment]::SetEnvironmentVariable(
    "Path",
    [System.Environment]::GetEnvironmentVariable("Path", "User") + ";C:\Program Files\gstreamer\1.0\msvc_x86_64\bin",
    "User"
)

# กำหนด GStreamer root directory
[System.Environment]::SetEnvironmentVariable(
    "GSTREAMER_1_0_ROOT_MSVC_X86_64",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64",
    "User"
)

# กำหนด path ไปยัง .pc files สำหรับ pkg-config
[System.Environment]::SetEnvironmentVariable(
    "PKG_CONFIG_PATH",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64\lib\pkgconfig",
    "User"
)

# กำหนด path ไปยัง GStreamer plugins สำหรับ runtime
[System.Environment]::SetEnvironmentVariable(
    "GST_PLUGIN_PATH",
    "C:\Program Files\gstreamer\1.0\msvc_x86_64\lib\gstreamer-1.0",
    "User"
)
```

> ⚠️ **ปิดและเปิด PowerShell ใหม่** หลังจากตั้งค่า environment variables ทุกครั้ง

##### 5. ตรวจสอบการติดตั้ง

```powershell
pkg-config --version
pkg-config --modversion gstreamer-1.0
```

ทั้งสองคำสั่งควรแสดง version number ถ้า `pkg-config` ไม่พบ ให้ตรวจสอบว่า GStreamer `bin` folder อยู่ใน PATH แล้วหรือยัง

---

#### Option B: Containerized Development (Docker + VcXsrv)

เหมาะสำหรับผู้ที่ต้องการ isolated environment หรือทำงานร่วมกับ Linux developers

##### Prerequisites

- [Docker Desktop for Windows](https://www.docker.com/products/docker-desktop/)
- [VcXsrv Windows X Server (XLaunch)](https://sourceforge.net/projects/vcxsrv/) — สำหรับแสดง video output จาก container

##### ขั้นตอนที่ 1: ตั้งค่า VcXsrv (X11 Forwarding)

ต้องทำก่อนเปิด container ทุกครั้ง:

1. เปิดแอป **XLaunch**
2. **Display Settings** → เลือก `Multiple windows` → ตั้ง Display number เป็น `-1` → Next
3. **Client Type** → เลือก `Start no client` → Next
4. **Extra Settings** (สำคัญมาก):
   - ✅ Check: `Clipboard & Primary Selection`
   - ✅ Check: `Native opengl`
   - ✅ **ต้อง Check: `Disable access control`** — ถ้าไม่ check Windows จะบล็อก video output
5. คลิก **Finish**

> 💡 Hover ที่ tray icon เพื่อดู Display ID ของคุณ (เช่น `:0.0` หรือ `:1.0`)
> ถ้าเป็น `:0.0` ให้ตรวจสอบว่า `docker-compose.yml` มี `DISPLAY=host.docker.internal:0.0`

##### ขั้นตอนที่ 2: Build และ Start Container

เปิด PowerShell ใน project root directory แล้วรัน:

```powershell
docker compose up -d --build
```

##### ขั้นตอนที่ 3: เข้าสู่ Container

```powershell
docker compose exec gstreamer-env /bin/bash
```

##### ขั้นตอนที่ 4: Build และ Run ภายใน Container

```bash
# ไปยัง workspace และ build
cd /workspace
cmake -S . -B build
cmake --build build

# ทดสอบ GStreamer display forwarding
gst-launch-1.0 videotestsrc ! ximagesink sync=false

# Run application
./build/myapp
```

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

**Windows (Native)**
```powershell
.\build\Debug\myapp.exe
```

**Windows (Docker)**
```bash
# รันภายใน container
./build/myapp
```

---

## Troubleshooting (Windows)

### Native

**CMake ไม่พบ GStreamer packages**
ตรวจสอบว่า `PKG_CONFIG_PATH` ชี้ไปยัง `.pc` files directory ที่ถูกต้อง และเปิด terminal ใหม่หลังจากตั้งค่า environment variables

**`myapp.exe` crash หรือไม่ start**
GStreamer DLLs ต้องเข้าถึงได้ตอน runtime ตรวจสอบว่า `C:\Program Files\gstreamer\1.0\msvc_x86_64\bin` อยู่ใน `PATH`

**`pkg-config` not found**
มากับ GStreamer — ยืนยันว่า GStreamer `bin` folder อยู่ใน `PATH` และเปิด terminal ใหม่ หรือติดตั้งผ่าน MSYS2 (ดูขั้นตอนที่ 3)

### Docker + VcXsrv

**Error: `Could not open display` / `Connection Refused`**
- ตรวจสอบว่า VcXsrv กำลัง run อยู่ใน system tray
- ยืนยันว่าเลือก `Disable access control` ใน XLaunch
- ตรวจสอบ Windows Defender Firewall ว่าอนุญาต VcXsrv ทั้ง Private และ Public networks

**Segmentation fault (core dumped) ตอน startup**
เกิดเมื่อ GStreamer พยายาม initialize video sink (`ximagesink` หรือ `autovideosink`) แต่ต่อกับ X Server ไม่ได้ แก้ไข display connection ก่อน

ถ้าต้องการรัน headless (ไม่มีหน้าจอ) ให้แก้ pipeline code ให้ใช้ `fakesink` หรือ `filesink` แทน video rendering sink