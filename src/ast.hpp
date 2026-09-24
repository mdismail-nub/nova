#ifndef NOVA_AST_HPP
#define NOVA_AST_HPP

#include <memory>
#include <string>
#include <vector>

struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    double value;

    explicit NumberExpr(double value)
        : value(value) {}
};

struct StringExpr : Expr {
    std::string value;

    explicit StringExpr(const std::string& value)
        : value(value) {}
};

struct BooleanExpr : Expr {
    bool value;

    explicit BooleanExpr(bool value)
        : value(value) {}
};

struct VariableExpr : Expr {
    std::string name;

    explicit VariableExpr(const std::string& name)
        : name(name) {}
};

struct BinaryExpr : Expr {
    std::unique_ptr<Expr> left;
    std::string op;
    std::unique_ptr<Expr> right;

    BinaryExpr(
        std::unique_ptr<Expr> left,
        const std::string& op,
        std::unique_ptr<Expr> right
    )
        : left(std::move(left)),
          op(op),
          right(std::move(right)) {}
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct LetStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> value;

    LetStmt(
        const std::string& name,
        std::unique_ptr<Expr> value
    )
        : name(name),
          value(std::move(value)) {}
};

struct SayStmt : Stmt {
    std::unique_ptr<Expr> expression;

    explicit SayStmt(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
};

struct Program {
    std::vector<std::unique_ptr<Stmt>> statements;
};

#endif
