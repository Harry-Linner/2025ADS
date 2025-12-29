#!/usr/bin/env python3
"""
MapReduce 结果验证脚本
验证串行和并行算法的结果正确性
"""

import os
import sys
from collections import Counter
import re

def clean_word(word):
    """
    清理单词，移除非字母字符并转换为小写
    与C程序中的clean_word函数保持一致
    """
    cleaned = ''.join(c.lower() for c in word if c.isalpha())
    return cleaned

def count_words_from_file(filename):
    """
    从文件中统计词频
    返回按要求排序的结果列表
    """
    word_count = Counter()
    
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            for line in f:
                words = line.strip().split()
                for word in words:
                    cleaned = clean_word(word)
                    if cleaned:  # 只统计非空单词
                        word_count[cleaned] += 1
    except FileNotFoundError:
        print(f"错误: 找不到文件 {filename}")
        return []
    except Exception as e:
        print(f"错误: 读取文件 {filename} 时出错: {e}")
        return []
    
    # 按要求排序：频率降序，相同频率时按字典序升序
    sorted_words = sorted(word_count.items(), key=lambda x: (-x[1], x[0]))
    return sorted_words

def parse_result_file(filename):
    """
    解析结果文件，返回单词和频率的列表
    """
    results = []
    
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            for line_num, line in enumerate(f, 1):
                line = line.strip()
                if not line:
                    continue
                
                # 跳过性能信息行
                if line.startswith('Starting') or line.startswith('---') or line.startswith('Total') or line.startswith('Time'):
                    continue
                
                # 解析 "word count" 格式
                parts = line.split()
                if len(parts) >= 2:
                    word = parts[0]
                    try:
                        count = int(parts[1])
                        results.append((word, count))
                    except ValueError:
                        print(f"警告: 第{line_num}行格式错误: {line}")
                else:
                    print(f"警告: 第{line_num}行格式错误: {line}")
    
    except FileNotFoundError:
        print(f"错误: 找不到结果文件 {filename}")
        return []
    except Exception as e:
        print(f"错误: 读取结果文件 {filename} 时出错: {e}")
        return []
    
    return results

def compare_results(expected, actual, test_name):
    """
    比较预期结果和实际结果
    """
    print(f"\n=== 验证 {test_name} ===")
    
    if not expected:
        print("错误: 没有预期结果")
        return False
    
    if not actual:
        print("错误: 没有实际结果")
        return False
    
    # 转换为字典便于比较
    expected_dict = dict(expected)
    actual_dict = dict(actual)
    
    # 检查单词数量
    if len(expected_dict) != len(actual_dict):
        print(f"错误: 单词数量不匹配")
        print(f"  预期: {len(expected_dict)} 个单词")
        print(f"  实际: {len(actual_dict)} 个单词")
        return False
    
    # 检查每个单词的频率
    errors = 0
    for word, expected_count in expected:
        if word not in actual_dict:
            print(f"错误: 缺少单词 '{word}'")
            errors += 1
        elif actual_dict[word] != expected_count:
            print(f"错误: 单词 '{word}' 频率不匹配")
            print(f"  预期: {expected_count}")
            print(f"  实际: {actual_dict[word]}")
            errors += 1
    
    # 检查额外的单词
    expected_words = set(expected_dict.keys())
    actual_words = set(actual_dict.keys())
    extra_words = actual_words - expected_words
    
    if extra_words:
        print(f"错误: 发现额外单词: {list(extra_words)[:10]}...")  # 只显示前10个
        errors += len(extra_words)
    
    # 检查排序
    if expected != actual:
        print("错误: 排序不正确")
        print("前10个结果比较:")
        print("预期 -> 实际")
        for i in range(min(10, len(expected), len(actual))):
            exp_word, exp_count = expected[i]
            act_word, act_count = actual[i] if i < len(actual) else ("N/A", 0)
            if (exp_word, exp_count) != (act_word, act_count):
                print(f"  {exp_word} {exp_count} -> {act_word} {act_count} ❌")
            else:
                print(f"  {exp_word} {exp_count} -> {act_word} {act_count} ✓")
        errors += 1
    
    if errors == 0:
        print("✓ 结果验证通过")
        return True
    else:
        print(f"✗ 发现 {errors} 个错误")
        return False

