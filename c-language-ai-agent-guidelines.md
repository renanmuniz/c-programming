# C Language — Good Practices, Performance & Secure Coding

## Purpose

When writing, reviewing, refactoring, or explaining C code, prioritize:

1. Correctness
2. Memory safety
3. Security
4. Undefined-behavior avoidance
5. Performance
6. Portability
7. Readability and maintainability

Do not optimize prematurely. First establish correct and safe behavior, then optimize based on evidence.

---

---

## Index

1. [Part 1. Foundations](#part-1-foundations)
   - [1.1 General C Principles](#11-general-c-principles)
2. [Part 2. Memory and Pointers](#part-2-memory-and-pointers)
   - [2.1 Memory Management](#21-memory-management)
     - [Ownership](#ownership)
     - [Allocation](#allocation)
     - [Integer Overflow During Allocation](#integer-overflow-during-allocation)
     - [realloc](#realloc)
     - [Freeing](#freeing)
   - [2.2 Pointers](#22-pointers)
   - [2.3 Arrays and Buffer Safety](#23-arrays-and-buffer-safety)
   - [2.4 Pointer Aliasing](#24-pointer-aliasing)
   - [2.5 Alignment](#25-alignment)
3. [Part 3. Language Fundamentals](#part-3-language-fundamentals)
   - [3.1 Strings](#31-strings)
   - [3.2 Integer Types](#32-integer-types)
   - [3.3 Undefined Behavior](#33-undefined-behavior)
   - [3.4 Initialization](#34-initialization)
   - [3.5 Structs](#35-structs)
   - [3.6 const Correctness](#36-const-correctness)
   - [3.7 Static and Linkage](#37-static-and-linkage)
   - [3.8 Header Files](#38-header-files)
4. [Part 4. Performance](#part-4-performance)
   - [4.1 Performance Fundamentals](#41-performance-fundamentals)
   - [4.2 Algorithmic Complexity](#42-algorithmic-complexity)
   - [4.3 Linear Search vs Binary Search](#43-linear-search-vs-binary-search)
   - [4.4 Cache Locality](#44-cache-locality)
   - [4.5 Allocations](#45-allocations)
   - [4.6 Copying Data](#46-copying-data)
   - [4.7 memcpy and Overlapping Memory](#47-memcpy-and-overlapping-memory)
   - [4.8 Compiler Optimization](#48-compiler-optimization)
   - [4.9 volatile](#49-volatile)
   - [4.10 Data-Oriented Performance](#410-data-oriented-performance)
   - [4.11 Branches and Predictability](#411-branches-and-predictability)
   - [4.12 Performance and I/O](#412-performance-and-io)
   - [4.13 Benchmarking](#413-benchmarking)
   - [4.14 Profiling](#414-profiling)
5. [Part 5. Concurrency](#part-5-concurrency)
   - [5.1 Concurrency](#51-concurrency)
6. [Part 6. Error Handling and Resources](#part-6-error-handling-and-resources)
   - [6.1 Error Handling](#61-error-handling)
   - [6.2 Resource Management](#62-resource-management)
7. [Part 7. I/O and Systems](#part-7-io-and-systems)
   - [7.1 File and I/O Safety](#71-file-and-io-safety)
   - [7.2 Endianness and Serialization](#72-endianness-and-serialization)
   - [7.3 Network Programming](#73-network-programming)
8. [Part 8. Security](#part-8-security)
   - [8.1 Secure Input Handling](#81-secure-input-handling)
   - [8.2 Format String Security](#82-format-string-security)
   - [8.3 Integer Conversion and Validation](#83-integer-conversion-and-validation)
   - [8.4 Command Execution](#84-command-execution)
   - [8.5 Path Traversal](#85-path-traversal)
   - [8.6 Sensitive Data](#86-sensitive-data)
   - [8.7 Cryptography](#87-cryptography)
9. [Part 9. API and Code Design](#part-9-api-and-code-design)
   - [9.1 API Design](#91-api-design)
   - [9.2 Assertions](#92-assertions)
   - [9.3 Macros](#93-macros)
   - [9.4 API and Library Selection](#94-api-and-library-selection)
10. [Part 10. Portability and Tooling](#part-10-portability-and-tooling)
   - [10.1 C Standard Version](#101-c-standard-version)
   - [10.2 Portability](#102-portability)
   - [10.3 Testing](#103-testing)
   - [10.4 Static Analysis](#104-static-analysis)
11. [Part 11. AI Guidance and Review](#part-11-ai-guidance-and-review)
   - [11.1 AI-Specific Rules](#111-ai-specific-rules)
   - [11.2 Code Review Checklist](#112-code-review-checklist)
     - [Correctness](#correctness)
     - [Memory Safety](#memory-safety)
     - [Undefined Behavior](#undefined-behavior)
     - [Security](#security)
     - [Performance](#performance)
     - [Maintainability](#maintainability)
   - [11.3 Priority Order for Fixes](#113-priority-order-for-fixes)
   - [11.4 Golden Rule](#114-golden-rule)

---

# Part 1. Foundations

## 1.1 General C Principles

- Prefer simple, explicit C over clever code.
- Make ownership and lifetime of memory obvious.
- Minimize global state.
- Keep functions small and focused.
- Prefer explicit error handling.
- Avoid unnecessary abstraction.
- Prefer well-defined behavior over implementation-specific behavior.
- Compile with warnings enabled and treat warnings as errors when practical.
- Do not assume that behavior is safe merely because it works on one compiler or architecture.

Recommended compiler flags during development:

```bash
-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror
```

Also consider `-Wformat=2`, `-Wvla`, and `-Wnull-dereference`, which catch
issues discussed later in this document (format-string misuse, variable-length
arrays, and potential null dereferences).

Consider additional warnings appropriate for the compiler/project.

Use sanitizers during development and testing:

```bash
-fsanitize=address,undefined
```

For supported environments, also consider:

```bash
-fsanitize=leak
```

---

# Part 2. Memory and Pointers

## 2.1 Memory Management

### Ownership

Every dynamically allocated object should have a clearly identifiable owner.

For every `malloc`, `calloc`, or `realloc`:

- Know who owns the resulting memory.
- Know who is responsible for freeing it.
- Know when its lifetime ends.
- Avoid ambiguous ownership.

Example:

```c
char *buffer = malloc(size);

if (buffer == NULL) {
    return ERROR;
}

/* use buffer */

free(buffer);
```

Prefer a clear allocation/free relationship.

### Allocation

Always check allocation failures when failure is possible and meaningful:

```c
int *values = malloc(count * sizeof(*values));

if (values == NULL) {
    return ERROR;
}
```

Prefer:

```c
malloc(count * sizeof(*ptr))
```

over:

```c
malloc(count * sizeof(int))
```

because the allocation remains correct if the pointed-to type changes.

### Integer Overflow During Allocation

Be especially careful with:

```c
malloc(count * sizeof(*ptr))
```

because `count * sizeof(*ptr)` can overflow before `malloc()` receives the value.

For security-sensitive or externally controlled sizes, validate the multiplication:

```c
if (count > SIZE_MAX / sizeof(*ptr)) {
    return ERROR;
}
```

Never allow an integer overflow to turn into an undersized allocation.

Note that `calloc(count, sizeof(*ptr))` performs this multiplication-overflow
check internally and zero-initializes the memory. On platforms that provide it,
`reallocarray()` offers the same overflow-safe behavior for growing arrays.

### realloc

Never directly overwrite the only pointer to an allocation:

```c
ptr = realloc(ptr, new_size);
```

If `realloc()` fails, the original allocation is still valid but the pointer has been lost.

Prefer:

```c
void *tmp = realloc(ptr, new_size);

if (tmp == NULL) {
    /* ptr is still valid */
    return ERROR;
}

ptr = tmp;
```

### Freeing

After `free(ptr)`, set `ptr = NULL;` when the pointer's lifetime does not
otherwise end immediately. A `NULL` pointer makes a subsequent accidental use
either a well-defined no-op (`free(NULL)` is safe) or an immediate, easier to
diagnose crash on dereference, rather than silent use-after-free or
double-free corruption.

## 2.2 Pointers

- Initialize pointers.
- Check pointers before dereferencing when `NULL` is a valid possibility.
- Do not dereference pointers after their lifetime ends.
- Do not return pointers to local variables.
- Do not use pointers after `free()`.
- Do not access memory outside the object they point to.
- Avoid pointer arithmetic unless its bounds are obvious.

Never do:

```c
int *get_value(void)
{
    int value = 42;
    return &value;
}
```

The returned pointer becomes invalid when the function returns.

## 2.3 Arrays and Buffer Safety

C does not automatically perform bounds checking.

Always know:

- Buffer capacity
- Current length
- Required length
- Whether a null terminator is required

Prefer APIs that receive buffer sizes:

```c
void process(char *buffer, size_t capacity);
```

rather than:

```c
void process(char *buffer);
```

Avoid unsafe string functions when appropriate:

```c
strcpy()
strcat()
sprintf()
gets()
```

Prefer bounded or explicitly size-aware alternatives.

However, do not blindly replace every function with `strncpy()` or similar functions. Understand their semantics first.

For formatted output, prefer:

```c
snprintf(buffer, sizeof(buffer), "%s", value);
```

and check the return value when truncation matters.

Note that `sizeof(buffer)` only yields the buffer's capacity when `buffer` is an
array. If `buffer` is a pointer, `sizeof` returns the size of the pointer, not
the buffer — pass the capacity explicitly in that case.

## 2.4 Pointer Aliasing

Be aware that the compiler makes assumptions about pointer aliasing.

Avoid violating the rules around effective types and strict aliasing.

Do not casually cast unrelated pointer types and access objects through them.

For binary serialization/deserialization, prefer safe approaches such as:

```c
memcpy()
```

when appropriate rather than relying on invalid pointer casts.

Conversely, when you can guarantee that two pointers never refer to the same
object, the `restrict` qualifier lets you communicate this to the compiler and
enables optimizations that strict-aliasing rules would otherwise prevent. Only
use `restrict` when the non-aliasing guarantee genuinely holds; violating it is
undefined behavior.

## 2.5 Alignment

Do not assume arbitrary memory is correctly aligned for every type.

For example, avoid assuming that a byte buffer can always safely be cast to an arbitrary struct pointer.

Bad:

```c
struct Header *header = (struct Header *)buffer;
```

when `buffer` may not have the required alignment or object representation.

Consider `memcpy()` into a properly aligned object when appropriate.

---

# Part 3. Language Fundamentals

## 3.1 Strings

C strings require a terminating `'\0'`.

Remember:

```text
"hello"
```

requires:

```text
'h' 'e' 'l' 'l' 'o' '\0'
```

Therefore:

```c
char buffer[5] = "hello";
```

is legal C, but it produces an array with **no null terminator**. Per the
standard, when a char array of known size has exactly enough room for the
characters but not the terminating `'\0'`, the terminator is silently dropped.
The array is therefore not a valid C string, and using it with functions such
as `strlen()`, `strcpy()`, or `printf("%s", ...)` is undefined behavior.

Use:

```c
char buffer[6] = "hello";
```

or let the compiler size the array:

```c
char buffer[] = "hello";  /* size is 6, including '\0' */
```

When working with strings, distinguish between:

- buffer capacity
- string length
- allocated size

Do not assume `strlen()` is O(1).

`strlen()` scans until `'\0'`, therefore:

```text
strlen() = O(n)
```

Avoid repeatedly calling `strlen()` inside loops when the string length is known.

Bad:

```c
for (size_t i = 0; i < strlen(str); i++) {
    ...
}
```

Potentially:

```text
O(n²)
```

Better:

```c
size_t len = strlen(str);

for (size_t i = 0; i < len; i++) {
    ...
}
```

## 3.2 Integer Types

Choose integer types intentionally.

Use:

```c
size_t
```

for:

- object sizes
- array indexes when appropriate
- results of `sizeof`
- memory allocation sizes

Example:

```c
for (size_t i = 0; i < count; i++) {
    ...
}
```

Be careful when mixing signed and unsigned integers.

Do not casually cast away warnings caused by signed/unsigned mismatches. Understand the conversion first.

Remember that integer overflow behavior differs:

- Unsigned integer arithmetic wraps modulo 2^N.
- Signed integer overflow is undefined behavior.

Never rely on signed overflow.

## 3.3 Undefined Behavior

Treat undefined behavior as a critical defect.

Common sources include:

- Out-of-bounds access
- Use-after-free
- Double-free
- Dereferencing `NULL`
- Dereferencing invalid pointers
- Returning pointers to local variables
- Signed integer overflow
- Invalid shifts
- Reading uninitialized values
- Violating alignment requirements
- Invalid type punning
- Modifying string literals
- Invalid format strings
- Incorrect lifetime assumptions

Do not justify undefined behavior with:

> "It works on my machine."

The compiler is allowed to assume undefined behavior never occurs and optimize accordingly.

## 3.4 Initialization

Prefer initializing variables when practical:

```c
int count = 0;
char *buffer = NULL;
```

Do not read variables before they have been initialized.

Be especially careful with:

```c
int value;
if (condition) {
    value = 10;
}

printf("%d", value);
```

Every control-flow path must establish a valid value before use.

## 3.5 Structs

Use structs to group related state.

Prefer clear initialization:

```c
struct User user = {
    .id = 10,
    .active = true
};
```

Design structs so invalid states are difficult to represent.

When copying large structs, consider whether copying is actually necessary.

Remember that:

```c
struct Large a = b;
```

copies the entire structure.

For performance-sensitive code, understand the size and cost of the copy.

## 3.6 const Correctness

Use `const` to communicate immutability.

Prefer:

```c
void print_name(const char *name);
```

over:

```c
void print_name(char *name);
```

when the function does not modify the string.

`const` improves:

- API clarity
- compiler diagnostics
- maintainability
- reasoning about side effects

Do not cast away `const` unless there is a very specific and justified reason.

## 3.7 Static and Linkage

Use `static` for internal functions that should not be visible outside a translation unit:

```c
static int calculate_value(int x);
```

This:

- reduces the public API
- prevents symbol collisions
- helps encapsulation
- can provide optimization opportunities

Do not expose functions globally unless they are part of the module's intended API.

## 3.8 Header Files

Headers should contain declarations and public types, not unnecessary implementation details.

Use include guards or the project's established alternative:

```c
#ifndef MY_MODULE_H
#define MY_MODULE_H

...

#endif
```

Include what you use.

Avoid relying on another header indirectly including something your file needs.

Bad:

```c
#include "other.h"

size_t function(void);
```

if `size_t` is only available accidentally through `other.h`.

Include the appropriate standard header explicitly.

---

# Part 4. Performance

## 4.1 Performance Fundamentals

Do not optimize based only on intuition.

Think about:

- Time complexity
- Space complexity
- Memory access patterns
- Cache locality
- Allocations
- Copies
- Branches
- System calls
- I/O
- Lock contention
- Algorithm choice

The biggest performance improvement is often choosing a better algorithm.

## 4.2 Algorithmic Complexity

Understand Big-O complexity.

Examples:

Linear search:

```text
O(n)
```

Binary search on sorted data:

```text
O(log n)
```

Nested linear searches:

```text
O(n²)
```

Hash-table lookup:

```text
Average: O(1)
Worst case: O(n)
```

When reviewing code, identify unnecessary complexity.

Do not replace a simple O(n) algorithm with a complicated optimization unless there is a demonstrated performance requirement.

## 4.3 Linear Search vs Binary Search

For a linear search:

```text
O(n)
```

The data does not need to be sorted.

Binary search:

```text
O(log n)
```

requires sorted data and random access to the search space.

For example, searching 1,000,000 sorted elements with binary search requires roughly:

```text
log₂(1,000,000) ≈ 20
```

comparisons in the ideal case.

However, binary search is not automatically faster in every real-world situation.

Consider:

- cost of maintaining sorted data
- cache locality
- data size
- number of searches
- cost of comparisons
- insertion/deletion requirements

For small arrays, a linear scan can outperform a theoretically better algorithm because of cache locality and lower constant overhead.

## 4.4 Cache Locality

Modern CPUs are much faster than main memory.

Prefer data layouts that provide good spatial locality.

For example, iterating sequentially over an array is generally cache-friendly:

```c
for (size_t i = 0; i < count; i++) {
    total += values[i];
}
```

Random memory access can be significantly more expensive.

When performance matters, consider:

- contiguous arrays
- sequential access
- structure layout
- pointer chasing
- cache misses

Do not assume that an algorithm with better Big-O complexity will always be faster for small datasets.

## 4.5 Allocations

Dynamic memory allocation can have non-trivial cost.

Avoid unnecessary repeated allocations:

```c
for (...) {
    buffer = malloc(...);
    ...
    free(buffer);
}
```

when the buffer could safely be reused.

Prefer reusing storage when appropriate.

For growing dynamic arrays, avoid increasing capacity by only one element at a time.

Prefer a growth strategy such as:

```text
capacity *= 2
```

or another measured strategy.

Be careful about integer overflow during capacity growth.

## 4.6 Copying Data

Avoid unnecessary copies of large objects.

Consider:

```c
memcpy()
```

when copying raw object representations is valid and appropriate.

However, do not use `memcpy()` blindly on objects containing pointers when a deep copy is required.

Understand whether the required operation is:

- shallow copy
- deep copy
- move/ownership transfer
- reference

## 4.7 memcpy and Overlapping Memory

`memcpy()` is not valid when source and destination overlap.

Use:

```c
memmove()
```

when overlap is possible.

Example:

```c
memmove(buffer + 1, buffer, length);
```

Do not assume that `memcpy()` will behave correctly for overlapping ranges.

## 4.8 Compiler Optimization

Do not manually reproduce optimizations that the compiler already performs unless profiling demonstrates a need.

Prefer clear code and allow the compiler to optimize it.

Understand optimization levels:

```text
-O0
-O1
-O2
-O3
-Os
```

For performance-sensitive applications, benchmark realistic builds rather than relying on debug builds.

Do not conclude that C code is slow merely because it is being executed with `-O0`.

## 4.9 volatile

Do not use `volatile` as a general-purpose synchronization mechanism or performance tool.

`volatile` is appropriate for specific cases such as:

- memory-mapped hardware
- certain signal-handler scenarios
- compiler-visible externally changing memory

`volatile` does not make operations atomic and does not replace mutexes or atomics.

## 4.10 Data-Oriented Performance

When performance is critical, consider data layout.

Compare:

```c
struct Particle {
    float x;
    float y;
    float z;
    float velocity;
    int active;
};

struct Particle particles[10000];
```

with layouts where individual fields are stored contiguously.

The appropriate layout depends on the workload.

Consider:

- cache locality
- sequential access
- memory footprint
- vectorization
- access patterns

Do not apply one layout universally.

## 4.11 Branches and Predictability

Modern CPUs use branch prediction.

Do not manually eliminate branches without measurement.

Instead:

- keep hot paths simple
- avoid unnecessary unpredictable branches
- structure frequently executed code clearly
- profile before micro-optimizing

Algorithm and memory-access improvements usually matter more than tiny branch-level optimizations.

## 4.12 Performance and I/O

System calls and I/O can be much more expensive than ordinary arithmetic.

Avoid unnecessary:

- filesystem calls
- network calls
- allocations
- synchronization
- logging
- database calls

Batch operations when appropriate.

Measure before optimizing.

## 4.13 Benchmarking

When performance matters, benchmark realistic workloads.

Measure:

- latency
- throughput
- memory usage
- allocations
- CPU usage
- cache behavior when relevant

Do not benchmark only tiny artificial inputs.

Compare:

```text
baseline
vs.
optimized version
```

using the same workload and environment.

Avoid relying on a single benchmark run.

## 4.14 Profiling

When optimization is requested, prefer profiling over guessing.

Useful categories of tools include:

- compiler optimization reports
- sanitizers
- Valgrind
- perf
- Instruments
- platform profilers
- heap profilers

Find the actual bottleneck before changing the algorithm or data structure.

---

# Part 5. Concurrency

## 5.1 Concurrency

When using threads:

- Protect shared mutable state.
- Avoid data races.
- Understand atomicity.
- Use mutexes, condition variables, or C11 atomics where appropriate.
- Keep lock scope as small as practical.
- Avoid holding locks while performing slow I/O.
- Establish a consistent lock-ordering strategy.

Data races can result in undefined behavior.

Use:

```c
#include <stdatomic.h>
```

for C11 atomic operations when appropriate.

Do not assume that:

```c
counter++;
```

is atomic.

It generally consists of multiple operations:

```text
read → modify → write
```

---

# Part 6. Error Handling and Resources

## 6.1 Error Handling

C does not have exceptions.

Errors must be represented explicitly.

Common approaches include:

- return codes
- `NULL`
- `errno`
- output parameters
- tagged result structures

Always document what a function returns on failure.

Example:

```c
int result = operation();

if (result != 0) {
    handle_error(result);
}
```

Do not silently ignore important errors.

Be especially careful with:

- `malloc`
- `fopen`
- `fread`
- `fwrite`
- `read`
- `write`
- `close`
- `pthread_*`
- network APIs
- parsing functions

## 6.2 Resource Management

Every acquired resource should have a corresponding release path.

Resources include:

- heap memory
- files
- sockets
- mutexes
- threads
- database connections
- OS handles

Pay attention to early returns.

Instead of:

```c
resource = acquire();

if (error1)
    return ERROR;

if (error2)
    return ERROR;

release(resource);
```

structure the code so all failure paths release resources appropriately.

A common C pattern is cleanup through a single exit path:

```c
int result = ERROR;
void *resource = NULL;

resource = acquire();

if (resource == NULL)
    goto cleanup;

if (operation(resource) != 0)
    goto cleanup;

result = SUCCESS;

cleanup:
release(resource);
return result;
```

Use `goto` carefully for structured cleanup. In C, this can be clearer and safer than deeply nested conditionals.

---

# Part 7. I/O and Systems

## 7.1 File and I/O Safety

Always check file operation results.

Bad:

```c
FILE *file = fopen(path, "r");
fgets(buffer, sizeof(buffer), file);
```

Better:

```c
FILE *file = fopen(path, "r");

if (file == NULL) {
    return ERROR;
}

if (fgets(buffer, sizeof(buffer), file) == NULL) {
    fclose(file);
    return ERROR;
}

fclose(file);
```

Do not assume I/O succeeds.

I/O can fail because of:

- permissions
- missing files
- disk errors
- network failures
- interruptions
- resource exhaustion

## 7.2 Endianness and Serialization

Do not assume all machines use the same byte order.

When serializing data for:

- networks
- files
- IPC
- distributed systems

define the representation explicitly.

Consider:

- byte order
- integer width
- floating-point representation
- structure padding
- alignment
- versioning

Do not blindly serialize a struct by writing:

```c
fwrite(&object, sizeof(object), 1, file);
```

for portable persistent formats.

Structs may contain padding and platform-dependent representations.

## 7.3 Network Programming

When dealing with sockets:

- Never assume `send()` writes everything.
- Never assume `recv()` returns a complete message.
- Handle partial reads/writes.
- Validate message lengths.
- Define explicit message framing.
- Validate all received data.
- Avoid trusting client-provided sizes.

For TCP, remember:

```text
TCP = byte stream
```

It does not preserve application-level message boundaries.

A single `send()` does not necessarily correspond to a single `recv()`.

---

# Part 8. Security

## 8.1 Secure Input Handling

Treat all external input as untrusted.

This includes:

- command-line arguments
- environment variables
- files
- network data
- HTTP requests
- JSON
- configuration files
- user input
- IPC messages

Validate:

- length
- format
- range
- encoding
- expected values

Do not trust input merely because it comes from an internal service.

## 8.2 Format String Security

Never pass untrusted input directly as the format string.

Dangerous:

```c
printf(user_input);
```

Prefer:

```c
printf("%s", user_input);
```

The format string should normally be a constant controlled by the program.

## 8.3 Integer Conversion and Validation

Be careful when converting external input to integers.

Do not assume:

```c
atoi()
```

provides sufficient error handling.

For robust parsing, consider:

```c
strtol()
strtoul()
strtoll()
strtoull()
```

Validate:

- conversion errors
- range
- trailing characters
- negative values when not allowed

`strtol()` and similar functions report overflow via `errno == ERANGE`, but
they do not reset `errno` to `0` themselves. Set `errno = 0;` immediately
before the call, or a stale `errno` value from an earlier, unrelated failure
can be mistaken for an overflow on this call.

Example input:

```text
123abc
```

should not silently become:

```text
123
```

when the application expects a complete integer.

## 8.4 Command Execution

Avoid constructing shell commands using untrusted input.

Dangerous:

```c
char command[512];

snprintf(command, sizeof(command), "rm %s", filename);
system(command);
```

This can create command-injection vulnerabilities.

Prefer APIs that execute programs without invoking a shell when possible.

If shell execution is unavoidable:

- strictly validate input
- avoid concatenating untrusted strings
- use safe argument boundaries
- minimize privileges

## 8.5 Path Traversal

When accepting filenames or paths from external input, consider:

```text
../
../../
absolute paths
symbolic links
unexpected encodings
```

Do not assume that a filename is safe simply because it does not contain obvious malicious characters.

Where appropriate, constrain file access to an intended directory and validate canonical paths.

## 8.6 Sensitive Data

Do not log:

- passwords
- authentication tokens
- API keys
- private keys
- session tokens
- sensitive personal information

Be careful when dumping structs or request objects to logs.

If sensitive data must be stored temporarily:

- minimize its lifetime
- minimize copies
- restrict access
- clear sensitive buffers when appropriate

Do not assume that `memset()` always reliably erases sensitive data if the compiler can prove the memory is no longer needed. Use an appropriate secure-zeroing mechanism when required by the platform, such as `explicit_bzero()` (BSD/glibc), `memset_s()` (C11 Annex K, where available), or `SecureZeroMemory()` (Windows).

## 8.7 Cryptography

Do not implement cryptographic algorithms yourself unless the purpose is educational or there is a highly specialized requirement.

Prefer established, audited libraries.

Do not invent:

- encryption algorithms
- password hashing algorithms
- random-number generators
- authentication protocols

Use cryptographically secure randomness for security-sensitive values, such as
`getrandom()` (Linux), `arc4random()` (BSD/macOS), `/dev/urandom`, or
`BCryptGenRandom()` (Windows). When a vetted cryptography library is already in
use, prefer its own RNG wrapper over calling these platform APIs directly.

Do not use:

```c
rand()
```

for:

- passwords
- tokens
- session IDs
- cryptographic keys
- security-sensitive nonces

---

# Part 9. API and Code Design

## 9.1 API Design

Design APIs around explicit contracts.

For every function, make clear:

- What arguments are valid?
- Can arguments be `NULL`?
- Who owns returned memory?
- Who owns input memory?
- What does the return value mean?
- What errors can occur?
- Is the function thread-safe?
- Can it modify its arguments?
- Is it blocking?
- What are the performance characteristics?

Avoid APIs where ownership is ambiguous.

## 9.2 Assertions

Use assertions for programmer assumptions and invariants:

```c
assert(ptr != NULL);
```

Do not use assertions as the only validation of untrusted input.

Assertions may be disabled in production builds.

Bad:

```c
assert(user_input_size < MAX_SIZE);
```

if the check is required for security.

Instead, explicitly validate:

```c
if (user_input_size >= MAX_SIZE) {
    return ERROR;
}
```

## 9.3 Macros

Prefer functions, `enum`, `const`, or `static inline` functions when they provide better type safety.

Macros can:

- evaluate arguments multiple times
- ignore type checking
- create difficult debugging problems
- introduce precedence bugs

Dangerous:

```c
#define SQUARE(x) x * x
```

Because:

```c
SQUARE(2 + 3)
```

becomes:

```c
2 + 3 * 2 + 3
```

If a macro is necessary, parenthesize carefully:

```c
#define SQUARE(x) ((x) * (x))
```

Still consider whether a function or `static inline` function would be preferable.

## 9.4 API and Library Selection

Before implementing functionality manually, consider whether a mature library already provides it.

Prefer established libraries for:

- JSON
- HTTP
- TLS
- cryptography
- compression
- image processing
- databases
- Unicode
- parsing

Evaluate:

- maintenance
- license
- security history
- portability
- performance
- dependency size
- API quality

Do not add a dependency for trivial functionality without justification.

---

# Part 10. Portability and Tooling

## 10.1 C Standard Version

Prefer modern, well-supported C standards when project constraints allow.

Understand the project's target:

```text
C11
C17
C23
```

Do not use features unavailable on the project's supported compilers or platforms.

When writing portable code, distinguish between:

- ISO C
- POSIX
- platform-specific APIs
- compiler extensions

Do not present platform-specific behavior as standard C.

## 10.2 Portability

Do not assume:

```c
sizeof(int) == 4
```

or that:

```c
sizeof(long) == 8
```

Use fixed-width types when exact widths matter:

```c
#include <stdint.h>

uint32_t
uint64_t
int32_t
int64_t
```

Use:

```c
size_t
```

for object sizes.

Use:

```c
ptrdiff_t
```

when a signed type capable of representing pointer differences is appropriate.

## 10.3 Testing

Test boundary conditions.

At minimum consider:

- zero
- one
- minimum valid value
- maximum valid value
- empty strings
- maximum-length strings
- `NULL` where permitted
- allocation failure
- invalid input
- duplicate values
- very large input
- malformed input

For algorithms, test both correctness and complexity-sensitive cases.

Example:

```text
empty array
single-element array
already sorted array
reverse-sorted array
duplicate-heavy array
large array
```

## 10.4 Static Analysis

Use static analysis when available.

Look for:

- memory leaks
- null dereferences
- buffer overflows
- use-after-free
- uninitialized values
- dead code
- suspicious conversions
- unreachable code
- concurrency issues

Compiler warnings are one of the first lines of defense.

---

# Part 11. AI Guidance and Review

## 11.1 AI-Specific Rules

When generating or modifying C code:

1. Do not invent APIs or library behavior.
2. Do not assume a function is safe without checking its semantics.
3. Do not hide memory ownership.
4. Do not introduce unnecessary dynamic allocation.
5. Do not suppress compiler warnings just to make code compile.
6. Do not use casts to silence type errors without understanding the underlying issue.
7. Do not optimize based solely on Big-O.
8. Do not claim code is thread-safe without analyzing shared state.
9. Do not claim code is secure without considering input validation and memory safety.
10. Prefer standard C and well-established APIs unless the project specifies otherwise.
11. Preserve existing project conventions unless they conflict with correctness or security.
12. When uncertain about undefined behavior, investigate rather than guessing.
13. When changing performance-sensitive code, explain the expected complexity and trade-offs.
14. When introducing dynamic memory, explicitly identify allocation and ownership.
15. When handling external input, explicitly identify validation and bounds checks.

## 11.2 Code Review Checklist

When reviewing C code, check:

### Correctness

- [ ] Are all variables initialized?
- [ ] Are all return values handled?
- [ ] Are all control-flow paths valid?
- [ ] Are boundary conditions handled?
- [ ] Are assumptions documented?

### Memory Safety

- [ ] Any buffer overflow?
- [ ] Any out-of-bounds access?
- [ ] Any use-after-free?
- [ ] Any double-free?
- [ ] Any memory leak?
- [ ] Any invalid pointer?
- [ ] Any allocation-size overflow?
- [ ] Is ownership clear?

### Undefined Behavior

- [ ] Signed integer overflow?
- [ ] Invalid shifts?
- [ ] Invalid pointer arithmetic?
- [ ] Uninitialized reads?
- [ ] Invalid alignment?
- [ ] Strict-aliasing violations?
- [ ] Lifetime violations?

### Security

- [ ] Is external input validated?
- [ ] Any format-string vulnerability?
- [ ] Any command injection?
- [ ] Any path traversal?
- [ ] Any integer overflow?
- [ ] Any sensitive data logged?
- [ ] Any unsafe cryptography?
- [ ] Any insecure randomness?

### Performance

- [ ] What is the time complexity?
- [ ] What is the space complexity?
- [ ] Are there unnecessary allocations?
- [ ] Are there unnecessary copies?
- [ ] Is cache locality reasonable?
- [ ] Are expensive operations repeated?
- [ ] Is there unnecessary I/O?
- [ ] Has optimization been justified by measurement?

### Maintainability

- [ ] Are names clear?
- [ ] Are functions focused?
- [ ] Is ownership obvious?
- [ ] Are APIs explicit?
- [ ] Is `const` used appropriately?
- [ ] Are global variables minimized?
- [ ] Are macros justified?
- [ ] Is platform-specific behavior clearly identified?

## 11.3 Priority Order for Fixes

When multiple problems exist, prioritize approximately in this order:

1. Undefined behavior
2. Memory corruption
3. Security vulnerabilities
4. Data races
5. Incorrect error handling
6. Resource leaks
7. Incorrect algorithms
8. Major performance problems
9. Portability problems
10. Maintainability/style issues
11. Micro-optimizations

Do not spend time optimizing code that is fundamentally incorrect or unsafe.

## 11.4 Golden Rule

When working with C:

> Make the memory, lifetime, ownership, bounds, and cost of an operation explicit.

Prefer code that is easy to reason about over code that is merely clever.

Correct and secure code comes first.

Then measure.

Then optimize the actual bottleneck.
