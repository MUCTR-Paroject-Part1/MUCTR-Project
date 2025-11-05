#pragma once

#include <mutex>
#include <condition_variable>

/**
 * @brief Thread-safe table management
 * 
 * Manages 5 tables in the restaurant. Customers wait for available tables
 * using condition variables.
 */
class TableManager {
public:
    static constexpr int TOTAL_TABLES = 5;
    
    /**
     * @brief Constructor
     */
    TableManager();
    
    /**
     * @brief Reserve a table (blocks if all tables are occupied)
     * Thread-safe operation that waits for available table
     */
    void reserveTable();
    
    /**
     * @brief Release a table
     * Thread-safe operation that notifies waiting customers
     */
    void releaseTable();
    
    /**
     * @brief Get number of available tables
     * @return Number of free tables
     */
    int getAvailableTables() const;
    
private:
    mutable std::mutex mutex_;
    std::condition_variable tableAvailable_;
    int availableTables_;
};

