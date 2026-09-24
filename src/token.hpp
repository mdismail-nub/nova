#ifndef NOVA_TOKEN_HPP
#define NOVA_TOKEN_HPP

#include <string>

enum class TokenType {
    // Keywords
    Let,
    Say,
    If,
    Otherwise,

    // Values
    Identifier,
    Number,
    String,
    True,
    False,

    // Operators
    Plus,
    Minus,
    Star,
    Slash,

    Equal,
    EqualEqual,
    NotEqual,

    Greater,
    Less,
    GreaterEqual,
    LessEqual,

    // Symbols
    LeftBrace,
    RightBrace,
    LeftParen,
    RightParen,

    // Special
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string text;
    int line;
};

std::string tokenTypeToString(TokenType type);

#endif
