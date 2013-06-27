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

enum {
	SHOULD_UNIT_TYPE_CASE,
	SHOULD_UNIT_TYPE_SUITE
};

typedef struct should_unit_t {
	unsigned char type;
	union {
		void (*case_func)();
		should_suite_t *suite;
	} unit; /* not all compiler support unnamed union */
} should_unit_t;

struct should_suite_t {
	const char *name;
	should_unit_t *units;
	void *(*setup)();
	void (*teardown)(void *);
	int size;
};

void should_be_(int expr, const char *expr_str, const char *file, 
		int line, const char *func)
{
	assert(expr_str && file && line && func);
	if (0 != expr) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be: failed (%s) in \"%s\", %s (%d)\n", 
				expr_str, func, file, line); 
	}
}

void should_be_equal_strings_(const char *val1, const char *val2, 
		const char *expr_str, const char *file, int line, 
		const char *func)
{
	assert(val1 && val2 && expr_str && file && line && func);
	if (0 == strcmp(val1, val2)) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be: failed (%s) -> (\"%s\" != \"%s\") "
				"in \"%s\", %s (%d)\n", 
				expr_str, val1, val2, func, 
				file, line); 
	}
}

void should_be_not_equal_strings_(const char *val1, const char *val2, 
		const char *expr_str, const char *file, int line, 
		const char *func)
{
	assert(val1 && val2 && expr_str && file && line && func);
	if (0 != strcmp(val1, val2)) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be: failed (%s) -> (\"%s\" == \"%s\") "
				"in \"%s\", %s (%d)\n", 
				expr_str, val1, val2, func, 
				file, line); 
	}
}

void should_be_equal_numbers_(int val1, int val2, const char *expr_str, 
		const char *file, int line, const char *func)
{
	assert(expr_str && file && line && func);
	if (val1 == val2) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be: failed (%s) -> (%d != %d) "
				"in \"%s\", %s (%d)\n", 
				expr_str, val1, val2, func, 
				file, line); 
	}
}

void should_be_not_equal_numbers_(int val1, int val2, const char *expr_str, 
		const char *file, int line, const char *func)
{
	assert(expr_str && file && line && func);
	if (val1 != val2) {
		++success_count_;
	} else {
		++failure_count_;
		printf("should_be: failed (%s) -> (%d == %d) "
				"in \"%s\", %s (%d)\n", 
				expr_str, val1, val2, func, 
				file, line); 
	}
}

should_suite_t *
should_create_suite(
		const char *name
		)
{
	should_suite_t *suite = 
		(should_suite_t *)malloc(sizeof(should_suite_t));

	if (0 == suite)
		return 0;

	suite->name = name;
	suite->units = 0;
	suite->setup = 0;
	suite->size = 0;
	suite->teardown = 0;

	return suite;
}

void
should_destroy_suite(
		should_suite_t *suite
		)
{
	assert(suite);

	if (suite->units)
		free(suite->units);
	free(suite);
}

void
should_destroy_suite_recursively(
		should_suite_t *suite
		)
{
	int i;
	assert(suite);

	for (i = 0; i < suite->size; ++i) {
		if (suite->units[i].type != SHOULD_UNIT_TYPE_CASE) {
			should_destroy_suite_recursively(
				suite->units[i].unit.suite);
		}
	}
	should_destroy_suite(suite);
}

int
should_add_case(
		should_suite_t *suite,
		void (*case_func)(void *)
	 )
{
	should_unit_t *new_case;

	assert(suite);
	assert(case_func);
	new_case = realloc(suite->units, 
			sizeof(should_unit_t) * (suite->size + 1));
	if (0 == new_case)
		return -1;

	++suite->size;
	suite->units = new_case;
	suite->units[suite->size - 1].type = SHOULD_UNIT_TYPE_CASE;
	suite->units[suite->size - 1].unit.case_func = case_func;

	return 0;
}

int
should_add_suite(
		should_suite_t *owner,
		should_suite_t *suite
	  )
{
	should_unit_t *new_suite;

	assert(owner && suite);
	new_suite = realloc(owner->units, 
			sizeof(should_unit_t) * (owner->size + 1));

	if (0 == new_suite)
		return -1;

	++owner->size;
	owner->units = new_suite;
	owner->units[owner->size - 1].type = SHOULD_UNIT_TYPE_SUITE;
	owner->units[owner->size - 1].unit.suite = suite;

	return 0;
}

void
should_set_fixture(
		should_suite_t *suite,
		void *(*setup)(void *),
		void (*teardown)(void *)
		)
{
	assert(suite);
	suite->setup = setup;
	suite->teardown = teardown;
}

static void run_a_case_(should_suite_t *suite, should_unit_t *unit)
{
	void *fxtr = 0;

	assert(unit && suite);

	if (suite->setup) {
		fxtr = (*suite->setup)();
		if (!fxtr) {
			printf("*** Failed to setup the fixture of `%s'. "
					"can not run a should_case\n", 
					suite->name);
			return;
		}
	}
	unit->unit.case_func(fxtr);
	if (suite->teardown)
		(*suite->teardown)(fxtr);
}

static void run_suite_recursively_(should_suite_t *suite)
{
	void *fxtr = 0;
	int i;

	assert(suite);

	for (i = 0; i < suite->size; ++i) {
		if (suite->units[i].type == SHOULD_UNIT_TYPE_CASE) {
			run_a_case_(suite, &(suite->units[i]));
		} else {
			printf("*** Running should_suite \"%s\"...\n", 
					suite->units[i].unit.suite->name);
			if (suite->setup)
				fxtr = (*suite->setup)();
			run_suite_recursively_(suite->units[i].unit.suite);
			if (suite->teardown)
				(*suite->teardown)(fxtr);
		}
	}
}

int
should_run_suite(
		should_suite_t *suite
	     )
{
	success_count_ = 0;
	failure_count_ = 0;

	assert(suite);

	printf("*** Running should_suite \"%s\"...\n", suite->name);

	run_suite_recursively_(suite);

	if (failure_count_ != 0) {
		printf("*** %d failures (out of %d should_bes) "
				"detected in should_suite \"%s\"\n", 
				failure_count_, 
				failure_count_ + success_count_, suite->name);
	} else {
		printf("*** No errors (out of %d should_bes) "
				"detected in should_suite \"%s\"\n",
				failure_count_ + success_count_, suite->name);
	}

	return (failure_count_ != 0);
}

int
should_run_and_destroy_suite(
		should_suite_t *suite
	     )
{
	int ret;

	assert(suite);

	ret = should_run_suite(suite);
	should_destroy_suite_recursively(suite);
	return ret;
}

