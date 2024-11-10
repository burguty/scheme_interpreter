#include "applier.h"
#include "error.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>

namespace {
    static constexpr int64_t kMaxValue = 1e18;
    static constexpr int64_t kMinValue = -1e18;
}

AST Applier::IntegerOperations::OpIsNumber(std::shared_ptr<Cell> ast) {
    bool ans = false;
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: number? expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in number?");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: unary number? expected one argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    ans = Is<Number>(value_ast);
    return std::make_shared<Boolean>(ans);
}

AST Applier::IntegerOperations::OpPlus(std::shared_ptr<Cell> ast) {
    int64_t sum = 0;
    AST operand = ast->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in +");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in +");
        }
        sum += As<Number>(value_ast)->GetValue();
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Number>(sum);
}

AST Applier::IntegerOperations::OpMinus(std::shared_ptr<Cell> ast) {
    int64_t sum = 0;
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: substraction expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in -");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (!Is<Number>(value_ast)) {
        throw RuntimeError("Runtime error: expected number in -");
    }
    sum = As<Number>(value_ast)->GetValue();
    operand = As<Cell>(operand)->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in -");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in -");
        }
        sum -= As<Number>(value_ast)->GetValue();
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Number>(sum);
}

AST Applier::IntegerOperations::OpMultiply(std::shared_ptr<Cell> ast) {
    int64_t mult = 1;
    std::shared_ptr<Object> operand = ast->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in *");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in *");
        }
        mult *= As<Number>(value_ast)->GetValue();
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Number>(mult);
}

AST Applier::IntegerOperations::OpDivide(std::shared_ptr<Cell> ast) {
    int64_t div = 0;
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: divide expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in /");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (!Is<Number>(value_ast)) {
        throw RuntimeError("Runtime error: expected number in /");
    }
    div = As<Number>(value_ast)->GetValue();
    operand = As<Cell>(operand)->GetSecond();

    if (!operand) {
        throw RuntimeError("Runtime error: divide expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in /");
    }
    value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (!Is<Number>(value_ast)) {
        throw RuntimeError("Runtime error: expected number in /");
    }
    if (As<Number>(value_ast)->GetValue() == 0) {
        throw RuntimeError("Runtime error: catched 0 in /");
    }
    div /= As<Number>(value_ast)->GetValue();
    operand = As<Cell>(operand)->GetSecond();

    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in /");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in /");
        }
        if (As<Number>(value_ast)->GetValue() == 0) {
            throw RuntimeError("Runtime error: catched 0 in /");
        }
        div /= As<Number>(value_ast)->GetValue();
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Number>(div);
}

AST Applier::IntegerOperations::OpMin(std::shared_ptr<Cell> ast) {
    int64_t ans = kMaxValue;
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: min() expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in min()");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (!Is<Number>(value_ast)) {
        throw RuntimeError("Runtime error: expected number in min()");
    }
    ans = std::min(ans, As<Number>(value_ast)->GetValue());
    operand = As<Cell>(operand)->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in min()");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in min()");
        }
        ans = std::min(ans, As<Number>(value_ast)->GetValue());
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Number>(ans);
}

AST Applier::IntegerOperations::OpMax(std::shared_ptr<Cell> ast) {
    int64_t ans = kMinValue;
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: max() expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in max()");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (!Is<Number>(value_ast)) {
        throw RuntimeError("Runtime error: expected number in max()");
    }
    ans = std::max(ans, As<Number>(value_ast)->GetValue());
    operand = As<Cell>(operand)->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in max()");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in max()");
        }
        ans = std::max(ans, As<Number>(value_ast)->GetValue());
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Number>(ans);
}

AST Applier::IntegerOperations::OpAbs(std::shared_ptr<Cell> ast) {
    int64_t ans = 0;
    AST operand = ast->GetSecond();
    if (!operand) {
        throw RuntimeError("Runtime error: abs() expects operands");
    }
    if (!Is<Cell>(operand)) {
        throw RuntimeError("Runtime error: expected expression in abs()");
    }
    if (As<Cell>(operand)->GetSecond() != nullptr) {
        throw RuntimeError("Runtime error: unary abs() expected one argument");
    }
    AST value_ast = Apply(As<Cell>(operand)->GetFirst());
    if (!Is<Number>(value_ast)) {
        throw RuntimeError("Runtime error: expected number in abs()");
    }
    ans = std::abs(As<Number>(value_ast)->GetValue());
    return std::make_shared<Number>(ans);
}

AST Applier::IntegerOperations::OpEqual(std::shared_ptr<Cell> ast) {
    bool ans = true;
    int64_t value = 0;
    bool started = false;
    AST operand = ast->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in =");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in max()");
        }
        if (started) {
            ans = (value == As<Number>(value_ast)->GetValue());
            if (!ans) {
                break;
            }
        } else {
            value = As<Number>(value_ast)->GetValue();
            started = true;
        }
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Boolean>(ans);
}

AST Applier::IntegerOperations::OpLess(std::shared_ptr<Cell> ast) {
    bool ans = true;
    int64_t last = kMinValue;
    AST operand = ast->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in <");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in <");
        }
        ans = (last < As<Number>(value_ast)->GetValue());
        last = As<Number>(value_ast)->GetValue();
        if (!ans) {
            break;
        }
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Boolean>(ans);
}

AST Applier::IntegerOperations::OpGreater(std::shared_ptr<Cell> ast) {
    bool ans = true;
    int64_t last = kMaxValue;
    AST operand = ast->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in >");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in >");
        }
        ans = (last > As<Number>(value_ast)->GetValue());
        last = As<Number>(value_ast)->GetValue();
        if (!ans) {
            break;
        }
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Boolean>(ans);
}

AST Applier::IntegerOperations::OpLessEqual(std::shared_ptr<Cell> ast) {
    bool ans = true;
    int64_t last = kMinValue;
    AST operand = ast->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in <=");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in <=");
        }
        ans = (last <= As<Number>(value_ast)->GetValue());
        last = As<Number>(value_ast)->GetValue();
        if (!ans) {
            break;
        }
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Boolean>(ans);
}

AST Applier::IntegerOperations::OpGreaterEqual(std::shared_ptr<Cell> ast) {
    bool ans = true;
    int64_t last = kMaxValue;
    AST operand = ast->GetSecond();
    while (operand) {
        if (!Is<Cell>(operand)) {
            throw RuntimeError("Runtime error: expected expression in >=");
        }
        AST value_ast = Apply(As<Cell>(operand)->GetFirst());
        if (!Is<Number>(value_ast)) {
            throw RuntimeError("Runtime error: expected number in >=");
        }
        ans = (last >= As<Number>(value_ast)->GetValue());
        last = As<Number>(value_ast)->GetValue();
        if (!ans) {
            break;
        }
        operand = As<Cell>(operand)->GetSecond();
    }
    return std::make_shared<Boolean>(ans);
}
