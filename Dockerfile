FROM ubuntu:24.04

RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    gstreamer1.0-tools \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-x \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace