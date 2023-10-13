/*
 * debug.h
 *
 *  Created on: Oct 13, 2023
 *      Author: MN
 */

#ifndef MAIN_DEBUG_SP_H_
#define MAIN_DEBUG_SP_H_


#include <stdarg.h>
#include <stdio.h>

// Define debug levels
#define DEBUG_LEVEL 2

void debug_print_word(int level, const char *format, ...);
void debug_print_statement(const char *module, int level, const char *format, ...);
#endif /* MAIN_DEBUG_SP_H_ */
