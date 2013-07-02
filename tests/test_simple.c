#include "should/should.h"

#include <stdlib.h>

void case_hello(void *fxtr)
{
	const char *a = "HELLO";
	const char *b = "hello";

	should_be_ne_str("hello", a);
	should_be_ne_str("HELLO", b);
	should_be_eq_str("hello", b);
}

void case_world(void *fxtr)
{
	int a = 10;
	int b = 20;

	should_be(a != b);
	should_be(a == 10);
	should_be_msg(a != 9, "OMG");

	should_be_eq(a, 10);
	should_be_eq(b, 20);
	should_be_ne(a, 20);
	should_be_ne(b, 10);
	should_be_ne(10, 20);
	should_be_ne(20, 10);
	should_be_lt(9, 20);
	should_be_le(9, 20);
	should_be_le(9, 9);
}

int main()
{
	int ret0, ret1;

	/* as a example, we gonna make two test suites */
	should_suite_t *s0;
	should_suite_t *s1;

	s0 = should_create_suite("simple test A");
	if (!s0) {
	    abort();
	}

	s1 = should_create_suite("simple test B");
	if (!s1) {
	    abort();
	}

	/* suite s0 has two test cases */
	should_add_case(s0, case_hello);
	should_add_case(s0, case_world);

	/* suite s1 has one test case */
	should_add_case(s1, case_hello);

	/* and finally ... */
	ret0 = should_run_and_destroy_suite(s0);
	ret1 = should_run_and_destroy_suite(s1);

	return ret0 || ret1;
}
