[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

ProjectX: Library for Injecting Binary Code into Applications
---

The library is developed by
[Vlad Kirichok](https://github.com/vlad1kirpichok),
[Ivan Kovalyonok](https://github.com/KIvy17),
[Artem Smirnov](https://github.com/artemdsmirnov)
as their group project at
[Faculty of Computer Science of Higher School of Economics](https://cs.hse.ru/en/).
The supervisor is [Andrei Tatarnikov](https://github.com/andrewt0301).

## Supported Platforms

* Windows (x64)
* Linux (x64 and RISC-V RV64I)
* MacOS (x64 and AArch64)

## How does the library work?

The library patches executable file to inject code that will be executed before the main function.

For example, consider a "Hello World" program like the one below.
It contains the `func` function defiction and its call inserted in the `main` function. 
The injection library provides an API to construct functions like `func`
and to inject them in the specified executable.

```C
#include <stdio.h>

/**
 * Type for a pointer to a functon that prints messages.
 */
typedef int (* fptr_t)(const char *);

/**
 * Example of an injected function:
 * prints the specified message woth the specified function
 * the specified number of times.
 *
 * @param print the print function
 * @param message the messge to be printed
 * @param count the count of times to print the message
 */
void func(const fptr_t print, const char* message, const int count) {
  for (int i = 0; i < count; ++i) {
    print(message);
  }
}

/**
 * Example: entry point of a program to be patched.
 *
 * @return exit code
 */
int main() {
  func(puts, "Injected message!", 3);
  puts("Hello, World!");
  return 0;
}
```
