#include "Customer.h"
#include "Manager.h"
#include <iostream>
#include <thread>

Customer::Customer(int id, 
                   BurgerInventory& inventory, 
                   TableManager& tableManager,
                   Manager& manager)
    : id_(id)
    , inventory_(inventory)
    , tableManager_(tableManager)
    , manager_(manager)
    , running_(false)
    , rng_(std::chrono::steady_clock::now().time_since_epoch().count() + id)
    , burgerTypeDist_(0, BurgerInventory::BURGER_TYPES - 1)
    , eatingTimeDist_(1000, 5000) { // 1-5 seconds
}

void Customer::start() {
    if (running_) {
        return;
    }
    
    running_ = true;
    customerThread_ = std::thread(&Customer::customerLoop, this);
}

void Customer::stop() {
    running_ = false;
}

void Customer::join() {
    if (customerThread_.joinable()) {
        customerThread_.join();
    }
}

int Customer::getId() const {
    return id_;
}

void Customer::customerLoop() {
    while (running_) {
        int burgerType = chooseBurgerType();
        
        // Try to buy burger
        bool bought = inventory_.tryBuyBurger(burgerType);
        
        if (!bought) {
            // Out of stock - call manager
            std::cout << "[Покупатель " << id_ << "] Бургеры типа " << burgerType 
                      << " закончились. Зову менеджера!" << std::endl;
            manager_.callForRestock(burgerType);
            
            // Wait a bit and try again
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }
        
        std::cout << "[Покупатель " << id_ << "] Купил бургер типа " << burgerType << std::endl;
        
        // Get a table and eat
        waitForTableAndEat();
        
        // Small delay before next purchase
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int Customer::chooseBurgerType() {
    return burgerTypeDist_(rng_);
}

void Customer::waitForTableAndEat() {
    // Reserve a table (blocks if all are occupied)
    tableManager_.reserveTable();
    
    std::cout << "[Покупатель " << id_ << "] Села за столик" << std::endl;
    
    // Eat for random time
    int eatingTime = eatingTimeDist_(rng_);
    std::this_thread::sleep_for(std::chrono::milliseconds(eatingTime));
    
    // Release table
    tableManager_.releaseTable();
    std::cout << "[Покупатель " << id_ << "] Освободила столик" << std::endl;
}

