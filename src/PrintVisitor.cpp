#include "cppstudy/PrintVisitor.hpp"

#include "cppstudy/AST.hpp"

#include <iostream>
#include <ostream>
namespace ast {

namespace {
void printBinary(ASTVisitor &v, std::ostream &out, const ASTNode &left, const char *op,
                 const ASTNode &right) {
    out << "(";
    left.accept(v);
    out << " " << op << " ";
    right.accept(v);
    out << ")";
}
} // namespace

PrintVisitor::PrintVisitor() : out_(&std::cout) {}

PrintVisitor::PrintVisitor(std::ostream &out) : out_(&out) {}

void PrintVisitor::visit(const UnaryPlus &n) {
    auto &os = *out_;
    os << "(+";
    n.getInput().accept(*this);
    os << ")";
}

void PrintVisitor::visit(const UnaryMinus &n) {
    auto &os = *out_;
    os << "(-";
    n.getInput().accept(*this);
    os << ")";
}

void PrintVisitor::visit(const Add &n) {
    printBinary(*this, *out_, n.getLeft(), "+", n.getRight());
}

void PrintVisitor::visit(const Subtract &n) {
    printBinary(*this, *out_, n.getLeft(), "-", n.getRight());
}

void PrintVisitor::visit(const Multiply &n) {
    printBinary(*this, *out_, n.getLeft(), "*", n.getRight());
}

void PrintVisitor::visit(const Divide &n) {
    printBinary(*this, *out_, n.getLeft(), "/", n.getRight());
}

void PrintVisitor::visit(const Power &n) {
    printBinary(*this, *out_, n.getLeft(), "^", n.getRight());
}

void PrintVisitor::visit(const Constant &n) { (*out_) << n.getValue(); }

void PrintVisitor::visit(const Parameter &n) { (*out_) << 'P' << n.getIndex(); }

} // namespace ast
//---------------------------------------------------------------------------
