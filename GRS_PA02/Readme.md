# Roll No: MT25059
# Project: PA02 - Analysis of Network I/O Primitives using "perf" tool

## Objective
The goal of this assignment is to experimentally study the cost of data movement in network I/O by implementing and comparing three socket communication versions:
1. [cite_start]Standard two-copy socket communication (Baseline)[cite: 16].
2. [cite_start]One-copy optimized socket communication[cite: 17].
3. [cite_start]Zero-copy socket communication[cite: 18].

[cite_start]The project implementation analyzes micro-architectural effects such as CPU cycles and cache behavior[cite: 20].

## Implementation Details
[cite_start]The project utilizes a multithreaded TCP-based client-server application written in C[cite: 19, 22].

### A1: Two-Copy Implementation (Baseline)
- [cite_start]**Server**: Uses standard `send()` primitives[cite: 40].
- [cite_start]**Mechanism**: Involves data copies between user-space and kernel-space[cite: 43].
- [cite_start]**Data Structure**: Uses a `ComplexMessage` structure with 8 dynamically allocated string fields[cite: 28].

### A2: One-Copy Implementation
- [cite_start]**Server**: Uses `sendmsg()` with an `iovec` structure (scatter-gather I/O)[cite: 46].
- [cite_start]**Optimization**: Reduces copy overhead by allowing the kernel to read directly from multiple user-space buffers[cite: 45].

### A3: Zero-Copy Implementation
- [cite_start]**Server**: Implements `sendmsg()` with the `MSG_ZEROCOPY` flag[cite: 51].
- [cite_start]**Mechanism**: Eliminates the copy between user-space and kernel-space by pinning memory pages[cite: 49].

## File Structure
- `MT25059_common.h`: Shared headers and `ComplexMessage` definitions.
- `MT25059_Part_A1_Server.c`: Baseline server.
- `MT25059_Part_A1_Client.c`: Multithreaded client used for all testing.
- `MT25059_Part_A2_Server.c`: One-copy optimized server.
- `MT25059_Part_A3_Server.c`: Zero-copy optimized server.
- [cite_start]`MT25059_Part_C_Bench.sh`: Automated bash script for benchmarking[cite: 70].
- [cite_start]`MT25059_Part_D_Plots.py`: Python script for generating visualizations using hardcoded data[cite: 121, 122].
- [cite_start]`Makefile`: Script to compile all implementations[cite: 104].

## Usage

### 1. Compilation
[cite_start]To compile the binaries as defined in the Makefile[cite: 135]:
```bash
make