#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  void *a;
  size_t mmapSize = 0;

  if ( argc < 3 )
  {
    printf("usage: %s <size> <flags> [nomemset]\n", argv[0]);
    fflush(stdout);
    return -1;
  }
  mmapSize = atoi(argv[1]);
  mmapSize = mmapSize * 1048576;
  
#if defined(_LP64)
  printf("Mmap'ing %lld bytes . . . . \n", (long long int)mmapSize);
#else
  printf("Mmap'ing %d bytes . . . . \n", mmapSize);
#endif
  fflush(stdout);

  int flags = MAP_ANONYMOUS;
  if ( strstr((const char *)(argv[2]), (const char *)"SHARED") != NULL )
    flags = flags | MAP_SHARED;
  else
    flags = flags | MAP_PRIVATE;

  if ( strstr((const char *)(argv[2]), (const char *)"GROWSDOWN") != NULL )
  {
    printf("Setting MAP_GROWSDOWN\n");
    flags = flags | MAP_GROWSDOWN;
  }
  if ( strstr((const char *)(argv[2]), (const char *)"HUGETLB") != NULL )
  {
    printf("Setting MAP_HUGETLB\n");
    flags = flags | MAP_HUGETLB;
  }
  if ( strstr((const char *)(argv[2]), (const char *)"LOCKED") != NULL )
  {
    printf("Setting MAP_LOCKED\n");
    flags = flags | MAP_LOCKED;
  }
  if ( strstr((const char *)(argv[2]), (const char *)"NONBLOCK") != NULL )
  {
    printf("Setting MAP_NONBLOCK\n");
    flags = flags | MAP_NONBLOCK;
  }
  if ( strstr((const char *)(argv[2]), (const char *)"NORESERVE") != NULL )
  {
    printf("Setting MAP_NORESERVE\n");
    flags = flags | MAP_NORESERVE;
  }
  if ( strstr((const char *)(argv[2]), (const char *)"POPULATE") != NULL )
  {
    printf("Setting MAP_POPULATE\n");
    flags = flags | MAP_POPULATE;
  }
  
  a = mmap(NULL /* no specific location */,
	   mmapSize,
	   PROT_READ | PROT_WRITE,
	   flags,
	   -1 /* fd */,
	   0 /* offset */);
  
  if ( ( a == NULL ) || ( a == (void *)-1 ) )
  {    
    printf("Mmap failed! errno %d\n", errno);
    fflush(stdout);
  }
  else
  {
    printf("mmap returned %p (pointer location %p)\n", a, &a);
    if ( argc != 4 )
    {
      printf("Doing memset . . . . \n");
      fflush(stdout);
      memset(a, '\0', mmapSize);
    }
    printf("Sleeping . . . . \n");
    fflush(stdout);
    sleep(10000000);
  }
  return 0;
}
