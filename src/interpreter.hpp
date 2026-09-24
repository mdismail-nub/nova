#ifndef NOVA_INTERPRETER_HPP
#define NOVA_INTERPRETER_HPP

#include "ast.hpp"

#include <string>
#include <unordered_map>

struct RuntimeValue {
    enum class Type {
        Number,
        String,
        Boolean,
        None
    };

    Type type = Type::None;

    double number = 0;
    std::string string;
    bool boolean = false;
};

class Interpreter {
public:
    void run(const Program& program);

private:
    std::unordered_map<std::string, RuntimeValue> variables;

    void execute(const Stmt* statement);

    RuntimeValue evaluate(const Expr* expression);

    RuntimeValue evaluateBinary(
        const BinaryExpr* expression
    );

    RuntimeValue getVariable(
        const std::string& name
    );

    void printValue(
        const RuntimeValue& value
    );

    bool isTruthy(
        const RuntimeValue& value
    );
};

#endif
