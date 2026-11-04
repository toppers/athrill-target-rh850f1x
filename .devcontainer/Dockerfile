ARG UBUNTU_VERSION=24.04
FROM --platform=linux/amd64 mcr.microsoft.com/devcontainers/cpp:1-ubuntu-${UBUNTU_VERSION}

# CMake, Ninja and gcc come with the base image; Python 3 runs the CTest
# helpers (test/test_hako.py, the core's malformed ELF test) and tools/hako.py.
RUN apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y --no-install-recommends python3 \
    && rm -rf /var/lib/apt/lists/*
