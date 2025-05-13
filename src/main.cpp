#include <Arduino.h>

static constexpr uint8_t BUF_SIZE = 64;
char inputBuf[BUF_SIZE];
uint8_t bufPos = 0;

// Command handlers.
void cmd_help(char* args);
void cmd_info(char *args);
void cmd_led(char *args);
void cmd_echo(char *args);

// Anonymous struct, command lookup table.
struct {
    const char* name;
    void (*handler)(char* args);
    const char* help;
} commands[] = {
    // Initialize struct instances.
    {"help", cmd_help, "show this menu"},
    {"info", cmd_info, "show board info"},
    {"led", cmd_led, "led on/off"},
    {"echo", cmd_echo, "repeat back your text"},
    {nullptr, nullptr, nullptr}
};

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println(F("Type 'help' for a list of commands"));
    Serial.print(F("> "));
}

void loop() {
    // Read incoming bytes.
    while (Serial.available()) {
        char c = Serial.read();

        //  Backspace support.
        if (c == '\b' && bufPos > 0) {
            bufPos--;
            Serial.print(F("\b \b"));
        } else if (c == '\n') {    // Process buffer on newline.
            Serial.println();

            if (bufPos > 0) {
                inputBuf[bufPos] = '\0';        // Null terminate.

                // Split into commands + args.
                char* cmd = strtok(inputBuf, " ");
                char* args = strtok(nullptr, "");

                // Look up command.
                bool found = false;
                for (int i = 0; commands[i].name; ++i) {
                    if (strcmp(cmd, commands[i].name) == 0) {
                        commands[i].handler(args);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    Serial.println("[-] Command not found: ");
                    Serial.println(cmd);
                    cmd_help(nullptr);
                }
            }
            // Rest buffer and prompt.
            bufPos = 0;
            Serial.print(F("> "));
        }
        // Store regular characters.
        else if (bufPos < BUF_SIZE - 1 && c >= ' ') {
            inputBuf[bufPos++] = c;
            Serial.print(c);
        }
    }
}

void cmd_help(char *args) {
    Serial.println("Available commands:");
    for (int i = 0; commands[i].name; ++i) {
        Serial.print(F(" "));
        Serial.print(commands[i].name);
        Serial.print(F(" - "));
        Serial.println(commands[i].help);
    }
}

void cmd_info(char *args) {
    // Rough SRAM free estimate.
    extern int __heap_start, *__brkval;
    int v;
    int freeRam = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
    Serial.println("Arduino UNO, ");
    Serial.print(freeRam);
    Serial.println(" B SRAM free");
}

void cmd_led(char *args) {
    if (!args) {
        Serial.println(F("Usage: led on or led off"));
        return;
    }

    if (strcasecmp(args, "on") == 0) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED ON");
    } else if (strcasecmp(args, "off") == 0) {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED OFF");
    } else {
        Serial.println(F("Usage: led on or led off"));
    }
}

void cmd_echo(char *args) {
    if(args) {
        Serial.println(args);
    }
}