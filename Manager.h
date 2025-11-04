#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <functional>
#include "BurgerInventory.h"

/**
 * @brief Manager thread that restocks burgers
 * 
 * Waits for calls when burger types run out, then restocks them.
 */
class Manager {
public:
    /**
     * @brief Constructor
     * @param inventory Reference to burger inventory
     * @param restockQuantity Amount to restock when called
     */
    Manager(BurgerInventory& inventory, int restockQuantity = 10);
    
    /**
     * @brief Destructor - stops the manager thread
     */
    ~Manager();
    
    /**
     * @brief Start the manager thread
     */
    void start();
    
    /**
     * @brief Stop the manager thread
     */
    void stop();
    
    /**
     * @brief Call manager to restock a burger type
     * @param burgerType Type of burger to restock
     */
    void callForRestock(int burgerType);
    
private:
    BurgerInventory& inventory_;
    int restockQuantity_;
    std::thread managerThread_;
    std::mutex mutex_;
    std::condition_variable restockRequest_;
    std::atomic<bool> running_;
    int pendingRestockType_;
    bool hasPendingRequest_;
    
    void managerLoop();
};

