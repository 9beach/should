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
	should_be(cnt == fwrite(buf, sizeof(char), cnt, file));
	should_be(ftell(file) == cnt);
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

