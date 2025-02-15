/**
 * @file common_test.h
 * @author jake-is-ESD-protected
 * @date 2024-03-07
 * @brief Importer for all tests
 *
 * 
 */

#ifndef _COMMON_TEST_H_
#define _COMMON_TEST_H_

void test_core_init(void);
void test_job_core(void);
void test_job_error_handler(void);
void test_job_init_cli(void);
void test_job_reprint_header(void);
void test_job_help(void);
void test_job_stats(void);

void test_register_job(void);
void test_launch_job(void);
void test_set_get_args(void);
void test_set_get_params(void);
void test_core_job_launch_prohibited(void);
void test_notify_job_and_wait(void);

#endif // _COMMON_TEST_H_