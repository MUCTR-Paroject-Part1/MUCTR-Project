#include "TestFramework.h"
#include "../src/core/Manager.h"
#include "../src/core/BurgerInventory.h"
#include <thread>
#include <chrono>
#include <vector>

TEST(Manager_Restock) {
    BurgerInventory inventory(5);
    Manager manager(inventory, 10);
    
    manager.start();
    
    // Call manager to restock
    manager.callForRestock(0);
    
    // Give manager time to process
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    ASSERT_EQ(15, inventory.getStock(0));
    
    manager.stop();
}

TEST(Manager_MultipleRestocks) {
    BurgerInventory inventory(5);
    Manager manager(inventory, 5);
    
    manager.start();
    
    manager.callForRestock(0);
    manager.callForRestock(1);
    manager.callForRestock(2);
    
    // Give manager time to process
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    ASSERT_EQ(10, inventory.getStock(0));
    ASSERT_EQ(10, inventory.getStock(1));
    ASSERT_EQ(10, inventory.getStock(2));
    
    manager.stop();
}

TEST(Manager_InvalidBurgerType) {
    BurgerInventory inventory(5);
    Manager manager(inventory, 10);
    
    manager.start();
    
    // Invalid burger type should not cause issues
    manager.callForRestock(-1);
    manager.callForRestock(3);
    manager.callForRestock(100);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Stock should remain unchanged
    ASSERT_EQ(5, inventory.getStock(0));
    ASSERT_EQ(5, inventory.getStock(1));
    ASSERT_EQ(5, inventory.getStock(2));
    
    manager.stop();
}

TEST(Manager_StopBeforeProcessing) {
    BurgerInventory inventory(5);
    Manager manager(inventory, 10);
    
    manager.start();
    
    manager.callForRestock(0);
    
    // Stop immediately
    manager.stop();
    
    // Manager should stop gracefully
    // Stock may or may not be updated depending on timing
    // This test mainly ensures no crash
}

TEST(Manager_ThreadSafety) {
    BurgerInventory inventory(5);
    Manager manager(inventory, 1);
    
    manager.start();
    
    const int numThreads = 5;
    std::vector<std::thread> threads;
    
    // Multiple threads calling manager
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&manager, i]() {
            manager.callForRestock(i % BurgerInventory::BURGER_TYPES);
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // Give manager time to process
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    manager.stop();
    
    // All types should have been restocked at least once
    ASSERT_GE(inventory.getStock(0), 6);
    ASSERT_GE(inventory.getStock(1), 6);
    ASSERT_GE(inventory.getStock(2), 6);
}

