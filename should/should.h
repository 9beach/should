#ifndef SHOULD_H_
#define SHOULD_H_

/*!
 * \file should.h
 *
 * \brief Minimal xUnit library for C programmers
 *
 * \author Hyunrae Cho (9beach@gmail.com)
 * \date 2007/08/08
 */

/* FIXME: Probably configure script is better place for the below logic. */
#ifdef _WIN32
#	define __func__ __FUNCTION__
#endif

/*!
 * \brief Checks if given expression is true.
 *
 * Puts error message if fail.
 */
#define should_be(expr)				should_be_(expr, #expr \
		, __FILE__, __LINE__, __func__)

/*!
 * \brief Checks if two strings contains same letters.
 *
 * Puts error message if fail.
 */
#define should_be_equal_strings(val1, val2)	should_be_equal_strings_(val1, \
 		val2, #val1 " == " #val2, __FILE__, __LINE__, __func__)

/*!
 * \brief Checks if two strings contains different letters.
 *
 * Puts error message if fail.
 */
#define should_be_not_equal_strings(val1, val2)	should_be_not_equal_strings_(\
		val1, val2, #val1 " != " #val2, __FILE__, __LINE__, __func__)

/*!
 * \brief Checks if two numbers are the same.
 *
 * Puts error message if fail.
 */
#define should_be_equal_numbers(val1, val2)	should_be_equal_numbers_(val1, \
 		val2, #val1 " == " #val2, __FILE__, __LINE__, __func__)

/*!
 * \brief Checks if two numbers are not the same.
 *
 * Puts error message if fail.
 */
#define should_be_not_equal_numbers(val1, val2)	should_be_not_equal_numbers_(\
		val1, val2, #val1 " != " #val2, __FILE__, __LINE__, __func__)


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Test suite structure
 *
 * Test suite can hold multiple test suites and/or test cases.  test is 
 * performed by running a test suite.
 */
typedef struct should_suite_t should_suite_t;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
void should_be_(int expr, const char *expr_str, const char *file, int line, 
		const char *func);

void should_be_equal_strings_(const char *val1, const char *val2, 
		const char *expr_str, const char *file, int line, 
		const char *func);

void should_be_not_equal_strings_(const char *val1, const char *val2, 
		const char *expr_str, const char *file, int line, 
		const char *func);

void should_be_equal_numbers_(int val1, int val2, const char *expr_str, 
		const char *file, int line, const char *func);

void should_be_not_equal_numbers_(int val1, int val2, const char *expr_str, 
		const char *file, int line, const char *func);
/** \endcond */

/*!
 * \brief Creates test suite
 *
 * In SHOULD, all pointer-type parametes are assigned, not duplicated.
 * \param name just for displaying
 * \return 0 if malloc fails
 */
should_suite_t *
should_create_suite(
		const char *name
		);

/*!
 * \brief Destroys test suite
 */
void
should_destroy_suite(
		should_suite_t *suite
		);

/*!
 * \brief Destroy a suite and its all children recursively
 */
void
should_destroy_suite_recursively(
		should_suite_t *suite
		);

/*!
 * \brief Adds the test suite to the parent suite
 * \return 0 if malloc succeeds
 */
int
should_add_suite(
		should_suite_t *parent,
		should_suite_t *suite
		);

/*!
 * \brief Sets the setup/teardown functions to the test suite
 *
 * if a suite has many suites and/or cases, the setup/teardown functions 
 * are called for each suite and/or case.
 */
void
should_set_fixture(
		should_suite_t *suite,
		void *(*setup)(),
		void (*teardown)(void *)
		);

/*!
 * \brief Adds the test case to the test suite
 * \return 0 if malloc succeeds
 */
int
should_add_case(
		should_suite_t *suite,
		void (*case_func)(void *)
		);

/*!
 * \brief Runs all the test cases and suites of the suite
 * \return 0 if all the test cases and suites of the suite succeed
 */
int
should_run_suite(
		should_suite_t *suite
		);

/*!
 * \brief Runs all the test cases and suites of the suite and destroys them.
 *
 * Internally, calls run_should_suite and destroy_should_suite_recursively.
 * \return 0 if all the test cases and suites of the suite succeed
 */
int
should_run_and_destroy_suite(
		should_suite_t *suite
		);

#ifdef __cplusplus
}
#endif

#endif /* SHOULD_H_ */

/*!
 * \mainpage SHOULD
 * \section intro_sec Introduction
 * 
 * SHOULD is a minimal xUnit for C programmers
 *
 * \section glossary Glossary
 * \subsection test_suite_sec Test suite
 *
 * Test suite can hold multiple test suites and/or test cases.
 * test is performed by running a test suite.
 *
 * \subsection test_case_sec Test case
 *
 * Test case is void (*)(void *) type function including at least one should_be 
 * macro.
 *
 * \subsection should_be_sec should_be
 *
 * should_be macro verifies a expression. if a expression is not true,
 * it puts error message (does not abort).
 *
 * \subsection fixture_sec setup, teardown and fixture
 *
 * Fixture is a environment which is shared among test cases and/or suites 
 * of a test suite.
 *
 * You can initialize/finalize the fixture with the setup/teardown function.
 * if a suite has many suites and/or cases, the setup/teardown functions 
 * are called for each suite and/or case.
 */
