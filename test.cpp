#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>

void cpuStressTest(int duration) {
    auto endTime = std::chrono::steady_clock::now() + std::chrono::seconds(duration);
    
    // Perform a large number of calculations
    while (std::chrono::steady_clock::now() < endTime) {
        volatile double result = 0.0; // Use volatile to prevent optimization
        for (long long i = 0; i < 1e8; ++i) {
            result += std::sin(i) * std::cos(i); // Some arbitrary calculations
        }
    }
}

int main() {
    int numThreads = std::thread::hardware_concurrency(); // Get the number of available threads
    int duration = 999999999; // Duration of the stress test in seconds

    std::vector<std::thread> threads;

    // Create threads to stress the CPU
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(cpuStressTest, duration);
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "CPU stress test completed." << std::endl;
    return 0;
}

