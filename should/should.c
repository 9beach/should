/*
 * should.c
 * Hyunrae Cho (9beach@gmail.com), 2007/08/08
 */

#include "should.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int success_count_ = 0;
static int failure_count_ = 0;

typedef void (*case_func_t)();

struct should_suite_t {
	const char *name;
	case_func_t *cases;
	void *(*setup)();
	void (*teardown)(void *);
	int size;
};

should_suite_t *should_create_suite(const char *name)
{
	should_suite_t *suite = malloc(sizeof(should_suite_t));

	if (0 == suite) {
		return 0;
	}

	suite->name = name;
	suite->cases = 0;
	suite->setup = 0;
	suite->size = 0;
	suite->teardown = 0;

	return suite;
}

void should_destroy_suite(should_suite_t *suite)
{
	assert(suite);
	if (suite->cases) {
		free(suite->cases);
	}
	free(suite);
}

int should_add_case(should_suite_t *suite, case_func_t a_case)
{
	case_func_t *new_cases;

	assert(suite && a_case);
	new_cases = realloc(suite->cases, 
			sizeof(case_func_t) * (suite->size + 1));
	if (0 == new_cases) {
		return -1;
	}

	++suite->size;
	suite->cases = new_cases;
	suite->cases[suite->size - 1] = a_case;

	return 0;
}

void should_set_fixture(should_suite_t *suite, void *(*setup)(void *),
		void (*teardown)(void *))
{
	assert(suite);
	suite->setup = setup;
	suite->teardown = teardown;
}

static void run_a_case_(should_suite_t *suite, case_func_t a_case)
{
	void *fxtr = 0;

	assert(suite && a_case);
	if (suite->setup) {
		fxtr = (*suite->setup)(); /* allow fxtr == 0 */
	}
	a_case(fxtr);
	if (suite->teardown) {
		(*suite->teardown)(fxtr);
	}
}

int should_run_suite(should_suite_t *suite)
{
	int i;

	success_count_ = 0;
	failure_count_ = 0;

	assert(suite && suite->name);
	printf("*** Running should_suite \"%s\"...\n", suite->name);

	for (i = 0; i < suite->size; ++i) {
		assert(suite->cases[i]);
		run_a_case_(suite, suite->cases[i]);
	}

	printf("*** Results: %d failures, %d successes\n",
			failure_count_, success_count_);

	return failure_count_;
}

int should_run_and_destroy_suite(should_suite_t *suite)
{
	int ret;

	assert(suite);
	ret = should_run_suite(suite);
	should_destroy_suite(suite);

	return ret;
}

void should_be_(int expr, const char *expr_str, const char *file, int line,
		const char *func)
{
	assert(expr_str && file && line && func);
	if (0 != expr) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be failed: (%s) in \"%s\", %s (%d)\n", 
				expr_str, func, file, line); 
	}
}

void should_be_msg_(int expr, const char *msg, const char *expr_str,
		const char *file, int line, const char *func)
{
	assert(expr_str && file && line && func);
	if (0 != expr) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be failed: \"%s\" (%s) in \"%s\", %s (%d)\n", 
				msg, expr_str, func, file, line); 
	}
}

void should_cmp_(int val1, int val2, const char *expr1, const char *expr2, 
		int expr, const char *cmp_str, const char *file, int line, 
		const char *func)
{
	assert(expr1 && expr2 && cmp_str && file && line && func);
	if (expr) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be failed: (%s %s %s) -> not (%d %s %d) "
				"in \"%s\", %s (%d)\n", 
				expr1, cmp_str, expr2, val1, cmp_str, val2, 
				func, file, line); 
	}
}

void should_cmp_str_(const char *val1, const char *val2, const char *expr1,
		const char *expr2, int expr, const char *cmp_str,
		const char *file, int line, const char *func)
{
	assert(expr1 && expr2 && cmp_str && file && line && func);
	if (expr) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be failed: (%s %s %s) -> not (\"%s\" %s \"%s\") "
				"in \"%s\", %s (%d)\n", 
				expr1, cmp_str, expr2, val1, cmp_str, val2, 
				func, file, line); 
	}
}
