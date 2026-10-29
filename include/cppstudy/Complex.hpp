#pragma once
namespace arithmetic {
// Complex number with double precision real and imaginary parts.
// Provides basic arithmetic operations and common functions like abs and arg.
class Complex {
  public:
    Complex() : real_(0.0), imag_(0.0) {}
    Complex(double r) : real_(r), imag_(0.0) {}
    Complex(double r, double i) : real_(r), imag_(i) {}

    double real() const { return real_; }
    double imag() const { return imag_; }

    double abs() const;
    double norm() const;
    double arg() const;
    Complex conj() const;

    Complex operator+() const { return *this; }
    Complex operator-() const { return Complex(-real_, -imag_); }

    Complex &operator+=(const Complex &other);
    Complex &operator-=(const Complex &other);
    Complex &operator*=(const Complex &other);
    Complex &operator/=(const Complex &other);

    friend Complex operator+(Complex lhs, const Complex &rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Complex operator-(Complex lhs, const Complex &rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend Complex operator*(Complex lhs, const Complex &rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend Complex operator/(Complex lhs, const Complex &rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend bool operator==(const Complex &a, const Complex &b) {
        return a.real_ == b.real_ && a.imag_ == b.imag_;
    }

    friend bool operator!=(const Complex &a, const Complex &b) { return !(a == b); }

  private:
    double real_;
    double imag_;
};
} // namespace arithmetic
