#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
  void *a;
  size_t mallocSize = 0;

  mallocSize = atoi(argv[1]);
  mallocSize = mallocSize * 1048576;
#if defined(_LP64)
  printf("Malloc'ing %lld bytes . . . . \n", mallocSize);
#else
  printf("Malloc'ing %d bytes . . . . \n", mallocSize);
#endif
  fflush(stdout);

  a = malloc(mallocSize);
  if ( a == NULL )
  {    
    printf("Malloc failed!\n");
    fflush(stdout);
  }
  else
  {
    printf("malloc returned %p (pointer location %p)\n", a, &a);
    if ( argc != 3 )
    {
      printf("Doing memset . . . . \n");
      fflush(stdout);
      memset(a, '\0', mallocSize);
    }
    printf("Sleeping . . . . \n");
    fflush(stdout);
    sleep(10000000);
  }
  return 0;
}
