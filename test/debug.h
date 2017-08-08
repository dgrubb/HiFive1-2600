/*
 * File: debug.h
 * Author: dgrubb
 * Date: 08/07/2017
 *
 * Provides helper functions for printing human-readable 
 * information.
 */

#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef EXEC_TESTS

#include "../cpu/mos6507.h"

void debug_print_status_flags();
void debug_print_memory_contents(uint16_t address);
void debug_print_special_register(mos6507_register_t reg);
void debug_print_buses();
int debug_get_status_flag(uint8_t flag);

#endif /* EXEC_TESTS */

#endif /* _DEBUG_H */
