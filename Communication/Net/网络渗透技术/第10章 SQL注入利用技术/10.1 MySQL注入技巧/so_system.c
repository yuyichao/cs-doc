/*
writen by Chris Anley [chris@ngssoftware.com]
compile with something like
gcc -g -c so_system.c
then
gcc -g -shared -W1,-soname,so_system.so.0 -o so_system.so so_system.o -lc
*/

#include <stdio.h>
#include <stdlib.h>

enum Item_result {STRING_RESULT,REAL_RESULT,INT_RESULT};

typedef struct st_udf_args
{
  unsigned int arg_count;               /* Number of arguments */
  enum Item_result *arg_type;           /* Pointer to item_results */
  char **args;                          /* Pointer to argument */
  unsigned long *lengths;               /* Length of string arguments */
  char *maybe_null;                     /* Set to 1 for all maybe_null args */
} UDF_ARGS;

  /* This holds information about the result */

typedef struct st_udf_init
{
  char maybe_null;                      /* 1 if function can return NULL */
  unsigned int decimals;                /* for real functions */
  unsigned int max_length;              /* For string functions */
  char    *ptr;                         /* free pointer for function data */
  char const_item;                      /* 0 if result is independent of arguments */
} UDF_INIT;

int do_system( UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
    if( args->arg_count != 1 )
        return 0;
    system( args->args[0] );
    return 0;
}
