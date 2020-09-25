#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "fixed_math.h"
#include "fftfinal.h"
#include "int_dec_math.h"
#include <math.h>


int main(void)
{
  FILE * fp;
  double buff[BUFFSIZE] = {0}; // stores float data from binary.
  FRACT16 Fract_Real_Input[BUFFSIZE] = {0}; // input into fft function.
  FRACT16 Fract_Complex_Input[BUFFSIZE] = {0}; // input into fft function.
  FRACT16 FRACT16_input_array[BUFFSIZE]= {0};

  fp = fopen("sigbin.bin", "rb");
  if(fp == 0) {
    perror("fopen");
    exit(1);
}
  while (fread(buff, sizeof(double), BUFFSIZE, fp) > 0){
    for (int i = 0; i < BUFFSIZE; i++){

      Fract_Real_Input[i] = (float_to_fract16(buff[i]));
    }
    fix_fft(Fract_Real_Input,Fract_Complex_Input,9);
  }
  fclose(fp);




// below all works verifed multi and add tested.
  long place, root;
        for (int k=0; k < BUFFSIZE/2; k++)
        {
	        Fract_Real_Input[k] = (fract16_add(fract16_multr(Fract_Real_Input[k],Fract_Real_Input[k]),
                   fract16_multr(Fract_Complex_Input[k],Fract_Complex_Input[k])));



			root = 0;

			if(Fract_Real_Input[k] >= 0) // Ensure we don't have a negative number
			{
        Fract_Real_Input[k]=float_to_fract16(sqrt(fract16_to_float(Fract_Real_Input[k])));
      }else{
        Fract_Real_Input[k] = root;
      }

      }




  FILE * fp2;
  fp2 = fopen("output.txt", "w");
  for(int i =0;i<BUFFSIZE/2;i=i+1){
    //fprintf(fp2,"%d \n",0);
    //fprintf(fp2,"%f %i \n",from_fixed32(fixed32_output_array[i]),i);
    //fprintf(fp2,"%i \n",(fixed32_output_array[i]));
    //fprintf(fp2,"%"PRIi16,float_to_fract16(sin((PI/512)*i)));
    fprintf(fp2,"%i \n",Fract_Real_Input[i]);
  }

  fclose(fp2);
  return 0;
}

