#include "applier.h"
#include "error.h"

#include <memory>

AST Applier::BooleanOperations::OpIsBoolean(std::shared_ptr<Cell> ast) {
    bool ans = false;
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: boolean? expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in boolean?");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: unary boolean? expected one argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    ans = Is<Boolean>(value_ast);
    return std::make_shared<Boolean>(ans);
}

AST Applier::BooleanOperations::OpNot(std::shared_ptr<Cell> ast) {
    bool ans = false;
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: not expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in not");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: unary not expected one argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (Is<Boolean>(value_ast)) {
        ans = !As<Boolean>(value_ast)->GetValue();
    }
    return std::make_shared<Boolean>(ans);
}

AST Applier::BooleanOperations::OpAnd(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    AST last_expr = std::make_shared<Boolean>(true);
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: and expects expression");
        }
        last_expr = Apply(As<Cell>(operand)->GetFirst());
        bool current = true;
        if (Is<Boolean>(last_expr)) {
            current = As<Boolean>(last_expr)->GetValue();
        }
        if (!current) {
            return last_expr;
        }
        operand = As<Cell>(operand)->GetSecond();
    }
    return last_expr;
}

AST Applier::BooleanOperations::OpOr(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    AST last_expr = std::make_shared<Boolean>(false);
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: and expects expression");
        }
        last_expr = Apply(As<Cell>(operand)->GetFirst());
        bool current = true;
        if (Is<Boolean>(last_expr)) {
            current = As<Boolean>(last_expr)->GetValue();
        }
        if (current) {
            return last_expr;
        }
        operand = As<Cell>(operand)->GetSecond();
    }
    return last_expr;
}

