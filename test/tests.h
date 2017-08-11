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
void test_LDA_Absolute_Y_Indexed();
void test_LDA_Absolute_Y_Indexed_Boundary_Cross();
void test_LDA_Indirect_X_Indexed();
void test_LDA_Indirect_Y_Indexed();

void test_LDX();
void test_LDX_Immediate();
void test_LDX_Zero_Page();
void test_LDX_Zero_Page_Y_Indexed();
void test_LDX_Absolute();
void test_LDX_Absolute_Y_Indexed();
void test_LDX_Absolute_Y_Indexed_Boundary_Cross();

void test_LDY();
void test_LDY_Immediate();
void test_LDY_Zero_Page();
void test_LDY_Zero_Page_X_Indexed();
void test_LDY_Absolute();
void test_LDY_Absolute_X_Indexed();
void test_LDY_Absolute_X_Indexed_Boundary_Cross();

void test_STA();
void test_STA_Zero_Page();
void test_STA_Zero_Page_X_Indexed();
void test_STA_Absolute();
void test_STA_Absolute_X_Indexed();
void test_STA_Absolute_Y_Indexed();
void test_STA_Indirect_X_Indexed();
void test_STA_Indirect_Y_Indexed();

void test_STX();
void test_STX_Zero_Page();
void test_STX_Zero_Page_Y_Indexed();
void test_STX_Absolute();

void test_STY();
void test_STY_Zero_Page();
void test_STY_Zero_Page_X_Indexed();
void test_STY_Absolute();

#endif /* EXEC_TESTS */

#endif /* _TEST_H */
