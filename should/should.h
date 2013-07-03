#ifndef SHOULD_H_
#define SHOULD_H_

/*!
 * \file should.h
 *
 * \brief Minimal xUnit library for C programmers
 * \author Hyunrae Cho (9beach@gmail.com)
 * \date 2007/08/08
 */

/*!
 * \mainpage SHOULD
 * \section intro_sec Introduction
 * 
 * SHOULD is a minimal xUnit for C programmers.
 *
 * I think today's most xUnit libraries are too much complex and there are some
 * good reasons to "reinvent the wheel". SHOULD simply and justly supports the 
 * key concepts of Kent Beck's original testing framework.
 *
 * \section glossary Glossary
 * \subsection test_suite_sec Test Suite
 *
 * A test suite can hold multiple test cases. Test is performed by running a 
 * test suite.
 *
 * \subsection test_case_sec Test Case
 *
 * Test case is void (*)(void *) type function including at least one should_be 
 * macro.
 *
 * \subsection should_be_sec should_be macros
 *
 * should_be macros verify a expression. If the expression is not true,
 * it puts error message (does not abort).
 *
 * \subsection fixture_sec Setup, Teardown, and Fixture
 *
 * Fixture is a environment which is shared among all the test cases of a test
 * suites.

 * You can initialize and finalize the fixture with setup and teardown
 * functions. If a test suite has many test cases, the setup and teardown
 * functions are called for each of all the test cases.
 */

#include <string.h>

/* FIXME: Probably configure script is better place for the logic below. */
#ifdef _MSC_VER
#	define __func__ 		__FUNCTION__
#endif

/*!
 * \brief Verifies that the expression is true.
 *
 * Prints error message if fails.
 */
#define should_be(expr)			should_be_((expr), #expr , \
			__FILE__, __LINE__, __func__)

/*!
 * \brief Verifies that the expression is true.
 *
 * Prints the given message if fails.
 */
#define should_be_msg(expr, msg)	should_be_msg_((expr), msg, \
			#expr , __FILE__, __LINE__, __func__)

/*!
 * \brief Verifies that two integers are the same.
 *
 * Prints error message if fails.
 */
#define should_be_eq(val1, val2)	do { \
			int x = (val1); int y = (val2); \
			should_cmp_(x, y, #val1, #val2, x == y, "==", \
			__FILE__, __LINE__, __func__); \
			} while (0)

/*!
 * \brief Verifies that two integers are not the same.
 *
 * Prints error message if fails.
 */
#define should_be_ne(val1, val2)	do { \
			int x = (val1); int y = (val2); \
			should_cmp_(x, y, #val1, #val2, x != y, "!=", \
			__FILE__, __LINE__, __func__); \
			} while (0)
/*!
 * \brief Verifies that the first integer is less than the second.
 *
 * Prints error message if fails.
 */
#define should_be_lt(val1, val2)	do { \
			int x = (val1); int y = (val2); \
			should_cmp_(x, y, #val1, #val2, x < y, "<", \
			__FILE__, __LINE__, __func__); \
			} while (0)

/*!
 * \brief Verifies that the first integer is less than or equal to the second.
 *
 * Prints error message if fails.
 */
#define should_be_le(val1, val2)	do { \
			int x = (val1); int y = (val2); \
			should_cmp_(x, y, #val1, #val2, x <= y, "<=", \
			__FILE__, __LINE__, __func__); \
			} while (0)

/*!
 * \brief Verifies that two strings contains the same letters.
 *
 * Prints error message if fails.
 */
#define should_be_eq_str(val1, val2)	do { \
			const char *x = (val1); const char *y = (val2); \
			should_cmp_str_(x, y, #val1, #val2, \
			!strcmp(x, y), "==", \
			__FILE__, __LINE__, __func__); \
			} while (0)

/*!
 * \brief Verifies that two strings contains the different letters.
 *
 * Prints error message if fail.
 */
#define should_be_ne_str(val1, val2)	do { \
			const char *x = (val1); const char *y = (val2); \
			should_cmp_str_(x, y, #val1, #val2, \
			strcmp(x, y), "!=", \
			__FILE__, __LINE__, __func__); \
			} while (0)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Test suite structure
 *
 * A test suite can hold multiple test cases. Test is performed by running a 
 * test suite.
 */
typedef struct should_suite_t should_suite_t;

/*!
 * \brief Creates the test suite.
 *
 * In SHOULD, all pointer-type parametes are assigned, not duplicated.
 * \param name just for displaying
 * \return 0 if malloc fails
 */
should_suite_t *should_create_suite(const char *name);

/*!
 * \brief Destroys the test suite.
 */
void should_destroy_suite(should_suite_t *suite);

/*!
 * \brief Sets the setup/teardown functions to the test suite.
 *
 * If the suite has many test cases, the setup/teardown functions 
 * are called for each of them.
 */
void should_set_fixture(should_suite_t *suite, void *(*setup)(),
		void (*teardown)(void *));

/*!
 * \brief Adds the test case to the test suite.
 * \return 0 if malloc succeeds
 */
int should_add_case(should_suite_t *suite, void (*case_func)(void *));

/*!
 * \brief Runs all the test cases of the test suite.
 * \return 0 if all the test cases of the suite succeed
 */
int should_run_suite(should_suite_t *suite);

/*!
 * \brief Runs all the test cases of the test suite, and destroys the test 
 * suite.
 *
 * Internally, calls should_run_suite and should_destroy_suite.
 * \return 0 if all the test cases of the suite succeed
 */
int should_run_and_destroy_suite(should_suite_t *suite);

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
void should_be_(int expr, const char *expr_str, const char *file, int line, 
		const char *func);

void should_be_msg_(int expr, const char *msg, const char *expr_str,
		const char *file, int line, const char *func);

void should_cmp_(int val1, int val2, const char *expr1, const char *expr2,
		int expr, const char *cmp_str, const char *file, int line,
		const char *func);

void should_cmp_str_(const char *val1, const char *val2, const char *expr1,
		const char *expr2, int expr, const char *cmp_str,
		const char *file, int line, const char *func);
/** \endcond */

#ifdef __cplusplus
}
#endif

#endif /* SHOULD_H_ */
