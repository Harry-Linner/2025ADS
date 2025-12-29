#!/usr/bin/env python3
"""
生成性能测试图表
"""

import matplotlib.pyplot as plt
import numpy as np
import os
import glob

def read_stats(filename):
    """读取统计文件"""
    stats = {}
    if os.path.exists(filename):
        with open(filename, 'r') as f:
            for line in f:
                key, value = line.strip().split(': ')
                stats[key] = float(value)
    return stats

def generate_charts():
    """生成性能图表"""
    # 测试名称
    test_names = ['small', 'medium', 'large', 'frankenstein']
    test_labels = ['Small Scale', 'Medium Scale', 'Large Scale', 'Frankenstein']
    
    serial_times = []
    parallel_times = []
    speedups = []
    
    # 收集数据
    for test_name in test_names:
        serial_file = f'serial_{test_name}_stats.txt'
        parallel_file = f'parallel_{test_name}_stats.txt'
        
        serial_stats = read_stats(serial_file)
        parallel_stats = read_stats(parallel_file)
        
        if 'average' in serial_stats and 'average' in parallel_stats:
            serial_avg = serial_stats['average']
            parallel_avg = parallel_stats['average']
            
            serial_times.append(serial_avg)
            parallel_times.append(parallel_avg)
            speedups.append(serial_avg / parallel_avg if parallel_avg > 0 else 0)
        else:
            serial_times.append(0)
            parallel_times.append(0)
            speedups.append(0)
    
    # 过滤掉没有数据的测试
    valid_indices = [i for i, (s, p) in enumerate(zip(serial_times, parallel_times)) if s > 0 and p > 0]
    
    if not valid_indices:
        print("No valid performance data found")
        return
    
    valid_labels = [test_labels[i] for i in valid_indices]
    valid_serial = [serial_times[i] for i in valid_indices]
    valid_parallel = [parallel_times[i] for i in valid_indices]
    valid_speedups = [speedups[i] for i in valid_indices]
    
    # 创建图表
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # 运行时间比较图
    x = np.arange(len(valid_labels))
    width = 0.35
    
    ax1.bar(x - width/2, valid_serial, width, label='Serial Algorithm', color='skyblue')
    ax1.bar(x + width/2, valid_parallel, width, label='Parallel Algorithm', color='lightcoral')
    
    ax1.set_xlabel('Test Dataset')
    ax1.set_ylabel('Execution Time (seconds)')
    ax1.set_title('Serial vs Parallel Algorithm Performance Comparison')
    ax1.set_xticks(x)
    ax1.set_xticklabels(valid_labels)
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # 加速比图
    ax2.bar(valid_labels, valid_speedups, color='lightgreen')
    ax2.set_xlabel('Test Dataset')
    ax2.set_ylabel('Speedup Ratio')
    ax2.set_title('Parallel Algorithm Speedup')
    ax2.grid(True, alpha=0.3)
    
    # 在柱状图上显示数值
    for i, v in enumerate(valid_speedups):
        ax2.text(i, v + 0.01, f'{v:.2f}x', ha='center', va='bottom')
    
    plt.tight_layout()
    plt.savefig('performance_comparison.png', dpi=300, bbox_inches='tight')
    plt.savefig('performance_comparison.pdf', bbox_inches='tight')
    
    print("Performance charts generated:")
    print("- performance_comparison.png")
    print("- performance_comparison.pdf")

if __name__ == "__main__":
    generate_charts()
