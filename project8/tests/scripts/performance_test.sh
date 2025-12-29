#!/bin/bash

# MapReduce 性能测试脚本
# 多次运行测试，收集性能数据，生成性能报告

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# 配置
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TEST_DIR="$PROJECT_ROOT/tests"
PERFORMANCE_DIR="$TEST_DIR/performance"
NUM_RUNS=5  # 每个测试运行的次数

echo -e "${BLUE}=== MapReduce 性能测试 ===${NC}"

# 创建性能测试目录
mkdir -p "$PERFORMANCE_DIR"

# 性能测试函数
run_performance_test() {
    local algorithm=$1  # "serial" 或 "parallel"
    local test_name=$2
    local input_file=$3
    local output_prefix="$PERFORMANCE_DIR/${algorithm}_${test_name}"
    
    echo -e "\n${YELLOW}性能测试: $algorithm - $test_name${NC}"
    echo "输入文件: $input_file"
    echo "运行次数: $NUM_RUNS"
    
    # 存储运行时间的文件
    times_file="${output_prefix}_times.txt"
    > "$times_file"  # 清空文件
    
    for i in $(seq 1 $NUM_RUNS); do
        echo -n "运行 $i/$NUM_RUNS... "
        
        start_time=$(date +%s.%N)
        
        if [ "$algorithm" = "serial" ]; then
            "$PROJECT_ROOT/src/serial/serial" "$input_file" > "${output_prefix}_run${i}.txt" 2>/dev/null
        else
            # 并行算法
            cat "$input_file" | "$PROJECT_ROOT/src/parallel/mapper" | sort | "$PROJECT_ROOT/src/parallel/reducer" > "${output_prefix}_run${i}.txt"
        fi
        
        end_time=$(date +%s.%N)
        runtime=$(echo "$end_time - $start_time" | bc -l)
        
        echo "$runtime" >> "$times_file"
        echo "${runtime}秒"
    done
    
    # 计算统计信息
    if command -v python3 > /dev/null; then
        python3 -c "
import sys
times = []
with open('$times_file', 'r') as f:
    for line in f:
        times.append(float(line.strip()))

if times:
    avg = sum(times) / len(times)
    min_time = min(times)
    max_time = max(times)
    
    # 计算标准差
    variance = sum((t - avg) ** 2 for t in times) / len(times)
    std_dev = variance ** 0.5
    
    print(f'平均时间: {avg:.4f}秒')
    print(f'最小时间: {min_time:.4f}秒')
    print(f'最大时间: {max_time:.4f}秒')
    print(f'标准差: {std_dev:.4f}秒')
    
    # 保存统计信息
    with open('${output_prefix}_stats.txt', 'w') as f:
        f.write(f'average: {avg:.4f}\\n')
        f.write(f'minimum: {min_time:.4f}\\n')
        f.write(f'maximum: {max_time:.4f}\\n')
        f.write(f'std_dev: {std_dev:.4f}\\n')
        f.write(f'runs: {len(times)}\\n')
"
    fi
    
    # 清理单次运行的结果文件
    rm -f "${output_prefix}_run"*.txt
}

# 生成性能报告
generate_performance_report() {
    local report_file="$PERFORMANCE_DIR/performance_report.txt"
    
    echo -e "\n${YELLOW}生成性能报告...${NC}"
    
    cat > "$report_file" << EOF
MapReduce 词频统计性能测试报告
================================
测试时间: $(date)
测试次数: $NUM_RUNS 次
测试环境: $(uname -a)

EOF

    # 处理每个测试的结果
    for test_name in small medium large frankenstein; do
        echo "=== $test_name 测试结果 ===" >> "$report_file"
        
        serial_stats="$PERFORMANCE_DIR/serial_${test_name}_stats.txt"
        parallel_stats="$PERFORMANCE_DIR/parallel_${test_name}_stats.txt"
        
        if [ -f "$serial_stats" ] && [ -f "$parallel_stats" ]; then
            echo "" >> "$report_file"
            echo "串行算法:" >> "$report_file"
            cat "$serial_stats" >> "$report_file"
            
            echo "" >> "$report_file"
            echo "并行算法:" >> "$report_file"
            cat "$parallel_stats" >> "$report_file"
            
            # 计算加速比
            serial_avg=$(grep "average:" "$serial_stats" | cut -d' ' -f2)
            parallel_avg=$(grep "average:" "$parallel_stats" | cut -d' ' -f2)
            
            if command -v bc > /dev/null && [ -n "$serial_avg" ] && [ -n "$parallel_avg" ]; then
                speedup=$(echo "scale=4; $serial_avg / $parallel_avg" | bc -l)
                echo "" >> "$report_file"
                echo "加速比: ${speedup}x" >> "$report_file"
            fi
        fi
        
        echo "" >> "$report_file"
        echo "----------------------------------------" >> "$report_file"
    done
    
    echo "性能报告已保存到: $report_file"
}

# 生成性能图表的Python脚本
generate_performance_charts() {
    cat > "$PERFORMANCE_DIR/generate_charts.py" << 'EOF'
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
EOF

    echo -e "\n${YELLOW}生成性能图表...${NC}"
    cd "$PERFORMANCE_DIR"
    
    if command -v python3 > /dev/null; then
        # 尝试安装matplotlib（如果没有的话）
        python3 -c "import matplotlib" 2>/dev/null || {
            echo "matplotlib未安装，尝试安装..."
            pip3 install matplotlib 2>/dev/null || echo "请手动安装matplotlib: pip3 install matplotlib"
        }
        
        python3 generate_charts.py 2>/dev/null || echo "生成图表失败，请确保安装了matplotlib"
    else
        echo "Python3未安装，跳过图表生成"
    fi
}

# 主测试流程
echo "开始性能测试..."

# 确保程序已编译
cd "$PROJECT_ROOT"
if [ ! -f "src/serial/serial" ]; then
    echo "编译串行程序..."
    gcc -o src/serial/serial src/serial/serial.c -std=c99
fi

if [ ! -f "src/parallel/mapper" ] || [ ! -f "src/parallel/reducer" ]; then
    echo "编译并行程序..."
    gcc -o src/parallel/mapper src/parallel/mapper.c -std=c99
    gcc -o src/parallel/reducer src/parallel/reducer.c -std=c99
fi

# 确保测试数据存在
if [ ! -f "$TEST_DIR/data/large_test.txt" ]; then
    echo "生成测试数据..."
    cd "$TEST_DIR"
    python3 scripts/generate_test_data.py
fi

# 运行性能测试
test_files=(
    "small:$TEST_DIR/data/small_test.txt"
    "medium:$TEST_DIR/data/medium_test.txt"
    "large:$TEST_DIR/data/large_test.txt"
    "frankenstein:$PROJECT_ROOT/testcase/Frankenstein.txt"
)

for test_spec in "${test_files[@]}"; do
    IFS=':' read -r test_name test_file <<< "$test_spec"
    
    if [ -f "$test_file" ]; then
        run_performance_test "serial" "$test_name" "$test_file"
        run_performance_test "parallel" "$test_name" "$test_file"
    else
        echo -e "${YELLOW}跳过测试 $test_name (文件不存在: $test_file)${NC}"
    fi
done

# 生成报告和图表
generate_performance_report
generate_performance_charts

echo -e "\n${GREEN}=== 性能测试完成 ===${NC}"
echo "结果保存在: $PERFORMANCE_DIR/"
echo "查看性能报告: cat $PERFORMANCE_DIR/performance_report.txt"