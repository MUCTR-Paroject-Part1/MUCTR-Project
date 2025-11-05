#pragma once

#include <mutex>
#include <condition_variable>
#include <array>
#include <atomic>
#include <functional>

/**
 * @brief Thread-safe inventory management for burger types
 * 
 * Manages inventory for 3 types of burgers with thread-safe operations.
 * Notifies manager when a burger type runs out.
 */
class BurgerInventory {
public:
    static constexpr int BURGER_TYPES = 3;
    
    /**
     * @brief Constructor
     * @param initialStock Initial stock for each burger type
     */
    explicit BurgerInventory(int initialStock = 10);
    
    /**
     * @brief Attempt to buy a burger of specified type
     * @param burgerType Type of burger (0-2)
     * @return true if burger was purchased, false if out of stock
     */
    bool tryBuyBurger(int burgerType);
    
    /**
     * @brief Restock a burger type
     * @param burgerType Type of burger to restock (0-2)
     * @param quantity Amount to add
     */
    void restock(int burgerType, int quantity);
    
    /**
     * @brief Get current stock for a burger type
     * @param burgerType Type of burger (0-2)
     * @return Current stock count
     */
    int getStock(int burgerType) const;
    
    /**
     * @brief Check if a burger type is out of stock
     * @param burgerType Type of burger (0-2)
     * @return true if out of stock
     */
    bool isOutOfStock(int burgerType) const;
    
    /**
     * @brief Register a callback for when a burger type runs out
     * Used to notify manager
     */
    void setOutOfStockCallback(std::function<void(int)> callback);
    
private:
    mutable std::mutex mutex_;
    std::array<int, BURGER_TYPES> stock_;
    std::function<void(int)> outOfStockCallback_;
    
    void notifyOutOfStock(int burgerType);
};

