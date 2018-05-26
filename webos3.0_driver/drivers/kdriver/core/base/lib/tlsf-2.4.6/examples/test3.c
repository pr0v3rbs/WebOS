#include <stdio.h>
#include <stdlib.h>
#include "tlsf.h"

#define POOL_SIZE 1024 * 2000


// Pool size is in bytes.
char p0[POOL_SIZE];
char p1[POOL_SIZE];
char p2[POOL_SIZE];
char p3[POOL_SIZE];

int main(void){
  int	i;
  char *ptr;
  
  printf("Pools' addresses: (0) %p (1) %p (2) %p (3) %p\n", p0, p1, p2, p3);
  init_memory_pool(POOL_SIZE, p0);
  init_memory_pool(POOL_SIZE, p1);
  init_memory_pool(POOL_SIZE, p2);
  init_memory_pool(POOL_SIZE, p3);
 
  for ( i=0; i<2050;i++)
  {
	  if (!(ptr=malloc_ex(1024, p0)))
	  {
 	  	 printf("P0 Error (%d)\n", i ); continue;
	  }

   	 printf("P0 Ok (%d)\n", i );
  }

  for ( i=0; i<2050;i++)
  {
	  if (!(ptr=malloc_ex(1024, p3)))
	  {
 	  	 printf("P3 Error (%d)\n", i ); continue;
	  }
   	 printf("P3 Ok (%d)\n", i );
  }

  destroy_memory_pool(p0);
  destroy_memory_pool(p1);
  destroy_memory_pool(p2);
  destroy_memory_pool(p3);
  printf("Test OK\n");
  exit(0);
}

    
	
