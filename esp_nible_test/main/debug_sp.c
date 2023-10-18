

#include "debug_sp.h"

// Custom debug function
void debug_print_statement(const char *module, int level, const char *format,
                           ...) {
  if (level <= DEBUG_LEVEL) {
    printf("Debug Level %d (%s): ", level, module);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
  }
}

void debug_print_word(int level, const char *format, ...) {
  if (level <= DEBUG_LEVEL) {

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
}

//int main() {
//  const char *IP = "192.168.1.100";
//  int Time_stamp = 123456;
//
//  debug_print_statement("WIFI", 1,
//                        "Wifi is connected, IP: %s Time: %d date: %d", IP,
//                        Time_stamp, 10);
//  debug_print_statement("WIFI", 2, "Another debug message");
//
//  debug_print_word(2, "Time: %d. %s", 1, "Fuck");
//  debug_print_word(2, "Time: %d", 2);
//  debug_print_word(2, "Time: %d\n", 3);
//  debug_print_word(2, "Time: %d", 4);
//
//  return 0;
//}
