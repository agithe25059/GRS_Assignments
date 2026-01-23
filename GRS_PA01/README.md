# Process vs. Thread Performance Analysis (Assignment 1)

**Name:** Prashant

**Roll No:** [Insert Your Roll Number]

**Course:** [Insert Course Name]

---

## 📖 Project Overview

This project benchmarks and analyzes the performance differences between **Multi-Processing** (using `fork`) and **Multi-Threading** (using `pthread`) on a single-core CPU environment.

The evaluation focuses on three distinct workloads:

1. **CPU-Intensive:** Computationally heavy floating-point arithmetic designed to saturate the Arithmetic Logic Unit (ALU).
2. **Memory-Intensive:** Large-scale memory allocations (50MB) to stress the RAM subsystem and memory controller.
3. **I/O-Intensive:** Repeated disk writes that force the CPU into `iowait` states while waiting for storage operations.

---

## 📂 File Structure

### Source Code

* **`MT25059_Part_A_Program_A.c`**: Implements multi-process logic using the `fork()` system call.
* **`MT25059_Part_B_Program_B.c`**: Implements multi-thread logic using the `pthread_create()` library function.
* **`MT25059_Part_B_worker.h`**: A shared header file defining the specific workload logic for `cpu`, `mem`, and `io` tasks.

### Automation & Tools

* **`Makefile`**: Automates compilation and cleanup of binaries and temporary data.
* **`MT25059_Part_C_shell.sh`**: Bash script for System Observation Analysis; captures CPU%, Memory, and Disk metrics for 2 concurrent workers.
* **`MT25059_Part_D_shell.sh`**: Bash script for Scaling Analysis; increments workers from 2 to 8 to observe performance trends.
* **`MT25059_Part_D_Plotter.py`**: Python script that reads CSV data and generates scaling graphs.

---

## ⚙️ Prerequisites

Ensure the following tools are installed on your Linux system (or WSL):

* **GCC Compiler:** `sudo apt install build-essential`
* **Make:** `sudo apt install make`
* **Python 3:** `sudo apt install python3 python3-pip`
* **Sysstat (for iostat):** `sudo apt install sysstat`

**Python Dependencies:**

```bash
pip3 install pandas matplotlib

```

---

## 🚀 Usage Instructions

### 1. Compilation

Build the executables using the provided `Makefile`:

```bash
make

```

*This creates the `program_a` and `program_b` executables.*

### 2. Run System Analysis (Part C)

This script runs 6 scenarios pinned to Core 0 using `taskset`:

```bash
chmod +x MT25059_Part_C_shell.sh
./MT25059_Part_C_shell.sh

```

* **Output:** `MT25059_Part_C_CSV.csv`

### 3. Run Scaling Analysis (Part D)

This script increments the load to observe scaling behavior:

```bash
chmod +x MT25059_Part_D_shell.sh
./MT25059_Part_D_shell.sh

```

* **Output:** `MT25059_Part_D_CSV.csv`

### 4. Generate Plots

Visualize the data collected in Part D:

```bash
python3 MT25059_Part_D_Plotter.py

```

* **Output:** Generates `MT25059_Part_D_plot_process_scaling.png` and `MT25059_Part_D_plot_thread_scaling.png`.

---

## 📊 Key Findings

* **CPU Tasks:** Showed linear time scaling on a single core as workers contended for the same ALU cycles.
* **Memory Overhead:** Processes showed linear memory growth with each new worker, while threads maintained a constant memory footprint due to shared address space.
* **IO Blocking:** Successfully demonstrated that CPU utilization remains low while the system is bottlenecked by disk latency.

---

## 🤖 AI Usage Declaration

* **Tool:** Google Gemini
* **Usage:** * Debugging C syntax for thread argument passing.
* Optimizing Bash script data collection logic.
* Refining `MT25059_Part_B_worker.h` loop durations for measurable benchmarks.


* **Verification:** All logic was verified against Linux man pages and experimental data; code was executed and validated in a local environment.

---

**Would you like me to help you draft the submission email for your professor?**