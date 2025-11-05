#include "TestFramework.h"
#include "../src/core/BurgerInventory.h"
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

TEST(BurgerInventory_InitialStock) {
    BurgerInventory inventory(5);
    ASSERT_EQ(5, inventory.getStock(0));
    ASSERT_EQ(5, inventory.getStock(1));
    ASSERT_EQ(5, inventory.getStock(2));
}

TEST(BurgerInventory_BuyBurger) {
    BurgerInventory inventory(10);
    
    bool bought = inventory.tryBuyBurger(0);
    ASSERT_TRUE(bought);
    ASSERT_EQ(9, inventory.getStock(0));
    
    bought = inventory.tryBuyBurger(0);
    ASSERT_TRUE(bought);
    ASSERT_EQ(8, inventory.getStock(0));
}

TEST(BurgerInventory_BuyOutOfStock) {
    BurgerInventory inventory(1);
    
    bool bought = inventory.tryBuyBurger(0);
    ASSERT_TRUE(bought);
    ASSERT_EQ(0, inventory.getStock(0));
    
    bought = inventory.tryBuyBurger(0);
    ASSERT_FALSE(bought);
    ASSERT_EQ(0, inventory.getStock(0));
}

TEST(BurgerInventory_Restock) {
    BurgerInventory inventory(5);
    
    inventory.restock(0, 3);
    ASSERT_EQ(8, inventory.getStock(0));
    
    inventory.restock(1, 10);
    ASSERT_EQ(15, inventory.getStock(1));
}

TEST(BurgerInventory_IsOutOfStock) {
    BurgerInventory inventory(1);
    
    ASSERT_FALSE(inventory.isOutOfStock(0));
    
    inventory.tryBuyBurger(0);
    ASSERT_TRUE(inventory.isOutOfStock(0));
    
    inventory.restock(0, 5);
    ASSERT_FALSE(inventory.isOutOfStock(0));
}

TEST(BurgerInventory_InvalidBurgerType) {
    BurgerInventory inventory(10);
    
    bool bought = inventory.tryBuyBurger(-1);
    ASSERT_FALSE(bought);
    
    bought = inventory.tryBuyBurger(3);
    ASSERT_FALSE(bought);
    
    bought = inventory.tryBuyBurger(100);
    ASSERT_FALSE(bought);
}

TEST(BurgerInventory_ThreadSafety) {
    BurgerInventory inventory(100);
    const int numThreads = 10;
    const int purchasesPerThread = 10;
    std::vector<std::thread> threads;
    
    // Multiple threads buying burgers
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&inventory, i]() {
            for (int j = 0; j < purchasesPerThread; ++j) {
                int burgerType = j % BurgerInventory::BURGER_TYPES;
                inventory.tryBuyBurger(burgerType);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // Should have bought 10 * 10 = 100 burgers total
    int totalPurchased = 0;
    for (int i = 0; i < BurgerInventory::BURGER_TYPES; ++i) {
        totalPurchased += (100 - inventory.getStock(i));
    }
    
    ASSERT_EQ(100, totalPurchased);
}

TEST(BurgerInventory_Callback) {
    BurgerInventory inventory(1);
    int callbackCount = 0;
    int lastBurgerType = -1;
    
    inventory.setOutOfStockCallback([&](int type) {
        ++callbackCount;
        lastBurgerType = type;
    });
    
    // Buy the last burger
    inventory.tryBuyBurger(0);
    ASSERT_EQ(1, callbackCount);
    ASSERT_EQ(0, lastBurgerType);
    
    // Try to buy again (should not trigger callback)
    inventory.tryBuyBurger(0);
    ASSERT_EQ(1, callbackCount); // Should still be 1
}

TEST(BurgerInventory_MultipleTypes) {
    BurgerInventory inventory(5);
    
    inventory.tryBuyBurger(0);
    inventory.tryBuyBurger(1);
    inventory.tryBuyBurger(2);
    
    ASSERT_EQ(4, inventory.getStock(0));
    ASSERT_EQ(4, inventory.getStock(1));
    ASSERT_EQ(4, inventory.getStock(2));
}

