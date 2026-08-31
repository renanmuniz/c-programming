# Secure C Programming — Quick Guide

Brief rules and examples for avoiding common C security problems.

**Structure:**

1. [Integer Safety](#part-i--integer-safety) (§1–3)
2. [Floating-Point Safety](#part-ii--floating-point-safety) (§4–8)
3. [Input Handling](#part-iii--input-handling) (§9–13)
4. [Memory Safety](#part-iv--memory-safety) (§14–18)
5. [Checklist & Core Principle](#secure-c-checklist)

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

`scanf_s` is not universally available because C11 Annex K is optional (and rarely implemented outside MSVC).

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

# Secure C Checklist

Before using external input:

- [ ] Check that parsing succeeded.
- [ ] Validate the input range.
- [ ] Check arithmetic before it can overflow.
- [ ] Check array bounds.
- [ ] Check buffer sizes.
- [ ] Check `malloc`/`calloc`/`realloc` results.
- [ ] Never use user input as a `printf` format string.
- [ ] Don't use memory after `free`.
- [ ] Avoid double-free; set pointers to `NULL` after freeing.
- [ ] Check floating-point values for `NaN`/`inf` when they matter.
- [ ] Validate data before using it in security-sensitive operations.

## Core Principle

> **Never trust input, never assume arithmetic is safe, and never access memory without proving that the access is valid.**
