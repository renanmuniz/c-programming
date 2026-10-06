# C Performance Programming - Quick Guide

Brief tips and examples for improving C program performance.

**Structure:**

1. [Static Local Arrays](#part-i--static-local-arrays)
2. [Passing Arrays Efficiently](#part-ii--passing-arrays-efficiently)
3. [Searching: Linear vs. Binary](#part-iii--searching-linear-vs-binary)

---

# Part I - Static Local Arrays

## 1. Avoid Recreating Large Local Arrays

A static local variable exists for the program's duration but is visible only
inside the function where it is declared. Applying `static` to a local array
prevents the array from being created and initialized on every function call
and destroyed when the function exits.

This can reduce execution time, especially in frequently called functions that
contain large arrays. A static array is initialized once before the program
starts. If it is not explicitly initialized, all of its elements are
initialized to zero by default.

```c
#include <stdio.h>

void record_call(void)
{
    static int call_history[1000];

    call_history[0]++;
    printf("Function called %d time(s)\n", call_history[0]);
}

int main(void)
{
    record_call();
    record_call();
    record_call();

    return 0;
}
```

`call_history` is allocated once and retains its values between calls. Without
`static`, the array would have automatic storage duration: it would be created
when `record_call` starts and its contents would no longer exist after the
function returns.

### Static default text

String literals already have static storage duration, but a local character
array initialized from a string is recreated on each call unless it is marked
`static`:

```c
const char *default_message(void)
{
    static const char message[] = "Ready";

    return message;
}
```

The `message` array is created and initialized once, then remains available for
the program's duration.

**Rule:** Consider a static local array when a frequently called function
needs a large array whose values should persist between calls. Confirm that
persistent state is intended, because static local data is shared by every
call and is not automatically reentrant or thread-safe.

---

# Part II - Passing Arrays Efficiently

## 1. Pass Large Arrays Without Copying Them

Passing arrays by reference makes sense for performance reasons. If arrays
were passed by value, a copy of each element would be passed. For large,
frequently passed arrays, this would be time-consuming and would consume
storage for the copies of the arrays.

In C, an array argument is converted to a pointer to its first element, so the
function accesses the original array and no elements are copied. Pass the
array's length separately when the function needs it:

```c
double average(const int values[], size_t length)
{
    long sum = 0;

    for (size_t i = 0; i < length; i++) {
        sum += values[i];
    }

    return (double)sum / length;
}
```

Only a pointer and a length are passed, regardless of the array's size.

### Preventing modifications with `const`

A way to pass an array without allowing modifications is to qualify the
function parameter with `const`. The function still receives a pointer to the
original array — so no copy is made — but the compiler rejects any attempt to
write through it:

```c
void print_all(const int values[], size_t length)
{
    for (size_t i = 0; i < length; i++) {
        printf("%d\n", values[i]);
    }

    values[0] = 0;   /* compilation error: read-only parameter */
}
```

This combines the speed of passing by reference with the safety of passing by
value.

## 2. When Passing by Value Makes Sense

Passing by value makes sense when the function should not be able to modify
the caller's data. The callee receives a copy, so any change it makes is local
and the original is untouched. This is the right choice for individual array
elements and other scalars, and for small structs, where the cost of copying
is negligible compared with the safety it provides.

Individual array elements are ordinary values, so they are passed by value
automatically:

```c
void print_element(int value)   /* copy of one element */
{
    printf("%d\n", value);
}

print_element(scores[3]);
```

An entire array can be passed by value by wrapping it in a struct, because
structs are copied when passed:

```c
struct point3 {
    double coords[3];
};

double norm_squared(struct point3 p)   /* whole array copied */
{
    return p.coords[0] * p.coords[0]
         + p.coords[1] * p.coords[1]
         + p.coords[2] * p.coords[2];
}
```

Reserve this technique for small, fixed-size arrays. For large arrays, prefer
passing a pointer and, if the function must not modify the data, declare the
parameter `const` to get the safety of pass-by-value without the copy:

```c
double average(const int values[], size_t length);
```

**Rule:** Pass scalars and small structs by value when the callee should work
on a copy. Pass large arrays as (`const`) pointers plus a length.

---

# Part III - Searching: Linear vs. Binary

## 1. Prefer Binary Search for Large Sorted Arrays

A linear search compares the key against every element until it finds a match,
so in the worst case it examines all `n` elements — O(n). A binary search
repeatedly halves the portion of a **sorted** array that can contain the key,
so it needs at most about log2(n) comparisons — O(log n). For an array of
10,000,000 elements, that is at most 10,000,000 comparisons versus about 24.

```c
/* requires a sorted array */
long binarySearch(const int array[], long key, size_t size)
{
    long low = 0;
    long high = (long)size - 1;

    while (low <= high) {
        long middle = low + (high - low) / 2;   /* avoids overflow */

        if (array[middle] == key) {
            return middle;
        } else if (array[middle] > key) {
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }

    return -1;
}
```

Each iteration discards half of the remaining elements, which is why the
number of comparisons grows with the logarithm of the array size rather than
the size itself.

### Measured difference

Searching the same key in a 10,000,000-element array — linear search on
unsorted data (`fig06_14`) versus binary search on sorted data (`fig06_15`):

```text
➜  build git:(main) ✗ ./fig06_14
Enter integer search key: 189756
Found value at subscript 9818282
Search took 21.354000 ms

➜  build git:(main) ✗ ./fig06_15
Enter integer search key: 189756
Found value at subscript 94878
Search took 0.046000 ms
```

The binary search was roughly 460× faster for this key.

### When linear search still makes sense

Binary search requires sorted data. Sorting costs O(n log n), so it only pays
off when the array is searched repeatedly or is already sorted. For a single
search of unsorted data, a linear search is cheaper than sorting first. Linear
search is also fine for small arrays, where the difference is negligible, and
it is the only option for data that can only be traversed sequentially, such
as a linked list.

```c
long linearSearch(const int array[], long key, size_t size)
{
    for (size_t n = 0; n < size; n++) {
        if (array[n] == key) {
            return n;
        }
    }

    return -1;
}
```

**Rule:** Use binary search for large arrays that are sorted or searched many
times — sort once, then search in O(log n). Use linear search for small
arrays, one-off searches of unsorted data, or sequential-access structures.
