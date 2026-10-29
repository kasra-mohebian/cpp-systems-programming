#include "cppstudy/SimpleVM.hpp"
#include <bit>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace simplevm {

// opcodes we need
enum class Op : int {
    HALT = 0,
    MOVI = 10,
    MOVF = 11,
    LOADA = 20,
    STOREA = 21,
    SWAPAB = 22,
    LOADX = 30,
    STOREX = 31,
    SWAPXY = 32,
    ITOF = 40,
    FTOI = 41,
    ADDI = 50,
    SUBI = 51,
    RSUBI = 52,
    MULI = 53,
    DIVI = 54,
    ADDF = 60,
    SUBF = 61,
    MULF = 62,
    DIVF = 63
};

namespace {
struct S {
    int32_t A{0}, B{0}, C{0}, D{0};
    float X{0.f}, Y{0.f};
};

// pick register quick
inline int32_t &I(S &s, char r) {
    switch (r) {
    case 'A':
        return s.A;
    case 'B':
        return s.B;
    case 'C':
        return s.C;
    case 'D':
        return s.D;
    default:
        throw std::invalid_argument("invalid integer register");
    }
}
inline float &F(S &s, char r) {
    if (r == 'X')
        return s.X;
    if (r == 'Y')
        return s.Y;
    throw std::invalid_argument("invalid float register");
}

// wrap on overflow (unsigned ops, then cast back)
inline int32_t wadd(int32_t a, int32_t b) {
    return std::bit_cast<int32_t>((uint32_t)a + (uint32_t)b);
}
inline int32_t wsub(int32_t a, int32_t b) {
    return std::bit_cast<int32_t>((uint32_t)a - (uint32_t)b);
}
inline int32_t wmul(int32_t a, int32_t b) {
    return std::bit_cast<int32_t>((uint32_t)a * (uint32_t)b);
}
} // namespace

int32_t runVM(std::istream &program, std::ostream &output) {
    S s;
    std::string line;

    (void)output;

    // read & execute one line per instruction
    while (std::getline(program, line)) {
        if (line.empty())
            continue;
        std::istringstream in(line);
        int code = 0;
        if (!(in >> code))
            throw std::invalid_argument("missing opcode");
        Op op = (Op)code;

        switch (op) {
        case Op::HALT: {
            std::string extra;
            if (in >> extra)
                throw std::invalid_argument("unexpected operand");
            return s.A;
        }

        case Op::MOVI: {
            char r{};
            long long v{};
            if (!(in >> r >> v))
                throw std::invalid_argument("invalid integer operand");
            if (v < INT32_MIN || v > INT32_MAX)
                throw std::out_of_range("integer operand");
            I(s, r) = static_cast<int32_t>(v);
            break;
        }
        case Op::MOVF: {
            char r{};
            float f{};
            if (!(in >> r >> f) || !std::isfinite(f))
                throw std::invalid_argument("invalid float operand");
            F(s, r) = f;
            break;
        }

        case Op::LOADA: {
            char r{};
            if (!(in >> r))
                throw std::invalid_argument("missing register");
            s.A = I(s, r);
            break;
        }
        case Op::STOREA: {
            char r{};
            if (!(in >> r))
                throw std::invalid_argument("missing register");
            I(s, r) = s.A;
            break;
        }

        case Op::LOADX: {
            char r{};
            if (!(in >> r))
                throw std::invalid_argument("missing register");
            s.X = F(s, r);
            break;
        }
        case Op::STOREX: {
            char r{};
            if (!(in >> r))
                throw std::invalid_argument("missing register");
            F(s, r) = s.X;
            break;
        }

        case Op::SWAPAB:
            std::swap(s.A, s.B);
            break;
        case Op::SWAPXY:
            std::swap(s.X, s.Y);
            break;

        case Op::ITOF:
            s.X = (float)s.A;
            break;
        case Op::FTOI:
            if (!std::isfinite(s.X) || static_cast<double>(s.X) < INT32_MIN ||
                static_cast<double>(s.X) > INT32_MAX)
                throw std::overflow_error("float-to-integer conversion");
            s.A = static_cast<int32_t>(s.X);
            break;

        case Op::ADDI:
            s.A = wadd(s.A, s.B);
            break;
        case Op::SUBI:
            s.A = wsub(s.A, s.B);
            break;
        case Op::RSUBI:
            s.A = wsub(s.B, s.A);
            break;
        case Op::MULI:
            s.A = wmul(s.A, s.B);
            break;

        case Op::DIVI: {
            if (s.B == 0)
                throw std::domain_error("division by zero");
            if (s.A == INT32_MIN && s.B == -1)
                throw std::overflow_error("integer division overflow");
            int32_t a = s.A, b = s.B;
            s.A = a / b;
            s.B = a % b;
            break;
        }

        case Op::ADDF:
            s.X = s.X + s.Y;
            break;
        case Op::SUBF:
            s.X = s.X - s.Y;
            break;
        case Op::MULF:
            s.X = s.X * s.Y;
            break;
        case Op::DIVF:
            if (s.Y == 0.0f)
                throw std::domain_error("division by zero");
            s.X = s.X / s.Y;
            break;

        default:
            throw std::invalid_argument("unknown opcode");
        }
        std::string extra;
        if (in >> extra)
            throw std::invalid_argument("unexpected operand");
    }
    return s.A;
}

void fibonacciProgram(unsigned n, std::ostream &output) {
    // tiny VM program for F(n) using A,B and add/swap
    if (n == 0) {
        output << (int)Op::HALT << "\n";
        return;
    }

    output << (int)Op::MOVI << " A 0\n";
    output << (int)Op::MOVI << " B 1\n";
    for (unsigned i = 0; i < n; ++i) {
        output << (int)Op::ADDI << "\n";
        output << (int)Op::SWAPAB << "\n";
    }
    output << (int)Op::HALT << "\n";
}

} // namespace simplevm
