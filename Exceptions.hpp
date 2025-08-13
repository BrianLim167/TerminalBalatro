#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

// Base exception class for all Balatro-specific errors
class BalatroException : public std::runtime_error {
public:
    explicit BalatroException(const std::string& message) 
        : std::runtime_error("Balatro Error: " + message) {}
};

// Game state related exceptions
class GameStateException : public BalatroException {
public:
    explicit GameStateException(const std::string& message)
        : BalatroException("Game State Error: " + message) {}
};

// Card and hand related exceptions
class CardException : public BalatroException {
public:
    explicit CardException(const std::string& message)
        : BalatroException("Card Error: " + message) {}
};

// Invalid hand operations
class InvalidHandException : public CardException {
public:
    explicit InvalidHandException(const std::string& message)
        : CardException("Invalid Hand: " + message) {}
};

// Shop and economy related exceptions
class ShopException : public BalatroException {
public:
    explicit ShopException(const std::string& message)
        : BalatroException("Shop Error: " + message) {}
};

// Insufficient funds for purchases
class InsufficientFundsException : public ShopException {
public:
    explicit InsufficientFundsException(const std::string& item, int cost, int available)
        : ShopException("Cannot afford " + item + " (cost: $" + std::to_string(cost) + 
                       ", available: $" + std::to_string(available) + ")") {}
};

// File I/O related exceptions
class FileException : public BalatroException {
public:
    explicit FileException(const std::string& message)
        : BalatroException("File Error: " + message) {}
};

// Save/load specific exceptions
class SaveLoadException : public FileException {
public:
    explicit SaveLoadException(const std::string& filename, const std::string& operation)
        : FileException("Failed to " + operation + " file: " + filename) {}
};

// Input validation exceptions
class InputException : public BalatroException {
public:
    explicit InputException(const std::string& message)
        : BalatroException("Input Error: " + message) {}
};

// Invalid range or bounds
class InvalidRangeException : public InputException {
public:
    InvalidRangeException(int value, int min, int max)
        : InputException("Value " + std::to_string(value) + " out of range [" + 
                        std::to_string(min) + ", " + std::to_string(max) + "]") {}
};

// Joker system exceptions
class JokerException : public BalatroException {
public:
    explicit JokerException(const std::string& message)
        : BalatroException("Joker Error: " + message) {}
};

// Unknown joker type
class UnknownJokerException : public JokerException {
public:
    explicit UnknownJokerException(const std::string& joker_name)
        : JokerException("Unknown joker type: " + joker_name) {}
};

#endif // EXCEPTIONS_HPP