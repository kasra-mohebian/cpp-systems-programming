#pragma once
#include <compare>
namespace arithmetic {
// rational number stored in canonical form.
// normalizes sign and reduces numerator and denominator using gcd.
class Rational {
  public:
    Rational() : num_(0), den_(1) {}
    Rational(long long n) : Rational(n, 1) {}
    Rational(long long n, long long d);

    long long num() const { return num_; }
    long long den() const { return den_; }

    Rational inv() const;

    Rational operator+() const { return *this; }
    Rational operator-() const { return Rational(-num_, den_); }

    Rational &operator+=(const Rational &other);
    Rational &operator-=(const Rational &other);
    Rational &operator*=(const Rational &other);
    Rational &operator/=(const Rational &other);

    friend Rational operator+(Rational lhs, const Rational &rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Rational operator-(Rational lhs, const Rational &rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend Rational operator*(Rational lhs, const Rational &rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend Rational operator/(Rational lhs, const Rational &rhs) {
        lhs /= rhs;
        return lhs;
    }

    explicit operator double() const {
        return static_cast<double>(num_) / static_cast<double>(den_);
    }

    friend bool operator==(const Rational &a, const Rational &b) {
        return a.num_ == b.num_ && a.den_ == b.den_;
    }

    friend std::strong_ordering operator<=>(const Rational &a, const Rational &b) {
        return compare(a, b);
    }

  private:
    long long num_;
    long long den_;
    void normalize();
    static std::strong_ordering compare(const Rational &, const Rational &);
};
} // namespace arithmetic
