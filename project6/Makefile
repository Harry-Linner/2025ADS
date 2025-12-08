# Makefile for Texture Packing Project

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

# Targets
all: texture_packing test_generator performance_test

texture_packing: texture_packing.cpp
	$(CXX) $(CXXFLAGS) -o texture_packing texture_packing.cpp

test_generator: test_generator.cpp
	$(CXX) $(CXXFLAGS) -o test_generator test_generator.cpp

performance_test: performance_test.cpp
	$(CXX) $(CXXFLAGS) -o performance_test performance_test.cpp

# Generate test data
generate_tests: test_generator
	./test_generator

# Run performance tests
run_tests: performance_test generate_tests
	./performance_test

# Plot results (requires Python with pandas and matplotlib)
plot: run_tests
	python plot_results.py

# Clean build files
clean:
	rm -f texture_packing test_generator performance_test
	rm -f *.exe
	rm -f test_n*.txt
	rm -f performance_results.csv
	rm -f *.png

.PHONY: all generate_tests run_tests plot clean
