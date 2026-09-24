#ifndef NOVA_LEXER_HPP
#define NOVA_LEXER_HPP

#include "token.hpp"

#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(const std::string& source);

    std::vector<Token> tokenize();

private:
    std::string source;
    std::vector<Token> tokens;

    size_t current = 0;
    int line = 1;

    char peek() const;
    char peekNext() const;
    char advance();

    void addToken(TokenType type, const std::string& text);

    void scanToken();

    void scanString();
    void scanNumber();
    void scanIdentifier();

    void skipWhitespace();
};

#endif

