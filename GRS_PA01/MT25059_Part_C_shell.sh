#!/bin/bash

OUTPUT_FILE="MT25059_Part_C_CSV.csv"
echo "Program+Function,CPU_Avg%,Mem_Max_KB,kB_Wrtn_Total,Duration_Sec" >$OUTPUT_FILE

PROGRAMS=("program_a" "program_b")
WORKERS=("cpu" "mem" "io")

monitor_process() {
    local main_pid=$1
    local cpu_tot=0
    local mem_max=0
    local count=0
    
    sleep 0.1

    while kill -0 $main_pid 2>/dev/null; do
        children=$(pgrep -P $main_pid | tr '\n' ',')
        target_pids="${children}${main_pid}"
        
        stats=$(ps -p "${target_pids%,}" -o %cpu,rss --no-headers 2>/dev/null | awk '{c+=$1; m+=$2} END {print c, m}')
        c=$(echo $stats |awk '{print $1}';[ -z "$stats" ]&& echo 0)
        m=$(echo $stats | awk '{print $2}';[ -z "$stats" ]&& echo 0)
        
        if(( $(echo "$m > $mem_max" | bc -l) )); then 
            mem_max=$m; 
        fi

        cpu_tot=$(echo "$cpu_tot + $c" | bc)
        count=$((count + 1))
        
        sleep 0.5
    done

    if [ $count -gt 0 ]; then
        cpu_avg=$(echo "scale=2; $cpu_tot / $count"|bc)
    else
        cpu_avg=0
    fi

    disk_size=$(du -k inOut_*.dat 2>/dev/null|tail -n 1|awk '{print $1}')
    disk_size=${disk_size:-0}

    echo "$cpu_avg $mem_max $disk_size"
}

for prog in "${PROGRAMS[@]}"; do
    for worker in "${WORKERS[@]}"; do
        echo "Running $prog + $worker..."
        rm -f inOut_*.dat
        
        COUNT=2
        start_time=$(date +%s.%N)
        taskset -c 0 ./$prog $COUNT $worker &
        PID=$!

        results=$(monitor_process $PID)
        
        end_time=$(date +%s.%N)
        duration=$(echo "$end_time - $start_time" | bc)

        cpu_res=$(echo $results | awk '{print $1}')
        mem_res=$(echo $results | awk '{print $2}')
        io_res=$(echo $results | awk '{print $3}')

        echo "${prog}+${worker},$cpu_res,$mem_res,$io_res,$duration">>$OUTPUT_FILE
        echo "completed ${prog}+${worker}. Time: ${duration}s"
        echo "            ----"
        rm -f inOut_*.dat
        sleep 1
    done
done