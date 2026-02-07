
import matplotlib.pyplot as plt

sizes = ['1KB', '32KB', '256KB', '1MB']  
threads = ['1', '2', '4', '8']           
tp_a1 = [14.7726, 46.8505, 60.6426, 54.9485]
tp_a2 = [10.6754, 42.0608, 57.6547, 24.4955]
tp_a3 = [2.3069, 26.6646, 50.9936, 44.0723]

lat_a1 = [34.5821, 19.5975, 55.3100, 67.1793]
lat_a2 = [36.3743, 32.2611, 60.9660, 66.9317]
lat_a3 = [41.1257, 29.1550, 49.4952, 64.6895]

cm_a1 = [5892704, 15235178, 1433194, 289740989]
cm_a2 = [911268, 19995947, 33601972, 264580773]
cm_a3 = [5754433, 5388876, 1144665, 101610743]

cpb_a1 = [1.6086, 0.3909, 0.2873, 0.3858]
cpb_a2 = [1.8042, 0.4081, 0.3115, 0.8039]
cpb_a3 = [7.0027, 0.6304, 0.3723, 0.4662]


def create_plot(x, y_list, labels, title, ylabel, xlabel, filename):
    plt.figure(figsize=(10, 6))
    markers = ['o', 's', '^']
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c'] 
    
    for i, y in enumerate(y_list):
        plt.plot(x, y, marker=markers[i],color=colors[i],label=labels[i], linewidth=2, markersize=8)
    
    plt.title(f"{title}\n(System: Intel Core i5-1135G7)", fontsize=14)
    plt.xlabel(xlabel, fontsize=12)
    plt.ylabel(ylabel, fontsize=12)
    plt.legend(fontsize=10)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.savefig(filename)
    print(f"Generated: {filename}")
    plt.close()
if __name__ == "__main__":
    legend_labels = ['Two-Copy (A1)', 'One-Copy (A2)', 'Zero-Copy (A3)']

    create_plot(sizes, [tp_a1, tp_a2, tp_a3], legend_labels,
                "Throughput vs Message Size (Threads=1)", 
                "Throughput (Gbps)", "Message Size", 
                "MT25059_Plot_Throughput.png")

    create_plot(threads, [lat_a1, lat_a2, lat_a3], legend_labels,
                "Latency vs Thread Count (MsgSize=256KB)", 
                "Latency (us)", "Thread Count", 
                "MT25059_Plot_Latency.png")

    create_plot(sizes, [cm_a1, cm_a2, cm_a3], legend_labels,
                "LLC Cache Misses vs Message Size (Threads=1)", 
                "Cache Misses", "Message Size", 
                "MT25059_Plot_CacheMisses.png")

    create_plot(sizes, [cpb_a1, cpb_a2, cpb_a3], legend_labels,
                "CPU Cycles per Byte vs Message Size (Threads=1)", 
                "Cycles / Byte", "Message Size", 
                "MT25059_Plot_CyclesPerByte.png")