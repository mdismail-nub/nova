#include "parser.hpp"

#include <iostream>
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens) {
}

const Token& Parser::peek() const {
    return tokens[current];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EndOfFile;
}

const Token& Parser::advance() {
    if (!isAtEnd()) {
        current++;
    }

    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) {
        return type == TokenType::EndOfFile;
    }

    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (!check(type)) {
        return false;
    }

    advance();
    return true;
}

const Token& Parser::consume(
    TokenType type,
    const std::string& message
) {
    if (check(type)) {
        return advance();
    }

    throw std::runtime_error(
        message + " at line " +
        std::to_string(peek().line)
    );
}

Program Parser::parse() {
    Program program;

    while (!isAtEnd()) {
        program.statements.push_back(statement());
    }

    return program;
}

std::unique_ptr<Stmt> Parser::statement() {

    if (match(TokenType::Let)) {
        return letStatement();
    }

    if (match(TokenType::Say)) {
        return sayStatement();
    }

    throw std::runtime_error(
        "Unexpected token '" +
        peek().text +
        "' at line " +
        std::to_string(peek().line)
    );
}

std::unique_ptr<Stmt> Parser::letStatement() {

    const Token& name = consume(
        TokenType::Identifier,
        "Expected variable name"
    );

    consume(
        TokenType::Equal,
        "Expected '=' after variable name"
    );

    auto value = expression();

    return std::make_unique<LetStmt>(
        name.text,
        std::move(value)
    );
}

std::unique_ptr<Stmt> Parser::sayStatement() {

    auto value = expression();

    return std::make_unique<SayStmt>(
        std::move(value)
    );
}

std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {

    auto expr = comparison();

    while (
        match(TokenType::EqualEqual) ||
        match(TokenType::NotEqual)
    ) {
        std::string op = previous().text;

        auto right = comparison();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {

    auto expr = term();

    while (
        match(TokenType::Greater) ||
        match(TokenType::GreaterEqual) ||
        match(TokenType::Less) ||
        match(TokenType::LessEqual)
    ) {
        std::string op = previous().text;

        auto right = term();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term() {

    auto expr = factor();

    while (
        match(TokenType::Plus) ||
        match(TokenType::Minus)
    ) {
        std::string op = previous().text;

        auto right = factor();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {

    auto expr = primary();

    while (
        match(TokenType::Star) ||
        match(TokenType::Slash)
    ) {
        std::string op = previous().text;

        auto right = primary();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            op,
            std::move(right)
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::primary() {

    if (match(TokenType::Number)) {
        return std::make_unique<NumberExpr>(
            std::stod(previous().text)
        );
    }

    if (match(TokenType::String)) {
        return std::make_unique<StringExpr>(
            previous().text
        );
    }

    if (match(TokenType::True)) {
        return std::make_unique<BooleanExpr>(true);
    }

    if (match(TokenType::False)) {
        return std::make_unique<BooleanExpr>(false);
    }

    if (match(TokenType::Identifier)) {
        return std::make_unique<VariableExpr>(
            previous().text
        );
    }

    if (match(TokenType::LeftParen)) {

        auto expr = expression();

        consume(
            TokenType::RightParen,
            "Expected ')'"
        );

        return expr;
    }

    throw std::runtime_error(
        "Expected expression at line " +
        std::to_string(peek().line)
    );
}
