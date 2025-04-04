/*
 * Performance Monitor Unit (PMU)
 * Copyright (C) shouran.ma@rwth-aachen.de
 *
 * libpmu is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * libpmu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PMU_H
#define PMU_H

#define _GNU_SOURCE
#include <stdbool.h>      /* bool   */
#include <stddef.h>       /* size_t */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>       /* malloc, abort, rand */
#include <string.h>       /* memset, memcpy, strcpy, strerror */
#include <sched.h>	  /* sched_getcpu */

/* resource */
#include <time.h>         /* clock  */
#include <sys/resource.h> /* rusage */

/* error */
#include <assert.h>
#include <errno.h>
#include <setjmp.h>
jmp_buf ERRNO_FLAG;

#ifdef __cplusplus
extern "C"{
#endif

/********************************
 *    Performance Evaluation    *
 ********************************/

/**
 * rdtsc - read time stamp counter (in CPU cycles/tick)
 */
static inline uint64_t rdtsc()
{
  uint64_t result;
  __asm__ volatile ("rdtsc; shlq $32,%%rdx; orq %%rdx,%%rax"
    : "=a" (result) : : "%rdx");
  return result;
}


/**
 * rdmaxrss - read max resident set size (rss) in Kb.
 */
static inline int64_t rdmaxrss()
{
  struct rusage rss;
  getrusage(RUSAGE_SELF, &rss);
  return rss.ru_maxrss;
}

void print_cpuinfo(const char *__restrict s, uint64_t *cpu, size_t ntests);
void print_clkinfo(const char *__restrict s, struct timespec *clk, size_t ntests);
void print_meminfo(const char *__restrict s, uint64_t *mem, size_t ntests);


/*******************
 *    Unit Test    *
 *******************/

void pmu_test_begin(const char *__restrict func_name);
void pmu_test_do(const char *__restrict case_name);
void pmu_test_placeholder(void);
void pmu_test_done(void);
void pmu_test_end(void);
int  pmu_test_reterr(void);
void pmu_test_pile(const char *__restrict file,
  const char *__restrict func, const int line);

#define TEST_BEGIN() pmu_test_begin(__func__)
#define TEST_DO(s)   pmu_test_do(s)
#define TEST_PLACEHOLDER() pmu_test_placeholder();
#define TEST_DONE()  pmu_test_done()
#define TEST_END()   pmu_test_end()
#define TEST_ERR     pmu_test_reterr()
#define TEST_PILE()  pmu_test_pile(__FILE__, __func__, __LINE__);


/*********************
 *    Assertation    *
 *********************/

void pmu_assert_fail(const char *__restrict expr,
  const char *__restrict file, const char *__restrict func, const int line);

#define ASSERT(expr)\
  ((expr)? (void)0 : pmu_assert_fail(#expr, __FILE__, __func__, __LINE__))



/**************
 *    Time    *
 **************/

#include <time.h>

static inline void pmu_timeit(void (*f)())
{
	struct timespec start_real, end_real;
	clock_t start_cpu, end_cpu;
	uint64_t start_cycles, end_cycles;

	/* get start status */
	clock_gettime(CLOCK_REALTIME, &start_real);
	start_cpu = clock();
	start_cycles = rdtsc(); /* get current time stamp (CPU cycles) */

	f();

	/* get end status */
	clock_gettime(CLOCK_REALTIME, &end_real);
	end_cpu = clock();
	end_cycles = rdtsc(); /* get current time stamp (CPU cycles) */

	/* calc real time */
	double real_time = (end_real.tv_sec - start_real.tv_sec) +
			   (end_real.tv_nsec - start_real.tv_nsec) / 1e9;

	double cpu_time = (double)(end_cpu - start_cpu) / CLOCKS_PER_SEC;
	uint64_t cycles = end_cycles - start_cycles;

	/* Output */
	printf("Evaluation took:\n");
	printf("  %.6f seconds of real time\n", real_time);
	printf("  %.6f seconds of total run time (%.6f user, 0.000000 system)\n",
	       cpu_time, cpu_time);
	printf("  %.2f%% CPU\n", (cpu_time / real_time) * 100);
	printf("  %lu processor cycles\n", cycles);
}

#ifdef __cplusplus
}
#endif

#endif /* PMU_H */
