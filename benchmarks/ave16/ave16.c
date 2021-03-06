/*****************************************************************
 *
 *
 * [Abstract]: Average of last 8 values
 * 
 * [Author]
 * October/10/2018 Original

 * 
 * [Function Detail]:

 * [Compile Option]:
 *   BDL:  For Cyber. (bdlpars -DBDL foo.c)
 *     C:  For C simulation with ANSI C compiler. (gcc -DC foo.c)
 * [Notice]:
 ****************************************************************/
#include <bdlstd.h>
#define SIZE 16
#include "attrs.h"

in ter(0:8)  in0;
out ter(0:8)  out0;
int data[SIZE] /* ATTR1 */= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


process ave16(){

   /* Variables declaration */
    int  out0_v, sum,  i; 

	/* ATTR2 */
        for (i = SIZE-1; i > 0; i--) { 	
            data[i] = data[i- 1];
        }
	data[0] = in0;
     sum= data[0]; 	
        
      /* ATTR3 */
      for (i= 1; i< SIZE; i++) { 	
            sum += data[i]; 	
        }
        out0_v= sum / 16; 
        out0 = out0_v;	
	
}

/*------------------------------------------------------  
 * ANSI-C test bench 
 *------------------------------------------------------  */
#ifdef C
int main(){

    FILE *fp_i, *fp_o;
    int i;
    

    if((fp_i = fopen("indata.txt", "r")) == NULL){
	printf(" \"indata.txt \" could not be opened.\n");
	exit(1);
    }
    if((fp_o = fopen("outdata.txt", "w")) == NULL){
	printf(" \"outdata.txt \" could not be opened.\n");
	exit(1);
    }

    for (;;){

    	if (fscanf(fp_i, "%d", &in0) == EOF) break	;
	ave8();
	fprintf(fp_o, "%d\n", out0);
    }

    fclose(fp_i);
    fclose(fp_o);
}
#endif
