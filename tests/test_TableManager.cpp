#include "TestFramework.h"
#include "../src/core/TableManager.h"
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

TEST(TableManager_InitialState) {
    TableManager manager;
    ASSERT_EQ(TableManager::TOTAL_TABLES, manager.getAvailableTables());
}

TEST(TableManager_ReserveAndRelease) {
    TableManager manager;
    
    manager.reserveTable();
    ASSERT_EQ(TableManager::TOTAL_TABLES - 1, manager.getAvailableTables());
    
    manager.releaseTable();
    ASSERT_EQ(TableManager::TOTAL_TABLES, manager.getAvailableTables());
}

TEST(TableManager_ReserveAllTables) {
    TableManager manager;
    
    for (int i = 0; i < TableManager::TOTAL_TABLES; ++i) {
        manager.reserveTable();
    }
    
    ASSERT_EQ(0, manager.getAvailableTables());
    
    // Release all
    for (int i = 0; i < TableManager::TOTAL_TABLES; ++i) {
        manager.releaseTable();
    }
    
    ASSERT_EQ(TableManager::TOTAL_TABLES, manager.getAvailableTables());
}

TEST(TableManager_ThreadSafety) {
    TableManager manager;
    const int numThreads = 10;
    std::vector<std::thread> threads;
    std::atomic<int> reservedCount(0);
    std::atomic<int> releasedCount(0);
    
    // Reserve tables from multiple threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&]() {
            manager.reserveTable();
            ++reservedCount;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            manager.releaseTable();
            ++releasedCount;
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    ASSERT_EQ(numThreads, reservedCount.load());
    ASSERT_EQ(numThreads, releasedCount.load());
    ASSERT_EQ(TableManager::TOTAL_TABLES, manager.getAvailableTables());
}

TEST(TableManager_WaitForTable) {
    TableManager manager;
    std::atomic<bool> tableReserved(false);
    std::atomic<bool> testPassed(false);
    
    // Reserve all tables
    for (int i = 0; i < TableManager::TOTAL_TABLES; ++i) {
        manager.reserveTable();
    }
    
    // Thread that waits for a table
    std::thread waiter([&]() {
        tableReserved = true;
        manager.reserveTable(); // Should block until table is available
        testPassed = true;
        manager.releaseTable();
    });
    
    // Wait a bit to ensure waiter is blocked
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_TRUE(tableReserved.load());
    ASSERT_FALSE(testPassed.load()); // Should still be waiting
    
    // Release a table
    manager.releaseTable();
    
    // Wait for waiter to finish
    waiter.join();
    
    ASSERT_TRUE(testPassed.load());
    ASSERT_EQ(TableManager::TOTAL_TABLES, manager.getAvailableTables());
}

TEST(TableManager_MultipleWaiters) {
    TableManager manager;
    const int numWaiters = 3;
    std::vector<std::thread> waiters;
    std::atomic<int> finishedCount(0);
    
    // Reserve all tables
    for (int i = 0; i < TableManager::TOTAL_TABLES; ++i) {
        manager.reserveTable();
    }
    
    // Create waiting threads
    for (int i = 0; i < numWaiters; ++i) {
        waiters.emplace_back([&]() {
            manager.reserveTable();
            ++finishedCount;
            manager.releaseTable();
        });
    }
    
    // Wait a bit
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_EQ(0, finishedCount.load());
    
    // Release tables one by one
    for (int i = 0; i < numWaiters; ++i) {
        manager.releaseTable();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Wait for all waiters
    for (auto& t : waiters) {
        t.join();
    }
    
    ASSERT_EQ(numWaiters, finishedCount.load());
}

