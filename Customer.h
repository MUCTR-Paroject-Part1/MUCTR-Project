#pragma once

#include <thread>
#include <atomic>
#include <random>
#include <chrono>
#include "BurgerInventory.h"
#include "TableManager.h"

/**
 * @brief Customer thread actor
 * 
 * Represents a customer that buys a burger and sits at a table.
 */
class Customer {
public:
    /**
     * @brief Constructor
     * @param id Customer ID
     * @param inventory Reference to burger inventory
     * @param tableManager Reference to table manager
     * @param manager Reference to manager (for calling when out of stock)
     */
    Customer(int id, 
             BurgerInventory& inventory, 
             TableManager& tableManager,
             class Manager& manager);
    
    /**
     * @brief Start customer thread
     */
    void start();
    
    /**
     * @brief Stop customer thread
     */
    void stop();
    
    /**
     * @brief Wait for customer thread to finish
     */
    void join();
    
    /**
     * @brief Get customer ID
     */
    int getId() const;
    
private:
    int id_;
    BurgerInventory& inventory_;
    TableManager& tableManager_;
    Manager& manager_;
    std::thread customerThread_;
    std::atomic<bool> running_;
    
    // Random number generators (thread-local would be better, but this works)
    std::mt19937 rng_;
    std::uniform_int_distribution<int> burgerTypeDist_;
    std::uniform_int_distribution<int> eatingTimeDist_;
    
    void customerLoop();
    int chooseBurgerType();
    void waitForTableAndEat();
};

