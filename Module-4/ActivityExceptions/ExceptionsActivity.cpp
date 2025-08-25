// Exceptions.cpp : Demonstrates secure exception handling in C++
// This version is updated to follow secure coding and best practices for exception safety

#include <iostream>
#include <stdexcept>    // For standard exceptions
#include <exception>    // For base exception types
#include <string>       // For string manipulation
#include <limits>       // For numeric limits
#include <cstdlib>      // For EXIT_SUCCESS and EXIT_FAILURE

// ========== Custom Exception Class ==========
class CustomException : public std::exception {
public:
    const char* what() const noexcept override {
        return "CustomException: A controlled application-specific failure occurred.";
    }
};

// ========== Function: Performs Custom Logic and Throws Standard Exception ==========
bool do_even_more_custom_application_logic()
{
    std::cout << "[INFO] Running Even More Custom Application Logic..." << std::endl;

    // Simulate failure with a standard exception
    throw std::runtime_error("RuntimeError: Simulated logic failure in deep processing.");

    return true;  // Unreachable but kept for completeness
}

// ========== Function: Wraps Nested Logic with std::exception Catch ==========
void do_custom_application_logic()
{
    std::cout << "[INFO] Running Custom Application Logic..." << std::endl;

    try {
        if (do_even_more_custom_application_logic()) {
            std::cout << "[INFO] Nested Logic Executed Successfully." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Exception Caught: " << e.what() << std::endl;
        // Recoverable: continue execution
    }

    // Throwing custom exception to test explicit catch
    throw CustomException();

    // Note: Unreachable below, but keeping for flow awareness
    std::cout << "[INFO] Leaving Custom Application Logic." << std::endl;
}

// ========== Function: Safely Performs Division ==========
float divide(const float num, const float den)
{
    if (den == 0.0f) {
        throw std::invalid_argument("InvalidArgument: Division by zero is not allowed.");
    }

    // Defensive: Check for overflow (though rare for float)
    if (num > std::numeric_limits<float>::max() / den) {
        throw std::overflow_error("OverflowError: Division result is too large.");
    }

    return num / den;
}

// ========== Function: Handles Division and Its Exceptions ==========
void do_division() noexcept
{
    constexpr float numerator = 10.0f;
    constexpr float denominator = 0.0f;

    try {
        float result = divide(numerator, denominator);
        std::cout << "[RESULT] divide(" << numerator << ", " << denominator << ") = " << result << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "[ERROR] Invalid Argument Caught: " << e.what() << std::endl;
    }
    catch (const std::overflow_error& e) {
        std::cerr << "[ERROR] Overflow Error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] General Exception Caught: " << e.what() << std::endl;
    }
}

// ========== MAIN Function: Top-Level Exception Guard ==========
int main()
{
    std::cout << "=== Exception Handling Test - Secure Edition ===" << std::endl;

    try {
        do_division();
        do_custom_application_logic();
    }
    catch (const CustomException& ce) {
        std::cerr << "[ERROR] Custom Exception: " << ce.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Standard Exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "[CRITICAL] Unknown Error Occurred. Aborting gracefully." << std::endl;
    }

    std::cout << "[INFO] Program completed without crashing. Exiting safely." << std::endl;

    return EXIT_SUCCESS;
}
