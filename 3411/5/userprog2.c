#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "client.h"
#include <string.h>
#include <errno.h>

int entry(int argc, char *argv[])
{
   int in_flags, out_flags;
   int in_fd, out_fd;

   if(argc < 3)
   {
      printf("Usage: %s local_output_file remote_input_file\n", argv[0]);
      exit(0);
   }

   /* Read from argv name of two files.
      First one is the name of the local output file.
      Second one is the name of the remote input file. */
   mode_t mask;
   mask = umask(0);
    // printf("umask: %o\n", mask);
   mode_t permission;
   permission = 0777;

   out_flags = 514; /* It should be something other than 0. */
   printf("USERPROG2: ARGV[1]:%s|\n", argv[1]);
   out_fd = open(argv[1], O_CREAT|O_RDWR, permission);  /* Open local file. */
   printf("USERPROG2: local open returned fd:%d, errno:%d\n", out_fd, errno);

   in_flags = 514;  /* It should be something other than 0. */
   in_fd = Open(argv[2], in_flags, 0777); /* Open remote file. */
   printf("USERPROG2: Open() returned %d, errno:%d\n", in_fd, errno);


   // Seek(in_fd, 10, SEEK_SET);

   /* Seek to position 10 in in_fd.
      In a while loop, read a number of bytes 
      from in_fd and write them to out_fd. */

   int n;
   int len;
   int writeRet;
   char *aBuff2;
   
   len = Seek(in_fd, 0, SEEK_END);
   printf("USERPROG2: Seek() to end returned:%d, errno:%d \n", len, errno);
   aBuff2 = (char *)malloc(len);
   if(aBuff2 ==NULL){
      printf("MALLOC FAILED\n");
      exit(1);
   }
   n = Seek(in_fd, 0, SEEK_SET);
   printf("USERPROG2: Seek() to begin returned:%d, errno:%d \n", n, errno);

   n = Read(in_fd, aBuff2, len);
   printf("USERPROG2: n bytes read from server:%d, errno:%d, Read from server:%s", n, errno, aBuff2);
 
   writeRet = write(out_fd, aBuff2, len);//issue here?
   printf("USERPROG2: write Returned:%d\n", writeRet);
   printf("USERPROG2: Errno:%d\n",errno );

   return 0;
}
