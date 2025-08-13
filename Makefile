# Makefile for Balatro C++ Clone
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g
OPTIMIZE = -O2

# Memory debugging flags
ASAN_FLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

# Directories
SRC_DIR = .
OBJ_DIR = obj
TEST_DIR = tests

# Source files (excluding main.cpp and test files)
SOURCES = Card.cpp Poker.cpp Joker.cpp GameManager.cpp Shop.cpp Utils.cpp Statistics.cpp HandPreview.cpp ImprovedShuffle.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

# Main executable
TARGET = balatro
MAIN_OBJ = $(OBJ_DIR)/main.o

# Test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*_test.cpp) $(TEST_DIR)/test_runner.cpp
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_TARGET = test_runner

# Benchmark files
BENCHMARK_TARGET = benchmark_exe
BENCHMARK_OBJ = $(OBJ_DIR)/benchmark_main.o

# Deck analysis files
DECK_ANALYSIS_TARGET = deck_analysis
DECK_ANALYSIS_OBJ = $(OBJ_DIR)/deck_analysis.o

# Hand size comparison files
HAND_COMPARISON_TARGET = hand_comparison
HAND_COMPARISON_OBJ = $(OBJ_DIR)/hand_size_comparison.o

# Default target
.PHONY: all
all: $(TARGET)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Main executable
$(TARGET): $(OBJECTS) $(MAIN_OBJ) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(OPTIMIZE) $^ -o $@

# Main object file
$(MAIN_OBJ): main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Object files for source (with headers)
$(OBJ_DIR)/Card.o: Card.cpp Card.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Poker.o: Poker.cpp Poker.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Joker.o: Joker.cpp Joker.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/GameManager.o: GameManager.cpp GameManager.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Shop.o: Shop.cpp Shop.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Utils.o: Utils.cpp Utils.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Statistics.o: Statistics.cpp Statistics.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/HandPreview.o: HandPreview.cpp HandPreview.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/ImprovedShuffle.o: ImprovedShuffle.cpp ImprovedShuffle.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test executable
$(TEST_TARGET): $(OBJECTS) $(TEST_OBJECTS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Benchmark executable
$(BENCHMARK_TARGET): $(OBJECTS) $(BENCHMARK_OBJ) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(OPTIMIZE) $^ -o $@

# Benchmark object file
$(BENCHMARK_OBJ): benchmark_main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Deck analysis executable
$(DECK_ANALYSIS_TARGET): $(OBJECTS) $(DECK_ANALYSIS_OBJ) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(OPTIMIZE) $^ -o $@

# Deck analysis object file
$(DECK_ANALYSIS_OBJ): deck_analysis.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Hand comparison executable
$(HAND_COMPARISON_TARGET): $(OBJECTS) $(HAND_COMPARISON_OBJ) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(OPTIMIZE) $^ -o $@

# Hand comparison object file
$(HAND_COMPARISON_OBJ): hand_size_comparison.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test object files
$(OBJ_DIR)/%_test.o: $(TEST_DIR)/%_test.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test_runner.o: $(TEST_DIR)/test_runner.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Phony targets
.PHONY: clean test debug release run memcheck asan asan-run asan-test asan-test-run profile benchmark deck-analysis hand-comparison

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET) $(BENCHMARK_TARGET) $(DECK_ANALYSIS_TARGET) $(HAND_COMPARISON_TARGET) benchmark gmon.out profile.txt

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Debug build (default includes -g)
debug: CXXFLAGS += -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -DNDEBUG $(OPTIMIZE)
release: clean $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Memory leak detection with Valgrind
memcheck: $(TARGET)
	@echo "Running Valgrind memory check..."
	valgrind $(VALGRIND_FLAGS) ./$(TARGET)

# AddressSanitizer build
asan: CXXFLAGS += $(ASAN_FLAGS)
asan: clean $(TARGET)
	@echo "Built with AddressSanitizer. Run './$(TARGET)' to execute with memory checking."

# AddressSanitizer build and run
asan-run: CXXFLAGS += $(ASAN_FLAGS)
asan-run: clean $(TARGET)
	@echo "Running with AddressSanitizer..."
	./$(TARGET)

# Memory leak test with Valgrind
memtest: $(TEST_TARGET)
	@echo "Running test suite with Valgrind..."
	valgrind $(VALGRIND_FLAGS) ./$(TEST_TARGET)

# AddressSanitizer test build
asan-test: CXXFLAGS += $(ASAN_FLAGS)
asan-test: clean $(TEST_TARGET)
	@echo "Built test suite with AddressSanitizer. Run './$(TEST_TARGET)' to execute with memory checking."

# AddressSanitizer test build and run
asan-test-run: CXXFLAGS += $(ASAN_FLAGS)
asan-test-run: clean $(TEST_TARGET)
	@echo "Running test suite with AddressSanitizer..."
	./$(TEST_TARGET)

# Profile with gprof
profile: CXXFLAGS += -pg
profile: clean $(TARGET)
	@echo "Built with profiling enabled. Run './$(TARGET)' then 'gprof $(TARGET) gmon.out > profile.txt'"

# Run benchmarks
benchmark: $(BENCHMARK_TARGET)
	@echo "Running performance benchmarks..."
	./$(BENCHMARK_TARGET)

# Run deck analysis
deck-analysis: $(DECK_ANALYSIS_TARGET)
	@echo "Running deck shuffling analysis..."
	./$(DECK_ANALYSIS_TARGET)

# Run hand size comparison
hand-comparison: $(HAND_COMPARISON_TARGET)
	@echo "Running hand size probability comparison..."
	./$(HAND_COMPARISON_TARGET)

# Install (optional - copies to /usr/local/bin)
.PHONY: install
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Check dependencies and compilation
.PHONY: check
check:
	$(CXX) --version
	@echo "Checking source files..."
	@ls -la *.cpp *.hpp

# Generate dependencies automatically
.PHONY: depend
depend:
	$(CXX) -MM $(SOURCES) main.cpp > .depend

# Include dependency file if it exists
-include .depend

# Help target
.PHONY: help
help:
	@echo "Balatro C++ Clone - Makefile Help"
	@echo "=================================="
	@echo "Available targets:"
	@echo "  all      - Build the main executable (default)"
	@echo "  test     - Build and run test suite"
	@echo "  clean    - Remove all build artifacts"
	@echo "  debug    - Build with debug symbols and debug macros"
	@echo "  release  - Build optimized release version"
	@echo "  run      - Build and run the program"
	@echo "  install  - Install to /usr/local/bin (requires sudo)"
	@echo "  check    - Check compiler and source files"
	@echo "  depend   - Generate dependency file"
	@echo "  help     - Show this help message"
	@echo "  memcheck - Run with Valgrind memory leak detection"
	@echo "  asan     - Build with AddressSanitizer (run manually)"
	@echo "  asan-run - Build and run with AddressSanitizer"
	@echo "  memtest  - Run test suite with Valgrind"
	@echo "  asan-test- Build test suite with AddressSanitizer (run manually)"
	@echo "  asan-test-run - Build and run test suite with AddressSanitizer"
	@echo "  profile  - Build with profiling enabled (gprof)"
	@echo "  benchmark- Build and run performance benchmarks"
	@echo ""
	@echo "Examples:"
	@echo "  make           # Build the program"
	@echo "  make run       # Build and run"
	@echo "  make test      # Run tests"
	@echo "  make clean     # Clean build files"
	@echo "  make release   # Optimized build"
	@echo "  ./memory_analysis.sh  # Interactive memory analysis helper"

# Force rebuild
.PHONY: rebuild
rebuild: clean all