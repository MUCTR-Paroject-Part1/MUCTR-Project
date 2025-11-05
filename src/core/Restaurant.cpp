#include "Restaurant.h"
#include <iostream>

Restaurant::Restaurant(int initialStock, int restockQuantity, int numCustomers)
    : inventory_(initialStock)
    , manager_(inventory_, restockQuantity) {
    
    // Create customers
    for (int i = 0; i < numCustomers; ++i) {
        customers_.push_back(std::make_unique<Customer>(
            i + 1, inventory_, tableManager_, manager_));
    }
    
    setupManagerCallback();
}

Restaurant::~Restaurant() {
    stop();
}

void Restaurant::start() {
    std::cout << "=== Ресторан 'Сытная еда' открылся ===" << std::endl;
    std::cout << "Начальный запас каждого типа бургеров: " 
              << inventory_.getStock(0) << std::endl;
    std::cout << "Количество столиков: " << TableManager::TOTAL_TABLES << std::endl;
    std::cout << "Количество покупателей: " << customers_.size() << std::endl;
    std::cout << std::endl;
    
    // Start manager
    manager_.start();
    
    // Start all customers
    for (auto& customer : customers_) {
        customer->start();
    }
}

void Restaurant::stop() {
    // Stop all customers
    for (auto& customer : customers_) {
        if (customer) {
            customer->stop();
        }
    }
    
    // Stop manager
    manager_.stop();
}

void Restaurant::waitForCustomers() {
    for (auto& customer : customers_) {
        customer->join();
    }
}

void Restaurant::printStatus() const {
    std::cout << "\n=== Статус ресторана ===" << std::endl;
    for (int i = 0; i < BurgerInventory::BURGER_TYPES; ++i) {
        std::cout << "Бургер типа " << i << ": " 
                  << inventory_.getStock(i) << " в наличии" << std::endl;
    }
    std::cout << "Свободных столиков: " 
              << tableManager_.getAvailableTables() << std::endl;
    std::cout << std::endl;
}

void Restaurant::setupManagerCallback() {
    // Set up callback so inventory can notify manager
    inventory_.setOutOfStockCallback([this](int burgerType) {
        manager_.callForRestock(burgerType);
    });
}

