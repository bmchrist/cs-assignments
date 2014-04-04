#include "func.h"
#include <stdio.h>

int main(){
  Node *root = NULL;
  while(1){
    char ccmd;
    int icmd;
    scanf("%c", &ccmd);
    if (ccmd == 'i'){ /* Insert */
      scanf("%d", &icmd);
      root = insertNode(icmd, root);
    }
    else if (ccmd == 'd'){ /* Delete */
      scanf("%d", &icmd);
      root = removeNode(icmd, root);
    }
    else if (ccmd == 's'){ /* Search */
      scanf("%d", &icmd);
      if (search(icmd, root)){
        printf("%d is present\n",icmd);
      }
      else{
        printf("%d is not present\n",icmd);
      }
    }
    else if (ccmd == 'e'){ /* Empty */
      root = empty(root);
    }
    else if (ccmd == 't'){ /* Traversal */
      scanf("%c", &ccmd); /* get the space */
      scanf("%c", &ccmd);
      if (ccmd == 'i'){ /* In order */
        iot(root);
        printf("\n");
      }
      else if (ccmd == 'l'){ /* Pre order */
        preot(root);
        printf("\n");
      }
      else if (ccmd == 'r'){ /* Post order */
        postot(root);
        printf("\n");
      }
    }
    else if (ccmd == 'q'){ /* Quit */
      return 0;
    }
    else{ /* Invalid. Ignore */
    }
  }
  return 0;
}
