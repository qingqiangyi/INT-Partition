// File: ThreadStats.h���½�����ͷ�ļ���
#pragma once
#define NOMINMAX
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <cstddef>  // ֧�� size_t
#include <chrono>   // ֧�� std::chrono
#include <limits>


struct ThreadStats {
    size_t task_count = 0;
    std::chrono::duration<double> total_time{};
    std::chrono::duration<double> min_time{ std::chrono::duration<double>::zero() }; // ��ʼ��Ϊ�����ֵ
    std::chrono::duration<double> max_time{};
};


// ����Ϊextern��ͷ�ļ���ֻ���������壩
extern std::unordered_map<std::thread::id, ThreadStats> thread_stats;
extern std::mutex stats_mutex;

// File: ThreadStats.cpp���½�Դ�ļ���
#include "ThreadStats.h"

// ��Դ�ļ��ж���ȫ�ֱ���
std::unordered_map<std::thread::id, ThreadStats> thread_stats;
std::mutex stats_mutex; 

void print_thread_stats() {
    std::lock_guard<std::mutex> lock(stats_mutex);
    for (const auto& entry : thread_stats) {  // ���� C++11/14
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