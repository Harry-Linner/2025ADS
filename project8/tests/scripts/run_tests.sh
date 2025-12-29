#!/bin/bash

# MapReduce 词频统计测试脚本
# 运行串行和并行算法，比较结果和性能

set -e  # 遇到错误时退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 项目根目录
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TEST_DIR="$PROJECT_ROOT/tests"

echo -e "${BLUE}=== MapReduce 词频统计测试框架 ===${NC}"
echo "项目根目录: $PROJECT_ROOT"
echo "测试目录: $TEST_DIR"

# 创建必要的目录
mkdir -p "$TEST_DIR/results/serial_results"
mkdir -p "$TEST_DIR/results/parallel_results"
mkdir -p "$TEST_DIR/expected"

# 编译程序
echo -e "\n${YELLOW}编译程序...${NC}"
cd "$PROJECT_ROOT"

# 编译串行程序
if [ -f "src/serial/serial.c" ]; then
    echo "编译串行程序..."
    gcc -o src/serial/serial src/serial/serial.c -std=c99
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}串行程序编译成功${NC}"
    else
        echo -e "${RED}串行程序编译失败${NC}"
        exit 1
    fi
else
    echo -e "${RED}找不到串行程序源文件${NC}"
    exit 1
fi

# 编译并行程序
if [ -f "src/parallel/mapper.c" ] && [ -f "src/parallel/reducer.c" ]; then
    echo "编译并行程序..."
    gcc -o src/parallel/mapper src/parallel/mapper.c -std=c99
    gcc -o src/parallel/reducer src/parallel/reducer.c -std=c99
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}并行程序编译成功${NC}"
    else
        echo -e "${RED}并行程序编译失败${NC}"
        exit 1
    fi
else
    echo -e "${RED}找不到并行程序源文件${NC}"
    exit 1
fi

# 生成测试数据
echo -e "\n${YELLOW}生成测试数据...${NC}"
cd "$TEST_DIR"
python3 scripts/generate_test_data.py

# 测试函数
run_serial_test() {
    local test_name=$1
    local input_file=$2
    local output_file=$3
    
    echo -e "\n${BLUE}运行串行测试: $test_name${NC}"
    echo "输入文件: $input_file"
    echo "输出文件: $output_file"
    
    # 记录开始时间
    start_time=$(date +%s.%N)
    
    # 运行串行程序
    "$PROJECT_ROOT/src/serial/serial" "$input_file" > "$output_file" 2> "${output_file}.log"
    
    # 记录结束时间
    end_time=$(date +%s.%N)
    runtime=$(echo "$end_time - $start_time" | bc -l)
    
    echo "运行时间: ${runtime}秒"
    echo "$runtime" > "${output_file}.time"
    
    # 显示统计信息
    if [ -f "${output_file}.log" ]; then
        echo "性能统计:"
        cat "${output_file}.log"
    fi
}

run_parallel_test() {
    local test_name=$1
    local input_file=$2
    local output_file=$3
    
    echo -e "\n${BLUE}运行并行测试: $test_name${NC}"
    echo "输入文件: $input_file"
    echo "输出文件: $output_file"
    
    # 记录开始时间
    start_time=$(date +%s.%N)
    
    # 模拟MapReduce流程
    # Map阶段：将输入传递给mapper
    cat "$input_file" | "$PROJECT_ROOT/src/parallel/mapper" | sort > "${output_file}.mapped"
    
    # Reduce阶段：将排序后的结果传递给reducer
    cat "${output_file}.mapped" | "$PROJECT_ROOT/src/parallel/reducer" > "$output_file"
    
    # 记录结束时间
    end_time=$(date +%s.%N)
    runtime=$(echo "$end_time - $start_time" | bc -l)
    
    echo "运行时间: ${runtime}秒"
    echo "$runtime" > "${output_file}.time"
    
    # 清理中间文件
    rm -f "${output_file}.mapped"
}

