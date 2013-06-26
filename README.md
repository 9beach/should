# should

Minimal xUnit library for C programmers

## Installation

```
$ # install should library
$ git clone git@github.com:9beach/should.git
$ cd should
$ mkdir build
$ cmake ..
$ make
$ sudo make install
$ # test should lib
$ ctest 
$ # or
$ ctest --output-on-failure
```

## Glossary

### Test suite

Test suite can hold multiple test suites and/or test cases. test is performed by running a test suite.

### Test case

Test case is void (*)(void *) type function including at least one should_be macro.

### should_be

should_be macro verifies a expression. if a expression is not true, it puts error message (does not abort).

### setup, teardown and fixture

Fixture is a environment which is shared among test cases and/or suites of a test suite.

You can initialize/finalize the fixture with the setup/teardown function. if a suite has many suites and/or cases, the setup/teardown functions are called for each suite and/or case.

## Examples

Minimal example
```
#include <should/should.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void case_hello(void *fxtr)
{
        const char *a = "HELLO";
        const char *b = "hello\0\0";

        should_be_not_equal_strings("hello", a);
        should_be_equal_strings("hello", b);

        /* using should_be_equal_string is better to debug */
        should_be(strcmp("hello", a) != 0);
        should_be(strcmp("hello", b) == 0);
}

void case_world(void *fxtr)
{
        int a = 10;
        int b = 20;
        should_be_not_equal_numbers(a, b);
        should_be_equal_numbers(a, 10);
        should_be(a != b);
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

        return should_run_and_destroy_suite(s0);
}
```

Example with the fixture
```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <should/should.h>

void *setup()
{
        FILE *file = tmpfile();
        if (!file)
                abort();
        return file;
}

void teardown(void * fxtr)
{
        FILE *file = (FILE *)fxtr;

        assert(file);
        fclose(file);
}

void case_fwrite_hello(void *fxtr)
{
        FILE *file = (FILE *)fxtr;
        const char *buf = "hello";
        const size_t cnt = strlen(buf);

        assert(file);
        should_be(cnt == fwrite(buf, sizeof(char), cnt, file));
        should_be(ftell(file) == cnt);
}

void case_fwrite_world(void *fxtr)
{
        FILE *file = (FILE *)fxtr;
        const char *buf = "world!!!";
        const size_t cnt = strlen(buf);

        assert(file);
        should_be_equal_numbers(cnt, fwrite(buf, sizeof(char), cnt, file));
        should_be_equal_numbers(ftell(file), cnt);
}

int main()
{
        should_suite_t *s0;

        s0 = should_create_suite("MAIN");

        should_set_fixture(s0, setup, teardown);

        should_add_case(s0, case_fwrite_hello);
        should_add_case(s0, case_fwrite_world);

        return should_run_and_destroy_suite(s0);
}
```
