// File: ThreadStats.h（新建独立头文件）
#pragma once
#define NOMINMAX
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <cstddef>  // 支持 size_t
#include <chrono>   // 支持 std::chrono
#include <limits>


struct ThreadStats {
    size_t task_count = 0;
    std::chrono::duration<double> total_time{};
    std::chrono::duration<double> min_time{ std::chrono::duration<double>::zero() }; // 初始化为最大正值
    std::chrono::duration<double> max_time{};
};


// 声明为extern（头文件中只声明不定义）
extern std::unordered_map<std::thread::id, ThreadStats> thread_stats;
extern std::mutex stats_mutex;

// File: ThreadStats.cpp（新建源文件）
#include "ThreadStats.h"

// 在源文件中定义全局变量
std::unordered_map<std::thread::id, ThreadStats> thread_stats;
std::mutex stats_mutex; 

void print_thread_stats() {
    std::lock_guard<std::mutex> lock(stats_mutex);
    for (const auto& entry : thread_stats) {  // 兼容 C++11/14
        const auto& thread_id = entry.first;
        const auto& stats = entry.second;

        std::cout << "\nThread " << thread_id << ":\n"
            << "  Tasks executed: " << stats.task_count << "\n"
            << "  Total time: " << stats.total_time.count() << " s\n"
            << "  Average time: " << stats.total_time.count() / stats.task_count << " s\n"
            << "  Min time: " << stats.min_time.count() << " s\n"
            << "  Max time: " << stats.max_time.count() << " s\n";
    }
}
