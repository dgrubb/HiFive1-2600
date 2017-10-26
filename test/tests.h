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

void execute_tests(void);

void test_LDA(void);
void test_LDA_Immediate(void);
void test_LDA_Zero_Page(void);
void test_LDA_Zero_Page_X_Indexed(void);
void test_LDA_Absolute(void);
void test_LDA_Absolute_X_Indexed(void);
void test_LDA_Absolute_X_Indexed_Boundary_Cross(void);
void test_LDA_Absolute_Y_Indexed(void);
void test_LDA_Absolute_Y_Indexed_Boundary_Cross(void);
void test_LDA_Indirect_X_Indexed(void);
void test_LDA_Indirect_Y_Indexed(void);

void test_LDX(void);
void test_LDX_Immediate(void);
void test_LDX_Zero_Page(void);
void test_LDX_Zero_Page_Y_Indexed(void);
void test_LDX_Absolute(void);
void test_LDX_Absolute_Y_Indexed(void);
void test_LDX_Absolute_Y_Indexed_Boundary_Cross(void);

void test_LDY(void);
void test_LDY_Immediate(void);
void test_LDY_Zero_Page(void);
void test_LDY_Zero_Page_X_Indexed(void);
void test_LDY_Absolute(void);
void test_LDY_Absolute_X_Indexed(void);
void test_LDY_Absolute_X_Indexed_Boundary_Cross(void);

void test_STA(void);
void test_STA_Zero_Page(void);
void test_STA_Zero_Page_X_Indexed(void);
void test_STA_Absolute(void);
void test_STA_Absolute_X_Indexed(void);
void test_STA_Absolute_Y_Indexed(void);
void test_STA_Indirect_X_Indexed(void);
void test_STA_Indirect_Y_Indexed(void);

void test_STX(void);
void test_STX_Zero_Page(void);
void test_STX_Zero_Page_Y_Indexed(void);
void test_STX_Absolute(void);

void test_STY(void);
void test_STY_Zero_Page(void);
void test_STY_Zero_Page_X_Indexed(void);
void test_STY_Absolute(void);

void test_ADC(void);
void test_ADC_Immediate(void);

#endif /* EXEC_TESTS */

#endif /* _TEST_H */
