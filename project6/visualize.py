import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
plt.rcParams['axes.unicode_minus'] = False

# 读取结果
df = pd.read_csv('results/results.csv')

print("=== Test Results Summary ===")
print(df.to_string(index=False))
print()

# 创建图表目录
os.makedirs('results/plots', exist_ok=True)

# 1. 高度对比图 - 按 binWidth 分组
fig, axes = plt.subplots(2, 2, figsize=(16, 12))
fig.suptitle('NFDH vs FFDH 高度对比（不同 binWidth）', fontsize=16, fontweight='bold')

binWidths = sorted(df['binWidth'].unique())

for idx, binWidth in enumerate(binWidths):
    row = idx // 2
    col = idx % 2
    ax = axes[row, col]
    
    subset = df[df['binWidth'] == binWidth]
    
    x = np.arange(len(subset))
    width = 0.35
    
    bars1 = ax.bar(x - width/2, subset['NFDH_Height'], width, label='NFDH', alpha=0.8, color='#3498db')
    bars2 = ax.bar(x + width/2, subset['FFDH_Height'], width, label='FFDH', alpha=0.8, color='#e74c3c')
    
    ax.set_xlabel('矩形数量 (n)', fontsize=11)
    ax.set_ylabel('总高度', fontsize=11)
    ax.set_title(f'binWidth = {binWidth}', fontsize=12, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels(subset['n'], rotation=45)
    ax.legend()
    ax.grid(axis='y', alpha=0.3)

plt.tight_layout()
plt.savefig('results/plots/height_comparison.png', dpi=300, bbox_inches='tight')
print("✓ 生成图表: results/plots/height_comparison.png")

# 2. 时间对比图 - 按 binWidth 分组
fig, axes = plt.subplots(2, 2, figsize=(18, 14))  # 增大图表尺寸
fig.suptitle('NFDH vs FFDH 运行时间对比（不同 binWidth）', fontsize=16, fontweight='bold')

for idx, binWidth in enumerate(binWidths):
    row = idx // 2
    col = idx % 2
    ax = axes[row, col]
    
    subset = df[df['binWidth'] == binWidth].sort_values('n')  # 按n排序
    
    ax.plot(subset['n'], subset['NFDH_Time'], marker='o', label='NFDH', linewidth=2, markersize=8, color='#3498db')
    ax.plot(subset['n'], subset['FFDH_Time'], marker='s', label='FFDH', linewidth=2, markersize=8, color='#e74c3c')
    
    ax.set_xlabel('矩形数量 (n)', fontsize=11)
    ax.set_ylabel('运行时间 (ms)', fontsize=11)
    ax.set_title(f'binWidth = {binWidth}', fontsize=12, fontweight='bold')
    
    # 只对x轴使用对数坐标，y轴使用线性坐标（从0开始）
    ax.set_xscale('log')
    ax.set_ylim(bottom=0)  # y轴从0开始
    
    # 添加网格线
    ax.grid(True, alpha=0.3, which='both', axis='both')
    ax.minorticks_on()
    ax.grid(True, which='minor', alpha=0.15, linestyle=':', axis='x')  # 只在x轴添加次网格线
    
    ax.legend()

plt.tight_layout()
plt.savefig('results/plots/time_comparison.png', dpi=300, bbox_inches='tight')
print("✓ 生成图表: results/plots/time_comparison.png")

# 3. FFDH 相对于 NFDH 的改进百分比
df['Height_Improvement_%'] = ((df['NFDH_Height'] - df['FFDH_Height']) / df['NFDH_Height'] * 100).round(2)

fig, ax = plt.subplots(figsize=(14, 8))

for binWidth in binWidths:
    subset = df[df['binWidth'] == binWidth].sort_values('n')  # 按n排序
    ax.plot(subset['n'], subset['Height_Improvement_%'], marker='o', label=f'binWidth={binWidth}', linewidth=2, markersize=8)

ax.set_xlabel('矩形数量 (n)', fontsize=12)
ax.set_ylabel('FFDH 相对 NFDH 的改进 (%)', fontsize=12)
ax.set_title('FFDH 算法的高度优化效果', fontsize=14, fontweight='bold')
ax.set_xscale('log')
ax.axhline(y=0, color='gray', linestyle='--', alpha=0.5)
ax.legend()
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('results/plots/improvement.png', dpi=300, bbox_inches='tight')
print("✓ 生成图表: results/plots/improvement.png")

# 4. 性能统计表格
print("\n=== FFDH 相对于 NFDH 的改进统计 ===")
summary = df.groupby('binWidth').agg({
    'Height_Improvement_%': ['mean', 'min', 'max'],
    'NFDH_Time': 'mean',
    'FFDH_Time': 'mean'
}).round(3)
summary.columns = ['平均改进%', '最小改进%', '最大改进%', 'NFDH平均时间(ms)', 'FFDH平均时间(ms)']
print(summary)
print()

# 5. 生成 HTML 报告
html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Texture Packing 测试报告</title>
    <style>
        body {{
            font-family: 'Microsoft YaHei', Arial, sans-serif;
            margin: 20px;
            background-color: #f5f5f5;
        }}
        h1 {{
            color: #2c3e50;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
        }}
        h2 {{
            color: #34495e;
            margin-top: 30px;
        }}
        table {{
            border-collapse: collapse;
            width: 100%;
            background-color: white;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            margin: 20px 0;
        }}
        th, td {{
            border: 1px solid #ddd;
            padding: 12px;
            text-align: center;
        }}
        th {{
            background-color: #3498db;
            color: white;
            font-weight: bold;
        }}
        tr:nth-child(even) {{
            background-color: #f9f9f9;
        }}
        tr:hover {{
            background-color: #e8f4f8;
        }}
        .positive {{
            color: #27ae60;
            font-weight: bold;
        }}
        .negative {{
            color: #e74c3c;
            font-weight: bold;
        }}
        img {{
            max-width: 100%;
            height: auto;
            margin: 20px 0;
            border: 1px solid #ddd;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        }}
        .summary {{
            background-color: #ecf0f1;
            padding: 15px;
            border-radius: 5px;
            margin: 20px 0;
        }}
    </style>
</head>
<body>
    <h1>📊 Texture Packing 算法测试报告</h1>
    
    <div class="summary">
        <h3>测试概览</h3>
        <ul>
            <li>测试样例总数: {len(df)} 个</li>
            <li>binWidth 取值: {', '.join(map(str, binWidths))}</li>
            <li>矩形数量范围: {df['n'].min()} ~ {df['n'].max()}</li>
            <li>测试算法: NFDH (Next Fit Decreasing Height) vs FFDH (First Fit Decreasing Height)</li>
        </ul>
    </div>
    
    <h2>📈 1. 高度对比</h2>
    <img src="plots/height_comparison.png" alt="高度对比图">
    
    <h2>⏱️ 2. 运行时间对比</h2>
    <img src="plots/time_comparison.png" alt="时间对比图">
    
    <h2>🎯 3. FFDH 算法改进效果</h2>
    <img src="plots/improvement.png" alt="改进百分比图">
    
    <h2>📋 4. 详细测试数据</h2>
    {df.to_html(index=False, classes='dataframe', border=0)}
    
    <h2>📊 5. 统计摘要</h2>
    {summary.to_html(classes='dataframe', border=0)}
    
    <hr>
    <p style="text-align: center; color: #7f8c8d; margin-top: 30px;">
        生成时间: {pd.Timestamp.now().strftime('%Y-%m-%d %H:%M:%S')}
    </p>
</body>
</html>
"""

with open('results/report.html', 'w', encoding='utf-8') as f:
    f.write(html_content)

print("✓ 生成 HTML 报告: results/report.html")
print("\n=== 可视化完成! ===")
print("请查看以下文件:")
print("  - results/results.csv (原始数据)")
print("  - results/report.html (完整报告)")
print("  - results/plots/*.png (图表)")
