# Balatro C++

A faithful C++ implementation of the popular poker-based roguelike card game Balatro. Beat 8 antes by building powerful poker hands and collecting game-changing jokers!

## 🎮 Game Overview

Balatro is a poker-based roguelike where you must score enough chips with poker hands to beat increasingly difficult blinds. Each ante consists of three blinds (Small, Big, Boss) with escalating chip requirements.

### Core Mechanics

- **Goal**: Beat 8 antes to win the game
- **Resources**: 4 hands, 3 discards, 8 cards per blind
- **Scoring**: `(Base Chips + Card Chips) × Multiplier`
- **Economy**: Earn $1 for each unused hand, spend money on jokers in the shop
- **Jokers**: Powerful modifiers that enhance your scoring potential

### Hand Types

Standard poker hands plus Balatro-exclusive hands:

- **Five of a Kind**: Five cards of the same rank
- **Flush House**: Full House + Flush (all same suit)
- **Flush Five**: Five of a Kind + Flush (all same suit)

## 🚀 Quick Start

### Prerequisites

- C++17 compatible compiler (GCC 7+ or Clang 5+)
- Make build system
- Unix-like environment (Linux, macOS, WSL)

### Installation

```bash
# Clone the repository
git clone <repository-url>
cd balatro-cpp

# Build the game
make

# Run the game
make run
```

### Alternative Build Commands

```bash
make debug    # Build with debug symbols
make release  # Build optimized version
make test     # Build and run test suite
make clean    # Clean build artifacts
```

## 🎯 How to Play

1. **Start a New Game**: Choose option 1 from the main menu
2. **Play Hands**: Select up to 5 cards to form poker hands
3. **Score Points**: Earn chips based on hand type and card values
4. **Beat Blinds**: Score enough chips to meet the blind requirement
5. **Shop Phase**: Buy jokers between blinds to enhance your scoring
6. **Progress**: Advance through 8 antes, each with 3 blinds

### Controls

- **Number Selection**: Enter card numbers (e.g., `1,3,5` or `2 4 7`)
- **Menu Navigation**: Use number keys to select options
- **Hand Preview**: Option 2 in game menu shows scoring without playing
- **Save/Load**: Save your progress anytime during gameplay

## 🏗️ Architecture

### Core Components

```
├── main.cpp           # Game entry point and main loop
├── GameManager.*      # Core game state and logic
├── Card.*            # Playing card representation
├── Poker.*           # Hand evaluation and scoring
├── Joker.*           # Joker system with polymorphic design
├── Shop.*            # Shop mechanics and inventory
├── Statistics.*      # Game statistics tracking
├── Utils.*           # UI utilities and helper functions
└── tests/            # Comprehensive test suite
```

### Key Classes

- **GameManager**: Orchestrates game flow, manages state transitions
- **PokerHand**: Evaluates poker hands including Balatro-specific types
- **Joker**: Abstract base class for scoring modifiers
- **Shop**: Handles joker purchasing with rarity-based pricing
- **Statistics**: Tracks performance metrics and achievements

## 🧪 Testing

Run the comprehensive test suite:

```bash
make test
```

Test coverage includes:

- Card and poker hand evaluation
- Joker effect calculations
- Game state management
- Utility functions
- Error handling scenarios

## 🔧 Development

### Building from Source

```bash
# Debug build with symbols
make debug

# Release build with optimizations
make release

# Check compiler and dependencies
make check

# Generate dependency information
make depend
```

### Code Style

- Modern C++17 features
- RAII and smart pointer usage
- Comprehensive error handling
- Extensive inline documentation with EFFECTS/REQUIRES/MODIFIES

### Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

## 📊 Features

### ✅ Implemented

- Complete poker hand evaluation (including Balatro hands)
- **Hand Preview System**: See detailed scoring breakdown before playing
- Joker system with multiple types and rarities
- Shop mechanics with dynamic pricing
- Save/load game functionality
- Statistics tracking
- ASCII art card display
- Sound effects
- Comprehensive test suite

### 🚧 Planned Improvements

- Memory leak detection and optimization
- Performance profiling and optimization
- Enhanced error handling and logging
- Extended joker collection
- Improved UI/UX

## 🎵 Audio

The game includes basic sound effects for:

- Menu navigation
- Successful actions
- Errors and warnings
- Game events

## 📈 Statistics

Track your performance with detailed statistics:

- Games played and win rate
- Highest ante reached
- Favorite hand types
- Joker usage patterns
- Total money earned

## 🐛 Troubleshooting

### Common Issues

**Build Errors**

```bash
# Ensure you have C++17 support
g++ --version

# Clean and rebuild
make clean && make
```

**Save File Issues**

- Save files are stored as `balatro_save.dat`
- Statistics saved as `balatro_stats.dat`
- Delete these files to reset progress

**Performance Issues**

- Use release build for better performance: `make release`
- Ensure terminal supports ANSI colors for best experience

## 📝 License

[Add your license information here]

## 🙏 Acknowledgments

- Inspired by the original Balatro game
- Built with modern C++ best practices
- Comprehensive testing framework

## 📞 Support

For issues, questions, or contributions:

- Open an issue on GitHub
- Check the troubleshooting section
- Review the test suite for usage examples

---

**Enjoy playing Balatro C++!** 🃏✨
