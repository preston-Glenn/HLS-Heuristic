//========================================================================================
// 
// File Name    : decfilt.c
// Description  : Main 5 stages decimation filter functionality
// Release Date : 29/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra 
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Author   Version     Description
//----------------------------------------------------------------------------------------
//29/07/2013   PolyU        1.0     Decimation filter 
//=======================================================================================
#include "attrs.h"


#define TAPS_STAGE1 4
#define TAPS_STAGE2 4
#define TAPS_STAGE3 4
#define TAPS_STAGE4 6
#define TAPS_STAGE5 12
#define STAGES 5

#define BUFFER_SIZE_STAGE1 7
#define BUFFER_SIZE_STAGE2 7
#define BUFFER_SIZE_STAGE3 7
#define BUFFER_SIZE_STAGE4 11
#define BUFFER_SIZE_STAGE5 24

// inputs

in ter(0..0) load_coeff;    
in ter(18..0) indata;

in ter(16..0) incoef1[TAPS_STAGE1];
in ter(16..0) incoef2[TAPS_STAGE2];
in ter(16..0) incoef3[TAPS_STAGE3];
in ter(16..0) incoef4[TAPS_STAGE4];
in ter(16..0) incoef5[TAPS_STAGE5];

 // Output   
out ter(18..0)  odata;



