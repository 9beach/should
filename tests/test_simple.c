#include "should/should.h"

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
	/* we gonna make two test suites */
	should_suite_t *s0;
	should_suite_t *s1;

	s0 = should_create_suite("main");
	s1 = should_create_suite("sub");

	/* suite s0 has two test cases */
	should_add_case(s0, case_hello);
	should_add_case(s0, case_world);

	/* suite s1 has one test case */
	should_add_case(s1, case_hello);

	/* a suite can have not only cases but also suites */
	should_add_suite(s0, s1);

	/* and finally ... */
	return should_run_and_destroy_suite(s0);
}
