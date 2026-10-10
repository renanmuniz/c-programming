# Secure C Programming — Quick Guide

Brief rules and examples for avoiding common C security problems.

**Structure:**

1. [Integer Safety](#part-i--integer-safety) (§1–3)
2. [Floating-Point Safety](#part-ii--floating-point-safety) (§4–8)
3. [Input Handling](#part-iii--input-handling) (§9–13)
4. [Memory Safety](#part-iv--memory-safety) (§14–18)
5. [Random Numbers](#part-v--random-numbers) (§19)
6. [Checklist & Core Principle](#secure-c-checklist)

---

# Part I — Integer Safety

## 1. Integer Overflow

### Problem

Signed integer overflow is **undefined behavior**.

```c
int total = quantity + price; // Could overflow
```

### Safe addition

Check before performing the operation:

```c
#include <limits.h>

if (b > 0 && a > INT_MAX - b) {
    // overflow
} else if (b < 0 && a < INT_MIN - b) {
    // underflow
} else {
    int result = a + b;
}
```

### Safe multiplication

```c
#include <stdint.h>

if (count > SIZE_MAX / sizeof(Item)) {
    // size overflow
} else {
    size_t size = (size_t)count * sizeof(Item);
}
```

> **Tip:** GCC and Clang provide `__builtin_add_overflow`, `__builtin_mul_overflow`, etc., which are both safer and clearer than manual checks when portability to other compilers is not required.

**Rule:** Validate arithmetic **before** performing it.

---

## 2. Integer Range Validation

Never assume user input is valid.

```c
int quantity;

if (scanf("%d", &quantity) != 1) {
    // Invalid input
    return 1;
}

if (quantity < 0 || quantity > 1000) {
    // Invalid range
    return 1;
}
```

**Rule:** Check both parsing success and acceptable ranges.

---

## 3. Signed vs. Unsigned Overflow

Unsigned overflow is defined to wrap:

```c
unsigned int x = UINT_MAX;
x++;
// x becomes 0
```

But this can still cause security bugs.

Don't rely on wrapping when calculating sizes:

```c
if (count > SIZE_MAX / element_size) {
    return 1;
}

size_t total = count * element_size;
```

**Rule:** Defined behavior does not necessarily mean safe behavior.

### Use `size_t` for Object Sizes

`size_t` is the standard unsigned integer type for representing the size of
objects. Use it for results from `sizeof`, array lengths, and indexes derived
from those lengths:

```c
#include <stddef.h>
#include <stdio.h>

int numbers[5] = {1, 2, 3, 4, 5};
size_t arraySize = sizeof numbers / sizeof numbers[0];

for (size_t i = 0; i < arraySize; ++i) {
    printf("%zu: %d\n", i, numbers[i]);
}
```

`size_t` is portable and helps avoid negative sizes and signed/unsigned
conversion problems. It does not prevent overflow or make an untrusted length
safe by itself. Validate external values before using them in size
calculations, and check multiplication before allocating or copying memory.

> **Note:** The `sizeof array / sizeof array[0]` pattern works only when the
> expression is an actual array. When an array is passed to a function, it is
> typically converted to a pointer, so the function must receive the length
> separately.

**Rule:** Use `size_t` for object sizes and indexes, but still validate lengths
and check size arithmetic before accessing or allocating memory.

---

# Part II — Floating-Point Safety

## 4. Floating-Point Overflow

`float` and `double` can also overflow, but unlike signed integer overflow, they typically produce infinity (`inf`).

```c
#include <float.h>
#include <math.h>

double x = DBL_MAX;
x = x * 2.0;

if (isinf(x)) {
    // Floating-point overflow
}
```

Limits are available in `<float.h>`:

```c
FLT_MAX
DBL_MAX
```

**Rule:** Check floating-point results for `NaN` and infinity when the value is security- or logic-sensitive.

---

## 5. Floating-Point `NaN`

Invalid floating-point operations can produce `NaN` ("Not a Number"):

```c
#include <math.h>

double x = 0.0 / 0.0;

if (isnan(x)) {
    // Invalid value
}
```

> **Note:** `NaN` compares unequal to everything, including itself — `x == x` is false when `x` is `NaN`. Always use `isnan()`/`isfinite()`.

**Rule:** Don't assume every `float` or `double` contains a normal numeric value.

---

## 6. Floating-Point Precision

Decimal values often cannot be represented exactly in binary floating point.

```c
double x = 0.1 + 0.2;

printf("%.17f\n", x);
// Often: 0.30000000000000004
```

Therefore, avoid exact equality for calculated values:

```c
if (x == 0.3) {  // Avoid when x was calculated
    // ...
}
```

Use a tolerance instead:

```c
#include <math.h>

#define EPSILON 1e-6

if (fabs(x - 0.3) < EPSILON) {
    // Close enough to 0.3
}
```

For more general comparisons, combine an absolute and a relative tolerance:

```c
#include <math.h>

int nearly_equal(double a, double b)
{
    double diff = fabs(a - b);

    if (diff < 1e-9)  // Absolute tolerance (near zero)
        return 1;

    return diff <= 1e-9 * fmax(fabs(a), fabs(b));  // Relative tolerance
}
```

Then:

```c
if (nearly_equal(x, 0.3)) {
    // Values are sufficiently close
}
```

**Rule:** Use an appropriate tolerance when comparing calculated floating-point values.

---

## 7. Don't Use Floating Point for Money

For exact monetary values, prefer integer units such as cents:

```c
long long price_cents = 30;

if (price_cents == 30) {
    // Exactly $0.30
}
```

Instead of:

```c
double price = 0.30;
```

This avoids floating-point representation and comparison problems. (Prefer a wide type such as `long long` — total amounts in cents can easily exceed `INT_MAX`, and integer overflow rules from §1 still apply.)

**Rule:** For exact financial calculations, use integer minor units or an appropriate decimal type instead of `float`/`double`.

---

## 8. Integer vs Floating-Point Safety Summary

| Type | Overflow behavior | Main concerns |
|---|---|---|
| `int` | Signed overflow is **undefined behavior** | UB, security vulnerabilities |
| `unsigned int` | Wraps modulo | Unexpected values, size bugs |
| `float` | Typically becomes `±inf` | Infinity, NaN, precision |
| `double` | Typically becomes `±inf` | Infinity, NaN, precision |

### Quick rule

- **Integers:** prevent overflow before the operation.
- **Unsigned integers:** don't rely on wrapping for security-sensitive calculations.
- **Floating point:** check for `NaN`/`inf` and handle precision carefully.
- **Money:** prefer integer cents/minor units or a suitable decimal representation.

---

# Part III — Input Handling

## 9. When Should Values Be Validated?

You don't need to validate every variable. Validate values when they come from an **untrusted source** or when an invalid value could cause a bad result, memory problem, or security issue.

Common trust boundaries:

| Source | Validate? | Why |
|---|---|---|
| User input | **Yes** | Completely untrusted |
| Network/API data | **Yes** | External/untrusted |
| File contents | **Yes** | Can be modified or corrupted |
| Environment variables / CLI args | **Yes** | Attacker-influenced |
| Database values | Usually | Data may be invalid or outdated |
| Internal calculations | Depends | Overflow or invalid results may be possible |
| Constants defined by you | Usually no | You control them |

Think of validation as a sequence:

```text
External input
      ↓
Can I parse it?
      ↓
Is the value allowed?
      ↓
Can I safely use it in this operation?
      ↓
Perform the operation
```

For example:

```c
int quantity;

if (scanf("%d", &quantity) != 1) {
    return 1; // Invalid input
}

if (quantity < 0 || quantity > 1000) {
    return 1; // Invalid range
}

if (price > 0 && quantity > INT_MAX / price) {
    return 1; // Multiplication would overflow
}

int total = quantity * price;
```

For `float`/`double`, validate when the value is external or security-/logic-sensitive:

```c
#include <math.h>

if (!isfinite(value)) {
    return 1; // Reject NaN or infinity
}
```

**Core principle:** Don't validate every variable mechanically. Validate values at **trust boundaries** and before operations where an invalid value could cause harm.

---

## 10. Format String Vulnerability

### Dangerous

```c
char input[100];

fgets(input, sizeof input, stdin);

printf(input); // BAD
```

User input becomes the format string. An attacker can leak stack memory with `%x`/`%p` or write memory with `%n`.

### Safe

```c
printf("%s", input);
```

### Preferred output forms

Even when displaying string literals, prefer forms that never treat data as a format string:

```c
puts("Welcome to C!"); // Outputs a '\n' automatically

printf("%s", "Enter first integer: "); // Cursor stays on the same line
```

Making these forms a habit ensures a character array that might contain user input is never accidentally used as a format-control string.

For more information, see CERT guideline FIO30-C at https://wiki.sei.cmu.edu/.

**Rule:** Never use untrusted input as a `printf` format string. The same applies to `fprintf`, `sprintf`, `syslog`, and any other `printf`-family function.

---

## 11. Input Sanitization

Don't trust external input.

```c
char username[32];

if (fgets(username, sizeof username, stdin) == NULL) {
    return 1;
}

username[strcspn(username, "\n")] = '\0'; // Strip newline
```

Then validate against an **allowlist** of what the application actually accepts:

```c
#include <ctype.h>

for (size_t i = 0; username[i] != '\0'; ++i) {
    if (!isalnum((unsigned char)username[i])) {
        return 1;
    }
}
```

> **Note:** Always cast the argument of `<ctype.h>` functions to `unsigned char` — passing a negative `char` value is undefined behavior.

**Rule:** Validate input against the application's expected format; prefer allowlists over denylists.

---

## 12. `scanf_s` Portability

The C11 standard's optional Annex K provides more secure versions of many string-processing and input/output functions. When reading a string into a character array, `scanf_s` checks that it does not write beyond the end of the array. It requires **two** arguments for each `%s` in the format string:

- a character array in which to place the input string, and
- the array's number of elements.

```c
char myString[20];

if (scanf_s("%19s", myString, 20) != 1) {
    return 1; // Conversion failed; myString is unaltered
}
```

If the number of characters input plus the terminating null character is larger than the specified number of elements, the `%s` conversion fails. For a format string with only one conversion specification, `scanf_s` returns `0` (no conversions performed) and the array is unaltered. This protects against field widths that are too long for the array — or omitted entirely.

However, `scanf_s` is not universally available because C11 Annex K is optional (and rarely implemented outside MSVC). Your compiler might also require a specific setting to enable the Annex K functions.

For portable C, prefer well-controlled input such as:

```c
char input[32];

if (fgets(input, sizeof input, stdin) == NULL) {
    return 1;
}
```

Then parse and validate the input separately (e.g., with `strtol`, which offers better error detection than `atoi` or `sscanf`).

**Rule:** Don't assume `_s` functions are portable across compilers.

---

## 13. Checking `scanf` Return Value

`scanf` returns the number of items successfully read. Capturing and inspecting that value is the correct way to detect a failed read.

```c
int parentsPermissionInput;
int readSuccessfully = scanf("%d", &parentsPermissionInput);
printf("Read successfully: %d\n", readSuccessfully);
```

`readSuccessfully` will be `1` if the integer was read correctly, `0` if the input didn't match the format, and `EOF` (typically `-1`) if the stream ended or an error occurred.

**Rule:** Always store and check the return value of `scanf` — do not assume the read succeeded.

---

# Part IV — Memory Safety

## 14. Buffer Overflow

### Dangerous

```c
char name[10];

scanf("%s", name);
```

A long input can exceed the buffer.

### Width-limited string input

```c
char string2[20];

if (scanf("%19s", string2) != 1) {
    return 1;
}
```

The array receiving the string must be large enough for the user's input and
the terminating null character. `scanf` does not know the size of the array;
with `%s`, it reads characters until a space, tab, newline, or end-of-file is
encountered. Because `string2` has room for 20 characters, the input string
must be limited to 19 characters so that the terminating `\0` also fits.

If the user enters 20 or more characters and `%s` is used without a field
width, `scanf` may write beyond the end of the array, causing a buffer
overflow, a crash, or a security vulnerability. The `%19s` conversion
specification limits the input to 19 characters and prevents this write past
the end of `string2`.

An overflow like this can overwrite other variables' values in memory — and
if the program later writes to those variables, it can overwrite the string's
terminating `'\0'`. Functions determine where a string ends by looking for
that `'\0'`: `printf`, for example, reads characters from the start of the
string until it encounters `'\0'`. If the terminator is missing, `printf`
keeps reading (and printing) memory until it finds some later `'\0'`, which
can produce strange results or crash the program.

`%s` reads only one whitespace-delimited word. For example, if the user types
`John Doe`, `scanf("%19s", string2)` stores only `John` and leaves `Doe` in
the input stream. If the input is allowed to contain spaces, use `fgets`
instead:

```c
char string2[20];

if (fgets(string2, sizeof string2, stdin) == NULL) {
    return 1;
}

string2[strcspn(string2, "\n")] = '\0';
```

When reviewing code, choose `fgets` for names, sentences, addresses, and other
text that may contain spaces. Use a field width with `scanf` only when reading
a single whitespace-delimited word is the intended behavior.

### Better

```c
char name[10];

if (scanf("%9s", name) != 1) {
    return 1;
}
```

Or, preferably, use `fgets`:

```c
char name[10];

if (fgets(name, sizeof name, stdin) == NULL) {
    return 1;
}
```

> **Note:** `fgets` keeps the trailing newline if it fits. Strip it when needed:
>
> ```c
> name[strcspn(name, "\n")] = '\0';
> ```

**Rule:** Always limit input to the destination buffer size.

---

## 15. `memcpy` / Buffer Size

### Dangerous

```c
char buffer[100];

memcpy(buffer, source, size);
```

If `size > 100`, memory corruption can occur.

### Safe

```c
if (size > sizeof buffer) {
    // Reject input
    return 1;
}

memcpy(buffer, source, size);
```

**Rule:** Verify the destination capacity before copying.

---

## 16. Dynamic Memory Allocation

### Dangerous

```c
int count;

scanf("%d", &count);

Item *items = malloc(count * sizeof(Item));
```

The multiplication can overflow.

### Safer

```c
if (count < 0 || (size_t)count > SIZE_MAX / sizeof(Item)) {
    return 1;
}

Item *items = malloc((size_t)count * sizeof(Item));

if (items == NULL) {
    return 1;
}
```

> **Tip:** `calloc(count, sizeof(Item))` performs this overflow check internally and returns `NULL` on overflow, which makes it a good alternative for array allocations.

**Rule:** Check size calculations and always check `malloc` results.

---

## 17. Array Bounds and Null Pointers

C provides **no automatic bounds checking** for arrays. Every subscript must be greater than or equal to `0` and less than the array's number of elements. For a two-dimensional array, the row and column subscripts must each be in range (`0` to rows − 1 and `0` to columns − 1, respectively) — and the same applies to arrays with additional dimensions.

Out-of-bounds accesses are common security flaws:

- **Reading** outside the bounds can crash the program — or let it appear to run correctly while using bad data.
- **Writing** outside the bounds (a buffer overflow) can corrupt data in memory, crash the program, and even allow attackers to execute their own code.

For additional prevention techniques, see CERT guideline ARR30-C at https://wiki.sei.cmu.edu/.

### Array bounds — dangerous

```c
int values[10];

values[index] = 42;
```

If `index >= 10` (or is negative), memory outside the array is accessed.

### Safe

```c
if (index < 0 || index >= 10) {
    return 1;
}

values[index] = 42;
```

For an array, prefer computing the length instead of hard-coding it:

```c
size_t length = sizeof values / sizeof values[0];

if (index >= length) {
    return 1;
}
```

### Protecting array parameters with `const`

When an array is passed to a function, the function receives a pointer to the
original elements — not a copy — so it can modify the caller's data. If the
function only needs to read the array, qualify the parameter with `const` so
the compiler rejects any accidental or malicious write:

```c
void print_all(const int values[], size_t length)
{
    for (size_t i = 0; i < length; i++) {
        printf("%d\n", values[i]);
    }

    values[0] = 0; // compilation error: read-only parameter
}
```

This applies the principle of least privilege: the function gets only the
access it needs, and unintended modifications become compile-time errors
instead of runtime bugs.

### Null pointers — dangerous

```c
int *ptr = malloc(sizeof *ptr);

*ptr = 10; // malloc can fail
```

### Safe

```c
int *ptr = malloc(sizeof *ptr);

if (ptr == NULL) {
    return 1;
}

*ptr = 10;
```

**Rule:** Never access an array without checking its bounds, and check pointers returned by allocation functions before dereferencing them.

---

## 18. Use-After-Free and Double Free

### Use-after-free — dangerous

```c
free(ptr);

*ptr = 10; // BAD
```

### Double free — dangerous

```c
free(ptr);
free(ptr); // BAD
```

### Safer

```c
free(ptr);
ptr = NULL;

free(ptr); // Safe: free(NULL) does nothing
```

Setting the pointer to `NULL` neutralizes both problems: dereferencing becomes a deterministic crash instead of silent corruption, and a second `free` becomes a no-op.

> **Caution:** This only protects the pointer you nulled — other copies (aliases) of the same pointer remain dangling.

**Rule:** After `free`, treat the pointer as invalid; set owned pointers to `NULL` after freeing them.

---

# Part V — Random Numbers

## 19. Don't Use `rand` for Security

### Dangerous

```c
#include <stdlib.h>

int token = rand(); // Predictable
```

`rand` is fine for textbook examples, but not for industrial-strength applications. The C standard makes no guarantees about the quality of the sequence, and some implementations produce sequences with "distressingly non-random low-order bits." CERT guideline **MSC30-C** requires implementation-specific random-number generators so that values are not predictable — critical in cryptography and other security applications.

### Safe — use your platform's secure generator

- **Windows:** `BCryptGenRandom` (Cryptography API: Next Generation) — https://docs.microsoft.com/en-us/windows/win32/seccng/cng-portal
- **POSIX/Linux:** `random` — see `man random`
- **macOS:** `arc4random` in `<stdlib.h>` — see `man arc4random`

For more information, see guideline MSC30-C at https://wiki.sei.cmu.edu/.

**Rule:** Never use `rand` when unpredictability matters; use the secure random-number generator recommended for your platform.

---

# Secure C Checklist

Before using external input:

- [ ] Check that parsing succeeded.
- [ ] Validate the input range.
- [ ] Check arithmetic before it can overflow.
- [ ] Check array bounds.
- [ ] Declare read-only array parameters as `const`.
- [ ] Check buffer sizes.
- [ ] Check `malloc`/`calloc`/`realloc` results.
- [ ] Never use user input as a `printf` format string.
- [ ] Don't use memory after `free`.
- [ ] Avoid double-free; set pointers to `NULL` after freeing.
- [ ] Check floating-point values for `NaN`/`inf` when they matter.
- [ ] Validate data before using it in security-sensitive operations.
- [ ] Use a platform-specific secure random generator instead of `rand` when predictability matters.

## Core Principle

> **Never trust input, never assume arithmetic is safe, and never access memory without proving that the access is valid.**
