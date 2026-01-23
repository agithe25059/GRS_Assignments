#!/bin/bash

# Output CSV file
OUTPUT_FILE="MT25059_Part_D_CSV.csv"
# Header includes: Count (Scale), CPU, Mem, IO Total, and Duration
echo "Program,Worker,Count,CPU_Avg_%,Mem_Max_KB,kB_Wrtn_Total,Duration_Sec" > $OUTPUT_FILE

WORKERS=("cpu" "mem" "io")

# --- Robust Monitoring Function (Same logic as Part C) ---
monitor_metrics() {
    local pid=$1
    local cpu_total=0
    local mem_max=0
    local count=0
    
    sleep 0.5
    
    while kill -0 $pid 2>/dev/null; do
        children=$(pgrep -P $pid | tr '\n' ',')
        target_pids="${children}${pid}"
        
        # Capture CPU and Memory (KB)
        stats=$(ps -p "${target_pids%,}" -o %cpu,rss --no-headers 2>/dev/null | awk '{c+=$1; m+=$2} END {print c, m}')
        c=$(echo $stats | awk '{print $1}')
        m=$(echo $stats | awk '{print $2}')
        
        # Sanitize defaults
        c=${c:-0}
        m=${m:-0}

        # Track Max Memory
        if (( $(echo "$m > $mem_max" | bc -l) )); then
            mem_max=$m
        fi

        cpu_total=$(echo "$cpu_total + $c" | bc)
        count=$((count + 1))
        
        sleep 1
    done

    if [ $count -gt 0 ]; then
        cpu_avg=$(echo "scale=2; $cpu_total / $count" | bc)
    else
        cpu_avg=0
    fi
    
    # Check Disk Artifacts for Total IO Written
    # This is the fail-proof method used in Part C
    disk_size=$(du -k inOut_*.dat 2>/dev/null | tail -n 1 | awk '{print $1}')
    disk_size=${disk_size:-0}

    echo "$cpu_avg $mem_max $disk_size"
}

# --- 1. Program A (Processes) Scaling: 2, 3, 4, 5 ---
PROG="program_a"
for worker in "${WORKERS[@]}"; do
    for count in 2 3 4 5; do
        echo "Running $PROG ($worker) with $count processes..."
        
        # Cleanup
        rm -f inOut_*.dat
        
        start_t=$(date +%s.%N)
        taskset -c 0 ./$PROG $count $worker &
        PID=$!
        
        metrics=$(monitor_metrics $PID)
        
        end_t=$(date +%s.%N)
        dur=$(echo "$end_t - $start_t" | bc)
        
        cpu=$(echo $metrics | awk '{print $1}')
        mem=$(echo $metrics | awk '{print $2}')
        io=$(echo $metrics | awk '{print $3}')
        
        echo "$PROG,$worker,$count,$cpu,$mem,$io,$dur" >> $OUTPUT_FILE
        
        # Cleanup
        rm -f inOut_*.dat
    done
done

# --- 2. Program B (Threads) Scaling: 2 to 8 ---
PROG="program_b"
for worker in "${WORKERS[@]}"; do
    for count in 2 3 4 5 6 7 8; do
        echo "Running $PROG ($worker) with $count threads..."
        
        # Cleanup
        rm -f inOut_*.dat
        
        start_t=$(date +%s.%N)
        taskset -c 0 ./$PROG $count $worker &
        PID=$!
        
        metrics=$(monitor_metrics $PID)
        
        end_t=$(date +%s.%N)
        dur=$(echo "$end_t - $start_t" | bc)
        
        cpu=$(echo $metrics | awk '{print $1}')
        mem=$(echo $metrics | awk '{print $2}')
        io=$(echo $metrics | awk '{print $3}')
        
        echo "$PROG,$worker,$count,$cpu,$mem,$io,$dur" >> $OUTPUT_FILE
        
        # Cleanup
        rm -f inOut_*.dat
    done
done

echo "Part D Complete. Data saved to $OUTPUT_FILE"