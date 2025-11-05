#include "Manager.h"
#include <iostream>

Manager::Manager(BurgerInventory& inventory, int restockQuantity)
    : inventory_(inventory)
    , restockQuantity_(restockQuantity)
    , running_(false)
    , pendingRestockType_(-1)
    , hasPendingRequest_(false) {
}

Manager::~Manager() {
    stop();
}

void Manager::start() {
    if (running_) {
        return;
    }
    
    running_ = true;
    managerThread_ = std::thread(&Manager::managerLoop, this);
}

void Manager::stop() {
    if (!running_) {
        return;
    }
    
    running_ = false;
    restockRequest_.notify_all();
    
    if (managerThread_.joinable()) {
        managerThread_.join();
    }
}

void Manager::callForRestock(int burgerType) {
    if (burgerType < 0 || burgerType >= BurgerInventory::BURGER_TYPES) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    pendingRestockType_ = burgerType;
    hasPendingRequest_ = true;
    restockRequest_.notify_one();
}

void Manager::managerLoop() {
    while (running_) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait for restock request
        restockRequest_.wait(lock, [this] { 
            return !running_ || hasPendingRequest_; 
        });
        
        if (!running_) {
            break;
        }
        
        if (hasPendingRequest_) {
            int burgerType = pendingRestockType_;
            hasPendingRequest_ = false;
            lock.unlock();
            
            // Restock the burger type
            inventory_.restock(burgerType, restockQuantity_);
            std::cout << "[Менеджер] Пополнил запас бургеров типа " << burgerType 
                      << " на " << restockQuantity_ << " штук" << std::endl;
        }
    }
}

