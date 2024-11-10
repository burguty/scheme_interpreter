#include "applier.h"
#include "error.h"

std::unordered_map<std::string, ApplyFunction> Applier::functors = {
    {"number?", Applier::IntegerOperations::OpIsNumber},
    {"+", Applier::IntegerOperations::OpPlus},
    {"-", Applier::IntegerOperations::OpMinus},
    {"*", Applier::IntegerOperations::OpMultiply},
    {"/", Applier::IntegerOperations::OpDivide},
    {"max", Applier::IntegerOperations::OpMax},
    {"min", Applier::IntegerOperations::OpMin},
    {"abs", Applier::IntegerOperations::OpAbs},
    {"=", Applier::IntegerOperations::OpEqual},
    {"<", Applier::IntegerOperations::OpLess},
    {">", Applier::IntegerOperations::OpGreater},
    {"<=", Applier::IntegerOperations::OpLessEqual},
    {">=", Applier::IntegerOperations::OpGreaterEqual},
    {"boolean?", Applier::BooleanOperations::OpIsBoolean},
    {"not", Applier::BooleanOperations::OpNot},
    {"and", Applier::BooleanOperations::OpAnd},
    {"or", Applier::BooleanOperations::OpOr},
    {"list?", Applier::ListOperations::OpIsList},
    {"null?", Applier::ListOperations::OpIsNull},
    {"pair?", Applier::ListOperations::OpIsPair},
    {"cons", Applier::ListOperations::OpCons},
    {"car", Applier::ListOperations::OpCar},
    {"cdr", Applier::ListOperations::OpCdr},
    {"list", Applier::ListOperations::OpList},
    {"list-ref", Applier::ListOperations::OpListRef},
    {"list-tail", Applier::ListOperations::OpListTail}};

AST Applier::Apply(AST ast) {
    if (ast == nullptr) {
        throw RuntimeError("Runtime error: empty command");
    }
    if (Is<Number>(ast)) {
        return ast;
    } else if (Is<Boolean>(ast)) {
        return ast;
    } else if (Is<Quote>(ast)) {
        return QuoteOperations::OpQuote(As<Quote>(ast));
    } else if (Is<Symbol>(ast)) {
        return ast;
    } else if (Is<Cell>(ast)) {
        auto cell_ast = As<Cell>(ast);
        auto operation_ast = Apply(cell_ast->GetFirst());
        if (!Is<Symbol>(operation_ast)) {
            throw RuntimeError("Runtime Error: incorrect operation");
        }
        std::string operation = As<Symbol>(operation_ast)->GetName();
        if (!functors.contains(operation)) {
            throw RuntimeError("Runtime error: unknown command");
        }
        return functors[operation](cell_ast);
    } else {
        throw RuntimeError("Runtime error: unknown command");
    }
}

ApplyFunction Applier::GetApplyFunction(const std::string& arg) {
    if (!functors.contains(arg)) {
        throw RuntimeError("Runtime error: unknown command");
    }
    return functors[arg];
}
