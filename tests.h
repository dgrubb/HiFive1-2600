/*
 * File: tests.h
 * Author: dgrubb
 * Date: 07/17/2017
 *
 * Adds a sequence of tests to verify emulation is correct.
 */

#ifndef _TEST_H
#define _TEST_H

#ifdef EXEC_TESTS

int execute_tests();

int test_LDA();
int test_LDA_Immediate();
int test_LDA_Zero_Page();

int test_LDX();

int test_LDY();

#endif /* EXEC_TESTS */

#endif /* _TEST_H */
