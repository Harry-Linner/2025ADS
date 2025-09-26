import matplotlib.pyplot as plt
import os

# 1. 设置 Matplotlib 的字体以支持中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False     # 用来正常显示负号

def plot_performance_data(file_path):
    """
    读取性能数据文件，生成三张折线图，并将其保存到figures文件夹。
    
    参数:
        file_path (str): 性能数据文件的相对路径。
    """
    data = {'inc': {}, 'dec': {}, 'rand': {}}
    
    if not os.path.exists(file_path):
        print(f"错误：找不到文件 '{file_path}'。请检查路径是否正确。")
        return

    output_dir = os.path.join('..', 'figures')
    
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"已创建文件夹: {output_dir}")

    tables_dir = os.path.join('..', 'tables')
    if not os.path.exists(tables_dir):
        os.makedirs(tables_dir)
        print(f"已创建文件夹: {tables_dir}")

    with open(file_path, 'r') as f:
        for line in f:
            if not line.strip():
                continue
            parts = line.strip().split(',')
            if len(parts) == 4:
                tree_type, size_str, order, time_str = parts
                size = int(size_str)
                time = float(time_str)
                
                if tree_type not in data[order]:
                    data[order][tree_type] = {'sizes': [], 'times': []}
                data[order][tree_type]['sizes'].append(size)
                data[order][tree_type]['times'].append(time)

    for order_type in ['inc', 'dec', 'rand']:
        plt.figure(figsize=(10, 6))
        
        # 收集表格数据
        table_lines = []
        header = "| 数据规模(N) | BST运行时间(s) | AVL运行时间(s) | Splay运行时间(s) |"
        sep = "|---|---|---|---|"
        table_lines.append(header)
        table_lines.append(sep)

        # 获取所有出现过的数据规模
        all_sizes = set()
        for tree_type in ['BST', 'AVL', 'Splay']:
            if tree_type in data[order_type]:
                all_sizes.update(data[order_type][tree_type]['sizes'])
        all_sizes = sorted(all_sizes)

        for idx, size in enumerate(all_sizes):
            row = [str(size)]
            for tree_type in ['BST', 'AVL', 'Splay']:
                if tree_type in data[order_type]:
                    try:
                        i = data[order_type][tree_type]['sizes'].index(size)
                        t = data[order_type][tree_type]['times'][i]
                        row.append(f"{t:.6f}")
                    except ValueError:
                        row.append("")
                else:
                    row.append("")
            table_lines.append("| " + " | ".join(row) + " |")

        for tree_type in ['BST', 'AVL', 'Splay']:
            if tree_type in data[order_type]:
                plt.plot(
                    data[order_type][tree_type]['sizes'],
                    data[order_type][tree_type]['times'],
                    label=tree_type
                )
        
        if order_type == 'inc':
            title = '递增数据下的性能表现'
        elif order_type == 'dec':
            title = '递减数据下的性能表现'
        else:
            title = '随机数据下的性能表现'

        plt.title(title)
        plt.xlabel('数据规模 (N)')
        plt.ylabel('运行时间 (s)')
        plt.legend()
        plt.grid(True)
        
        # 自动递增文件名
        n = 1
        while True:
            filename = f'performance_{order_type}_{n}.png'
            save_path = os.path.join(output_dir, filename)
            table_filename = f'performance_{order_type}_{n}.md'
            table_save_path = os.path.join(tables_dir, table_filename)
            if not os.path.exists(save_path) and not os.path.exists(table_save_path):
                break
            n += 1

        plt.savefig(save_path)
        print(f"图表已保存到: {save_path}")
        plt.close() # 关闭当前图形，以避免内存问题

        # 保存markdown表格
        with open(table_save_path, 'w', encoding='utf-8') as f:
            f.write('\n'.join(table_lines))
        print(f"表格已保存到: {table_save_path}")

    # 清空原始数据文件内容
    with open(file_path, 'w') as f:
        f.truncate(0)
    print(f"已清空数据文件: {file_path}")

# 使用相对路径指定文件位置
file_path_to_plot = os.path.join('..', 'test_data', 'performance_results.txt')

# 调用函数并传入文件路径
plot_performance_data(file_path_to_plot)