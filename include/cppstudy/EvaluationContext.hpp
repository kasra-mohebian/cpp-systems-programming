#pragma once
#include <cstddef>
#include <vector>
namespace ast {

class EvaluationContext {
  public:
    /// add a value for the next parameter index
    void pushParameter(double value);

    /// retrieve the value of a parameter by index
    double getParameter(std::size_t index) const;

  private:
    std::vector<double> params_;
};

} // namespace ast
