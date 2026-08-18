# Format String Vulnerability (Single-Argument printf Attack)
What it is
When user input is passed directly as the format string to printf, an attacker can inject format specifiers to read or write arbitrary memory.

Vulnerable code:
`printf(input);`

Safe code:
`printf("%s", input);`

If an attacker types `%x %x %x %x`, printf will consume values off the stack and print them — leaking memory addresses or sensitive data.
With `%n`, which writes the number of characters printed so far to a pointer argument, an attacker can write to arbitrary memory addresses, enabling code execution.

Or use the `puts` command:
`puts("Enter your age:");`

