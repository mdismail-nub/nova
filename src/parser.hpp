#ifndef NOVA_PARSER_HPP
#define NOVA_PARSER_HPP

#include "ast.hpp"
#include "token.hpp"

#include <vector>
#include <memory>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    Program parse();

private:
    const std::vector<Token>& tokens;
    size_t current = 0;

    const Token& peek() const;
    const Token& previous() const;

    bool isAtEnd() const;
    const Token& advance();

    bool check(TokenType type) const;
    bool match(TokenType type);

    const Token& consume(
        TokenType type,
        const std::string& message
    );

    std::unique_ptr<Stmt> statement();

    std::unique_ptr<Stmt> letStatement();
    std::unique_ptr<Stmt> sayStatement();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> primary();
};

#endif
