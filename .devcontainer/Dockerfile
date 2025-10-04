ARG UBUNTU_VERSION=22.04
FROM --platform=linux/amd64 mcr.microsoft.com/devcontainers/cpp:1-ubuntu-${UBUNTU_VERSION}

RUN apt-get update && apt-get install -y libspdlog-dev python3-dev && rm -rf /var/lib/apt/lists/*
