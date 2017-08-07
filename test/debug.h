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

void debug_print_status_flags();
int debug_get_status_flag(uint8_t flag);

#endif /* EXEC_TESTS */

#endif /* _DEBUG_H */
