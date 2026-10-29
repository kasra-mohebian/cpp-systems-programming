# From coursework to a public edition

The starting point was completed individual C++ coursework. Before publication, the local submission history was compared with the supplied starting versions to identify the implementation work I had added. Original submissions and that detailed comparison stay private.

This repository presents those implementations in a standalone project. It does not reproduce the assignment sheets or claim that the public build and tests were the original course submission.

## Retained and adapted

- **Heap, pointer sorting and object representation:** retain my algorithm implementations. Public headers are independent declarations. Empty-heap extraction now throws; quicksort recurses into the smaller partition; floating-point layout assumptions are checked at compilation.
- **Rational, Complex and BitSet:** retain the value-type and packed-container designs. Rational arithmetic now checks overflow and zero denominators; comparison avoids cross-product overflow. Complex magnitude uses `hypot`. BitSet checks indices in Release as well as Debug, and moving resets the source size.
- **Chaining hash table:** retains my bucket, iterator, rehash and iterative-destruction implementation. A new `BytePayload` replaces the course-provided value type. Move operations reset the source count, and erasure detaches a node before releasing it.
- **Expression tree and visitor:** retain my node hierarchy, traversal and evaluation implementation behind a standalone interface. Parameter access is checked. The public optimizer is deliberately narrower: constant folding and unary-plus removal, without parameter-dependent algebraic rewrites that can change NaN, infinity or signed-zero behavior.
- **Temporary resource classes and command dispatcher:** retain the ownership design and command logic. A new, small POSIX adapter replaces the provided operating-system wrapper. Creation is exclusive, entry names are validated, failures throw, construction rolls back and cleanup does not throw.
- **Register VM:** retains my instruction-dispatch and Fibonacci generator implementation with stream-based entry points. The public version validates input and register names, rejects unsafe conversions and handles the signed division overflow case. Integer add/subtract/multiply use explicit modulo-2^32 arithmetic.
- **String search, Varint and union-find:** retain my matching, encoding and union/compression algorithms. Search uses `string_view` instead of a supplied C-string helper. Varint rejects truncation and overflow. Union-find has an independent checked public interface.
- **Multimaps:** the course supplied much of the surrounding container framework. That framework and its iterators are excluded. The public edition uses new, minimal sequential, mutex and atomic containers to present the insertion/synchronization concepts from my work. These are adaptations, not unchanged submitted files.

## New for this repository

The single CMake project, test harness, seven test suites, demo, CI, documentation and public interfaces/adapters were prepared for this portfolio. The tests use synthetic values and temporary test directories; no original course tests or participant data are redistributed.

## Deliberately excluded

Assignment wording, lecture notes, provided solutions and test suites, grading or CI infrastructure, screenshots of course systems, private URLs, server configuration, personal contact information and original Git metadata are excluded. Introductory tooling answers are replaced by [short original notes](tooling.md).

The MIT license applies to the material actually included here. It does not grant rights to the private course materials. Generic algorithm names identify the techniques; they are not claims that I invented heap sort, KMP, union-find or variable-length integer encoding.
