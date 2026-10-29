#pragma once
#include "cppstudy/ASTVisitor.hpp"

#include <iosfwd>
namespace ast {

class PrintVisitor final : public ASTVisitor {
  public:
    /// Default: print to std::cout
    PrintVisitor();

    /// print to a custom output stream
    explicit PrintVisitor(std::ostream &out);

    void visit(const UnaryPlus &n) override;
    void visit(const UnaryMinus &n) override;
    void visit(const Add &n) override;
    void visit(const Subtract &n) override;
    void visit(const Multiply &n) override;
    void visit(const Divide &n) override;
    void visit(const Power &n) override;
    void visit(const Constant &n) override;
    void visit(const Parameter &n) override;

  private:
    std::ostream *out_;
};

} // namespace ast
