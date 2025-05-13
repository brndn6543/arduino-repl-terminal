# Arduino Terminal Template
This is a template for a REPL-style terminal on an Arduino board. The idea here
is that this can be used as the starting point for building out terminals for
issuing commands on an Arduino.

The commands do not do much as this is meant as a staring point for building
out more robust terminals.

- **Available commands:**
  - help - show this menu 
  - info - show board info 
  - led - led on/off *(mock-up)*
  - echo - repeat back your text

## Code

```c++
static constexpr uint8_t BUF_SIZE = 64;
```
`unint8_t` is used here to ensure that we only use 1 byte of RAM. On an AVR
8-bit variables cost 1 byte, normal `int` is 2 bytes. `byte` or `char` could be used
here as well.

```c++
struct {
    const char* name;
    void (*handler)(char* args);
    const char* help;
} commands[] = {
    {"help", cmd_help, "show this menu"},
    {"info", cmd_info, "show board info"},
    {"led", cmd_led, "led on/off"},
    {"echo", cmd_echo, "repeat back your text"},
    {nullptr, nullptr, nullptr}
};
```
Each handler is a function pointer to one of the `cmd_*` functions. At runtime
the user's input is compared against `name`; if it matches it's handler is called 
to run that command.

A C-style initializer list is used to initialize one struct instance, and then
a final null-sentinal is defined.

```c++
Serial.println(F("Type 'help' for a list of commands"));
```
On AVR Arduinos, string literals normally sit in RAM at runtime. `F()` wraps
the literal so it lives in program memory, this saves SRAM. This macro fetches
each character flash as it prints.

```c++
extern int __heap_start, *__brkval;
```
These symbols are provided byt the AVR libc runtime and they state where
dynamic memory begins `(__head_start)` and where the heap currently ends
`(__brkval)`. They are declared `extern` so their addresses are readable.