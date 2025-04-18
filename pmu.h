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
void pmu_timeit(void (*f)());

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



/*********************
 *    Debugging      *
 *********************/

void print_ind(const char name[], const int *v, const int n);

void print_vec_u8(const char name[], const uint8_t *v, const int n);
void print_vec_u32(const char name[], const uint32_t *v, const int n);
void print_vec_u64(const char name[], const uint64_t *v, const int n);

#define print_vec(name, array, len)  \
_Generic((array),                    \
         uint8_t*:  print_vec_u8,    \
         uint32_t*: print_vec_u32,   \
         uint64_t*: print_vec_u64    \
)(name, array, len)

void print_mat_u8(const char name[], const uint8_t *v, const int m, const int n);
void print_mat_u32(const char name[], const uint32_t *v, const int m, const int n);
void print_mat_u64(const char name[], const uint64_t *v, const int m, const int n);

#define print_mat(name, array, row, col) \
_Generic((array),                        \
         uint8_t*:  print_mat_u8,        \
         uint32_t*: print_mat_u32,       \
         uint64_t*: print_mat_u64        \
)(name, array, row, col)

#ifdef __cplusplus
}
#endif

#endif /* PMU_H */
