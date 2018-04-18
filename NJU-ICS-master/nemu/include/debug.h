#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <assert.h>

#ifdef DEBUG
extern FILE* log_fp;
#define Log_write(format, ...) \
  do { \
    if (log_fp != NULL) { \
      fprintf(log_fp, format, ## __VA_ARGS__); \
      fflush(log_fp); \
    } \
  } while (0)
#else
#define Log_write(format, ...)
#endif

/* The function Log is used to print debug informations.
 * And it will also print the file who uses Log function,
 * and alongwith the file's row number and function.
*/
#define Log(format, ...) \
  do { \
    fprintf(stdout, "\33[1;34m[%s,%d,%s] " format "\33[0m\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
    fflush(stdout); \
    Log_write("[%s,%d,%s] " format "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0)

/* Assert is a upgraded edition of assert(), when test condition is false,
 * it will print some information before assertion fail.
*/
#define Assert(cond, ...) \
  do { \
    if (!(cond)) { \
      fflush(stdout); \
      fprintf(stderr, "\33[1;31m"); \
      fprintf(stderr, __VA_ARGS__); \
      fprintf(stderr, "\33[0m\n"); \
      assert(cond); \
    } \
  } while (0)

/* panic is used to print information and close the function,
 * it just like a assertion fail without test condition.
*/
#define panic(format, ...) \
  Assert(0, format, ## __VA_ARGS__)

#define TODO() panic("please implement me")

#endif
