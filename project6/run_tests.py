import subprocess
import os
import re
import csv
from pathlib import Path

print("=== Running All Tests ===\n")

# Create results directory
os.makedirs('results', exist_ok=True)

# CSV file for results
csv_file = 'results/results.csv'

with open(csv_file, 'w', newline='', encoding='utf-8') as f:
    writer = csv.writer(f)
    writer.writerow(['binWidth', 'n', 'NFDH_Height', 'NFDH_Time', 'FFDH_Height', 'FFDH_Time'])

# Get all test files
test_files = sorted(Path('test_cases').glob('test_w*.txt'))

total = len(test_files)
current = 0

for test_file in test_files:
    current += 1
    
    # Extract binWidth and n from filename
    match = re.match(r'test_w(\d+)_n(\d+)\.txt', test_file.name)
    if not match:
        continue
    
    binWidth = match.group(1)
    n = match.group(2)
    
    print(f"[{current}/{total}] Testing: binWidth={binWidth}, n={n} ... ", end='', flush=True)
    
    # Run the test
    try:
        with open(test_file, 'r') as f:
            result = subprocess.run(
                ['packing.exe'],
                stdin=f,
                capture_output=True,
                text=True,
                timeout=60
            )
        
        output = result.stdout.strip().split('\n')
        
        # Parse output
        nfdh_line = output[0]
        ffdh_line = output[1]
        
        nfdh_match = re.search(r'NFDH: (\d+) \(Time: ([\d.]+) ms\)', nfdh_line)
        ffdh_match = re.search(r'FFDH: (\d+) \(Time: ([\d.]+) ms\)', ffdh_line)
        
        if nfdh_match and ffdh_match:
            nfdh_height = nfdh_match.group(1)
            nfdh_time = nfdh_match.group(2)
            ffdh_height = ffdh_match.group(1)
            ffdh_time = ffdh_match.group(2)
            
            # Write to CSV
            with open(csv_file, 'a', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                writer.writerow([binWidth, n, nfdh_height, nfdh_time, ffdh_height, ffdh_time])
            
            print("✓ Done")
        else:
            print("✗ Failed to parse output")
    
    except subprocess.TimeoutExpired:
        print("✗ Timeout")
    except Exception as e:
        print(f"✗ Error: {e}")

print(f"\n=== All tests completed! ===")
print(f"Results saved to: {csv_file}")
print("\nNow run: python visualize.py")
