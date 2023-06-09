# libpmu

A performance monitoring unit.

## Usage

```c
#include "path/to/pmu.h"

static int test_foo()
{
TEST_BEGIN();

TEST_DO("foo 1");
	TEST_ERR |= foo1();
TEST_DONE();

TEST_DO("foo 2");
	TEST_ERR |= foo2();
TEST_DONE();

TEST_END();
	return TEST_ERR;
}

static int test_bar()
{
TEST_BEGIN();

TEST_DO("bar 1");
	TEST_ERR |= bar1();
TEST_DONE();

TEST_DO("bar 2");
	TEST_ERR |= bar2();
TEST_DONE();

TEST_END();
	return TEST_ERR;
}

int main(int argc, char *argv[])
{
	int err = 0;
	err |= test_foo();
	err |= test_bar();
	return err;
}
```
