#include "cppstudy/EvaluationContext.hpp"
namespace ast {

void EvaluationContext::pushParameter(double value) { params_.push_back(value); }

double EvaluationContext::getParameter(std::size_t index) const { return params_.at(index); }

} // namespace ast
//---------------------------------------------------------------------------
