#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  unsigned char *a;
  size_t mmapSize = 0;

  if ( argc < 5 )
  {
    printf("usage: %s <filename> <size> <ro/rw> <flags> [nomemset]\n", argv[0]);
    fflush(stdout);
    return -1;
  }

  int openFlags = 0;
  int mmapProt  = 0;
  if ( strstr((const char *)(argv[3]), (const char *)"rw") != NULL )
  {
    // Must match mmap() and open()
    mmapProt  = PROT_READ | PROT_WRITE,
    openFlags = openFlags | O_RDWR;
  }
  else
  {
    mmapProt = PROT_READ;
    openFlags = openFlags | O_RDONLY;
  }
  int fd = -1;

  fd = open((const char *)argv[1], openFlags);
  
  if ( fd == -1 )
  {    
    printf("open failed! errno %d\n", errno);
    fflush(stdout);
    return -1;
  }
  mmapSize = atoi(argv[2]);
  mmapSize = mmapSize * 1048576;
  
#if defined(_LP64)
  printf("Mmap'ing %lld bytes . . . . \n", (long long int)mmapSize);
#else
  printf("Mmap'ing %d bytes . . . . \n", mmapSize);
#endif
  fflush(stdout);

  int flags = MAP_ANONYMOUS;
  if ( strstr((const char *)(argv[4]), (const char *)"SHARED") != NULL )
    flags = flags | MAP_SHARED;
  else
    flags = flags | MAP_PRIVATE;

  if ( strstr((const char *)(argv[4]), (const char *)"GROWSDOWN") != NULL )
  {
    printf("Setting MAP_GROWSDOWN\n");
    flags = flags | MAP_GROWSDOWN;
  }
  if ( strstr((const char *)(argv[4]), (const char *)"HUGETLB") != NULL )
  {
    printf("Setting MAP_HUGETLB\n");
    flags = flags | MAP_HUGETLB;
  }
  if ( strstr((const char *)(argv[4]), (const char *)"LOCKED") != NULL )
  {
    printf("Setting MAP_LOCKED\n");
    flags = flags | MAP_LOCKED;
  }
  if ( strstr((const char *)(argv[4]), (const char *)"NONBLOCK") != NULL )
  {
    printf("Setting MAP_NONBLOCK\n");
    flags = flags | MAP_NONBLOCK;
  }
  if ( strstr((const char *)(argv[4]), (const char *)"NORESERVE") != NULL )
  {
    printf("Setting MAP_NORESERVE\n");
    flags = flags | MAP_NORESERVE;
  }
  if ( strstr((const char *)(argv[4]), (const char *)"POPULATE") != NULL )
  {
    printf("Setting MAP_POPULATE\n");
    flags = flags | MAP_POPULATE;
  }
  
  a = (unsigned char *)mmap(NULL /* no specific location */,
			    mmapSize,
			    mmapProt,
			    flags,
			    fd,
			    0 /* offset */);
  
  if ( ( a == NULL ) || ( a == (unsigned char *)-1 ) )
  {    
    printf("Mmap failed! errno %d\n", errno);
    fflush(stdout);
  }
  else
  {
    printf("mmap returned %p (pointer location %p)\n", a, &a);
    if ( argc == 6 )
    {
      int eventualValue = 0;
      if ( strstr((const char *)(argv[5]), (const char *)"read") != NULL )
      {
	printf("Reading bytes . . . . \n");
	fflush(stdout);
	int i;
	// We don't care about the contents, but do want to avoid the whole loop
	// being optimized out!
	for ( i=0 ; i<mmapSize ; i++ )
	{
	  eventualValue += a[i];
	}
	printf("eventualValue %d\n", eventualValue);
	fflush(stdout);
      }
    }
    else
    {
      printf("Doing memset . . . . \n");
      fflush(stdout);
      memset((void *)a, '\0', mmapSize);
    }
    printf("Sleeping . . . . \n");
    fflush(stdout);
    sleep(10000000);
  }
  return 0;
}
