#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main( int argc, char *argv[])
{
  if( argc < 3){
    write(2, "Incorrect number of arguments\n", 30);
    return 1;
  }

  int blocksize;

  /* Set the blocksize */
  if( argc == 3 ) { /* Blocksize not specified */
    blocksize = 65536;
  }
  else{
    blocksize = atoi(argv[3]);
    if ( blocksize%4 != 0){ /* Not a multiple of four */
      write(2, "Blocksize must be a multiple of 4. Rounding up.\n", 47 );
      blocksize = blocksize + 4-blocksize%4;
    }
  }

  /* Open the input file */
  int readfd = open(argv[1], O_RDONLY);
  int writefd = open(argv[2], O_WRONLY | O_CREAT, 0600);
  if( readfd <= 0 ){
    write(2,"Could not open input file\n",26);
  }
  if( writefd <= 0){
    write(2,"Could not open output file\n",27);
  }

  ssize_t rnum, wnum; /* Number of bytes read */
  int i;
  unsigned int checksum = 0; /* Stores an xor of each consecutive 4 bytes */
  unsigned int buf[blocksize/4]; /* Store blocksize bytes */ 

  do{
    rnum = read(readfd, (char *)buf, blocksize);
    
    /* If we had 4 bytes last time, and that was the end of the file... */
    if( rnum <= 0 ){ 
      printf("breaking\n");
      break; /* Then we're done */
    }

    /* Zero fill if we read less than blocksize bytes */
    for( i=blocksize; i>rnum; i--){
      *(((char*) buf)+i-1) = 0 ;
    }

    /* Calculate checksum so far */
    for(i=0; i<blocksize/4; i++){
      checksum = checksum ^ buf[i];
    }

    /* Write blocksize bytes to the outfile */
    wnum = write(writefd, buf, blocksize);
    if( wnum != blocksize){
      write(2, "There was an error writing to the file\n", 39); 
    }

  }while( rnum == blocksize); /* Continue until we reach the end of the file */

  /* Sprintf + write TODO */
  
  char strbuf[19];
  i = sprintf(strbuf, "Checksum %x\n", checksum);
  write(1,strbuf,i);

  
}
