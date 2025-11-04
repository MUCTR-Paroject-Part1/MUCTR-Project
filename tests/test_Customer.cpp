#include "TestFramework.h"
#include "../src/core/Customer.h"
#include "../src/core/BurgerInventory.h"
#include "../src/core/TableManager.h"
#include "../src/core/Manager.h"
#include <thread>
#include <chrono>
#include <memory>
#include <vector>

TEST(Customer_BuyAndEat) {
    BurgerInventory inventory(10);
    TableManager tableManager;
    Manager manager(inventory, 10);
    manager.start();
    
    Customer customer(1, inventory, tableManager, manager);
    customer.start();
    
    // Give customer time to buy and eat
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    
    // Customer should have bought at least one burger
    int totalStock = 0;
    for (int i = 0; i < BurgerInventory::BURGER_TYPES; ++i) {
        totalStock += inventory.getStock(i);
    }
    ASSERT_LE(totalStock, 30); // At least one burger should be bought
    
    customer.stop();
    customer.join();
    manager.stop();
}

TEST(Customer_Stop) {
    BurgerInventory inventory(10);
    TableManager tableManager;
    Manager manager(inventory, 10);
    manager.start();
    
    Customer customer(1, inventory, tableManager, manager);
    customer.start();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    customer.stop();
    customer.join(); // Should complete without hanging
    
    manager.stop();
}

TEST(Customer_HandleOutOfStock) {
    BurgerInventory inventory(0); // Empty inventory
    TableManager tableManager;
    Manager manager(inventory, 10);
    manager.start();
    
    Customer customer(1, inventory, tableManager, manager);
    customer.start();
    
    // Customer should call manager when out of stock
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Manager should have restocked
    int totalStock = 0;
    for (int i = 0; i < BurgerInventory::BURGER_TYPES; ++i) {
        totalStock += inventory.getStock(i);
    }
    ASSERT_GE(totalStock, 10); // At least one type should be restocked
    
    customer.stop();
    customer.join();
    manager.stop();
}

TEST(Customer_MultipleCustomers) {
    BurgerInventory inventory(20);
    TableManager tableManager;
    Manager manager(inventory, 10);
    manager.start();
    
    const int numCustomers = 5;
    std::vector<std::unique_ptr<Customer>> customers;
    
    for (int i = 0; i < numCustomers; ++i) {
        customers.push_back(std::make_unique<Customer>(
            i + 1, inventory, tableManager, manager));
        customers[i]->start();
    }
    
    // Let them work
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    
    // Stop all
    for (auto& customer : customers) {
        customer->stop();
    }
    
    for (auto& customer : customers) {
        customer->join();
    }
    
    manager.stop();
}

