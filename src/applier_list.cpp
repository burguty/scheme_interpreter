#include "applier.h"
#include "error.h"

#include <memory>

AST Applier::ListOperations::OpIsList(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: list? expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in list?");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: list? expected only 1 argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (value_ast == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    if (!Is<Cell>(value_ast)) {
        return std::make_shared<Boolean>(false);
    }
    operand = As<Cell>(value_ast)->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            return std::make_shared<Boolean>(false);
        }
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Boolean>(true);
}

AST Applier::ListOperations::OpIsNull(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: null? expects operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in null?");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: null? expected only 1 argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    return std::make_shared<Boolean>(value_ast == nullptr);
}

AST Applier::ListOperations::OpIsPair(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: pair? expects operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in pair?");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: pair? expected only 1 argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (!Is<Cell>(value_ast)) {
        return std::make_shared<Boolean>(false);
    }
    auto cell_ast = As<Cell>(value_ast);
    bool ans = (cell_ast->GetFirst() != nullptr) && (cell_ast->GetSecond() != nullptr);
    return std::make_shared<Boolean>(ans);
}

AST Applier::ListOperations::OpList(std::shared_ptr<Cell> ast) {
    return ast->GetSecond();
}

AST Applier::ListOperations::OpCons(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: cons expects 1st operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in cons");
    }
    AST first_value = Apply(As<Cell>(operand)->GetFirst());

    operand = As<Cell>(operand)->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: cons expects 2nd operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in cons");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: cons expected only 2 arguments");
    }
    AST second_value = Apply(As<Cell>(operand)->GetFirst());

    return std::make_shared<Cell>(first_value, second_value);
}

AST Applier::ListOperations::OpCar(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: car expects operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in car");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: car expected only 1 argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (value_ast == nullptr || !Is<Cell>(value_ast)) {
        throw RuntimeError("Runtime error: car expected not empty list");
    }
    return As<Cell>(value_ast)->GetFirst();
}

AST Applier::ListOperations::OpCdr(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: cdr expects operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in cdr");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: cdr expected only 1 argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (value_ast == nullptr || !Is<Cell>(value_ast)) {
        throw RuntimeError("Runtime error: cdr expected not empty list");
    }
    return As<Cell>(value_ast)->GetSecond();
}

AST Applier::ListOperations::OpListRef(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: list-ref expects 1st operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in list-ref");
    }
    AST copy_second_son = std::make_shared<Cell>(As<Cell>(operand)->GetFirst(), nullptr);
    AST first_arg = As<Cell>(operand)->GetFirst();

    operand = As<Cell>(operand)->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: list-ref expects 2st operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in list-ref");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: list-ref expected only 2 arguments");
    }
    AST second_arg = As<Cell>(operand)->GetFirst();

    auto to_check_list = std::make_shared<Cell>(std::make_shared<Number>(0), copy_second_son);
    if (!As<Boolean>(OpIsList(As<Cell>(to_check_list)))->GetValue()) {
        throw RuntimeError("Runtime error: list-ref catched invalid list");
    }

    AST list_ast = Apply(first_arg);
    if (list_ast == nullptr || !Is<Cell>(list_ast)) {
        throw RuntimeError("Runtime error: list-ref catched invalid list");
    }

    AST index_ast = Apply(second_arg);
    if (!Is<Number>(index_ast)) {
        throw RuntimeError("Runtime error: invalid index in list-ref");
    }
    int index = As<Number>(index_ast)->GetValue();
    if (index < 0) {
        throw RuntimeError("Runtime error: invalid index in list-ref");
    }

    operand = list_ast;
    int current = 0;
    while (operand) {
        if (current == index) {
            return As<Cell>(operand)->GetFirst();
        }
        ++current;
        operand = As<Cell>(operand)->GetSecond();
    }
    throw RuntimeError("Runtime error: index out of range in list-ref");
}

AST Applier::ListOperations::OpListTail(std::shared_ptr<Cell> ast) {
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: list-tail expects 1st operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in list-tail");
    }
    AST copy_second_son = std::make_shared<Cell>(As<Cell>(operand)->GetFirst(), nullptr);
    AST first_arg = As<Cell>(operand)->GetFirst();

    operand = As<Cell>(operand)->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: list-tail expects 2st operand");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in list-tail");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: list-tail expected only 2 arguments");
    }
    AST second_arg = As<Cell>(operand)->GetFirst();

    auto to_check_list = std::make_shared<Cell>(std::make_shared<Number>(0), copy_second_son);
    if (!As<Boolean>(OpIsList(to_check_list))->GetValue()) {
        throw RuntimeError("Runtime error: list-tail catched invalid list");
    }

    AST list_ast = Apply(first_arg);
    if (list_ast == nullptr || !Is<Cell>(list_ast)) {
        throw RuntimeError("Runtime error: list-tail catched invalid list");
    }

    AST index_ast = Apply(second_arg);
    if (!Is<Number>(index_ast)) {
        throw RuntimeError("Runtime error: invalid index in list-tail");
    }
    int index = As<Number>(index_ast)->GetValue();
    if (index < 0) {
        throw RuntimeError("Runtime error: invalid index in list-tail");
    }

    operand = list_ast;
    int current = 0;
    while (operand) {
        if (current == index) {
            return operand;
        }
        ++current;
        operand = As<Cell>(operand)->GetSecond();
    }
    if (current == index) {
        return nullptr;
    }
    throw RuntimeError("Runtime error: index out of range in list-ref");
}
