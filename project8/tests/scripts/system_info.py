#!/usr/bin/env python3
"""
系统信息收集脚本
收集测试环境的硬件和软件信息，用于性能分析
"""

import platform
import subprocess
import os
import sys
from datetime import datetime

def get_cpu_info():
    """获取CPU信息"""
    try:
        if platform.system() == "Linux":
            # Linux系统
            with open('/proc/cpuinfo', 'r') as f:
                lines = f.readlines()
            
            cpu_info = {}
            for line in lines:
                if ':' in line:
                    key, value = line.split(':', 1)
                    key = key.strip()
                    value = value.strip()
                    
                    if key == 'model name' and 'model_name' not in cpu_info:
                        cpu_info['model_name'] = value
                    elif key == 'cpu cores' and 'cores' not in cpu_info:
                        cpu_info['cores'] = value
                    elif key == 'processor':
                        cpu_info['processors'] = int(value) + 1
            
            return cpu_info
        
        elif platform.system() == "Darwin":
            # macOS系统
            result = subprocess.run(['sysctl', '-n', 'machdep.cpu.brand_string'], 
                                  capture_output=True, text=True)
            model_name = result.stdout.strip() if result.returncode == 0 else "Unknown"
            
            result = subprocess.run(['sysctl', '-n', 'hw.ncpu'], 
                                  capture_output=True, text=True)
            cores = result.stdout.strip() if result.returncode == 0 else "Unknown"
            
            return {
                'model_name': model_name,
                'cores': cores,
                'processors': cores
            }
        
        elif platform.system() == "Windows":
            # Windows系统
            try:
                result = subprocess.run(['wmic', 'cpu', 'get', 'name'], 
                                      capture_output=True, text=True)
                lines = result.stdout.strip().split('\n')
                model_name = lines[1].strip() if len(lines) > 1 else "Unknown"
                
                result = subprocess.run(['wmic', 'cpu', 'get', 'NumberOfCores'], 
                                      capture_output=True, text=True)
                lines = result.stdout.strip().split('\n')
                cores = lines[1].strip() if len(lines) > 1 else "Unknown"
                
                return {
                    'model_name': model_name,
                    'cores': cores,
                    'processors': cores
                }
            except:
                return {'model_name': 'Unknown', 'cores': 'Unknown', 'processors': 'Unknown'}
        
    except Exception as e:
        print(f"获取CPU信息失败: {e}")
        return {'model_name': 'Unknown', 'cores': 'Unknown', 'processors': 'Unknown'}

def get_memory_info():
    """获取内存信息"""
    try:
        if platform.system() == "Linux":
            with open('/proc/meminfo', 'r') as f:
                lines = f.readlines()
            
            for line in lines:
                if line.startswith('MemTotal:'):
                    # 转换KB到GB
                    mem_kb = int(line.split()[1])
                    mem_gb = round(mem_kb / 1024 / 1024, 2)
                    return f"{mem_gb} GB"
        
        elif platform.system() == "Darwin":
            result = subprocess.run(['sysctl', '-n', 'hw.memsize'], 
                                  capture_output=True, text=True)
            if result.returncode == 0:
                mem_bytes = int(result.stdout.strip())
                mem_gb = round(mem_bytes / 1024 / 1024 / 1024, 2)
                return f"{mem_gb} GB"
        
        elif platform.system() == "Windows":
            result = subprocess.run(['wmic', 'computersystem', 'get', 'TotalPhysicalMemory'], 
                                  capture_output=True, text=True)
            lines = result.stdout.strip().split('\n')
            if len(lines) > 1:
                mem_bytes = int(lines[1].strip())
                mem_gb = round(mem_bytes / 1024 / 1024 / 1024, 2)
                return f"{mem_gb} GB"
        
    except Exception as e:
        print(f"获取内存信息失败: {e}")
    
    return "Unknown"

