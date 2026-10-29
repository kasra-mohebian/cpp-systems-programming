#pragma once

namespace ast {

// forward declarations of all AST node types
class UnaryPlus;
class UnaryMinus;
class Add;
class Subtract;
class Multiply;
class Divide;
class Power;
class Constant;
class Parameter;

/// Base class for AST visitors (visitor pattern)
class ASTVisitor {
  public:
    virtual ~ASTVisitor() = default;

    // Visit methods for each concrete AST node type
    virtual void visit(const UnaryPlus &n) = 0;
    virtual void visit(const UnaryMinus &n) = 0;
    virtual void visit(const Add &n) = 0;
    virtual void visit(const Subtract &n) = 0;
    virtual void visit(const Multiply &n) = 0;
    virtual void visit(const Divide &n) = 0;
    virtual void visit(const Power &n) = 0;
    virtual void visit(const Constant &n) = 0;
    virtual void visit(const Parameter &n) = 0;
};

} // namespace ast
