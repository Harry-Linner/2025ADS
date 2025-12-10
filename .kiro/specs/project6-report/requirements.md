# Requirements Document

## Introduction

This specification defines the requirements for completing a comprehensive report for project6, which implements and compares two strip packing algorithms (NFDH and FFDH). The report should follow the format established in project_1/report.md and include improved timing measurements with higher precision, new visualization charts, and detailed performance analysis.

## Glossary

- **NFDH**: Next Fit Decreasing Height - A strip packing algorithm that sorts rectangles by height and places them on the current shelf only
- **FFDH**: First Fit Decreasing Height - A strip packing algorithm that sorts rectangles by height and places them on the first shelf that can accommodate them
- **Strip Packing**: The problem of packing rectangles into a strip of fixed width and unlimited height, minimizing the total height used
- **Timing Precision**: The accuracy of time measurements, requiring microsecond-level precision instead of millisecond-level
- **Performance Testing System**: The automated system that generates test cases, runs algorithms, and collects performance data

## Requirements

### Requirement 1: High-Precision Timing Measurement

**User Story:** As a researcher, I want accurate timing measurements with microsecond precision, so that I can properly compare algorithm performance even for small input sizes.

#### Acceptance Criteria

1. WHEN the Performance Testing System measures algorithm execution time, THE System SHALL use C++ chrono library with high_resolution_clock to achieve microsecond precision
2. WHEN the Performance Testing System outputs timing results, THE System SHALL format time values with at least 6 decimal places in milliseconds
3. WHEN the Performance Testing System runs tests, THE System SHALL execute each test case multiple times and report average timing to reduce measurement variance

### Requirement 2: Automated Performance Data Collection

**User Story:** As a researcher, I want an automated script to collect performance data across multiple test configurations, so that I can efficiently gather comprehensive results.

#### Acceptance Criteria

1. THE Performance Testing System SHALL generate test cases with varying binWidth values (1000, 2000, 5000)
2. THE Performance Testing System SHALL generate test cases with varying rectangle counts (1000, 3000, 5000, 8000, 10000, 30000, 50000)
3. WHEN the Performance Testing System collects results, THE System SHALL save data in CSV format with columns for binWidth, n, NFDH_Height, NFDH_Time, FFDH_Height, and FFDH_Time
4. THE Performance Testing System SHALL output timing measurements in milliseconds with microsecond precision

### Requirement 3: Data Visualization

**User Story:** As a researcher, I want clear visualizations of algorithm performance, so that I can easily understand and present the comparative results.

#### Acceptance Criteria

1. THE Visualization System SHALL generate a chart comparing NFDH and FFDH heights for different binWidth values
2. THE Visualization System SHALL generate a chart comparing NFDH and FFDH execution times for different input sizes
3. THE Visualization System SHALL generate a chart showing the percentage improvement of FFDH over NFDH
4. WHEN the Visualization System creates charts, THE System SHALL use matplotlib with appropriate labels, legends, and formatting
5. THE Visualization System SHALL save all generated charts as PNG files with at least 300 DPI resolution

### Requirement 4: Comprehensive Report Document

**User Story:** As a researcher, I want a well-structured report document following the established format, so that the project documentation is consistent and professional.

#### Acceptance Criteria

1. THE Report Document SHALL include a Background Introduction section explaining strip packing algorithms, existing problems, and solutions
2. THE Report Document SHALL include an Experiments and Performance Evaluation section with procedure description, results tables, and performance graphs
3. THE Report Document SHALL include performance analysis comparing NFDH and FFDH across different input patterns
4. THE Report Document SHALL include a Conclusions section summarizing findings and algorithm trade-offs
5. WHEN the Report Document presents data, THE Document SHALL include embedded images of generated charts
6. THE Report Document SHALL be written in Markdown format following the structure of project_1/report.md