process decimation(){

  var(0..0) load_coeff_in;


  // signals for STAGE 1
  var(18..0) in_stage_1;
  var(18..0) buffer_1;
  var(35..0) SoP1;
  var(18..0) stage_1;
  var (0..0) stage_control_1 = 0;

  // signals for STAGE 2
  var(18..0) in_stage_2;
  var(18..0) buffer_2;
  var(35..0) SoP2;
  var(18..0) stage_2;
  var(0..0) stage_control_2 = 0;

  // signals for STAGE 3
  var(18..0) in_stage_3;
  var(18..0) buffer_3;
  var(35..0) SoP3;
  var(18..0) stage_3;
  var(0..0) stage_control_3 = 0;

  // signals for STAGE 4
  var(18..0) in_stage_4;
  var(18..0) buffer_4;
  var(35..0) SoP4;
  var(18..0) stage_4;
  var(0..0) stage_control_4 = 0;

  // signals for STAGE 5
  var(18..0) in_stage_5;
  var(35..0) SoP5;
  var(18..0) stage_5;
  var(0..0) stage_control_5 = 0;

 unsigned int x=0;

 var(16..0) incoef1_var[TAPS_STAGE1] ;
 var(16..0) incoef2_var[TAPS_STAGE2] ;
 var(16..0) incoef3_var[TAPS_STAGE3];
 var(16..0) incoef4_var[TAPS_STAGE4] ;
 var(16..0) incoef5_var[TAPS_STAGE5];

 int stage_counter_1=0;   // decimation stages counter. HLS toold should 
 int stage_counter_2=0;   // adust bitwidth automatically
 int stage_counter_3=0;
 int stage_counter_4=0;
 int stage_counter_5=0;
   
 var(18..0) bufferline_1[7];
 var(18..0) bufferline_2[7];
 var(18..0) bufferline_3[7];
 var(18..0) bufferline_4[11];
 var(18..0) bufferline_5[24];
 
 // Main stage filter body
 load_coeff_in = load_coeff;


   if(load_coeff_in){

     // Read coefficients into coef arrays
     for (x = 0; x < TAPS_STAGE1; x++) {
       incoef1_var[x] = incoef1[x];

     }
     for (x = 0; x < TAPS_STAGE2; x++) {
       incoef2_var[x] = incoef2[x];

     }
     for (x = 0; x < TAPS_STAGE3; x++) {
       incoef3_var[x] = incoef3[x];

     }
     for (x = 0; x < TAPS_STAGE4; x++) {
       incoef4_var[x] = incoef4[x];

     }
     for (x = 0; x < TAPS_STAGE5; x++) {
       incoef5_var[x] = incoef5[x];

     }
   }

  // Read filter in_stage data 
   in_stage_1 = indata;

  //================================
  // Stage 1
  //================================
      
  // Shift data and store new data
   /* ATTR1 */
  for( x = BUFFER_SIZE_STAGE1-1; x > 0; x--) {
   bufferline_1[x] = bufferline_1[x-1];
  }
  bufferline_1[0] = in_stage_1;

  SoP1 = 0;
  stage_control_1 = 0;

  /* ATTR2 */
  for (x = 0; x < TAPS_STAGE1; x++) {
    if( x == TAPS_STAGE1-1 )
      buffer_1 = 0;
    else 
      buffer_1 = bufferline_1[ BUFFER_SIZE_STAGE1-1 - x];
  

   SoP1 = SoP1 + (bufferline_1[x]+buffer_1 )*incoef1_var[x];
  }
  
  stage_1 = SoP1;

  
  if(stage_counter_1 == 2) {
    stage_counter_1 = 0;
    stage_control_1 = 1;
  }
  else {
    stage_counter_1++;
  }

  //================================
  // Stage 2
  //================================
  if (stage_control_1)
    in_stage_2 = stage_1;

  /* ATTR3 */
  for(x =  BUFFER_SIZE_STAGE2-1; x > 0; x--) {
    bufferline_2[x] = bufferline_2[x-1];
  }
  bufferline_2[0] = in_stage_2;

  SoP2 = 0;
  stage_control_2 = 0;


  if(stage_control_1){

    /* ATTR4 */
    for (x = 0; x < TAPS_STAGE2; x++) {
      if( x == TAPS_STAGE2-1 )
	buffer_2 = 0;
      else
	buffer_2 = bufferline_2[ BUFFER_SIZE_STAGE2-1 - x];
      
      SoP2 = SoP2 + (bufferline_2[x]+buffer_2 )*incoef2_var[x];
    }


  stage_2 = SoP2;

  if(stage_counter_2 == 2){
    stage_counter_2 = 0;
    stage_control_2 = 1;
  }
  else {
   stage_counter_2++;
  }
}
  
  //================================
  // Stage 3
  //================================
  if (stage_control_2)
      in_stage_3 = stage_2;

  /* ATTR5 */
  for(x = BUFFER_SIZE_STAGE3-1; x > 0; x--) {
    bufferline_3[x] = bufferline_3[x-1];
  }
  bufferline_3[0] = in_stage_3;

  SoP3 = 0;
  stage_control_3 = 0;

  if(stage_control_2){

    /* ATTR6 */
    for (x = 0; x < TAPS_STAGE3; x++) {
      if( x == TAPS_STAGE3-1 )
	buffer_3 = 0;
      else 
	buffer_3 = bufferline_3[BUFFER_SIZE_STAGE3-1 - x];
      SoP3 = SoP3 + (bufferline_3[x]+buffer_3)*incoef3_var[x];
    }
  


  stage_3 = SoP3;

  if(stage_counter_3 == 1) { 
   	stage_counter_3 = 0;
   	stage_control_3 = 1;
  } else {
   	stage_counter_3++;
  }
}

  //================================
  // Stage 4
  //================================
  if (stage_control_3)
   in_stage_4 = stage_3;
 
  /* ATTR7 */
  for(x = BUFFER_SIZE_STAGE4-1; x > 0; x--) {
   bufferline_4[x] = bufferline_4[x-1];
  }
  bufferline_4[0] = in_stage_4;

  SoP4 = 0;
  stage_control_4= 0;

if(stage_control_3){

  /* ATTR8 */
  for (x = 0; x < TAPS_STAGE4; x++) {
   if( x == TAPS_STAGE4-1 )
     buffer_4 = 0;
   else
     buffer_4 = bufferline_4[BUFFER_SIZE_STAGE4-1 - x];

   SoP4 = SoP4 + (bufferline_4[x]+buffer_4)*incoef4_var[x];
  }
  
  stage_4 = SoP4;
 
  if(stage_counter_4 == 1) {
    stage_counter_4 = 0;
    stage_control_4 = 1;
  }
  else {
   stage_counter_4++;
  }
}
 
  //================================
  // Stage 5
  //================================
  
  if (stage_control_4)
   in_stage_5 = stage_4;
 
  /* ATTR9 */
  for(x = BUFFER_SIZE_STAGE5-1; x > 0; x--) {
   bufferline_5[x] = bufferline_5[x-1];
  }
  bufferline_5[0] = in_stage_5;

  SoP5 = 0;
  stage_control_5 = 0;


if(stage_control_4){
  /* ATTR10 */
  for (x = 0; x < TAPS_STAGE5; x++) {
    SoP5 = SoP5 + (bufferline_5[x]+bufferline_5[ BUFFER_SIZE_STAGE5-1 - x])*incoef5_var[x];
  }


  stage_5 = SoP5;

  if(stage_counter_5 == 1) {
    stage_counter_5 = 0;
    stage_control_5 = 1;
  }
  else {
    stage_counter_5++;
  }
}


// Write decimation filter output
 if(stage_control_5 == 1){
   odata = stage_5;
   stage_control_5 = 0;
 }	 


}
