
echo "--- Re-compiling Project ---"
make clean && make
echo "----------------------------"

SIZES=(1024 32768 262144 1048576)
THREADS=(1 2 4 8)
DURATION=5
OUTPUT_CSV="MT25059_results.csv"

echo "Type,MsgSize,Threads,Throughput_Gbps,Latency_us,Cycles,Instructions,CacheMisses,ContextSwitches" > $OUTPUT_CSV

run_test() {
    TYPE=$1
    SERVER_BIN=$2
    CLIENT_BIN=$3

    echo "Starting Experiment Set: $TYPE"
    ./$SERVER_BIN &
    SERVER_PID=$!
    sleep 2 

    for SIZE in "${SIZES[@]}"; do
        for THREAD in "${THREADS[@]}"; do
            echo "   -> Running: Size=$SIZE bytes, Threads=$THREAD"
            
            PERF_OUT="temp_perf.txt"
            CLIENT_OUT="temp_client.txt"
            
            sudo perf stat -e cycles,instructions,cache-misses,context-switches \
                -o $PERF_OUT \
                ./$CLIENT_BIN $SIZE $THREAD $DURATION > $CLIENT_OUT
            
            THROUGHPUT=$(cat $CLIENT_OUT | grep "Total Throughput" | awk '{print $3}')
            
            if (( $(echo "$THROUGHPUT > 0" | bc -l) )); then
                LATENCY=$(echo "scale=4; ($SIZE * 8) / ($THROUGHPUT * 1000)" | bc -l)
            else
                LATENCY=0
            fi
            
            CYCLES=$(grep "cycles" $PERF_OUT | awk '{print $1}' | tr -d ',')
            INSTR=$(grep "instructions" $PERF_OUT | awk '{print $1}' | tr -d ',')
            CACHE_MISS=$(grep "cache-misses" $PERF_OUT | awk '{print $1}' | tr -d ',')
            CTX_SWITCH=$(grep "context-switches" $PERF_OUT | awk '{print $1}' | tr -d ',')

            [ -z "$CYCLES" ] && CYCLES=0
            [ -z "$INSTR" ] && INSTR=0
            [ -z "$CACHE_MISS" ] && CACHE_MISS=0
            [ -z "$CTX_SWITCH" ] && CTX_SWITCH=0
            echo "$TYPE,$SIZE,$THREAD,$THROUGHPUT,$LATENCY,$CYCLES,$INSTR,$CACHE_MISS,$CTX_SWITCH" >> $OUTPUT_CSV
        done
    done
    
    kill $SERVER_PID
    wait $SERVER_PID 2>/dev/null
    sleep 1
}

run_test "A1_TwoCopy" "MT25059_Part_A1_Server" "MT25059_Part_A1_Client"
run_test "A2_OneCopy" "MT25059_Part_A2_Server" "MT25059_Part_A1_Client"
run_test "A3_ZeroCopy" "MT25059_Part_A3_Server" "MT25059_Part_A1_Client"

rm -f temp_perf.txt temp_client.txt
echo "Experiments Completed. Data in $OUTPUT_CSV"