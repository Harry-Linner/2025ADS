import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Set English font
plt.rcParams['font.sans-serif'] = ['Arial', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

# 读取结果
df = pd.read_csv('results/results.csv')

print("=== Test Results Summary ===")
print(df.to_string(index=False))
print()

# Create plots directory
os.makedirs('results/plots', exist_ok=True)

# 1. Height comparison - Grouped by binWidth (only 3 plots)
fig, axes = plt.subplots(2, 2, figsize=(16, 12))
fig.suptitle('NFDH vs FFDH Height Comparison (Different binWidth)', fontsize=16, fontweight='bold')

binWidths = sorted(df['binWidth'].unique())

# Only use the first 3 binWidth values
for idx, binWidth in enumerate(binWidths[:3]):
    row = idx // 2
    col = idx % 2
    ax = axes[row, col]
    
    subset = df[df['binWidth'] == binWidth]
    
    x = np.arange(len(subset))
    width = 0.35
    
    bars1 = ax.bar(x - width/2, subset['NFDH_Height'], width, label='NFDH', alpha=0.8, color='#3498db')
    bars2 = ax.bar(x + width/2, subset['FFDH_Height'], width, label='FFDH', alpha=0.8, color='#e74c3c')
    
    ax.set_xlabel('Number of Rectangles (n)', fontsize=11)
    ax.set_ylabel('Total Height', fontsize=11)
    ax.set_title(f'binWidth = {binWidth}', fontsize=12, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels(subset['n'], rotation=45)
    ax.legend()
    ax.grid(axis='y', alpha=0.3)

# Hide the 4th subplot (bottom right)
axes[1, 1].set_visible(False)

plt.tight_layout()
plt.savefig('results/plots/height_comparison.png', dpi=300, bbox_inches='tight')
print("✓ Generated plot: results/plots/height_comparison.png")

# 2. Time comparison - Grouped by binWidth (only 3 plots)
fig, axes = plt.subplots(2, 2, figsize=(18, 14))  # Increase plot size
fig.suptitle('NFDH vs FFDH Running Time Comparison (Different binWidth)', fontsize=16, fontweight='bold')

# Only use the first 3 binWidth values
for idx, binWidth in enumerate(binWidths[:3]):
    row = idx // 2
    col = idx % 2
    ax = axes[row, col]
    
    subset = df[df['binWidth'] == binWidth].sort_values('n')  # Sort by n
    
    ax.plot(subset['n'], subset['NFDH_Time'], marker='o', label='NFDH', linewidth=2, markersize=8, color='#3498db')
    ax.plot(subset['n'], subset['FFDH_Time'], marker='s', label='FFDH', linewidth=2, markersize=8, color='#e74c3c')
    
    ax.set_xlabel('Number of Rectangles (n)', fontsize=11)
    ax.set_ylabel('Running Time (ms)', fontsize=11)
    ax.set_title(f'binWidth = {binWidth}', fontsize=12, fontweight='bold')
    
    # Use log scale for x-axis only, linear scale for y-axis (starting from 0)
    ax.set_xscale('log')
    ax.set_ylim(bottom=0)  # y-axis starts from 0
    
    # Add grid lines
    ax.grid(True, alpha=0.3, which='both', axis='both')
    ax.minorticks_on()
    ax.grid(True, which='minor', alpha=0.15, linestyle=':', axis='x')  # Only add minor grid lines on x-axis
    
    ax.legend()

# Hide the 4th subplot (bottom right)
axes[1, 1].set_visible(False)

plt.tight_layout()
plt.savefig('results/plots/time_comparison.png', dpi=300, bbox_inches='tight')
print("✓ Generated plot: results/plots/time_comparison.png")

# 3. FFDH improvement percentage over NFDH
df['Height_Improvement_%'] = ((df['NFDH_Height'] - df['FFDH_Height']) / df['NFDH_Height'] * 100).round(2)

fig, ax = plt.subplots(figsize=(14, 8))

for binWidth in binWidths:
    subset = df[df['binWidth'] == binWidth].sort_values('n')  # Sort by n
    ax.plot(subset['n'], subset['Height_Improvement_%'], marker='o', label=f'binWidth={binWidth}', linewidth=2, markersize=8)

ax.set_xlabel('Number of Rectangles (n)', fontsize=12)
ax.set_ylabel('FFDH Improvement over NFDH (%)', fontsize=12)
ax.set_title('Height Optimization Effect of FFDH Algorithm', fontsize=14, fontweight='bold')
ax.set_xscale('log')
ax.axhline(y=0, color='gray', linestyle='--', alpha=0.5)
ax.legend()
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('results/plots/improvement.png', dpi=300, bbox_inches='tight')
print("✓ Generated plot: results/plots/improvement.png")

# 4. Performance statistics table
print("\n=== FFDH Improvement Statistics over NFDH ===")
summary = df.groupby('binWidth').agg({
    'Height_Improvement_%': ['mean', 'min', 'max'],
    'NFDH_Time': 'mean',
    'FFDH_Time': 'mean'
}).round(3)
summary.columns = ['Average Improvement%', 'Minimum Improvement%', 'Maximum Improvement%', 'NFDH Avg Time(ms)', 'FFDH Avg Time(ms)']
print(summary)
print()

# 5. Generate HTML report
html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Texture Packing Test Report</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
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
    <h1>📊 Texture Packing Algorithm Test Report</h1>
    
    <div class="summary">
        <h3>Test Overview</h3>
        <ul>
            <li>Total test cases: {len(df)} </li>
            <li>binWidth values: {', '.join(map(str, binWidths))}</li>
            <li>Rectangle count range: {df['n'].min()} ~ {df['n'].max()}</li>
            <li>Test algorithms: NFDH (Next Fit Decreasing Height) vs FFDH (First Fit Decreasing Height)</li>
        </ul>
    </div>
    
    <h2>📈 1. Height Comparison</h2>
    <img src="plots/height_comparison.png" alt="Height comparison plot">
    
    <h2>⏱️ 2. Running Time Comparison</h2>
    <img src="plots/time_comparison.png" alt="Time comparison plot">
    
    <h2>🎯 3. FFDH Algorithm Improvement Effect</h2>
    <img src="plots/improvement.png" alt="Improvement percentage plot">
    
    <h2>📋 4. Detailed Test Data</h2>
    {df.to_html(index=False, classes='dataframe', border=0)}
    
    <h2>📊 5. Statistical Summary</h2>
    {summary.to_html(classes='dataframe', border=0)}
    
    <hr>
    <p style="text-align: center; color: #7f8c8d; margin-top: 30px;">
        Generated at: {pd.Timestamp.now().strftime('%Y-%m-%d %H:%M:%S')}
    </p>
</body>
</html>
"""

with open('results/report.html', 'w', encoding='utf-8') as f:
    f.write(html_content)

print("✓ Generated HTML report: results/report.html")
print("\n=== Visualization Complete! ===")
print("Please check the following files:")
print("  - results/results.csv (raw data)")
print("  - results/report.html (complete report)")
print("  - results/plots/*.png (plots)")
