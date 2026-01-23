import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_scaling():
    if not os.path.exists('MT25059_Part_D_CSV.csv'):
        print("Error: MT25059_Part_D_CSV.csv not found.")
        return

    try:
        df=pd.read_csv('MT25059_Part_D_CSV.csv')
        df.columns=df.columns.str.strip()
    except Exception as e:
        print(f"Error while reading the CSV file: {e}")
        return

    if 'Program' not in df.columns:
        print("Error: Column 'Program' not found in CSV. Found:",df.columns)
        return

    plt.figure(figsize=(10, 6))
    proc_df = df[df['Program'] == 'program_a']
    if not proc_df.empty:
        for worker in proc_df['Worker'].unique():
            subset = proc_df[proc_df['Worker'] == worker]
            subset = subset.sort_values(by='Count')
            plt.plot(subset['Count'], subset['Duration_Sec'], marker='o', label=f'{worker.upper()}')
        plt.title('Program A: Multi-Process Scaling (Linear Growth)')
        plt.xlabel('No.of Processes')
        plt.ylabel('Time(Seconds)')
        plt.legend(title="Worker Type")
        plt.grid(True)
        plt.savefig('MT25059_Part_D_plot_process_scaling.png')
        print("Generated MT25059_Part_D_plot_process_scaling.png which contains the plot.")

    plt.figure(figsize=(10,6))
    thread_df = df[df['Program'] == 'program_b']
    if not thread_df.empty:
        for worker in thread_df['Worker'].unique():
            subset = thread_df[thread_df['Worker'] == worker]
            subset = subset.sort_values(by='Count')
            plt.plot(subset['Count'], subset['Duration_Sec'], marker='s',linestyle='--', label=f'{worker.upper()}')
        plt.title('Program B: Multi-Thread Scaling')
        plt.xlabel('Number of Threads')
        plt.ylabel('Time (Seconds)')
        plt.legend(title="Worker Type")
        plt.grid(True)
        plt.savefig('MT25059_Part_D_plot_thread_scaling.png')
        print("Generated MT25059_Part_D_plot_thread_scaling.png")

if __name__ == "__main__":
    plot_scaling()