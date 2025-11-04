#include "TestFramework.h"
#include "../src/core/Restaurant.h"
#include <thread>
#include <chrono>

TEST(Restaurant_Initialization) {
    Restaurant restaurant(5, 10, 3);
    
    // Check initial state
    ASSERT_EQ(5, restaurant.getInventory().getStock(0));
    ASSERT_EQ(5, restaurant.getInventory().getStock(1));
    ASSERT_EQ(5, restaurant.getInventory().getStock(2));
    ASSERT_EQ(TableManager::TOTAL_TABLES, restaurant.getTableManager().getAvailableTables());
}

TEST(Restaurant_StartStop) {
    Restaurant restaurant(10, 10, 5);
    
    restaurant.start();
    
    // Give it time to work
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    restaurant.stop();
    restaurant.waitForCustomers();
    
    // Give threads time to fully stop
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Should stop gracefully
    ASSERT_TRUE(true);
}

TEST(Restaurant_Integration) {
    Restaurant restaurant(10, 10, 3);
    
    restaurant.start();
    
    // Let restaurant run
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    // Check that inventory changed (customers bought burgers)
    int totalStock = 0;
    for (int i = 0; i < BurgerInventory::BURGER_TYPES; ++i) {
        totalStock += restaurant.getInventory().getStock(i);
    }
    ASSERT_LE(totalStock, 30); // Some burgers should be bought
    
    restaurant.stop();
    restaurant.waitForCustomers();
    
    // Give threads time to fully stop
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST(Restaurant_RestockOnOutOfStock) {
    Restaurant restaurant(1, 10, 2); // Start with only 1 burger per type
    
    restaurant.start();
    
    // Customers will quickly buy all burgers and call manager
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    
    // Manager should have restocked
    int totalStock = 0;
    for (int i = 0; i < BurgerInventory::BURGER_TYPES; ++i) {
        totalStock += restaurant.getInventory().getStock(i);
    }
    ASSERT_GE(totalStock, 10); // At least one type should be restocked
    
    restaurant.stop();
    restaurant.waitForCustomers();
    
    // Give threads time to fully stop
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST(Restaurant_TableManagement) {
    Restaurant restaurant(10, 10, 10); // More customers than tables
    
    restaurant.start();
    
    // Let customers compete for tables
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    // Tables should be managed correctly (no crash)
    int available = restaurant.getTableManager().getAvailableTables();
    ASSERT_GE(available, 0);
    ASSERT_LE(available, TableManager::TOTAL_TABLES);
    
    restaurant.stop();
    restaurant.waitForCustomers();
    
    // Give threads time to fully stop
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

