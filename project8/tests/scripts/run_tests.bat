@echo off
REM MapReduce 词频统计测试脚本 (Windows版本)
REM 运行串行和并行算法，比较结果和性能

setlocal enabledelayedexpansion

echo === MapReduce 词频统计测试框架 ===

REM 获取项目根目录
set "PROJECT_ROOT=%~dp0..\.."
set "TEST_DIR=%PROJECT_ROOT%\tests"

echo 项目根目录: %PROJECT_ROOT%
echo 测试目录: %TEST_DIR%

REM 创建必要的目录
if not exist "%TEST_DIR%\results\serial_results" mkdir "%TEST_DIR%\results\serial_results"
if not exist "%TEST_DIR%\results\parallel_results" mkdir "%TEST_DIR%\results\parallel_results"
if not exist "%TEST_DIR%\expected" mkdir "%TEST_DIR%\expected"

REM 编译程序
echo.
echo 编译程序...
cd /d "%PROJECT_ROOT%"

REM 编译串行程序
if exist "src\serial\serial.c" (
    echo 编译串行程序...
    gcc -o src\serial\serial.exe src\serial\serial.c -std=c99
    if !errorlevel! equ 0 (
        echo 串行程序编译成功
    ) else (
        echo 串行程序编译失败
        exit /b 1
    )
) else (
    echo 找不到串行程序源文件
    exit /b 1
)

REM 编译并行程序
if exist "src\parallel\mapper.c" (
    if exist "src\parallel\reducer.c" (
        echo 编译并行程序...
        gcc -o src\parallel\mapper.exe src\parallel\mapper.c -std=c99
        gcc -o src\parallel\reducer.exe src\parallel\reducer.c -std=c99
        if !errorlevel! equ 0 (
            echo 并行程序编译成功
        ) else (
            echo 并行程序编译失败
            exit /b 1
        )
    ) else (
        echo 找不到reducer源文件
        exit /b 1
    )
) else (
    echo 找不到mapper源文件
    exit /b 1
)

REM 生成测试数据
echo.
echo 生成测试数据...
cd /d "%TEST_DIR%"
python scripts\generate_test_data.py

REM 运行测试
echo.
echo === 开始测试 ===

REM 小规模测试
if exist "data\small_test.txt" (
    echo.
    echo 运行小规模测试...
    
    REM 串行测试
    echo 串行算法...
    "%PROJECT_ROOT%\src\serial\serial.exe" "data\small_test.txt" > "results\serial_results\small_result.txt" 2> "results\serial_results\small_result.log"
    
    REM 并行测试
    echo 并行算法...
    type "data\small_test.txt" | "%PROJECT_ROOT%\src\parallel\mapper.exe" | sort | "%PROJECT_ROOT%\src\parallel\reducer.exe" > "results\parallel_results\small_result.txt"
    
    REM 比较结果
    echo 比较结果...
    fc "results\serial_results\small_result.txt" "results\parallel_results\small_result.txt" > nul
    if !errorlevel! equ 0 (
        echo ✓ 小规模测试：串行和并行结果一致
    ) else (
        echo ✗ 小规模测试：串行和并行结果不一致
    )
)

REM 中等规模测试
if exist "data\medium_test.txt" (
    echo.
    echo 运行中等规模测试...
    
    REM 串行测试
    echo 串行算法...
    "%PROJECT_ROOT%\src\serial\serial.exe" "data\medium_test.txt" > "results\serial_results\medium_result.txt" 2> "results\serial_results\medium_result.log"
    
    REM 并行测试
    echo 并行算法...
    type "data\medium_test.txt" | "%PROJECT_ROOT%\src\parallel\mapper.exe" | sort | "%PROJECT_ROOT%\src\parallel\reducer.exe" > "results\parallel_results\medium_result.txt"
    
    REM 比较结果
    echo 比较结果...
    fc "results\serial_results\medium_result.txt" "results\parallel_results\medium_result.txt" > nul
    if !errorlevel! equ 0 (
        echo ✓ 中等规模测试：串行和并行结果一致
    ) else (
        echo ✗ 中等规模测试：串行和并行结果不一致
    )
)

REM 大规模测试
if exist "data\large_test.txt" (
    echo.
    echo 运行大规模测试...
    
    REM 串行测试
    echo 串行算法...
    "%PROJECT_ROOT%\src\serial\serial.exe" "data\large_test.txt" > "results\serial_results\large_result.txt" 2> "results\serial_results\large_result.log"
    
    REM 并行测试
    echo 并行算法...
    type "data\large_test.txt" | "%PROJECT_ROOT%\src\parallel\mapper.exe" | sort | "%PROJECT_ROOT%\src\parallel\reducer.exe" > "results\parallel_results\large_result.txt"
    
    REM 比较结果
    echo 比较结果...
    fc "results\serial_results\large_result.txt" "results\parallel_results\large_result.txt" > nul
    if !errorlevel! equ 0 (
        echo ✓ 大规模测试：串行和并行结果一致
    ) else (
        echo ✗ 大规模测试：串行和并行结果不一致
    )
)

REM Frankenstein测试
if exist "%PROJECT_ROOT%\testcase\Frankenstein.txt" (
    echo.
    echo 运行Frankenstein测试...
    
    REM 串行测试
    echo 串行算法...
    "%PROJECT_ROOT%\src\serial\serial.exe" "%PROJECT_ROOT%\testcase\Frankenstein.txt" > "results\serial_results\frankenstein_result.txt" 2> "results\serial_results\frankenstein_result.log"
    
    REM 并行测试
    echo 并行算法...
    type "%PROJECT_ROOT%\testcase\Frankenstein.txt" | "%PROJECT_ROOT%\src\parallel\mapper.exe" | sort | "%PROJECT_ROOT%\src\parallel\reducer.exe" > "results\parallel_results\frankenstein_result.txt"
    
    REM 比较结果
    echo 比较结果...
    fc "results\serial_results\frankenstein_result.txt" "results\parallel_results\frankenstein_result.txt" > nul
    if !errorlevel! equ 0 (
        echo ✓ Frankenstein测试：串行和并行结果一致
    ) else (
        echo ✗ Frankenstein测试：串行和并行结果不一致
    )
)

echo.
echo === 测试完成 ===
echo 结果文件保存在: %TEST_DIR%\results\
echo 运行性能测试: scripts\performance_test.bat
echo 验证结果正确性: python scripts\validate_results.py

pause