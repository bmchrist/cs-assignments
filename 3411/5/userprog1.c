#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "client.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>

int entry(int argc, char *argv[]){
  int in_flags, out_flags;
   int in_fd, out_fd;

   if(argc < 3)
   {
      printf("Usage: %s remote_output_file local_input_file\n", argv[0]);
      return 0;
   }

   /* Read from argv name of two files.
      First one is the name of the remote output file.
      Second one is the name of the local input file. */

   out_flags = O_CREAT|O_RDWR; /* It should be something other than 0. */
   out_fd = Open(argv[1], out_flags, 0777);  /* Open remote file. */
   printf("USERPROG1: Open() returned:%d, errno:%d\n", out_fd, errno);

   in_flags = O_CREAT|O_RDWR;  /* It should be something other than 0. */
   in_fd = open(argv[2], in_flags, 0777); /* Open local file. */

   /* In a while loop, read a number of bytes 
      from in_fd and write them to out_fd. */
   int n;
   int len;
   int writeRet;
   char *abuff;

   len = lseek(in_fd, 0, SEEK_END); //find size
   n = lseek(in_fd, 0, SEEK_SET); //go to beginning
   abuff = malloc(len);
   
   n = read(in_fd, abuff, len);
   printf("USERPROG1: %d bytes read from file:%s\n", n, abuff);

   writeRet = Write(out_fd, abuff, len); //problem in write as well
   printf("Write Returned:%d, errno%d\n", writeRet, errno);
   close(in_fd);
   int closeRet = Close(out_fd);
   printf("Close Returned:%d, errno%d\n", closeRet, errno);

   return 0;
}
