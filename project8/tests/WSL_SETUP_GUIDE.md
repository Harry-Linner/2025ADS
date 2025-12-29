# WSL环境下运行MapReduce测试指南

## 1. 准备WSL环境

### 启动WSL
```bash
# 在Windows命令行或PowerShell中启动WSL
wsl
```

### 安装必要的工具
```bash
# 更新包管理器
sudo apt update

# 安装编译工具
sudo apt install build-essential gcc

# 安装Python3和pip（如果没有的话）
sudo apt install python3 python3-pip

# 安装bc计算器（用于性能测试中的数学计算）
sudo apt install bc

# 安装matplotlib（用于生成性能图表）
pip3 install matplotlib
```

## 2. 将项目文件复制到WSL

有几种方法可以访问Windows文件：

### 方法1：直接访问Windows文件系统
```bash
# Windows的C盘在WSL中的路径是 /mnt/c/
# 假设你的项目在 C:\Users\YourName\MapReduce\
cd /mnt/c/Users/YourName/MapReduce/

# 或者创建软链接方便访问
ln -s /mnt/c/Users/YourName/MapReduce ~/mapreduce-project
cd ~/mapreduce-project
```

### 方法2：复制到WSL文件系统（推荐，性能更好）
```bash
# 复制整个项目到WSL的home目录
cp -r /mnt/c/Users/YourName/MapReduce ~/mapreduce-project
cd ~/mapreduce-project
```

## 3. 设置执行权限

```bash
# 进入项目目录
cd ~/mapreduce-project

# 给脚本文件添加执行权限
chmod +x tests/scripts/*.sh
chmod +x tests/scripts/*.py
```

## 4. 运行测试

### 运行完整测试套件
```bash
cd tests
./scripts/run_tests.sh
```

### 运行性能测试
```bash
cd tests
./scripts/performance_test.sh
```

### 验证结果正确性
```bash
cd tests
python3 scripts/validate_results.py
```

### 生成测试数据
```bash
cd tests
python3 scripts/generate_test_data.py
```

### 收集系统信息
```bash
cd tests
python3 scripts/system_info.py --save
```

## 5. 查看结果

### 查看测试结果
```bash
# 查看串行结果
ls tests/results/serial_results/
cat tests/results/serial_results/small_result.txt

# 查看并行结果
ls tests/results/parallel_results/
cat tests/results/parallel_results/small_result.txt

# 查看性能报告
cat tests/performance/performance_report.txt
```

### 查看性能图表
```bash
# 如果生成了图表文件
ls tests/performance/*.png
ls tests/performance/*.pdf

# 可以通过Windows文件管理器查看图片
# 路径：\\wsl$\Ubuntu\home\yourusername\mapreduce-project\tests\performance\
```

## 6. 可能遇到的问题和解决方案

### 问题1：权限问题
```bash
# 如果遇到权限问题，确保文件有执行权限
chmod +x tests/scripts/run_tests.sh
chmod +x tests/scripts/performance_test.sh
```

### 问题2：Python模块缺失
```bash
# 安装缺失的Python模块
pip3 install matplotlib numpy
```

### 问题3：编译错误
```bash
# 确保安装了gcc
gcc --version

# 如果没有安装
sudo apt install gcc
```

### 问题4：bc命令不存在
```bash
# 安装bc计算器
sudo apt install bc
```

## 7. 性能优化建议

1. **使用WSL文件系统**：将项目复制到WSL的文件系统中（如~/mapreduce-project）而不是直接在/mnt/c/下运行，这样性能会更好。

2. **增加测试数据规模**：可以修改`generate_test_data.py`中的参数来生成更大规模的测试数据。

3. **多次运行测试**：性能测试脚本会自动运行多次并计算平均值，确保结果的可靠性。

## 8. 文件路径说明

在WSL中：
- Windows的C盘：`/mnt/c/`
- WSL的home目录：`~/` 或 `/home/yourusername/`
- 项目目录（推荐）：`~/mapreduce-project/`

## 9. 快速开始命令

```bash
# 一键运行所有测试（在项目根目录下）
cd tests && ./scripts/run_tests.sh && ./scripts/performance_test.sh && python3 scripts/validate_results.py
```

这样你就可以在WSL环境下完整地运行MapReduce测试了！