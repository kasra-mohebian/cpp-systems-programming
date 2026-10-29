#include "cppstudy/AST.hpp"

#include "cppstudy/ASTVisitor.hpp"
#include "cppstudy/EvaluationContext.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>
namespace ast {

// base helper nodes
UnaryASTNode::UnaryASTNode(std::unique_ptr<ASTNode> input) : input_(std::move(input)) {
    if (!input_)
        throw std::invalid_argument("null expression child");
}

const ASTNode &UnaryASTNode::getInput() const { return *input_; }

std::unique_ptr<ASTNode> UnaryASTNode::releaseInput() { return std::move(input_); }

std::unique_ptr<ASTNode> &UnaryASTNode::inputPtr() { return input_; }

BinaryASTNode::BinaryASTNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
    : left_(std::move(left)), right_(std::move(right)) {
    if (!left_ || !right_)
        throw std::invalid_argument("null expression child");
}

const ASTNode &BinaryASTNode::getLeft() const { return *left_; }

const ASTNode &BinaryASTNode::getRight() const { return *right_; }

std::unique_ptr<ASTNode> BinaryASTNode::releaseLeft() { return std::move(left_); }

std::unique_ptr<ASTNode> BinaryASTNode::releaseRight() { return std::move(right_); }

std::unique_ptr<ASTNode> &BinaryASTNode::leftPtr() { return left_; }

std::unique_ptr<ASTNode> &BinaryASTNode::rightPtr() { return right_; }

// small helpers for optimization
static bool isConstNode(const ASTNode &n) { return n.getType() == ASTNode::Type::Constant; }

static double constValue(const ASTNode &n) { return static_cast<const Constant &>(n).getValue(); }

static std::unique_ptr<ASTNode> makeConst(double v) { return std::make_unique<Constant>(v); }

// leaf nodes
Constant::Constant(double value) : value_(value) {}

ASTNode::Type Constant::getType() const { return Type::Constant; }

double Constant::getValue() const { return value_; }

double Constant::evaluate(const EvaluationContext &) const { return value_; }

void Constant::optimize(std::unique_ptr<ASTNode> &) {}

void Constant::accept(ASTVisitor &v) const { v.visit(*this); }

Parameter::Parameter(std::size_t index) : index_(index) {}

ASTNode::Type Parameter::getType() const { return Type::Parameter; }

std::size_t Parameter::getIndex() const { return index_; }

double Parameter::evaluate(const EvaluationContext &ctx) const { return ctx.getParameter(index_); }

void Parameter::optimize(std::unique_ptr<ASTNode> &) {}

void Parameter::accept(ASTVisitor &v) const { v.visit(*this); }

// unary operators
ASTNode::Type UnaryPlus::getType() const { return Type::UnaryPlus; }

double UnaryPlus::evaluate(const EvaluationContext &ctx) const { return getInput().evaluate(ctx); }

void UnaryPlus::accept(ASTVisitor &v) const { v.visit(*this); }

ASTNode::Type UnaryMinus::getType() const { return Type::UnaryMinus; }

double UnaryMinus::evaluate(const EvaluationContext &ctx) const {
    return -getInput().evaluate(ctx);
}

void UnaryMinus::accept(ASTVisitor &v) const { v.visit(*this); }

// binary operators
ASTNode::Type Add::getType() const { return Type::Add; }
double Add::evaluate(const EvaluationContext &ctx) const {
    return getLeft().evaluate(ctx) + getRight().evaluate(ctx);
}
void Add::accept(ASTVisitor &v) const { v.visit(*this); }

ASTNode::Type Subtract::getType() const { return Type::Subtract; }
double Subtract::evaluate(const EvaluationContext &ctx) const {
    return getLeft().evaluate(ctx) - getRight().evaluate(ctx);
}
void Subtract::accept(ASTVisitor &v) const { v.visit(*this); }

ASTNode::Type Multiply::getType() const { return Type::Multiply; }
double Multiply::evaluate(const EvaluationContext &ctx) const {
    return getLeft().evaluate(ctx) * getRight().evaluate(ctx);
}
void Multiply::accept(ASTVisitor &v) const { v.visit(*this); }

ASTNode::Type Divide::getType() const { return Type::Divide; }
double Divide::evaluate(const EvaluationContext &ctx) const {
    return getLeft().evaluate(ctx) / getRight().evaluate(ctx);
}
void Divide::accept(ASTVisitor &v) const { v.visit(*this); }

ASTNode::Type Power::getType() const { return Type::Power; }
double Power::evaluate(const EvaluationContext &ctx) const {
    return std::pow(getLeft().evaluate(ctx), getRight().evaluate(ctx));
}
void Power::accept(ASTVisitor &v) const { v.visit(*this); }

// optimization helpers
static void optimizeChild(std::unique_ptr<ASTNode> &child) { child->optimize(child); }

static bool tryConstFoldUnary(const UnaryASTNode &n, double &out) {
    if (!isConstNode(n.getInput()))
        return false;

    const double v = constValue(n.getInput());
    if (n.getType() == ASTNode::Type::UnaryPlus) {
        out = v;
        return true;
    }
    if (n.getType() == ASTNode::Type::UnaryMinus) {
        out = -v;
        return true;
    }
    return false;
}

static bool tryConstFoldBinary(const BinaryASTNode &n, double &out) {
    if (!isConstNode(n.getLeft()) || !isConstNode(n.getRight()))
        return false;

    const double l = constValue(n.getLeft());
    const double r = constValue(n.getRight());

    switch (n.getType()) {
    case ASTNode::Type::Add:
        out = l + r;
        return true;
    case ASTNode::Type::Subtract:
        out = l - r;
        return true;
    case ASTNode::Type::Multiply:
        out = l * r;
        return true;
    case ASTNode::Type::Divide:
        out = l / r;
        return true;
    case ASTNode::Type::Power:
        out = std::pow(l, r);
        return true;
    default:
        return false;
    }
}

// Public-edition simplification deliberately keeps parameter-dependent expressions.
void UnaryPlus::optimize(std::unique_ptr<ASTNode> &self) {
    optimizeChild(inputPtr());
    auto child = releaseInput();
    self = std::move(child);
}
void UnaryMinus::optimize(std::unique_ptr<ASTNode> &self) {
    optimizeChild(inputPtr());
    double value = 0;
    if (tryConstFoldUnary(*this, value))
        self = makeConst(value);
}
void Add::optimize(std::unique_ptr<ASTNode> &self) {
    optimizeChild(leftPtr());
    optimizeChild(rightPtr());
    double value = 0;
    if (tryConstFoldBinary(*this, value))
        self = makeConst(value);
}
void Subtract::optimize(std::unique_ptr<ASTNode> &self) {
    optimizeChild(leftPtr());
    optimizeChild(rightPtr());
    double value = 0;
    if (tryConstFoldBinary(*this, value))
        self = makeConst(value);
}
void Multiply::optimize(std::unique_ptr<ASTNode> &self) {
    optimizeChild(leftPtr());
    optimizeChild(rightPtr());
    double value = 0;
    if (tryConstFoldBinary(*this, value))
        self = makeConst(value);
}
void Divide::optimize(std::unique_ptr<ASTNode> &self) {
    optimizeChild(leftPtr());
    optimizeChild(rightPtr());
    double value = 0;
    if (tryConstFoldBinary(*this, value))
        self = makeConst(value);
}
void Power::optimize(std::unique_ptr<ASTNode> &self) {
    optimizeChild(leftPtr());
    optimizeChild(rightPtr());
    double value = 0;
    if (tryConstFoldBinary(*this, value))
        self = makeConst(value);
}
} // namespace ast
