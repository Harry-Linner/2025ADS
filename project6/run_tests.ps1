# PowerShell script to run all tests and collect results

Write-Host "=== Compiling packing.cpp ===" -ForegroundColor Cyan
g++ packing.cpp -o packing.exe -std=c++11 -O2

if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!" -ForegroundColor Red
    exit 1
}

Write-Host "Compilation successful!" -ForegroundColor Green
Write-Host ""

# Create results directory
New-Item -ItemType Directory -Force -Path "results" | Out-Null

# CSV file for results
$csvFile = "results\results.csv"
"binWidth,n,NFDH_Height,NFDH_Time,FFDH_Height,FFDH_Time" | Out-File -FilePath $csvFile -Encoding UTF8

Write-Host "=== Running Tests ===" -ForegroundColor Cyan
Write-Host ""

# Get all test files and sort them
$testFiles = Get-ChildItem "test_cases\test_w*.txt" | Sort-Object Name

$totalTests = $testFiles.Count
$currentTest = 0

foreach ($file in $testFiles) {
    $currentTest++
    
    # Extract binWidth and n from filename
    if ($file.Name -match "test_w(\d+)_n(\d+)\.txt") {
        $binWidth = $matches[1]
        $n = $matches[2]
        
        Write-Host "[$currentTest/$totalTests] Testing: binWidth=$binWidth, n=$n" -NoNewline
        
        # Run the test
        $output = Get-Content $file.FullName | .\packing.exe
        
        # Parse output
        $nfdhLine = $output[0]
        $ffdhLine = $output[1]
        
        if ($nfdhLine -match "NFDH: (\d+) \(Time: ([\d.]+) ms\)") {
            $nfdhHeight = $matches[1]
            $nfdhTime = $matches[2]
        }
        
        if ($ffdhLine -match "FFDH: (\d+) \(Time: ([\d.]+) ms\)") {
            $ffdhHeight = $matches[1]
            $ffdhTime = $matches[2]
        }
        
        # Write to CSV
        "$binWidth,$n,$nfdhHeight,$nfdhTime,$ffdhHeight,$ffdhTime" | Out-File -FilePath $csvFile -Append -Encoding UTF8
        
        Write-Host " ... Done" -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "=== All tests completed! ===" -ForegroundColor Green
Write-Host "Results saved to: $csvFile" -ForegroundColor Yellow
Write-Host ""
Write-Host "Now generating visualization..." -ForegroundColor Cyan
