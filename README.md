# RISC-V Emulator

A lightweight C++ RISC-V (RV64GC) emulator.

## Features
- **RV64IMAFDC**: Base integer, M-extension, atomics, compressed instructions, and single/double-precision floating point.
- **Privilege Modes**: Machine mode (M) and Supervisor mode (S).
- **MMU**: Sv39 page-based virtual memory translation with TLB caching.
- **Devices**: CLINT (timer), PLIC (interrupt controller), UART 8250, and VirtIO block device.

## Building
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Running
```bash
./riscv-emulator <binary> [options]
```
