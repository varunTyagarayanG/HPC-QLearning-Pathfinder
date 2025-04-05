import matplotlib.pyplot as plt
import numpy as np

# Hypothetical training times in milliseconds (dummy values)
methods = ['Sequential', 'OpenMP', 'CUDA']
training_time = [500, 300, 100]  # Example: Sequential takes 500ms, OpenMP 300ms, CUDA 100ms

# Calculate speedup relative to sequential
speedup = [training_time[0]/t for t in training_time]

plt.figure(figsize=(8,6))
bars = plt.bar(methods, training_time, color=['blue', 'orange', 'green'])

plt.title('Hypothetical Training Time Comparison')
plt.xlabel('Method')
plt.ylabel('Training Time (ms)')

# Annotate bars with their training times and speedup factors
for bar, t, s in zip(bars, training_time, speedup):
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2.0, height + 10, f"{t}ms\n({s:.1f}x speedup)",
            ha='center', va='bottom', fontsize=10)

plt.ylim(0, max(training_time)*1.2)
plt.tight_layout()
plt.show()
