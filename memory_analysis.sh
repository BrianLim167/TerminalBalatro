#!/bin/bash

# Memory Analysis Helper Script for Balatro C++
# This script provides easy access to various memory analysis tools

set -e

echo "Balatro C++ Memory Analysis Helper"
echo "=================================="
echo

# Check if required tools are available
check_tool() {
    if ! command -v $1 &> /dev/null; then
        echo "❌ $1 is not installed"
        return 1
    else
        echo "✅ $1 is available"
        return 0
    fi
}

echo "Checking available tools:"
VALGRIND_AVAILABLE=false
ASAN_AVAILABLE=false

if check_tool valgrind; then
    VALGRIND_AVAILABLE=true
fi

if check_tool g++; then
    # Test if AddressSanitizer is available
    if echo 'int main(){}' | g++ -fsanitize=address -x c++ - -o /tmp/asan_test 2>/dev/null; then
        echo "✅ AddressSanitizer is available"
        ASAN_AVAILABLE=true
        rm -f /tmp/asan_test
    else
        echo "❌ AddressSanitizer is not available"
    fi
fi

echo

# Menu system
show_menu() {
    echo "Available Memory Analysis Options:"
    echo "1. Run tests with AddressSanitizer (recommended)"
    echo "2. Run game with AddressSanitizer"
    echo "3. Run tests with Valgrind (detailed analysis)"
    echo "4. Run game with Valgrind"
    echo "5. Run performance benchmarks"
    echo "6. Quick memory leak test"
    echo "7. Exit"
    echo
}

run_asan_tests() {
    echo "Building and running tests with AddressSanitizer..."
    echo "This will detect memory leaks, buffer overflows, and other memory errors."
    echo
    make asan-test
    echo
    echo "Running tests with AddressSanitizer..."
    echo "Press Ctrl+C to stop if needed."
    echo
    ./test_runner
}

run_asan_game() {
    echo "Building game with AddressSanitizer..."
    echo "This will detect memory leaks, buffer overflows, and other memory errors."
    echo
    make asan
    echo
    echo "Game built with AddressSanitizer. Starting game..."
    echo "Press Ctrl+C to stop if needed."
    echo
    ./balatro
}

run_valgrind_tests() {
    if [ "$VALGRIND_AVAILABLE" = false ]; then
        echo "❌ Valgrind is not available. Please install it first."
        echo "On macOS: brew install valgrind"
        echo "On Ubuntu: sudo apt-get install valgrind"
        return 1
    fi
    
    echo "Running tests with Valgrind..."
    echo "This provides detailed memory analysis but runs slower."
    echo
    make memtest
}

run_valgrind_game() {
    if [ "$VALGRIND_AVAILABLE" = false ]; then
        echo "❌ Valgrind is not available. Please install it first."
        return 1
    fi
    
    echo "Running game with Valgrind..."
    echo "This provides detailed memory analysis but runs slower."
    echo
    make memcheck
}

run_benchmarks() {
    echo "Running performance benchmarks..."
    echo "This helps identify performance bottlenecks."
    echo
    make benchmark
}

quick_memory_test() {
    echo "Running quick memory leak test..."
    echo "This builds with AddressSanitizer and runs a subset of tests."
    echo
    
    if [ "$ASAN_AVAILABLE" = true ]; then
        make asan-test
        echo
        echo "Running memory-focused tests only..."
        # We could create a specific memory test target here
        ./test_runner | grep -A 20 "Memory Management Tests"
    else
        echo "❌ AddressSanitizer not available. Falling back to regular tests."
        make test
    fi
}

# Main loop
while true; do
    show_menu
    read -p "Choose an option (1-7): " choice
    echo
    
    case $choice in
        1)
            run_asan_tests
            ;;
        2)
            run_asan_game
            ;;
        3)
            run_valgrind_tests
            ;;
        4)
            run_valgrind_game
            ;;
        5)
            run_benchmarks
            ;;
        6)
            quick_memory_test
            ;;
        7)
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "❌ Invalid option. Please choose 1-7."
            ;;
    esac
    
    echo
    echo "Press Enter to continue..."
    read
    echo
done