def validate_test_case(test_name, input_file, serial_result, parallel_result, expected_result=None):
    """
    验证单个测试用例
    """
    print(f"\n{'='*50}")
    print(f"验证测试用例: {test_name}")
    print(f"输入文件: {input_file}")
    print(f"串行结果: {serial_result}")
    print(f"并行结果: {parallel_result}")
    if expected_result:
        print(f"预期结果: {expected_result}")
    
    # 如果没有预期结果，从输入文件生成
    if not expected_result or not os.path.exists(expected_result):
        print("从输入文件生成预期结果...")
        expected = count_words_from_file(input_file)
    else:
        expected = parse_result_file(expected_result)
    
    # 解析实际结果
    serial_actual = parse_result_file(serial_result)
    parallel_actual = parse_result_file(parallel_result)
    
    # 验证结果
    serial_valid = compare_results(expected, serial_actual, f"{test_name} - 串行")
    parallel_valid = compare_results(expected, parallel_actual, f"{test_name} - 并行")
    
    # 比较串行和并行结果
    print(f"\n=== 比较串行和并行结果 ===")
    if serial_actual == parallel_actual:
        print("✓ 串行和并行结果一致")
        consistency_valid = True
    else:
        print("✗ 串行和并行结果不一致")
        consistency_valid = False
        
        # 显示差异
        print("差异详情 (前10个):")
        max_len = max(len(serial_actual), len(parallel_actual))
        for i in range(min(10, max_len)):
            s_item = serial_actual[i] if i < len(serial_actual) else ("N/A", 0)
            p_item = parallel_actual[i] if i < len(parallel_actual) else ("N/A", 0)
            
            if s_item != p_item:
                print(f"  位置 {i+1}: 串行={s_item} vs 并行={p_item}")
    
    return serial_valid and parallel_valid and consistency_valid

def main():
    """主函数"""
    # 获取项目路径
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(os.path.dirname(script_dir))
    test_dir = os.path.join(project_root, 'tests')
    
    print("MapReduce 结果验证工具")
    print(f"项目根目录: {project_root}")
    print(f"测试目录: {test_dir}")
    
    # 测试用例列表
    test_cases = [
        {
            'name': '小规模测试',
            'input': os.path.join(test_dir, 'data', 'small_test.txt'),
            'serial': os.path.join(test_dir, 'results', 'serial_results', 'small_result.txt'),
            'parallel': os.path.join(test_dir, 'results', 'parallel_results', 'small_result.txt'),
            'expected': os.path.join(test_dir, 'expected', 'small_expected.txt')
        },
        {
            'name': '中等规模测试',
            'input': os.path.join(test_dir, 'data', 'medium_test.txt'),
            'serial': os.path.join(test_dir, 'results', 'serial_results', 'medium_result.txt'),
            'parallel': os.path.join(test_dir, 'results', 'parallel_results', 'medium_result.txt'),
            'expected': os.path.join(test_dir, 'expected', 'medium_expected.txt')
        },
        {
            'name': '大规模测试',
            'input': os.path.join(test_dir, 'data', 'large_test.txt'),
            'serial': os.path.join(test_dir, 'results', 'serial_results', 'large_result.txt'),
            'parallel': os.path.join(test_dir, 'results', 'parallel_results', 'large_result.txt'),
            'expected': os.path.join(test_dir, 'expected', 'large_expected.txt')
        },
        {
            'name': 'Frankenstein测试',
            'input': os.path.join(project_root, 'testcase', 'Frankenstein.txt'),
            'serial': os.path.join(test_dir, 'results', 'serial_results', 'frankenstein_result.txt'),
            'parallel': os.path.join(test_dir, 'results', 'parallel_results', 'frankenstein_result.txt'),
            'expected': None
        }
    ]
    
    # 运行验证
    all_passed = True
    
    for test_case in test_cases:
        # 检查必要文件是否存在
        if not os.path.exists(test_case['input']):
            print(f"\n跳过 {test_case['name']}: 输入文件不存在")
            continue
        
        if not os.path.exists(test_case['serial']) or not os.path.exists(test_case['parallel']):
            print(f"\n跳过 {test_case['name']}: 结果文件不存在")
            print("请先运行测试: ./scripts/run_tests.sh")
            continue
        
        # 验证测试用例
        passed = validate_test_case(
            test_case['name'],
            test_case['input'],
            test_case['serial'],
            test_case['parallel'],
            test_case['expected']
        )
        
        if not passed:
            all_passed = False
    
    # 总结
    print(f"\n{'='*50}")
    if all_passed:
        print("🎉 所有测试验证通过！")
        sys.exit(0)
    else:
        print("❌ 部分测试验证失败")
        sys.exit(1)

if __name__ == "__main__":
    main()