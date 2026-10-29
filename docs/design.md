# Design notes

## Containers and algorithms

The max heap stores unsigned values in a vector. Insertion and extraction repair one root-to-leaf path, giving O(log n) work; extracting from an empty heap throws.

The pointer sorts rearrange pointers rather than the underlying unsigned values. Mergesort is stable and O(n log n), with temporary storage. Quicksort uses a first-element pivot, so ordered or equal-heavy input can still take O(n²) time. Recursing into the smaller partition bounds its call-stack depth. Pass a valid half-open range from one array; every stored pointer must refer to a live unsigned value. An empty equal-pointer range is accepted.

KMP builds an O(m) prefix table and scans in O(n + m) time. Both searches return `string_view::npos` for no match and zero for an empty pattern. Views can contain embedded NUL bytes.

Union-find uses union by rank and path compression. Its amortized cost is near constant, O(α(n)); `find` modifies parent links even though its public signature is const. It is not thread-safe.

The chaining table owns nodes with `unique_ptr`, grows above a load factor of 0.5 and replaces a value when inserting an existing key. Hashing is a simple integer remainder, intended for learning rather than adversarial keys. Lookup is expected O(1), worst-case O(n). Rehashing invalidates iterators but leaves surviving node/value references valid; erasure invalidates references to that entry. Do not use an iterator after moving or destroying its table. The replacement payload is a zero-initialized, move-only array of 64 bytes.

BitSet packs values into bytes and returns proxy references. A proxy contains a pointer into the byte vector: growth that reallocates, assignment, moving ownership or destruction can invalidate it. Do not retain proxies across mutations that change storage. End/default iterators cannot be dereferenced. Cardinality scans all bits in O(n).

The byte-stack example uses `memcpy` and size tags to round-trip float/double object representations. Wrong-sized or incomplete pops leave the buffer unchanged. The bytes are native-machine representations, not a portable serialization format. This module requires IEEE-754 32-bit float and 64-bit double.

## Numbers and encoding

Rational normalizes signs and reduces using gcd. It uses bounded `long long` arithmetic with GCC/Clang overflow intrinsics. Zero denominators and inversion of zero throw `domain_error`; overflow throws `overflow_error`, leaving compound-operation operands unchanged. `LLONG_MIN` is intentionally outside its representable input domain. Intermediate products may overflow even when a mathematically reduced result would fit: this is not an arbitrary-precision rational library. Comparison uses continued fractions to avoid overflowing cross-products.

Complex is a small double-precision teaching type. Magnitude uses `hypot`; the other formulas retain ordinary floating-point behavior and may overflow or produce NaN/infinity at extreme magnitudes or zero division. Use `std::complex` for a general-purpose implementation.

Varint is unsigned base-128 little-endian encoding, not compression for every value distribution. It accepts non-minimal but numerically valid encodings; the encoder always emits minimal ones. Truncated streams and values beyond uint64 are rejected, including invalid tenth-byte payloads.

## Expressions and execution

Expression nodes own children with `unique_ptr`. Construct children before parents, supply non-null children, and call optimization as `root->optimize(root)` with the owning pointer. Released children leave a node unsuitable for evaluation until replaced. Evaluation uses double arithmetic and may return NaN/infinity. Optimization folds constant subtrees; it does not apply identities such as `0 / x -> 0`, which fail at zero.

The VM reads a line-oriented numeric instruction stream with four signed integer and two floating registers. It is an instructional interpreter, not a sandbox for untrusted programs. Integer arithmetic wraps modulo 2^32 except signed division, which rejects zero and `INT32_MIN / -1`. Float-to-integer conversion checks range and finiteness. Malformed instructions throw. EOF returns register A; HALT returns immediately. Fibonacci generation emits O(n) instructions and the result wraps when it exceeds the VM integer range.

## Resource lifetime

A file holds shared ownership of its directory state; a child directory similarly keeps its parent alive. Destruction closes owned descriptors and removes owned entries. The public adapter uses exclusive creation and refuses existing target paths. Names must be one path component without NUL bytes. It does not follow the final entry as a symlink.

Use a fresh absolute path under a private parent you control. Parent components must be trusted. Other processes can rename or replace paths, so this teaching wrapper does not provide a hostile-filesystem guarantee. Cleanup errors are ignored by the non-throwing destructors: unexpected external files or permissions can prevent removal. No recursive deletion is performed by the resource classes. Test cleanup is limited to its own newly created temporary tree.

## Concurrent insertion

The sequential multimap needs external synchronization. The mutex variant serializes reads and insertions. The atomic variant allocates a node, prepares its immutable link and publishes it using release compare-and-exchange. Readers acquire a bucket head and traverse immutable nodes. Atomic read-modify-write publication chains keep the previously published nodes visible.

There is no erase, reclamation or resize while the map is in use. A reader sees one bucket-head snapshot; there is no global multi-key snapshot. All threads must finish before destruction. No ABA reclamation scheme is needed under this append-only, no-reuse lifetime rule. Do not extend it with deletion without redesigning reclamation.

The CAS insertion loop uses atomic bucket heads, but the entire operation is not promised to be lock-free: allocation and `atomic<Entry*>` are platform-dependent. `bucketHeadsAreLockFree()` reports the latter property only. There are no published throughput claims or benchmark results.
