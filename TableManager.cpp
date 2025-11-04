#include "TableManager.h"

TableManager::TableManager() : availableTables_(TOTAL_TABLES) {
}

void TableManager::reserveTable() {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // Wait until at least one table is available
    tableAvailable_.wait(lock, [this] { return availableTables_ > 0; });
    
    --availableTables_;
}

void TableManager::releaseTable() {
    std::lock_guard<std::mutex> lock(mutex_);
    ++availableTables_;
    tableAvailable_.notify_one();
}

int TableManager::getAvailableTables() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return availableTables_;
}

