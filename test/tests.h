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
void test_LDA_Immediate();
void test_LDA_Zero_Page();
void test_LDA_Zero_Page_X_Indexed();
void test_LDA_Absolute();
void test_LDA_Absolute_X_Indexed();
void test_LDA_Absolute_X_Indexed_Boundary_Cross();

void test_LDX();
void test_LDX_Immediate();

void test_LDY();
void test_LDY_Immediate();

#endif /* EXEC_TESTS */

#endif /* _TEST_H */