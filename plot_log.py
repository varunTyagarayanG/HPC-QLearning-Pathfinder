import matplotlib.pyplot as plt
import csv

# Read log_time.csv: Iteration, IterationTime_ms, CumulativeTime_ms
iterations_time = []
iter_times = []
cumulative_times = []

with open('LOGS\log_time.csv', 'r') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        iterations_time.append(int(row['Iteration']))
        iter_times.append(float(row['IterationTime_ms']))
        cumulative_times.append(float(row['CumulativeTime_ms']))

# Read log_mem.csv: Iteration, MemoryUsage_KB
iterations_mem = []
mem_usages = []

with open('LOGS\log_mem.csv', 'r') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        iterations_mem.append(int(row['Iteration']))
        mem_usages.append(float(row['MemoryUsage_KB']))

# -------------------------------
# Plot 1: Cumulative Time vs Iteration
# -------------------------------
plt.figure(figsize=(10, 6))
plt.plot(iterations_time, cumulative_times, marker='o', linestyle='-', color='blue')
plt.title('Cumulative Training Time vs. Iteration')
plt.xlabel('Iteration')
plt.ylabel('Cumulative Time (ms)')
plt.grid(True)
plt.tight_layout()
plt.savefig('cumulative_time_graph.png')
plt.show()

# -------------------------------
# Plot 2: Memory Usage vs Iteration
# -------------------------------
plt.figure(figsize=(10, 6))
plt.plot(iterations_mem, mem_usages, marker='o', linestyle='-', color='red')
plt.title('Memory Usage vs. Iteration')
plt.xlabel('Iteration')
plt.ylabel('Memory Usage (KB)')
plt.grid(True)
plt.tight_layout()
plt.savefig('memory_usage_graph.png')
plt.show()
