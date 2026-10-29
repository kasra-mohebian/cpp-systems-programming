#include "cppstudy/Rational.hpp"
#include <limits>
#include <numeric>
#include <stdexcept>
namespace arithmetic {
namespace {
long long mul(long long a, long long b) {
    long long result;
    if (__builtin_mul_overflow(a, b, &result))
        throw std::overflow_error("rational product overflow");
    return result;
}
long long add(long long a, long long b) {
    long long result;
    if (__builtin_add_overflow(a, b, &result))
        throw std::overflow_error("rational sum overflow");
    return result;
}
} // namespace
Rational::Rational(long long n, long long d) : num_(n), den_(d) { normalize(); }
Rational Rational::inv() const { return Rational(den_, num_); }
Rational &Rational::operator+=(const Rational &other) {
    Rational next(add(mul(num_, other.den_), mul(other.num_, den_)), mul(den_, other.den_));
    return *this = next;
}
Rational &Rational::operator-=(const Rational &other) { return *this += -other; }
Rational &Rational::operator*=(const Rational &other) {
    Rational next(mul(num_, other.num_), mul(den_, other.den_));
    return *this = next;
}
Rational &Rational::operator/=(const Rational &other) { return *this *= other.inv(); }
void Rational::normalize() {
    if (den_ == 0)
        throw std::domain_error("zero rational denominator");
    // This teaching type reserves LLONG_MIN, whose positive magnitude is not representable.
    if (num_ == std::numeric_limits<long long>::min() ||
        den_ == std::numeric_limits<long long>::min())
        throw std::overflow_error("rational magnitude is not representable");
    if (den_ < 0) {
        den_ = -den_;
        num_ = -num_;
    }
    if (num_ == 0) {
        den_ = 1;
        return;
    }
    auto g = std::gcd(num_ < 0 ? -num_ : num_, den_);
    num_ /= g;
    den_ /= g;
}
std::strong_ordering Rational::compare(const Rational &a, const Rational &b) {
    // Compare positive fractions by continued fractions, avoiding cross-product overflow.
    if ((a.num_ < 0) != (b.num_ < 0))
        return a.num_ < 0 ? std::strong_ordering::less : std::strong_ordering::greater;
    auto n1 = static_cast<unsigned long long>(a.num_ < 0 ? -a.num_ : a.num_);
    auto n2 = static_cast<unsigned long long>(b.num_ < 0 ? -b.num_ : b.num_);
    auto d1 = static_cast<unsigned long long>(a.den_), d2 = static_cast<unsigned long long>(b.den_);
    bool reverse = a.num_ < 0;
    for (;;) {
        auto q1 = n1 / d1, q2 = n2 / d2;
        if (q1 != q2)
            return ((q1 < q2) != reverse) ? std::strong_ordering::less
                                          : std::strong_ordering::greater;
        auto r1 = n1 % d1, r2 = n2 % d2;
        if (r1 == 0 || r2 == 0) {
            if (r1 == r2)
                return std::strong_ordering::equal;
            return ((r1 == 0) != reverse) ? std::strong_ordering::less
                                          : std::strong_ordering::greater;
        }
        n1 = d1;
        d1 = r1;
        n2 = d2;
        d2 = r2;
        reverse = !reverse;
    }
}
} // namespace arithmetic
