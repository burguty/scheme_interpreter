#include "applier.h"

AST Applier::QuoteOperations::OpQuote(std::shared_ptr<Quote> ast) {
    return ast->GetCommand();
}