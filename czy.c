#include <unistd.h>
#include <stdio.h>

int in_set(unsigned char value){
  if( (value <= 15 && value >= 9) ||
      (value == 32) ||
      (
        value <= 93 && value >= 40 && value != 64
      ) ||
      (value == 27) ||
      (value == 127) ||
      (value <= 122 && value >= 97) ){
    return 1;
  }
  return 0;
}

/* TODO: reword
 * Takes an 8 bit char and output 'bit' bits of it 
 * Outputs 8 bits at a time, storing bits until there are at 
 * least 8 bits, and storing any overflow after
 * until there are at least 8 bits, and holding any overflow
 * When pad is > 0, it pads the remaining bits and outputs
 * even if it is not at 8 bits yet.
 * Index is the index of the next available bit for storage
 */ 
void output( unsigned char print, int bits, unsigned char *store, int *index, int should_pad){
  /*printf(" | "); */
  /*printf("idx:%d ", *index);*/
  /*if( bits == 6){*/
    /*printf("ecma %d:%x, ", print, print);*/
  /*}*/
  /*else{*/
    /*printf("ascii %d, ", print);*/
  /*}*/

  /*printf(" | s:%x, ", *store);*/
  /*printf("%x, ", print);*/
  int available = 8-(*index);
  if( available <= bits ){ /* reaches the end or overflows */
    int overflow = bits - available;
    unsigned char result = print >> overflow; /* bitshift to put only bits that fill fit at end */ 
    *store = *store | result; /* or it into store */
    /*printf("w:%x ", * store);*/
    write(1, store, 1); //[> print it, since we're full now <]
    /*printf("bw:%x, ", *store);*/

    *store = 0; /* reset store */
    *index = 0;

    if( overflow > 0 ){ /* Store the remaining bits */
      result = print << (8-overflow); /* bitshift to put only remaining bits at beginning */
      *store = result;
      *index = overflow; /* set index to next available spot */
    }
  }
  else{ /* Fits without reaching end of store */
    int shift = available - bits;
    print = print << shift;
    *store = *store | print;
    *index = *index + bits ; /* Update index. eg: index is 1 and add 6 bits. Index is now 7 */
  }
  if( should_pad ){ /* pad the results and print, even if we're not full */
    if( index != 0){ /* there's at least SOMETHING to print */
      /*printf("w2:%x ", * store);*/
      write(1,store,1); //[> store is already padded. print it <]
      /*printf("bwf:%x, ", store);*/
      /* and reset */
      *index = 0;
      *store = 0;
    }
  }
}

int main( int argc, char *argv[]){
  int case_flag = 0; /* 0 means upper case, 1 means lower */
  
  int read_size = 1; /* Used to loop until end of stream */

  /* Used by output function to track remaining bits */
  unsigned char store = 0; 
  int store_idx = 0;

  while(read_size > 0){ /* Read until end of stream */
    unsigned char buf;
    read_size = read(0, &buf, 1);
    /*printf("hex: %x, ", buf);*/
    if ( read_size == 0){
      break;
    }
    /*printf("size: %d value: %x ", read_size, buf);*/
    
    /* If it's a character in the set */
    if (in_set(buf)){
      /*printf("in set %x, ", buf);*/
      /* Handle case */
      if( buf <= 'Z' && buf >= 'A') {
        if ( case_flag == 1 ){ /* previously lower case */
          case_flag = 0;
          /* Output toggle case */
          output(32, 6, &store, &store_idx, 0); /* ecma-1 null */
          output(27, 7, &store, &store_idx, 0); /* ascii esc */
        }
      }
      if( buf <= 'z' && buf >= 'a'){
        buf = buf - 32; /* convert to upper case */
        if ( case_flag == 0 ){ /* previously upper case */
          case_flag = 1;
          /* Output toggle case */
          output(32, 6, &store, &store_idx, 0); /* ecma-1 null */
          output(27, 7, &store, &store_idx, 0); /* ascii esc */
        }
      }

      /* Output characters */
      unsigned char code;
      if( (buf <= 93 && buf >= 40) && buf != 64){
        code = buf - 32;
      }
      else if ( buf == 32){
        code = 0;
      }
      else if( buf <= 15 && buf >= 9){
        code = buf - 8;
      }
      else if ( buf == 27){
        code = 62;
      }
      else if ( buf == 127){
        code = 63;
      }
      else{
        write(1,"Something bad happened\n",23);
        return -1;
      }
      output(code, 6, &store, &store_idx, 0); /* 6 bit ecma representation */
    }
    /* Any other character not in the set */
    else{
      /*printf("not in set %x, ", buf);*/
      /* Output null and then ascii character */
      output(32, 6, &store, &store_idx, 0); /* ecma-1 null */
      output(buf, 7, &store, &store_idx, 0); /* 7 bit ascii character */
    }
  }
  /* Output end of file */
  output(32, 6, &store, &store_idx, 0); /* ecma-1 null */
  output(48, 7, &store, &store_idx, 1); /* ascii 0 , and pad */
}
