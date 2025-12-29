# MapReduce 测试框架

本目录包含了MapReduce词频统计项目的完整测试框架。

## 目录结构

```
tests/
├── data/                    # 测试数据目录
│   ├── small_test.txt      # 小规模测试数据
│   ├── medium_test.txt     # 中等规模测试数据
│   └── large_test.txt      # 大规模测试数据 (100k+ words)
├── scripts/                # 测试脚本
│   ├── run_tests.sh        # 主测试脚本
│   ├── performance_test.sh # 性能测试脚本
│   └── validate_results.py # 结果验证脚本
├── expected/               # 预期结果
│   └── small_expected.txt  # 小测试的预期结果
└── results/                # 测试结果输出
    ├── serial_results/     # 串行算法结果
    └── parallel_results/   # 并行算法结果
```

## 使用方法

1. 运行所有测试：`./scripts/run_tests.sh`
2. 运行性能测试：`./scripts/performance_test.sh`
3. 验证结果正确性：`python scripts/validate_results.py`

## 测试数据说明

- `small_test.txt`: 约1000个单词，用于快速功能验证
- `medium_test.txt`: 约10000个单词，用于中等规模测试
- `large_test.txt`: 超过100000个单词，符合作业要求的最小规模