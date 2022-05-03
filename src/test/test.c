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
