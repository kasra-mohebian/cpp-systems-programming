#pragma once
// AST node interfaces for simple arithmetic expressions
#include <cstddef>
#include <memory>
#include <utility>

namespace ast {

class EvaluationContext;
class ASTVisitor;

/// base interface for all AST nodes
class ASTNode {
  public:
    enum class Type {
        Add,
        Constant,
        Divide,
        Multiply,
        Parameter,
        Power,
        Subtract,
        UnaryMinus,
        UnaryPlus
    };

    virtual ~ASTNode() = default;

    virtual Type getType() const = 0;
    virtual double evaluate(const EvaluationContext &ctx) const = 0;
    virtual void optimize(std::unique_ptr<ASTNode> &thisRef) = 0;
    virtual void accept(ASTVisitor &v) const = 0;
};

/// Base class for unary operations (one child)
class UnaryASTNode : public ASTNode {
  public:
    explicit UnaryASTNode(std::unique_ptr<ASTNode> input);

    const ASTNode &getInput() const;
    std::unique_ptr<ASTNode> releaseInput();

  protected:
    std::unique_ptr<ASTNode> &inputPtr();

  private:
    std::unique_ptr<ASTNode> input_;
};

/// Base class for binary operations (left and right child).
class BinaryASTNode : public ASTNode {
  public:
    BinaryASTNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);

    const ASTNode &getLeft() const;
    const ASTNode &getRight() const;

    std::unique_ptr<ASTNode> releaseLeft();
    std::unique_ptr<ASTNode> releaseRight();

  protected:
    std::unique_ptr<ASTNode> &leftPtr();
    std::unique_ptr<ASTNode> &rightPtr();

  private:
    std::unique_ptr<ASTNode> left_;
    std::unique_ptr<ASTNode> right_;
};

class UnaryPlus final : public UnaryASTNode {
  public:
    using UnaryASTNode::UnaryASTNode;

    Type getType() const override;
    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;
};

class UnaryMinus final : public UnaryASTNode {
  public:
    using UnaryASTNode::UnaryASTNode;

    Type getType() const override;
    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;
};

class Add final : public BinaryASTNode {
  public:
    using BinaryASTNode::BinaryASTNode;

    Type getType() const override;
    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;
};

class Subtract final : public BinaryASTNode {
  public:
    using BinaryASTNode::BinaryASTNode;

    Type getType() const override;
    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;
};

class Multiply final : public BinaryASTNode {
  public:
    using BinaryASTNode::BinaryASTNode;

    Type getType() const override;
    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;
};

class Divide final : public BinaryASTNode {
  public:
    using BinaryASTNode::BinaryASTNode;

    Type getType() const override;
    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;
};

class Power final : public BinaryASTNode {
  public:
    using BinaryASTNode::BinaryASTNode;

    Type getType() const override;
    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;
};

/// Leaf node storing a numeric constant.
class Constant final : public ASTNode {
  public:
    explicit Constant(double value);

    Type getType() const override;
    double getValue() const;

    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;

  private:
    double value_;
};

/// leaf node referring to a parameter by index
class Parameter final : public ASTNode {
  public:
    explicit Parameter(std::size_t index);

    Type getType() const override;
    std::size_t getIndex() const;

    double evaluate(const EvaluationContext &ctx) const override;
    void optimize(std::unique_ptr<ASTNode> &thisRef) override;
    void accept(ASTVisitor &v) const override;

  private:
    std::size_t index_;
};

} // namespace ast
