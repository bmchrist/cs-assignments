#include <unistd.h>
#include <stdio.h>

unsigned char input(int num_bits, unsigned char *buf, int *index){
  /*printf("| s:%x ", *buf);*/
  /*printf("idx:%d ", *index);*/
  int bits_needed = num_bits;
  unsigned char ret = 0;
  if( *index < bits_needed ){ /* not enough bits in the buffer */
    /* puts the bits at the index we need in the return */
    /* example: need 7 bits, buf = 111xxxxx (has 3 bits) shift 1 bit x111xxxx */
    *buf = *buf >> (8-bits_needed); 
    /*printf("shift:%x ", *buf);*/
    ret = ret | *buf; /* or them into ret */
    /*printf("reta:%x ", ret);*/
    read(0, buf, 1); /* Read in another byte */
    /*printf("newbuf:%x ", *buf);*/
    bits_needed = bits_needed - *index; /* now we need *index less bytes */
    *index = 8; 
    /* we'll get the rest of the bits we need next */
  }
  if ( *index >= bits_needed ){ /* enough bits in the buffer */
    /*char temp = ((unsigned int *)buf >> (8-bits_needed)); */
    /*printf("shift:%d ", 8-bits_needed);*/
    ret = ret | (*buf >> (8-bits_needed)); /* Get the bits we need */
    /*printf("retb:%x ", ret);*/
    *buf = *buf << bits_needed;
    /*printf("newbuf:%x ", *buf);*/
    *index = (*index - bits_needed);
  }
  else{
    write(2,"Something bad happened\n",23);
    return -1;
  }
  return ret;
}

int main(){
  int case_flag = 0;

  unsigned char buf = 0;
  int index = 0;

  unsigned char value;

  while(1){
    value = input(6, &buf, &index);
    /*printf("6_");*/
    if (value == 32){ /* ecma-1 null */
      /*printf("NULL, 7_");*/
      value = input(7, &buf, &index);
      /* Either an ascii character or zero or esc */
      if ( value == '0'){ /* zero - end of file */
        return 1;
      }
      else if (value == 27){ /* esc - toggle case */
        case_flag = case_flag == 1 ? 0 : 1;
      }
      else{ /* ASCII character */
        write(1, &value, 1);
        /*printf("%c, ", value);*/
      }
    }
    else{
      if (value == 0){
        value = 32;
        write(1, &value, 1);
        /*printf("SPC, ");*/
      }
      else if ( value >= 1 && value <= 7 ){
        /*printf("SPL, ");*/
        value = value + 8;
        write(1, &value, 1);
      }
      else if ( value >= 8 && value <= 61 && value != 32 ){
        /* adjust case of alpha letters if case_flag is 1 */
        if( case_flag == 1 && value <= 58 && value >= 33 ){
          value = value + 64;
        }
        else{
          value = value+32;
        }
        write(1, &value, 1);
        /*printf("%c, ", value);*/
      }
      else if (value == 62){
        value = 27;
        write(1, &value, 1);
        /*printf("ESC, ");*/
      }
      else if (value == 63){
        value = 127;
        write(1, &value, 1);
        /*printf("DEL, ");*/
      }
    }
  }
}