def get_disk_info():
    """获取磁盘信息"""
    try:
        if platform.system() in ["Linux", "Darwin"]:
            result = subprocess.run(['df', '-h', '.'], capture_output=True, text=True)
            if result.returncode == 0:
                lines = result.stdout.strip().split('\n')
                if len(lines) > 1:
                    parts = lines[1].split()
                    return {
                        'total': parts[1],
                        'used': parts[2],
                        'available': parts[3],
                        'usage': parts[4]
                    }
        
        elif platform.system() == "Windows":
            # 获取当前驱动器信息
            import shutil
            total, used, free = shutil.disk_usage('.')
            total_gb = round(total / 1024 / 1024 / 1024, 2)
            used_gb = round(used / 1024 / 1024 / 1024, 2)
            free_gb = round(free / 1024 / 1024 / 1024, 2)
            usage_percent = round((used / total) * 100, 1)
            
            return {
                'total': f"{total_gb}G",
                'used': f"{used_gb}G",
                'available': f"{free_gb}G",
                'usage': f"{usage_percent}%"
            }
    
    except Exception as e:
        print(f"获取磁盘信息失败: {e}")
    
    return {'total': 'Unknown', 'used': 'Unknown', 'available': 'Unknown', 'usage': 'Unknown'}

def get_compiler_info():
    """获取编译器信息"""
    compilers = {}
    
    # 检查GCC
    try:
        result = subprocess.run(['gcc', '--version'], capture_output=True, text=True)
        if result.returncode == 0:
            version_line = result.stdout.split('\n')[0]
            compilers['gcc'] = version_line
    except:
        compilers['gcc'] = 'Not found'
    
    # 检查Clang
    try:
        result = subprocess.run(['clang', '--version'], capture_output=True, text=True)
        if result.returncode == 0:
            version_line = result.stdout.split('\n')[0]
            compilers['clang'] = version_line
    except:
        compilers['clang'] = 'Not found'
    
    return compilers

def get_java_hadoop_info():
    """获取Java和Hadoop信息"""
    info = {}
    
    # 检查Java
    try:
        result = subprocess.run(['java', '-version'], capture_output=True, text=True)
        if result.returncode == 0:
            # Java版本信息通常在stderr中
            version_info = result.stderr.split('\n')[0] if result.stderr else result.stdout.split('\n')[0]
            info['java'] = version_info
    except:
        info['java'] = 'Not found'
    
    # 检查Hadoop
    try:
        result = subprocess.run(['hadoop', 'version'], capture_output=True, text=True)
        if result.returncode == 0:
            version_line = result.stdout.split('\n')[0]
            info['hadoop'] = version_line
    except:
        info['hadoop'] = 'Not found'
    
    return info

def generate_system_report():
    """生成系统信息报告"""
    print("正在收集系统信息...")
    
    # 收集信息
    cpu_info = get_cpu_info()
    memory_info = get_memory_info()
    disk_info = get_disk_info()
    compiler_info = get_compiler_info()
    java_hadoop_info = get_java_hadoop_info()
    
    # 生成报告
    report = f"""
MapReduce 测试环境系统信息报告
================================
生成时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

操作系统信息:
- 系统: {platform.system()}
- 版本: {platform.release()}
- 架构: {platform.machine()}
- Python版本: {platform.python_version()}

硬件信息:
- CPU型号: {cpu_info.get('model_name', 'Unknown')}
- CPU核心数: {cpu_info.get('cores', 'Unknown')}
- 逻辑处理器数: {cpu_info.get('processors', 'Unknown')}
- 内存: {memory_info}

磁盘信息:
- 总容量: {disk_info.get('total', 'Unknown')}
- 已使用: {disk_info.get('used', 'Unknown')}
- 可用空间: {disk_info.get('available', 'Unknown')}
- 使用率: {disk_info.get('usage', 'Unknown')}

编译器信息:
- GCC: {compiler_info.get('gcc', 'Not found')}
- Clang: {compiler_info.get('clang', 'Not found')}

运行环境:
- Java: {java_hadoop_info.get('java', 'Not found')}
- Hadoop: {java_hadoop_info.get('hadoop', 'Not found')}

环境变量:
- JAVA_HOME: {os.environ.get('JAVA_HOME', 'Not set')}
- HADOOP_HOME: {os.environ.get('HADOOP_HOME', 'Not set')}
- PATH: {os.environ.get('PATH', 'Not set')[:100]}...
"""
    
    return report

def main():
    """主函数"""
    if len(sys.argv) > 1 and sys.argv[1] == '--save':
        # 保存到文件
        report = generate_system_report()
        
        # 确定输出文件路径
        script_dir = os.path.dirname(os.path.abspath(__file__))
        output_file = os.path.join(os.path.dirname(script_dir), 'system_info.txt')
        
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(report)
        
        print(f"系统信息已保存到: {output_file}")
    else:
        # 直接输出到控制台
        report = generate_system_report()
        print(report)

if __name__ == "__main__":
    main()