# Edge-AI-Vision-Pipeline-Optimization

Optimizing NVIDIA-based vision pipelines to achieve ultra-low latency (17ms) for real-time industrial and medical AI applications.

---

# Installation

## Requirements

- CMake (3.16 or newer)
- GStreamer 1.0

## Linux (Arch Linux)

```bash
sudo pacman -S cmake gstreamer gst-plugins-base gst-plugins-good
```

## Linux (Ubuntu/Debian)

```bash
sudo apt install cmake \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev
```

## Windows

Install:

- CMake: <https://cmake.org/download/>
- GStreamer SDK: <https://gstreamer.freedesktop.org/download/>

Make sure both are added to your `PATH`.

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/myapp
```
