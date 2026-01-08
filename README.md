# Memory Management & Cache Simulator

A C++ simulation of memory allocation strategies (First Fit, Best Fit, Worst Fit) and a multi-level cache system (L1/L2) with FIFO replacement policy.

## 📋 Features

### Memory Management
- **Allocation Strategies:** Supports First Fit, Best Fit, and Worst Fit.
- **Fragmentation Handling:** Tracks internal and external fragmentation.
- **Coalescing:** Automatically merges adjacent free blocks upon deallocation.
- **Visualization:** Dumps the current state of memory blocks (Used vs. Free).

### Cache Simulation
- **Multi-Level Cache:** Simulates L1 and L2 caches.
- **Mapping:** Set-Associative mapping.
- **Replacement Policy:** FIFO (First-In-First-Out).
- **Hit/Miss Tracking:** Reports L1 hits, L2 hits, and Main Memory accesses.

---

## 🚀 Getting Started

### Prerequisites
- A C++ Compiler (GCC/G++ recommended)
### Command Guide

| Command | Description |
| :--- | :--- |
| `init memory <size>` | Resets memory and initializes it with the given total size (in bytes). |
| `set allocator <type>` | Switch allocation strategy. Options: `first_fit`, `best_fit`, `worst_fit`. |
| `malloc <size>` | Requests a memory block of the specified size. |
| `free <id>` | Frees the allocated block with the specific ID and merges adjacent free blocks. |
| `dump memory` | Prints the address range and status (Used/Free) of every block. |
| `stats` | Displays memory utilization, fragmentation percentages, and allocation success rates. |
| `access <address>` | Simulates reading an address to check for L1/L2 hits or misses. |
| `exit` | Closes the simulator. |

---

## 🚀 How to Run

### Run with Test Inputs (Windows PowerShell)
To execute the simulator using the pre-defined test workload inside the `tests` folder:

```powershell
Get-Content tests\test_inputs.txt | .\main.exe
```
### Run manually
To run the simulator in interactive mode

```powershell
make run
```
