#include "cppstudy/Complex.hpp"
#include <cmath>

namespace arithmetic {

double Complex::abs() const { return std::hypot(real_, imag_); }

double Complex::norm() const { return real_ * real_ + imag_ * imag_; }

double Complex::arg() const { return std::atan2(imag_, real_); }

Complex Complex::conj() const { return Complex(real_, -imag_); }

Complex &Complex::operator+=(const Complex &other) {
    real_ += other.real_;
    imag_ += other.imag_;
    return *this;
}

Complex &Complex::operator-=(const Complex &other) {
    real_ -= other.real_;
    imag_ -= other.imag_;
    return *this;
}

Complex &Complex::operator*=(const Complex &other) {
    double r = real_ * other.real_ - imag_ * other.imag_;
    double i = real_ * other.imag_ + imag_ * other.real_;
    real_ = r;
    imag_ = i;
    return *this;
}

Complex &Complex::operator/=(const Complex &other) {
    double denom = other.real_ * other.real_ + other.imag_ * other.imag_;
    double r = (real_ * other.real_ + imag_ * other.imag_) / denom;
    double i = (imag_ * other.real_ - real_ * other.imag_) / denom;
    real_ = r;
    imag_ = i;
    return *this;
}

} // namespace arithmetic
