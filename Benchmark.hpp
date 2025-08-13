#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>

class Benchmark {
public:
    struct Result {
        std::string name;
        double avg_time_ms;
        double min_time_ms;
        double max_time_ms;
        size_t iterations;
    };

    //EFFECTS Runs a benchmark function multiple times and records timing
    static Result time_function(const std::string& name, 
                               std::function<void()> func, 
                               size_t iterations = 1000) {
        std::vector<double> times;
        times.reserve(iterations);
        
        // Warm up
        for (size_t i = 0; i < 10; ++i) {
            func();
        }
        
        // Actual timing
        for (size_t i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            func();
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            times.push_back(duration.count() / 1000000.0); // Convert to milliseconds
        }
        
        // Calculate statistics
        double sum = 0.0;
        double min_time = times[0];
        double max_time = times[0];
        
        for (double time : times) {
            sum += time;
            min_time = std::min(min_time, time);
            max_time = std::max(max_time, time);
        }
        
        return {name, sum / iterations, min_time, max_time, iterations};
    }
    
    //EFFECTS Prints benchmark results in a formatted table
    static void print_results(const std::vector<Result>& results) {
        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << "PERFORMANCE BENCHMARK RESULTS" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        
        std::cout << std::left << std::setw(30) << "Benchmark Name"
                  << std::setw(12) << "Avg (ms)"
                  << std::setw(12) << "Min (ms)"
                  << std::setw(12) << "Max (ms)"
                  << std::setw(12) << "Iterations" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& result : results) {
            std::cout << std::left << std::setw(30) << result.name
                      << std::fixed << std::setprecision(4)
                      << std::setw(12) << result.avg_time_ms
                      << std::setw(12) << result.min_time_ms
                      << std::setw(12) << result.max_time_ms
                      << std::setw(12) << result.iterations << std::endl;
        }
        std::cout << std::string(80, '=') << std::endl;
    }
    
    //EFFECTS Compares two benchmark results and shows improvement/regression
    static void compare_results(const Result& baseline, const Result& current) {
        double improvement = ((baseline.avg_time_ms - current.avg_time_ms) / baseline.avg_time_ms) * 100.0;
        
        std::cout << "\nComparison for " << baseline.name << ":" << std::endl;
        std::cout << "Baseline: " << std::fixed << std::setprecision(4) << baseline.avg_time_ms << " ms" << std::endl;
        std::cout << "Current:  " << std::fixed << std::setprecision(4) << current.avg_time_ms << " ms" << std::endl;
        
        if (improvement > 0) {
            std::cout << "Improvement: " << std::fixed << std::setprecision(2) << improvement << "% faster" << std::endl;
        } else {
            std::cout << "Regression: " << std::fixed << std::setprecision(2) << -improvement << "% slower" << std::endl;
        }
    }
};

// Macro for easy benchmarking
#define BENCHMARK(name, code, iterations) \
    Benchmark::time_function(name, [&]() { code; }, iterations)

#endif // BENCHMARK_HPP