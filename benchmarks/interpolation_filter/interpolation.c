
//========================================================================================
// 
// File Name    : filter_interp.cpp
// Description  : Main interpolation filter function
// Release Date : 23/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra 
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version   Author      Description
//----------------------------------------------------------------------------------------
//23/07/2013     1.0      PolyU      Interpolation filter description
//=======================================================================================

#define TAPS 8
#include "attrs.h"

// Inputs
in ter(15..0) indata;
in ter(8..0) infactor;

// Outputs
out ter(0..0) odata_en;
out ter(15..0) odata;


process interpolation() {

   // Variables
  var(15..0) buffer[TAPS] ;

  var(15..0) indata_read;
  var(8..0) infactor_read;
  var(15..0) odata_write;

  var(35..0) SoP1;
  var(35..0) SoP2;
  var(35..0) SoP3;
  var(35..0) SoP4;

  unsigned int n;

  var(16..0) coeff1[TAPS] = {                          2561103,
						       -3745602,
						       261339038,
						       236616189,
						       -17537187,
						       36621093,
						       407050085,
						       -6378187};

  var(16..0) coeff2[TAPS] = {                          -1222071,
						       -99252628,
						        2832641,
						       -3875641,
						        109252,
						        111770,
						       -109252,
						       283261};

  var(16..0) coeff3[TAPS] = {                         -2543591,
						       12369433,
						       -876655,
						       -89445,
						       4443443,
						       -1721191,
						       116943,
						       222743};

 var(16..0) coeff4[TAPS] = {-224324531,
						        56155029,
						       -56234125,
						        34291255,
						       -78155092,
						        34341245,
						        45560314,
						        1455631};
   odata_en = 0;
   odata = 0;


      // Read inputs
      indata_read = indata;
      infactor_read = infactor;
      

      odata_en = 0;

    // Read inputs by shifting previous data
    /* ATTR1 */
    for ( n = 5; n>0; n--)
         buffer[n] = buffer[n-1];
      buffer[0] = indata_read;
  
   
      // FIR 1 : Sum of Products of 1st filter
      SoP1 = 0;
      /* ATTR2 */
      for (n = 0; n < TAPS; n++ ) 
	SoP1 = SoP1 + buffer[n] * coeff1[n];
      
      // FIR 2 : Sum of Products of 2nd filter
      SoP2 = 0;
      /* ATTR3 */
      for (n = 0; n < TAPS; n++ ) 
	SoP2 =SoP2 + buffer[n] * coeff2[n];

      // FIR 3 : Sum of Products of 3rd filter
      SoP3 = 0;
      /* ATTR4 */
      for ( n = 0; n < TAPS; n++ ) 
	SoP3 = SoP3 + buffer[n] * coeff3[n];

      // FIR 4 : Sum of Products of 4th filter
      SoP4 = 0;
      /* ATTR5 */
      for (n = 0; n < TAPS; n++ ) 
	SoP4 = SoP4 + buffer[n] * coeff4[n];


   // Output computation 
   odata_write = SoP1 +
               SoP2 * infactor_read +
               SoP3 * infactor_read*infactor_read + 
               SoP4 * infactor_read*infactor*infactor_read;

   // Manual polynomial decomposition (reduces the number of arithmetic operations -> leads to smaller design)
//	tmp1 = SoP4*infactor_read + SoP3;
//	tmp2 = tmp1 * infactor_read + SoP2;
//    	odata_write = tmp2 * infactor_read + SoP1;


     // Write results back
      odata_en = 1;
      odata =  odata_write;
      

}

