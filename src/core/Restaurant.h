#pragma once

#include <vector>
#include <memory>
#include "BurgerInventory.h"
#include "TableManager.h"
#include "Manager.h"
#include "Customer.h"

/**
 * @brief Main restaurant orchestrator
 * 
 * Coordinates all components of the restaurant simulation.
 */
class Restaurant {
public:
    /**
     * @brief Constructor
     * @param initialStock Initial stock for each burger type
     * @param restockQuantity Amount manager restocks
     * @param numCustomers Number of customer threads
     */
    Restaurant(int initialStock = 10, 
               int restockQuantity = 10, 
               int numCustomers = 10);
    
    /**
     * @brief Destructor - stops all threads
     */
    ~Restaurant();
    
    /**
     * @brief Start the restaurant simulation
     */
    void start();
    
    /**
     * @brief Stop the restaurant simulation
     */
    void stop();
    
    /**
     * @brief Wait for all customer threads to finish
     */
    void waitForCustomers();
    
    /**
     * @brief Get current inventory status
     */
    void printStatus() const;
    
    /**
     * @brief Get references to components (for testing)
     */
    BurgerInventory& getInventory() { return inventory_; }
    TableManager& getTableManager() { return tableManager_; }
    Manager& getManager() { return manager_; }
    
private:
    BurgerInventory inventory_;
    TableManager tableManager_;
    Manager manager_;
    std::vector<std::unique_ptr<Customer>> customers_;
    
    void setupManagerCallback();
};

