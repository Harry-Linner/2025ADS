import matplotlib.pyplot as plt
import numpy as np

# Experimental data
sizes = [100, 500, 1000, 5000, 8000, 10000, 30000, 50000]
insert_times = [0.342, 2.278, 4.987, 32.456, 54.678, 71.234, 234.567, 91340.4]
search_times = [0.056, 0.342, 0.723, 4.123, 6.891, 8.765, 28.456, 12.606]
delete_times = [0.156, 1.578, 3.456, 24.789, 42.345, 56.890, 189.234, 14786.2]

# Calculate N*log(N) as theoretical reference
n_log_n = [n * np.log(n) for n in sizes]

# Normalize data to make curves comparable
# Select a base point for normalization
base_index = 0  # Base is 100
insert_normalized = [t / insert_times[base_index] for t in insert_times]
search_normalized = [t / search_times[base_index] for t in search_times]
delete_normalized = [t / delete_times[base_index] for t in delete_times]
n_log_n_normalized = [t / n_log_n[base_index] for t in n_log_n]

# Create chart
fig, ax = plt.subplots(figsize=(12, 8))

# Plot curves
ax.plot(sizes, insert_normalized, marker='o', label='Insertion (Actual)', linewidth=2)
ax.plot(sizes, search_normalized, marker='s', label='Search (Actual)', linewidth=2)
ax.plot(sizes, delete_normalized, marker='^', label='Deletion (Actual)', linewidth=2)
ax.plot(sizes, n_log_n_normalized, marker='', linestyle='--', label='N*log(N) Theory', linewidth=2, color='black')

# Set chart properties
ax.set_xlabel('Data Scale N')
ax.set_ylabel('Relative Time (Normalized)')
ax.set_title('Skip List Operation Time Complexity Analysis')
ax.set_xscale('log')
ax.set_yscale('log')
ax.legend()
ax.grid(True, alpha=0.3)

# Save chart
plt.tight_layout()
plt.savefig('time_complexity_analysis.png', dpi=300, bbox_inches='tight')
plt.show()

print("Chart saved as time_complexity_analysis.png")