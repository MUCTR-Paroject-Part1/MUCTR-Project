#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

/**
 * @brief Simple unit test framework
 */
class TestFramework {
public:
    struct TestCase {
        std::string name;
        std::function<void()> test;
        bool passed;
    };
    
    static TestFramework& instance() {
        static TestFramework inst;
        return inst;
    }
    
    void addTest(const std::string& name, std::function<void()> test) {
        tests_.push_back({name, test, false});
    }
    
    int runAll() {
        std::cout << "=== Запуск тестов ===" << std::endl;
        std::cout << "Всего тестов: " << tests_.size() << std::endl << std::endl;
        
        int passed = 0;
        int failed = 0;
        
        for (auto& test : tests_) {
            std::cout << "[TEST] " << test.name << " ... ";
            try {
                test.test();
                test.passed = true;
                ++passed;
                std::cout << "PASS" << std::endl;
            } catch (const std::exception& e) {
                test.passed = false;
                ++failed;
                std::cout << "FAIL: " << e.what() << std::endl;
            } catch (...) {
                test.passed = false;
                ++failed;
                std::cout << "FAIL: Unknown error" << std::endl;
            }
        }
        
        std::cout << std::endl;
        std::cout << "=== Результаты ===" << std::endl;
        std::cout << "Пройдено: " << passed << std::endl;
        std::cout << "Провалено: " << failed << std::endl;
        std::cout << "Всего: " << tests_.size() << std::endl;
        
        return failed;
    }
    
private:
    std::vector<TestCase> tests_;
};

// Macros for assertions
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition " is false"); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw std::runtime_error("Assertion failed: " #condition " is true"); \
    }

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw std::runtime_error("Assertion failed: expected " + std::to_string(expected) + \
                                ", got " + std::to_string(actual)); \
    }

#define ASSERT_NE(expected, actual) \
    if ((expected) == (actual)) { \
        throw std::runtime_error("Assertion failed: values are equal"); \
    }

#define ASSERT_GE(expected, actual) \
    if ((actual) < (expected)) { \
        throw std::runtime_error("Assertion failed: " + std::to_string(actual) + \
                                " < " + std::to_string(expected)); \
    }

#define ASSERT_LE(expected, actual) \
    if ((actual) > (expected)) { \
        throw std::runtime_error("Assertion failed: " + std::to_string(actual) + \
                                " > " + std::to_string(expected)); \
    }

// Test registration macro
#define TEST(name) \
    void test_##name(); \
    namespace { \
        struct TestRegistrar_##name { \
            TestRegistrar_##name() { \
                TestFramework::instance().addTest(#name, test_##name); \
            } \
        } test_registrar_##name; \
    } \
    void test_##name()

