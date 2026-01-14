# LINUX High-Performance TCP Client-Server

A clean, robust, and high-performance TCP protocol for transmitting user data (ID, name, email) in C++ using Qt.

## ✅ Features

- Binary protocol (fast, compact)
- Email validation
- Cross-platform (Linux, macOS)
- Linux-optimized: `TCP_NODELAY`, tuned buffers
- Reusable buffers to reduce allocations
- Unit tested (correctness + performance)

## 🛠️ Build Instructions

### Prerequisites

- CMake 3.16+
- Qt 5.15+ or Qt 6.x
- C++17 compiler (g++/clang++)

### Build

```bash
mkdir build && cd build
cmake ..
make
```
# Run
```bash
./server/linx_server & ./client/linx_client &
```

### Build & Run Tests

```bash
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
make -j$(nproc)
./tests/tst_protocol
```
