#pragma once

#include <cstdint>
#include <istream>
#include <optional>
#include <string>
#include <variant>
#include <vector>


struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int64_t value;

    bool operator==(const ConstantToken& other) const;
};

struct BooleanToken {
    bool value;

    bool operator==(const BooleanToken& other) const;
};

using Token =
    std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken, BooleanToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    bool IsEnd();

    void Next();

    Token GetToken();

private:
    std::istream* is_;
    std::optional<Token> current_token_;
};

std::vector<Token> Read(const std::string& stream);
