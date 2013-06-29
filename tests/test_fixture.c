#include "should/should.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* suppose that you implemented fwrite/ftell functions and 
 * want to verify them working correctly. */

void *setup()
{
        FILE *file = tmpfile();
        if (!file)
                abort();
        return file;
}

void teardown(void *fxtr)
{
        FILE *file = (FILE *)fxtr;

        assert(file);
        fclose(file);
}

void case_hello(void *fxtr)
{
        FILE *file = fxtr;
        const char *buf = "hello, ";
        const size_t cnt = strlen(buf);

        assert(file && 0 == ftell(file));
        should_be_eq(cnt, fwrite(buf, sizeof(char), cnt, file));
        should_be_eq(cnt, ftell(file));
}

void case_world(void *fxtr)
{
        FILE *file = fxtr;
        const char *buf = "world!";
        const size_t cnt = strlen(buf);

        assert(file && 0 == ftell(file));
        should_be_eq(cnt, fwrite(buf, sizeof(char), cnt, file));
        should_be_eq(cnt, ftell(file));
}

int main()
{
        should_suite_t *s0;

        s0 = should_create_suite("main");

        should_set_fixture(s0, setup, teardown);

        should_add_case(s0, case_world);
        should_add_case(s0, case_hello);

        return should_run_and_destroy_suite(s0);
}
