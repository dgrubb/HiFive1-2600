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

void execute_tests();
void test_LDA();
int test_LDA_Immediate();

#endif /* EXEC_TESTS */

#endif /* _TEST_H */
