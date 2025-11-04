#include "Restaurant.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Set up console for UTF-8 (Windows)
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif
    
    // Create restaurant with:
    // - Initial stock: 10 burgers of each type
    // - Restock quantity: 10 burgers
    // - Number of customers: 8
    Restaurant restaurant(10, 10, 8);
    
    // Start simulation
    restaurant.start();
    
    // Run simulation for 30 seconds
    std::this_thread::sleep_for(std::chrono::seconds(30));
    
    // Print status periodically
    for (int i = 0; i < 6; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        restaurant.printStatus();
    }
    
    // Stop restaurant
    std::cout << "\n=== Закрытие ресторана ===" << std::endl;
    restaurant.stop();
    
    // Wait for all threads to finish
    restaurant.waitForCustomers();
    
    // Final status
    restaurant.printStatus();
    
    std::cout << "=== Ресторан закрыт ===" << std::endl;
    
    return 0;
}