compare_results() {
    local test_name=$1
    local serial_result=$2
    local parallel_result=$3
    local expected_result=$4
    
    echo -e "\n${YELLOW}比较结果: $test_name${NC}"
    
    # 比较串行和并行结果
    if diff -q "$serial_result" "$parallel_result" > /dev/null; then
        echo -e "${GREEN}✓ 串行和并行结果一致${NC}"
    else
        echo -e "${RED}✗ 串行和并行结果不一致${NC}"
        echo "差异详情:"
        diff "$serial_result" "$parallel_result" | head -20
    fi
    
    # 如果有预期结果，进行比较
    if [ -f "$expected_result" ]; then
        if diff -q "$serial_result" "$expected_result" > /dev/null; then
            echo -e "${GREEN}✓ 结果与预期一致${NC}"
        else
            echo -e "${RED}✗ 结果与预期不一致${NC}"
            echo "差异详情:"
            diff "$serial_result" "$expected_result" | head -10
        fi
    fi
    
    # 显示性能比较
    if [ -f "${serial_result}.time" ] && [ -f "${parallel_result}.time" ]; then
        serial_time=$(cat "${serial_result}.time")
        parallel_time=$(cat "${parallel_result}.time")
        
        echo "性能比较:"
        echo "  串行时间: ${serial_time}秒"
        echo "  并行时间: ${parallel_time}秒"
        
        # 计算加速比
        if command -v bc > /dev/null; then
            speedup=$(echo "scale=2; $serial_time / $parallel_time" | bc -l)
            echo "  加速比: ${speedup}x"
        fi
    fi
}

# 运行所有测试
echo -e "\n${YELLOW}=== 开始测试 ===${NC}"

# 小规模测试
if [ -f "$TEST_DIR/data/small_test.txt" ]; then
    run_serial_test "小规模" "$TEST_DIR/data/small_test.txt" "$TEST_DIR/results/serial_results/small_result.txt"
    run_parallel_test "小规模" "$TEST_DIR/data/small_test.txt" "$TEST_DIR/results/parallel_results/small_result.txt"
    compare_results "小规模" "$TEST_DIR/results/serial_results/small_result.txt" "$TEST_DIR/results/parallel_results/small_result.txt" "$TEST_DIR/expected/small_expected.txt"
fi

# 中等规模测试
if [ -f "$TEST_DIR/data/medium_test.txt" ]; then
    run_serial_test "中等规模" "$TEST_DIR/data/medium_test.txt" "$TEST_DIR/results/serial_results/medium_result.txt"
    run_parallel_test "中等规模" "$TEST_DIR/data/medium_test.txt" "$TEST_DIR/results/parallel_results/medium_result.txt"
    compare_results "中等规模" "$TEST_DIR/results/serial_results/medium_result.txt" "$TEST_DIR/results/parallel_results/medium_result.txt" "$TEST_DIR/expected/medium_expected.txt"
fi

# 大规模测试
if [ -f "$TEST_DIR/data/large_test.txt" ]; then
    run_serial_test "大规模" "$TEST_DIR/data/large_test.txt" "$TEST_DIR/results/serial_results/large_result.txt"
    run_parallel_test "大规模" "$TEST_DIR/data/large_test.txt" "$TEST_DIR/results/parallel_results/large_result.txt"
    compare_results "大规模" "$TEST_DIR/results/serial_results/large_result.txt" "$TEST_DIR/results/parallel_results/large_result.txt" "$TEST_DIR/expected/large_expected.txt"
fi

# 使用现有的Frankenstein测试数据
if [ -f "$PROJECT_ROOT/testcase/Frankenstein.txt" ]; then
    echo -e "\n${BLUE}使用Frankenstein.txt进行测试${NC}"
    run_serial_test "Frankenstein" "$PROJECT_ROOT/testcase/Frankenstein.txt" "$TEST_DIR/results/serial_results/frankenstein_result.txt"
    run_parallel_test "Frankenstein" "$PROJECT_ROOT/testcase/Frankenstein.txt" "$TEST_DIR/results/parallel_results/frankenstein_result.txt"
    compare_results "Frankenstein" "$TEST_DIR/results/serial_results/frankenstein_result.txt" "$TEST_DIR/results/parallel_results/frankenstein_result.txt" ""
fi

echo -e "\n${GREEN}=== 测试完成 ===${NC}"
echo "结果文件保存在: $TEST_DIR/results/"
echo "运行性能测试: ./scripts/performance_test.sh"
echo "验证结果正确性: python3 scripts/validate_results.py"