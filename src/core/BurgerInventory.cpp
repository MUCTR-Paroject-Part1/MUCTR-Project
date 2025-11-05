#include "BurgerInventory.h"
#include <algorithm>

BurgerInventory::BurgerInventory(int initialStock) {
    stock_.fill(initialStock);
}

bool BurgerInventory::tryBuyBurger(int burgerType) {
    if (burgerType < 0 || burgerType >= BURGER_TYPES) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (stock_[burgerType] > 0) {
        --stock_[burgerType];
        
        // Check if this was the last burger
        if (stock_[burgerType] == 0) {
            notifyOutOfStock(burgerType);
        }
        
        return true;
    }
    
    return false;
}

void BurgerInventory::restock(int burgerType, int quantity) {
    if (burgerType < 0 || burgerType >= BURGER_TYPES || quantity <= 0) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    stock_[burgerType] += quantity;
}

int BurgerInventory::getStock(int burgerType) const {
    if (burgerType < 0 || burgerType >= BURGER_TYPES) {
        return 0;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    return stock_[burgerType];
}

bool BurgerInventory::isOutOfStock(int burgerType) const {
    if (burgerType < 0 || burgerType >= BURGER_TYPES) {
        return true;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    return stock_[burgerType] == 0;
}

void BurgerInventory::setOutOfStockCallback(std::function<void(int)> callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    outOfStockCallback_ = std::move(callback);
}

void BurgerInventory::notifyOutOfStock(int burgerType) {
    if (outOfStockCallback_) {
        outOfStockCallback_(burgerType);
    }
}

