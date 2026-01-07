/**
 * @file 05_combined_advanced.cpp
 * @brief Advanced examples combining future, async, and promise
 * 
 * This file demonstrates real-world scenarios using all three concepts together:
 * - Complex producer-consumer patterns
 * - Parallel algorithms
 * - Task pipelines
 * - Error handling strategies
 * - Performance patterns
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <queue>
#include <numeric>
#include <random>
#include <algorithm>
#include <memory>

using namespace std::chrono_literals;

// ============================================================================
// EXAMPLE 1: Parallel Map-Reduce Pattern
// ============================================================================

// Map function: process single element
int square(int x) {
    std::this_thread::sleep_for(50ms);  // Simulate work
    return x * x;
}

// Reduce function: combine results
int sum(int a, int b) {
    return a + b;
}

void demo_map_reduce() {
    std::cout << "\n=== EXAMPLE 1: Parallel Map-Reduce ===" << std::endl;
    
    // Input data
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << "Input: ";
    for (int val : data) std::cout << val << " ";
    std::cout << std::endl;
    
    auto start = std::chrono::steady_clock::now();
    
    // MAP phase: Process each element in parallel
    std::vector<std::future<int>> futures;
    for (int val : data) {
        futures.push_back(std::async(std::launch::async, square, val));
    }
    
    // Collect mapped results
    std::vector<int> mapped;
    for (auto& fut : futures) {
        mapped.push_back(fut.get());
    }
    
    std::cout << "Mapped (squared): ";
    for (int val : mapped) std::cout << val << " ";
    std::cout << std::endl;
    
    // REDUCE phase: Sum all results
    int total = std::accumulate(mapped.begin(), mapped.end(), 0, sum);
    
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Total sum: " << total << std::endl;
    std::cout << "Time (parallel): " << elapsed.count() << "ms" << std::endl;
    
    // Compare with sequential
    start = std::chrono::steady_clock::now();
    int sequential_total = 0;
    for (int val : data) {
        sequential_total += square(val);
    }
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time (sequential): " << elapsed.count() << "ms" << std::endl;
}

// ============================================================================
// EXAMPLE 2: Pipeline Pattern with Stages
// ============================================================================

// Stage 1: Generate data
std::vector<int> generate_data(int count) {
    std::cout << "  [Stage 1] Generating " << count << " items..." << std::endl;
    std::vector<int> data(count);
    std::iota(data.begin(), data.end(), 1);
    std::this_thread::sleep_for(500ms);
    return data;
}

// Stage 2: Transform data
std::vector<int> transform_data(std::vector<int> data) {
    std::cout << "  [Stage 2] Transforming data..." << std::endl;
    for (auto& val : data) {
        val = val * 2;
    }
    std::this_thread::sleep_for(500ms);
    return data;
}

// Stage 3: Filter data
std::vector<int> filter_data(std::vector<int> data) {
    std::cout << "  [Stage 3] Filtering data..." << std::endl;
    std::vector<int> filtered;
    std::copy_if(data.begin(), data.end(), std::back_inserter(filtered),
                 [](int x) { return x % 4 == 0; });
    std::this_thread::sleep_for(500ms);
    return filtered;
}

// Stage 4: Aggregate data
int aggregate_data(std::vector<int> data) {
    std::cout << "  [Stage 4] Aggregating data..." << std::endl;
    std::this_thread::sleep_for(500ms);
    return std::accumulate(data.begin(), data.end(), 0);
}

void demo_pipeline() {
    std::cout << "\n=== EXAMPLE 2: Pipeline Pattern ===" << std::endl;
    
    auto start = std::chrono::steady_clock::now();
    
    // Create pipeline of async operations
    auto fut1 = std::async(std::launch::async, generate_data, 10);
    
    // Chain subsequent operations
    auto fut2 = std::async(std::launch::async, [&fut1]() {
        return transform_data(fut1.get());
    });
    
    auto fut3 = std::async(std::launch::async, [&fut2]() {
        return filter_data(fut2.get());
    });
    
    auto fut4 = std::async(std::launch::async, [&fut3]() {
        return aggregate_data(fut3.get());
    });
    
    int final_result = fut4.get();
    
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Final result: " << final_result << std::endl;
    std::cout << "Pipeline time: " << elapsed.count() << "ms" << std::endl;
}

// ============================================================================
// EXAMPLE 3: Work Queue with Promise/Future
// ============================================================================

class Task {
public:
    virtual ~Task() = default;
    virtual int execute() = 0;
};

class AddTask : public Task {
    int a, b;
public:
    AddTask(int x, int y) : a(x), b(y) {}
    int execute() override {
        std::this_thread::sleep_for(200ms);
        return a + b;
    }
};

class MultiplyTask : public Task {
    int a, b;
public:
    MultiplyTask(int x, int y) : a(x), b(y) {}
    int execute() override {
        std::this_thread::sleep_for(300ms);
        return a * b;
    }
};

void demo_work_queue() {
    std::cout << "\n=== EXAMPLE 3: Work Queue Pattern ===" << std::endl;
    
    // Create work queue
    std::vector<std::unique_ptr<Task>> tasks;
    tasks.push_back(std::make_unique<AddTask>(10, 20));
    tasks.push_back(std::make_unique<MultiplyTask>(5, 6));
    tasks.push_back(std::make_unique<AddTask>(100, 200));
    tasks.push_back(std::make_unique<MultiplyTask>(7, 8));
    
    // Process tasks with promises
    std::vector<std::promise<int>> promises(tasks.size());
    std::vector<std::future<int>> futures;
    
    for (auto& prom : promises) {
        futures.push_back(prom.get_future());
    }
    
    // Worker threads
    std::vector<std::thread> workers;
    for (size_t i = 0; i < tasks.size(); ++i) {
        workers.emplace_back([&tasks, &promises, i]() {
            std::cout << "  [Worker " << i << "] Executing task..." << std::endl;
            int result = tasks[i]->execute();
            promises[i].set_value(result);
            std::cout << "  [Worker " << i << "] Task complete, result: " << result << std::endl;
        });
    }
    
    // Collect results
    std::cout << "\n[Main] Collecting results:" << std::endl;
    for (size_t i = 0; i < futures.size(); ++i) {
        int result = futures[i].get();
        std::cout << "  Task " << i << " result: " << result << std::endl;
    }
    
    for (auto& t : workers) {
        t.join();
    }
}

// ============================================================================
// EXAMPLE 4: Timeout and Cancellation Pattern
// ============================================================================

struct ComputationResult {
    bool success;
    int value;
    std::string message;
};

ComputationResult long_computation(int timeout_ms) {
    std::cout << "  [Computation] Starting..." << std::endl;
    
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms / 10));
        std::cout << "  [Computation] Progress: " << ((i + 1) * 10) << "%" << std::endl;
    }
    
    return {true, 42, "Success"};
}

void demo_timeout_handling() {
    std::cout << "\n=== EXAMPLE 4: Timeout Handling ===" << std::endl;
    
    // Test 1: Task completes within timeout
    std::cout << "\n1. Task completes within timeout:" << std::endl;
    auto fut1 = std::async(std::launch::async, long_computation, 500);
    
    if (fut1.wait_for(2s) == std::future_status::ready) {
        auto result = fut1.get();
        std::cout << "   Result: " << result.message << ", Value: " << result.value << std::endl;
    } else {
        std::cout << "   Task timed out!" << std::endl;
    }
    
    // Test 2: Task exceeds timeout
    std::cout << "\n2. Task exceeds timeout:" << std::endl;
    auto fut2 = std::async(std::launch::async, long_computation, 2000);
    
    if (fut2.wait_for(500ms) == std::future_status::ready) {
        auto result = fut2.get();
        std::cout << "   Result: " << result.message << std::endl;
    } else {
        std::cout << "   Task timed out! Continuing without result..." << std::endl;
        std::cout << "   (Note: Task still running in background)" << std::endl;
        // In real applications, you might need a cancellation mechanism
        fut2.wait();  // Wait for completion to avoid dangling thread
    }
}

// ============================================================================
// EXAMPLE 5: Error Handling Strategy - Multiple Fallbacks
// ============================================================================

int primary_service(int data) {
    std::cout << "  [Primary] Attempting..." << std::endl;
    std::this_thread::sleep_for(300ms);
    
    if (data < 0) {
        throw std::runtime_error("Primary service failed!");
    }
    return data * 10;
}

int backup_service(int data) {
    std::cout << "  [Backup] Attempting..." << std::endl;
    std::this_thread::sleep_for(200ms);
    
    if (data < -10) {
        throw std::runtime_error("Backup service failed!");
    }
    return data * 5;
}

int fallback_service(int data) {
    std::cout << "  [Fallback] Using default..." << std::endl;
    std::this_thread::sleep_for(100ms);
    return 0;  // Safe default
}

void demo_error_fallback() {
    std::cout << "\n=== EXAMPLE 5: Error Handling with Fallbacks ===" << std::endl;
    
    auto process_with_fallback = [](int data) {
        // Try primary service
        auto fut_primary = std::async(std::launch::async, primary_service, data);
        
        try {
            return fut_primary.get();
        } catch (const std::exception& e) {
            std::cout << "   Primary failed: " << e.what() << std::endl;
            
            // Try backup service
            auto fut_backup = std::async(std::launch::async, backup_service, data);
            try {
                return fut_backup.get();
            } catch (const std::exception& e) {
                std::cout << "   Backup failed: " << e.what() << std::endl;
                
                // Use fallback
                auto fut_fallback = std::async(std::launch::async, fallback_service, data);
                return fut_fallback.get();
            }
        }
    };
    
    // Test different scenarios
    std::cout << "\n1. Successful primary:" << std::endl;
    int result1 = process_with_fallback(5);
    std::cout << "   Final result: " << result1 << std::endl;
    
    std::cout << "\n2. Primary fails, backup succeeds:" << std::endl;
    int result2 = process_with_fallback(-5);
    std::cout << "   Final result: " << result2 << std::endl;
    
    std::cout << "\n3. Both fail, use fallback:" << std::endl;
    int result3 = process_with_fallback(-15);
    std::cout << "   Final result: " << result3 << std::endl;
}

// ============================================================================
// EXAMPLE 6: Parallel QuickSort
// ============================================================================

void parallel_quicksort(std::vector<int>& data, int left, int right, int depth = 0) {
    if (left >= right) return;
    
    // Partition
    int pivot = data[left + (right - left) / 2];
    int i = left, j = right;
    
    while (i <= j) {
        while (data[i] < pivot) i++;
        while (data[j] > pivot) j--;
        if (i <= j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }
    
    // Recursively sort partitions
    // Use async for parallel execution only at top levels
    if (depth < 3 && (right - left) > 100) {
        auto fut = std::async(std::launch::async, parallel_quicksort, 
                             std::ref(data), left, j, depth + 1);
        parallel_quicksort(data, i, right, depth + 1);
        fut.wait();
    } else {
        parallel_quicksort(data, left, j, depth + 1);
        parallel_quicksort(data, i, right, depth + 1);
    }
}

void demo_parallel_sort() {
    std::cout << "\n=== EXAMPLE 6: Parallel QuickSort ===" << std::endl;
    
    // Generate random data
    std::vector<int> data(1000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);
    
    for (auto& val : data) {
        val = dis(gen);
    }
    
    std::cout << "Sorting " << data.size() << " elements..." << std::endl;
    
    // Parallel sort
    auto start = std::chrono::steady_clock::now();
    parallel_quicksort(data, 0, data.size() - 1);
    auto end = std::chrono::steady_clock::now();
    auto parallel_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Verify sorted
    bool is_sorted = std::is_sorted(data.begin(), data.end());
    std::cout << "Sorted correctly: " << std::boolalpha << is_sorted << std::endl;
    std::cout << "Parallel time: " << parallel_time.count() << " μs" << std::endl;
    
    std::cout << "First 10 elements: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║     ADVANCED FUTURE/ASYNC/PROMISE PATTERNS              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    
    demo_map_reduce();
    demo_pipeline();
    demo_work_queue();
    demo_timeout_handling();
    demo_error_fallback();
    demo_parallel_sort();
    
    std::cout << "\n✓ All advanced examples completed!" << std::endl;
    return 0;
}
