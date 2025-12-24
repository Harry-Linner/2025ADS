# Design Document

## Overview

This design outlines the implementation approach for creating a comprehensive project6 report with improved timing precision, automated data collection, visualization, and documentation. The solution consists of four main components: a high-precision timing test program, a data collection script, a visualization script, and a markdown report document.

## Architecture

```
project6/
├── packing_benchmark.cpp      # New: High-precision timing test program
├── collect_data.py            # New: Automated data collection script
├── plot_results.py            # New: Visualization script
├── report.md                  # New: Final report document
├── results/
│   ├── results.csv           # Performance data
│   └── plots/                # Generated charts
│       ├── height_comparison.png
│       ├── time_comparison.png
│       └── improvement.png
└── [existing files...]
```

## Components and Interfaces

### 1. High-Precision Timing Test Program (packing_benchmark.cpp)

**Purpose**: Execute NFDH and FFDH algorithms with microsecond-precision timing

**Key Features**:
- Uses `std::chrono::high_resolution_clock` for timing
- Reads test input from stdin (binWidth, n, rectangles)
- Outputs results in parseable format: `NFDH: <height> <time_ms>` and `FFDH: <height> <time_ms>`
- Time reported in milliseconds with 6 decimal places

**Interface**:
```
Input (stdin):
  binWidth n
  w1 h1
  w2 h2
  ...
  wn hn

Output (stdout):
  NFDH: <height> <time_ms>
  FFDH: <height> <time_ms>
```

**Implementation Details**:
- Copy NFDH and FFDH algorithm implementations from existing packing.cpp
- Replace `clock()` timing with `chrono::high_resolution_clock`
- Use `chrono::duration<double, milli>` for millisecond output with high precision
- Format output with `std::fixed` and `std::setprecision(6)`

### 2. Data Collection Script (collect_data.py)

**Purpose**: Automate test execution and data collection

**Key Features**:
- Generates or uses existing test cases
- Executes packing_benchmark.exe for each test case
- Parses output and saves to CSV
- Provides progress feedback

**Interface**:
```python
# Output: results/results.csv
# Format: binWidth,n,NFDH_Height,NFDH_Time,FFDH_Height,FFDH_Time
```

**Implementation Details**:
- Use subprocess to run packing_benchmark.exe
- Parse output using regex to extract height and time values
- Write results incrementally to CSV to preserve partial results
- Handle errors and timeouts gracefully

### 3. Visualization Script (plot_results.py)

**Purpose**: Generate publication-quality charts from performance data

**Key Features**:
- Reads results.csv
- Generates 3 main charts:
  1. Height comparison (bar chart grouped by binWidth)
  2. Time comparison (line chart with log scale for x-axis)
  3. Improvement percentage (line chart showing FFDH advantage)
- Saves charts as high-resolution PNG files

**Interface**:
```python
# Input: results/results.csv
# Output: 
#   results/plots/height_comparison.png
#   results/plots/time_comparison.png
#   results/plots/improvement.png
```

**Implementation Details**:
- Use pandas for data manipulation
- Use matplotlib for plotting
- Configure Chinese font support (SimHei, Microsoft YaHei)
- Use 300 DPI for output images
- Apply consistent color scheme and styling

**Chart Specifications**:

1. **Height Comparison Chart**:
   - Type: Grouped bar chart
   - X-axis: Rectangle count (n)
   - Y-axis: Total height
   - Groups: NFDH vs FFDH
   - Subplots: One per binWidth value

2. **Time Comparison Chart**:
   - Type: Line chart
   - X-axis: Rectangle count (n) - logarithmic scale
   - Y-axis: Execution time (ms) - linear scale starting from 0
   - Lines: NFDH and FFDH
   - Subplots: One per binWidth value

3. **Improvement Chart**:
   - Type: Line chart
   - X-axis: Rectangle count (n) - logarithmic scale
   - Y-axis: Improvement percentage
   - Lines: One per binWidth value
   - Formula: ((NFDH_Height - FFDH_Height) / NFDH_Height) * 100

### 4. Report Document (report.md)

**Purpose**: Comprehensive markdown report following project_1 format

**Structure**:
```markdown
# 1. Background Introduction
## 1.1 Strip Packing Problem
## 1.2 Existing Problems
## 1.3 Possible Solutions

# 2. Experiments and Performance Evaluation
## 2.1 Experiments Procedure
## 2.2 Tables and Graphs of Results
## 2.3 Performance Evaluation & Analysis

# 3. Conclusions

# Appendix: Source Code
```

**Content Guidelines**:
- Background: Explain strip packing, NFDH, and FFDH algorithms
- Procedure: Describe test methodology and data collection
- Results: Include data tables and embedded chart images
- Analysis: Compare algorithms across different scenarios
- Conclusions: Summarize findings and discuss trade-offs

## Data Models

### Performance Data Record
```
{
  binWidth: integer,      // Strip width (1000, 2000, 5000)
  n: integer,            // Number of rectangles
  NFDH_Height: integer,  // Total height used by NFDH
  NFDH_Time: float,      // Execution time in ms (6 decimals)
  FFDH_Height: integer,  // Total height used by FFDH
  FFDH_Time: float       // Execution time in ms (6 decimals)
}
```

### Test Case Format
```
binWidth n
w1 h1
w2 h2
...
wn hn
```

## Error Handling

### Timing Program
- Handle invalid input gracefully
- Ensure rectangles wider than binWidth are skipped
- Return non-zero exit code on errors

### Data Collection Script
- Handle subprocess timeouts (60 second limit)
- Continue on individual test failures
- Log errors to console
- Preserve partial results in CSV

### Visualization Script
- Verify CSV file exists before reading
- Handle missing or malformed data
- Provide informative error messages
- Skip missing chart generation if data insufficient

## Testing Strategy

### Verification Steps
1. Compile packing_benchmark.cpp and verify it runs
2. Test with small input to verify output format
3. Run collect_data.py on subset of test cases
4. Verify CSV output format and data accuracy
5. Run plot_results.py and verify chart generation
6. Review report.md for completeness and formatting

### Performance Validation
- Compare timing results with existing results.csv to ensure reasonable values
- Verify FFDH consistently produces equal or better heights than NFDH
- Confirm timing precision shows microsecond-level variations
- Validate that larger inputs show expected time complexity trends

## Implementation Notes

### Timing Precision
The key improvement is using C++ chrono instead of clock():
```cpp
auto start = chrono::high_resolution_clock::now();
// ... algorithm execution ...
auto end = chrono::high_resolution_clock::now();
chrono::duration<double, milli> elapsed = end - start;
```

### Chart Styling
Consistent visual style across all charts:
- NFDH: Blue (#3498db)
- FFDH: Red (#e74c3c)
- Grid: Light gray with alpha=0.3
- Font: Microsoft YaHei for Chinese support
- DPI: 300 for publication quality

### Report Format
Follow project_1 structure closely:
- Use same section numbering
- Include similar level of detail
- Embed images with `![alt text](path)`
- Use tables for numerical data
- Provide clear analysis and conclusions
