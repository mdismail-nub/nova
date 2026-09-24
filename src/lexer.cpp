#include "lexer.hpp"

#include <cctype>

Lexer::Lexer(const std::string& source)
    : source(source) {
}

char Lexer::peek() const {
    if (current >= source.length()) {
        return '\0';
    }

    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.length()) {
        return '\0';
    }

    return source[current + 1];
}

char Lexer::advance() {
    if (current >= source.length()) {
        return '\0';
    }

    return source[current++];
}

void Lexer::addToken(TokenType type, const std::string& text) {
    tokens.push_back({
        type,
        text,
        line
    });
}

void Lexer::skipWhitespace() {
    while (true) {
        char c = peek();

        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        }
        else if (c == '\n') {
            advance();
            line++;
        }
        else {
            break;
        }
    }
}

void Lexer::scanString() {
    // Skip opening quote
    advance();

    std::string value;

    while (peek() != '"' && peek() != '\0') {
        if (peek() == '\n') {
            line++;
        }

        value += advance();
    }

    // Closing quote
    if (peek() == '"') {
        advance();
    }

    addToken(TokenType::String, value);
}

void Lexer::scanNumber() {
    std::string number;

    while (std::isdigit(peek())) {
        number += advance();
    }

    // Decimal number
    if (peek() == '.' && std::isdigit(peekNext())) {
        number += advance();

        while (std::isdigit(peek())) {
            number += advance();
        }
    }

    addToken(TokenType::Number, number);
}

void Lexer::scanIdentifier() {
    std::string text;

    while (
        std::isalnum(static_cast<unsigned char>(peek())) ||
        peek() == '_'
    ) {
        text += advance();
    }

    if (text == "let") {
        addToken(TokenType::Let, text);
    }
    else if (text == "say") {
        addToken(TokenType::Say, text);
    }
    else if (text == "if") {
        addToken(TokenType::If, text);
    }
    else if (text == "otherwise") {
        addToken(TokenType::Otherwise, text);
    }
    else if (text == "true") {
        addToken(TokenType::True, text);
    }
    else if (text == "false") {
        addToken(TokenType::False, text);
    }
    else {
        addToken(TokenType::Identifier, text);
    }
}

void Lexer::scanToken() {
    char c = peek();

    // Comments
    if (c == '#') {
        while (peek() != '\n' && peek() != '\0') {
            advance();
        }

        return;
    }

    // Strings
    if (c == '"') {
        scanString();
        return;
    }

    // Numbers
    if (std::isdigit(static_cast<unsigned char>(c))) {
        scanNumber();
        return;
    }

    // Identifiers / keywords
    if (
        std::isalpha(static_cast<unsigned char>(c)) ||
        c == '_'
    ) {
        scanIdentifier();
        return;
    }

    advance();

    switch (c) {
        case '+':
            addToken(TokenType::Plus, "+");
            break;

        case '-':
            addToken(TokenType::Minus, "-");
            break;

        case '*':
            addToken(TokenType::Star, "*");
            break;

        case '/':
            addToken(TokenType::Slash, "/");
            break;

        case '{':
            addToken(TokenType::LeftBrace, "{");
            break;

        case '}':
            addToken(TokenType::RightBrace, "}");
            break;

        case '(':
            addToken(TokenType::LeftParen, "(");
            break;

        case ')':
            addToken(TokenType::RightParen, ")");
            break;

        case '=':
            if (peek() == '=') {
                advance();
                addToken(TokenType::EqualEqual, "==");
            }
            else {
                addToken(TokenType::Equal, "=");
            }
            break;

        case '!':
            if (peek() == '=') {
                advance();
                addToken(TokenType::NotEqual, "!=");
            }
            else {
                addToken(TokenType::Unknown, "!");
            }
            break;

        case '>':
            if (peek() == '=') {
                advance();
                addToken(TokenType::GreaterEqual, ">=");
            }
            else {
                addToken(TokenType::Greater, ">");
            }
            break;

        case '<':
            if (peek() == '=') {
                advance();
                addToken(TokenType::LessEqual, "<=");
            }
            else {
                addToken(TokenType::Less, "<");
            }
            break;

        default:
            addToken(TokenType::Unknown, std::string(1, c));
            break;
    }
}

std::vector<Token> Lexer::tokenize() {
    tokens.clear();
    current = 0;
    line = 1;

    while (current < source.length()) {
        skipWhitespace();

        if (current >= source.length()) {
            break;
        }

        scanToken();
    }

    tokens.push_back({
        TokenType::EndOfFile,
        "",
        line
    });

    return tokens;
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Let: return "LET";
        case TokenType::Say: return "SAY";
        case TokenType::If: return "IF";
        case TokenType::Otherwise: return "OTHERWISE";

        case TokenType::Identifier: return "IDENTIFIER";
        case TokenType::Number: return "NUMBER";
        case TokenType::String: return "STRING";
        case TokenType::True: return "TRUE";
        case TokenType::False: return "FALSE";

        case TokenType::Plus: return "PLUS";
        case TokenType::Minus: return "MINUS";
        case TokenType::Star: return "STAR";
        case TokenType::Slash: return "SLASH";

        case TokenType::Equal: return "EQUAL";
        case TokenType::EqualEqual: return "EQUAL_EQUAL";
        case TokenType::NotEqual: return "NOT_EQUAL";

        case TokenType::Greater: return "GREATER";
        case TokenType::Less: return "LESS";
        case TokenType::GreaterEqual: return "GREATER_EQUAL";
        case TokenType::LessEqual: return "LESS_EQUAL";

        case TokenType::LeftBrace: return "LEFT_BRACE";
        case TokenType::RightBrace: return "RIGHT_BRACE";
        case TokenType::LeftParen: return "LEFT_PAREN";
        case TokenType::RightParen: return "RIGHT_PAREN";

        case TokenType::EndOfFile: return "EOF";
        case TokenType::Unknown: return "UNKNOWN";
    }

    return "UNKNOWN";
}
