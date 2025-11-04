#include "TestFramework.h"

// Include all test files
// Tests are auto-registered via TEST() macro when files are compiled

int main() {
    // Set up console for UTF-8 (Windows)
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif
    
    return TestFramework::instance().runAll();
}

