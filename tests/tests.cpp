#include "cppstudy/AST.hpp"
#include "cppstudy/BinaryHeap.hpp"
#include "cppstudy/BitSet.hpp"
#include "cppstudy/ChainingHashTable.hpp"
#include "cppstudy/CommandLine.hpp"
#include "cppstudy/Complex.hpp"
#include "cppstudy/EvaluationContext.hpp"
#include "cppstudy/MultiMaps.hpp"
#include "cppstudy/ObjectRepresentation.hpp"
#include "cppstudy/PrintVisitor.hpp"
#include "cppstudy/Rational.hpp"
#include "cppstudy/SimpleVM.hpp"
#include "cppstudy/SortPointers.hpp"
#include "cppstudy/StringMatching.hpp"
#include "cppstudy/TempFile.hpp"
#include "cppstudy/UnionFind.hpp"
#include "cppstudy/Varint.hpp"
#include <algorithm>
#include <atomic>
#include <climits>
#include <cmath>
#include <complex>
#include <filesystem>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <unistd.h>
#include <unordered_map>
namespace {
unsigned checks = 0;
#define CHECK(...)                                                                                 \
    do {                                                                                           \
        ++checks;                                                                                  \
        if (!(__VA_ARGS__))                                                                        \
            throw std::runtime_error(std::string("check failed at line ") +                        \
                                     std::to_string(__LINE__) + ": " #__VA_ARGS__);                \
    } while (false)
template <class E, class F> void throws(F f) {
    ++checks;
    try {
        f();
    } catch (const E &) {
        return;
    }
    throw std::runtime_error("expected exception was not thrown");
}
void near(double a, double b) { CHECK(std::abs(a - b) <= 1e-10 * std::max(1.0, std::abs(b))); }
std::mt19937 rng(20261029);
void algorithms() {
    std::vector<unsigned> heap;
    std::priority_queue<unsigned> reference;
    for (unsigned i = 0; i < 5000; ++i) {
        if (reference.empty() || rng() % 3) {
            unsigned v = rng();
            binary_heap::insert(heap, v);
            reference.push(v);
        } else {
            CHECK(binary_heap::extract(heap) == reference.top());
            reference.pop();
        }
        CHECK(std::is_heap(heap.begin(), heap.end()));
    }
    while (!reference.empty()) {
        CHECK(binary_heap::extract(heap) == reference.top());
        reference.pop();
    }
    throws<std::out_of_range>([&] { binary_heap::extract(heap); });
    std::ostringstream dot;
    binary_heap::printDot(dot, {2, 1});
    CHECK(dot.str().find("0 -> 1") != std::string::npos);
    for (unsigned n : {0u, 1u, 2u, 3u, 127u, 1024u}) {
        std::vector<unsigned> data(n);
        for (auto &v : data)
            v = rng() % 13;
        std::vector<const unsigned *> pointers;
        for (const auto &v : data)
            pointers.push_back(&v);
        auto expected = pointers;
        std::stable_sort(expected.begin(), expected.end(), [](auto a, auto b) { return *a < *b; });
        auto quick = pointers;
        sort_pointers::quicksort(quick.data(),
                                 quick.empty() ? quick.data() : quick.data() + quick.size());
        CHECK(std::is_sorted(quick.begin(), quick.end(), [](auto a, auto b) { return *a < *b; }));
        CHECK(std::multiset<const unsigned *>(quick.begin(), quick.end()) ==
              std::multiset<const unsigned *>(pointers.begin(), pointers.end()));
        sort_pointers::mergesort(pointers.data(), pointers.empty()
                                                      ? pointers.data()
                                                      : pointers.data() + pointers.size());
        CHECK(pointers == expected);
    }
    std::vector<unsigned> ordered(3000);
    std::iota(ordered.begin(), ordered.end(), 0);
    std::vector<const unsigned *> pointers;
    for (const auto &v : ordered)
        pointers.push_back(&v);
    sort_pointers::quicksort(pointers.data(), pointers.data() + pointers.size());
    CHECK(std::is_sorted(pointers.begin(), pointers.end(), [](auto a, auto b) { return *a < *b; }));
    for (unsigned i = 0; i < 2000; ++i) {
        std::string text(rng() % 100, 'a'), pattern(rng() % 25, 'a');
        for (char &c : text)
            c = static_cast<char>('a' + rng() % 4);
        for (char &c : pattern)
            c = static_cast<char>('a' + rng() % 4);
        CHECK(string_matching::matchKMP(text, pattern) == text.find(pattern));
        CHECK(string_matching::matchNaive(text, pattern) == text.find(pattern));
    }
    std::string embedded("x\0yz", 4);
    CHECK(string_matching::matchKMP(embedded, std::string_view("\0y", 2)) == 1);
    std::vector<std::uint64_t> values{0, 1, 127, 128, 16383, 16384, UINT64_MAX};
    for (unsigned i = 0; i < 5000; ++i)
        values.push_back((std::uint64_t(rng()) << 32) | rng());
    CHECK(varint::decompress(varint::compress(values)) == values);
    CHECK(varint::compress({127, 128}) == std::vector<std::uint8_t>({127, 128, 1}));
    CHECK(varint::decompress({}).empty());
    throws<std::invalid_argument>([] { varint::decompress({128}); });
    throws<std::overflow_error>([] { varint::decompress(std::vector<std::uint8_t>(10, 255)); });
    auto over = std::vector<std::uint8_t>(9, 128);
    over.push_back(2);
    throws<std::overflow_error>([&] { varint::decompress(over); });
    union_find::UnionFind sets(100);
    std::vector<unsigned> labels(100);
    std::iota(labels.begin(), labels.end(), 0);
    for (unsigned i = 0; i < 400; ++i) {
        auto a = rng() % 100, b = rng() % 100;
        auto from = labels[b], to = labels[a];
        for (auto &label : labels)
            if (label == from)
                label = to;
        sets.unionSets(a, b);
        for (unsigned k = 0; k < 100; ++k)
            CHECK(sets.disjoint(a, k) == (labels[a] != labels[k]));
    }
    CHECK(sets.add() == 100);
    CHECK(sets.disjoint(0, 100));
    throws<std::out_of_range>([&] { sets.find(101); });
    std::vector<unsigned char> stack;
    object_representation::push(stack, 1.25f);
    object_representation::push(stack, -2.5);
    auto before = stack;
    float f = 0;
    double d = 0;
    CHECK(!object_representation::pop(stack, f));
    CHECK(stack == before);
    CHECK(object_representation::pop(stack, d));
    CHECK(d == -2.5);
    CHECK(object_representation::pop(stack, f));
    CHECK(f == 1.25f);
    CHECK(!object_representation::pop(stack, f));
    stack = {4};
    CHECK(!object_representation::pop(stack, f));
}
void containers() {
    bitset::BitSet bits;
    std::vector<bool> ref;
    for (unsigned i = 0; i < 1500; ++i) {
        bool v = rng() % 2;
        bits.push_back(v);
        ref.push_back(v);
    }
    for (unsigned i = 0; i < 3000; ++i) {
        auto index = rng() % ref.size();
        bool v = rng() % 2;
        bits[index] = v;
        ref[index] = v;
    }
    CHECK(bits.size() == ref.size());
    CHECK(bits.cardinality() == static_cast<std::size_t>(std::count(ref.begin(), ref.end(), true)));
    unsigned i = 0;
    for (bool v : bits)
        CHECK(v == ref[i++]);
    auto it = bits.end();
    for (auto j = ref.size(); j > 0; --j) {
        --it;
        CHECK(bool(*it) == ref[j - 1]);
    }
    const auto &cb = bits;
    i = 0;
    for (bool v : cb)
        CHECK(v == ref[i++]);
    bitset::BitSet moved(std::move(bits));
    CHECK(bits.size() == 0);
    bits.push_back(true);
    CHECK(bits.front());
    CHECK(moved.size() == ref.size());
    throws<std::out_of_range>([&] { moved[moved.size()] = true; });
    bitset::BitSet empty;
    throws<std::out_of_range>([&] { (void)empty.back(); });
    bitset::BitSet sized(17);
    CHECK(sized.cardinality() == 0);
    sized[8] = true;
    sized[9] = sized[8];
    CHECK(sized.cardinality() == 2);
    hashtable::ChainingHashTable table;
    std::unordered_map<std::int64_t, std::byte> model;
    for (unsigned n = 0; n < 8000; ++n) {
        std::int64_t key = std::int64_t(rng() % 1000) - 500;
        if (rng() % 3 == 0) {
            table.erase(key);
            model.erase(key);
        } else {
            hashtable::BytePayload p;
            p.data()[0] = std::byte(rng() % 256);
            model[key] = p.data()[0];
            table.insert(key, std::move(p));
        }
        CHECK(table.size() == model.size());
        CHECK(table.contains(key) == model.contains(key));
    }
    std::set<std::int64_t> seen;
    for (auto &item : table) {
        CHECK(item.value.data()[0] == model.at(item.key));
        seen.insert(item.key);
    }
    CHECK(seen.size() == model.size());
    table[INT64_MIN].data()[0] = std::byte{23};
    CHECK(table.find(INT64_MIN)->value.data()[0] == std::byte{23});
    auto count = table.size();
    hashtable::ChainingHashTable movedTable(std::move(table));
    CHECK(table.size() == 0);
    CHECK(table.begin() == table.end());
    table[42];
    CHECK(table.size() == 1);
    table = std::move(movedTable);
    CHECK(table.size() == count);
    CHECK(movedTable.size() == 0);
    auto *sameTable = &table;
    table = std::move(*sameTable);
    CHECK(table.size() == count);
}
void numbers() {
    using arithmetic::Complex;
    using arithmetic::Rational;
    CHECK(Rational(2, -4) == Rational(-1, 2));
    CHECK(Rational(0, -7).den() == 1);
    CHECK(Rational(1, 3) + Rational(1, 6) == Rational(1, 2));
    CHECK(Rational(2, 3) / Rational(4, 5) == Rational(5, 6));
    CHECK(Rational(LLONG_MAX, 2) > Rational(LLONG_MAX, 3));
    CHECK(Rational(-LLONG_MAX, 2) < Rational(-LLONG_MAX, 3));
    for (int a = -20; a <= 20; ++a)
        for (int b = 1; b <= 20; ++b)
            for (int c = -5; c <= 5; ++c) {
                Rational x(a, b), y(c, 7);
                CHECK((x < y) == (a * 7 < c * b));
                CHECK((x == y) == (a * 7 == c * b));
                near(double(x + y), double(a) / b + double(c) / 7);
            }
    throws<std::domain_error>([] { Rational(1, 0); });
    throws<std::domain_error>([] { Rational(0).inv(); });
    throws<std::overflow_error>([] { Rational(LLONG_MIN); });
    Rational x(LLONG_MAX);
    throws<std::overflow_error>([&] { x += Rational(1); });
    CHECK(x == Rational(LLONG_MAX));
    for (int i = 0; i < 500; ++i) {
        double a = int(rng() % 100) - 50, b = int(rng() % 100) - 50, c = int(rng() % 99) + 1,
               d = int(rng() % 100) - 50;
        Complex x(a, b), y(c, d);
        std::complex<double> sx(a, b), sy(c, d);
        auto add = x + y, mul = x * y, div = x / y;
        near(add.real(), (sx + sy).real());
        near(mul.imag(), (sx * sy).imag());
        near(div.real(), (sx / sy).real());
        near(div.imag(), (sx / sy).imag());
        near(x.abs(), std::abs(sx));
    }
    CHECK(std::isfinite(Complex(1e308, 1e308).abs()));
}
void expressions() {
    using namespace ast;
    EvaluationContext ctx;
    ctx.pushParameter(5);
    std::unique_ptr<ASTNode> tree = std::make_unique<Multiply>(
        std::make_unique<Add>(std::make_unique<Constant>(2), std::make_unique<Constant>(3)),
        std::make_unique<Parameter>(0));
    near(tree->evaluate(ctx), 25);
    std::ostringstream printed;
    PrintVisitor visitor(printed);
    tree->accept(visitor);
    CHECK(!printed.str().empty());
    tree->optimize(tree);
    near(tree->evaluate(ctx), 25);
    CHECK(static_cast<const Multiply &>(*tree).getLeft().getType() == ASTNode::Type::Constant);
    std::unique_ptr<ASTNode> invalid =
        std::make_unique<Divide>(std::make_unique<Constant>(0), std::make_unique<Parameter>(0));
    EvaluationContext zero;
    zero.pushParameter(0);
    CHECK(std::isnan(invalid->evaluate(zero)));
    invalid->optimize(invalid);
    CHECK(std::isnan(invalid->evaluate(zero)));
    std::unique_ptr<ASTNode> power =
        std::make_unique<Power>(std::make_unique<Constant>(0), std::make_unique<Parameter>(0));
    EvaluationContext negative;
    negative.pushParameter(-1);
    CHECK(std::isinf(power->evaluate(negative)));
    power->optimize(power);
    CHECK(std::isinf(power->evaluate(negative)));
    throws<std::out_of_range>([&] { ctx.getParameter(1); });
    throws<std::invalid_argument>([] { UnaryMinus bad(nullptr); });
    auto constant = [&](auto factory, double expected) {
        std::unique_ptr<ASTNode> n = factory();
        near(n->evaluate(ctx), expected);
        n->optimize(n);
        CHECK(n->getType() == ASTNode::Type::Constant);
        near(n->evaluate(ctx), expected);
    };
    constant(
        [] {
            return std::make_unique<Power>(std::make_unique<Constant>(2),
                                           std::make_unique<Constant>(3));
        },
        8);
    constant(
        [] {
            return std::make_unique<Subtract>(std::make_unique<Constant>(5),
                                              std::make_unique<Constant>(2));
        },
        3);
    constant(
        [] {
            return std::make_unique<Divide>(std::make_unique<Constant>(8),
                                            std::make_unique<Constant>(2));
        },
        4);
    constant([] { return std::make_unique<UnaryMinus>(std::make_unique<Constant>(2)); }, -2);
    std::unique_ptr<ASTNode> unary = std::make_unique<UnaryPlus>(std::make_unique<Parameter>(0));
    unary->optimize(unary);
    CHECK(unary->getType() == ASTNode::Type::Parameter);
}
std::int32_t run(std::string text) {
    std::istringstream input(text);
    std::ostringstream output;
    return simplevm::runVM(input, output);
}
void vm() {
    unsigned a = 0, b = 1;
    for (unsigned n = 0; n <= 30; ++n) {
        std::ostringstream program;
        simplevm::fibonacciProgram(n, program);
        CHECK(run(program.str()) == static_cast<std::int32_t>(a));
        auto next = a + b;
        a = b;
        b = next;
    }
    CHECK(run("10 A 2147483647\n10 B 1\n50\n0\n") == INT32_MIN);
    CHECK(run("10 A -7\n10 B 3\n54\n0\n") == -2);
    CHECK(run("10 A 7\n10 B 3\n54\n20 B\n0\n") == 1);
    CHECK(run("11 X 12.5\n41\n0\n") == 12);
    throws<std::overflow_error>([] { run("10 A -2147483648\n10 B -1\n54\n"); });
    throws<std::domain_error>([] { run("54\n"); });
    for (auto malformed :
         {"10 A\n", "10 Q 3\n", "11 Z 2\n", "99\n", "0 extra\n", "50 extra\n", "20\n", "abc\n"})
        throws<std::invalid_argument>([&] { run(malformed); });
    throws<std::out_of_range>([] { run("10 A 2147483648\n"); });
    throws<std::overflow_error>([] { run("11 X 2147483648\n41\n"); });
}
void resources() {
    namespace fs = std::filesystem;
    // Unique private parent. Nothing under a caller's working directory is touched.
    auto pattern = (fs::temp_directory_path() / "cppstudy-tests-XXXXXX").string();
    std::vector<char> buffer(pattern.begin(), pattern.end());
    buffer.push_back('\0');
    auto created = ::mkdtemp(buffer.data());
    if (!created)
        throw std::runtime_error("mkdtemp failed");
    fs::path parent(created);
    struct Cleanup {
        fs::path p;
        ~Cleanup() {
            std::error_code ec;
            fs::remove_all(p, ec);
        }
    } cleanup{parent};
    auto root = (parent / "owned").string();
    std::unique_ptr<raii::TempFile> held;
    {
        raii::TempDirectory dir(root);
        CHECK(fs::exists(root));
        throws<std::system_error>([&] { raii::TempDirectory duplicate(root); });
        CHECK(fs::exists(root));
        throws<std::invalid_argument>([&] { raii::TempFile bad(dir, "../escape"); });
        throws<std::invalid_argument>([&] { raii::TempFile bad(dir, std::string("a\0b", 3)); });
        {
            raii::TempDirectory child(dir, "child");
            held = std::make_unique<raii::TempFile>(child, "owned.txt");
            throws<std::system_error>([&] { raii::TempFile duplicate(child, "owned.txt"); });
            CHECK(fs::exists(held->path()));
            raii::TempFile a(child, "a"), b(child, "b");
            auto old = b.path();
            b = std::move(a);
            CHECK(!fs::exists(old));
            CHECK(fs::exists(b.path()));
        }
        CHECK(fs::exists(root + "/child/owned.txt"));
    }
    CHECK(fs::exists(root));
    held.reset();
    CHECK(!fs::exists(root));
    throws<std::invalid_argument>([] { raii::TempDirectory relative("relative"); });
    // Exercise the original command dispatcher using isolated streams and directory.
    auto cliRoot = (parent / "cli").string();
    std::istringstream input("current\nenter\ncreate\nlist\nremove 0\nleave\nquit\n");
    std::ostringstream output;
    struct Streams {
        std::streambuf *in;
        std::streambuf *out;
        ~Streams() {
            std::cin.rdbuf(in);
            std::cout.rdbuf(out);
        }
    } restore{std::cin.rdbuf(input.rdbuf()), std::cout.rdbuf(output.rdbuf())};
    raii::CommandLine().run(cliRoot);
    CHECK(!fs::exists(cliRoot));
    CHECK(output.str().find("created file") != std::string::npos);
}
template <class Map> void stress() {
    Map map(7);
    std::atomic<bool> start{false};
    std::vector<std::thread> threads;
    for (int worker = 0; worker < 4; ++worker)
        threads.emplace_back([&, worker] {
            while (!start.load(std::memory_order_acquire))
                std::this_thread::yield();
            for (int i = 0; i < 800; ++i)
                map.insert(i % 11 - 5, std::to_string(worker) + ":" + std::to_string(i));
        });
    std::atomic<bool> readerOkay{true};
    threads.emplace_back([&] {
        while (!start.load(std::memory_order_acquire))
            std::this_thread::yield();
        for (int i = 0; i < 400; ++i)
            for (const auto &value : map.values(i % 11 - 5))
                if (value.find(':') == std::string::npos)
                    readerOkay = false;
    });
    start.store(true, std::memory_order_release);
    for (auto &t : threads)
        t.join();
    CHECK(readerOkay.load());
    for (int key = -5; key <= 5; ++key) {
        auto values = map.values(key);
        std::set<std::string> actual(values.begin(), values.end()), expected;
        for (int w = 0; w < 4; ++w)
            for (int i = 0; i < 800; ++i)
                if (i % 11 - 5 == key)
                    expected.insert(std::to_string(w) + ":" + std::to_string(i));
        CHECK(actual == expected);
        CHECK(values.size() == expected.size());
    }
}
void concurrency() {
    multimap::SequentialMultiMap seq(1);
    seq.insert(4, "a");
    seq.insert(4, "a");
    seq.insert(-1, "b");
    CHECK(seq.values(4).size() == 2);
    CHECK(seq.values(-1) == std::vector<std::string>{"b"});
    throws<std::invalid_argument>([] { multimap::AtomicMultiMap bad(0); });
    stress<multimap::MutexMultiMap>();
    stress<multimap::AtomicMultiMap>();
}
} // namespace
int main(int argc, char **argv) {
    try {
        if (argc != 2)
            throw std::invalid_argument("provide a test suite name");
        const std::unordered_map<std::string, std::function<void()>> suites{
            {"algorithms", algorithms},
            {"containers", containers},
            {"numbers", numbers},
            {"expressions", expressions},
            {"vm", vm},
            {"resources", resources},
            {"concurrency", concurrency}};
        suites.at(argv[1])();
        std::cout << argv[1] << ": " << checks << " checks passed\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
