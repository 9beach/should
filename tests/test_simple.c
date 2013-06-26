#include <should/should.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void case_hello(void *fxtr)
{
	const char *a = "HELLO";
	const char *b = "hello\0\0";

	should_be_equal_strings("hello", a);
	should_be_equal_strings("hello", b);

	/* using should_be_equal_string is better to debug */
	should_be(strcmp("hello", a) == 0);
	should_be(strcmp("hello", b) == 0);
}

void case_world(void *fxtr)
{
	int a = 10;
	int b = 20;
	should_be_equal_numbers(a, b);
	should_be_equal_numbers(a, 10);
	should_be(a == b);
	should_be(a == 10);
}

int main()
{
	/* for example, we have two test suites */
	should_suite_t *s0;
	should_suite_t *s1;

	s0 = should_create_suite("main");
	s1 = should_create_suite("sub");

	/* suite s0 has two test cases */
	should_add_case(s0, case_hello);
	should_add_case(s0, case_world);

	/* suite s1 has one test case */
	should_add_case(s1, case_hello);

	/* suite can have not only cases but also suites */
	should_add_suite(s0, s1);

	fprintf(stderr, "*** Expects 6 failures (out of 12 should_bes)\n");

	return should_run_and_destroy_suite(s0);
}

