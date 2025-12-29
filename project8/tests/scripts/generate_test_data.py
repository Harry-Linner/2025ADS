#!/usr/bin/env python3
"""
测试数据生成器
生成不同规模的测试数据用于MapReduce词频统计测试
"""

import os
import random
import string
from collections import Counter

def generate_words(num_words, vocabulary_size=1000):
    """
    生成指定数量的单词
    
    Args:
        num_words: 要生成的单词总数
        vocabulary_size: 词汇表大小（不同单词的数量）
    
    Returns:
        list: 生成的单词列表
    """
    # 创建基础词汇表
    base_words = [
        'the', 'and', 'of', 'to', 'a', 'in', 'is', 'it', 'you', 'that',
        'he', 'was', 'for', 'on', 'are', 'as', 'with', 'his', 'they', 'i',
        'at', 'be', 'this', 'have', 'from', 'or', 'one', 'had', 'by', 'word',
        'but', 'not', 'what', 'all', 'were', 'we', 'when', 'your', 'can', 'said',
        'there', 'each', 'which', 'she', 'do', 'how', 'their', 'if', 'will', 'up',
        'other', 'about', 'out', 'many', 'then', 'them', 'these', 'so', 'some', 'her',
        'would', 'make', 'like', 'into', 'him', 'time', 'has', 'two', 'more', 'very',
        'after', 'words', 'first', 'where', 'much', 'through', 'back', 'years', 'work', 'way',
        'even', 'new', 'want', 'because', 'any', 'these', 'give', 'day', 'most', 'us',
        'computer', 'data', 'algorithm', 'programming', 'software', 'system', 'network', 'database',
        'application', 'development', 'technology', 'internet', 'web', 'server', 'client', 'user',
        'interface', 'design', 'analysis', 'implementation', 'testing', 'debugging', 'optimization',
        'performance', 'scalability', 'security', 'framework', 'library', 'function', 'method',
        'class', 'object', 'variable', 'parameter', 'return', 'loop', 'condition', 'array',
        'string', 'integer', 'boolean', 'file', 'directory', 'path', 'configuration', 'deployment'
    ]
    
    # 如果需要更多词汇，生成随机单词
    while len(base_words) < vocabulary_size:
        word_length = random.randint(3, 10)
        word = ''.join(random.choices(string.ascii_lowercase, k=word_length))
        if word not in base_words:
            base_words.append(word)
    
    # 使用Zipf分布生成单词频率（符合自然语言规律）
    words = []
    weights = [1.0 / (i + 1) for i in range(len(base_words))]
    
    for _ in range(num_words):
        word = random.choices(base_words, weights=weights)[0]
        words.append(word)
    
    return words

def write_test_file(filename, words, words_per_line=10):
    """
    将单词列表写入文件
    
    Args:
        filename: 输出文件名
        words: 单词列表
        words_per_line: 每行的单词数
    """
    with open(filename, 'w', encoding='utf-8') as f:
        for i in range(0, len(words), words_per_line):
            line_words = words[i:i + words_per_line]
            f.write(' '.join(line_words) + '\n')

def generate_expected_output(words, output_file):
    """
    生成预期的输出结果（按要求排序）
    
    Args:
        words: 单词列表
        output_file: 输出文件名
    """
    # 统计词频
    word_count = Counter(words)
    
    # 按要求排序：频率降序，相同频率时按字典序升序
    sorted_words = sorted(word_count.items(), key=lambda x: (-x[1], x[0]))
    
    # 写入文件
    with open(output_file, 'w', encoding='utf-8') as f:
        for word, count in sorted_words:
            f.write(f"{word} {count}\n")

def main():
    """主函数：生成所有测试数据"""
    print("正在生成测试数据...")
    
    # 获取脚本目录和项目根目录
    script_dir = os.path.dirname(os.path.abspath(__file__))
    test_dir = os.path.dirname(script_dir)
    
    # 生成中等规模测试数据 (约10,000个单词)
    print("生成中等规模测试数据...")
    medium_words = generate_words(10000, 500)
    write_test_file(os.path.join(test_dir, 'data', 'medium_test.txt'), medium_words)
    generate_expected_output(medium_words, os.path.join(test_dir, 'expected', 'medium_expected.txt'))
    
    # 生成大规模测试数据 (超过100,000个单词)
    print("生成大规模测试数据...")
    large_words = generate_words(120000, 2000)
    write_test_file(os.path.join(test_dir, 'data', 'large_test.txt'), large_words)
    generate_expected_output(large_words, os.path.join(test_dir, 'expected', 'large_expected.txt'))
    
    # 为小测试数据生成预期结果
    print("生成小测试数据的预期结果...")
    small_test_file = os.path.join(test_dir, 'data', 'small_test.txt')
    with open(small_test_file, 'r', encoding='utf-8') as f:
        content = f.read()
    small_words = content.lower().split()
    # 清理单词（移除标点符号）
    cleaned_words = []
    for word in small_words:
        cleaned = ''.join(c for c in word if c.isalpha())
        if cleaned:
            cleaned_words.append(cleaned)
    
    generate_expected_output(cleaned_words, os.path.join(test_dir, 'expected', 'small_expected.txt'))
    
    print("测试数据生成完成！")
    print(f"- 小规模测试: {len(cleaned_words)} 个单词")
    print(f"- 中等规模测试: {len(medium_words)} 个单词")
    print(f"- 大规模测试: {len(large_words)} 个单词")

if __name__ == "__main__":
    main()