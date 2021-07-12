//*****************************************************************
// Some functions to work with the strings, arrays and calculations
// 
// 
// 
//*****************************************************************
#include "stm32f10x.h"
#include "string_convertor.h"
	
// Merge two bytes into uint16_t
// E.g. input char 'A'+'B' = output uint16_t 'AB'
uint16_t MergeTwoBytes_into_Uint16(unsigned char *Highbyte, unsigned char *Lowbyte){
	uint16_t retValue =0;
	retValue |= (uint16_t)*Lowbyte;
	retValue |= ((uint16_t)*Highbyte << 8);
	return retValue;
}

// Convert numbers in a uint16_t into a string[5]
// E.g. input uint16_t 'ABCDE' = output unsigned char[5] {E,D,C,B,A}
void Convert_Uint16_into_String5(uint16_t input16t, uint8_t *String5){
	int i;
	uint16_t div;
	for (i =0; i < 5; i++){
		div =	input16t % 10;
		String5[4-i]= div + '0';
		input16t = input16t - div;
		input16t	= input16t/10;
	}
}

// Convert numbers in a uint8_t into a string[3]
// E.g. input char 'ABC' = output unsigned char[3] {C,B,A} 
void Convert_Char_into_String3(unsigned char char255, unsigned char *String3) {
	int i;
	uint16_t div;
	for (i =0; i < 3; i++){
		div =	char255 % 10;
		String3[i]= div + '0'; 
		char255 = char255 - div;
		char255	= char255/10;
	}
}

// Translate an uint16_t into uint8_t in proportion to a range
// E.g. 0 into 0, 1200 into 255, 600 into 127 
uint8_t Translate_Uint16t_wRatio_Uint8t(uint16_t integer16, uint16_t rangeMin, uint16_t rangeMax) {
	// check if a value goes beyond the range
	if (integer16<rangeMin){ return 0;};
	if (integer16>rangeMax){ return 255;};
	uint8_t out_val;
	// create a ratio	
	uint16_t div; 
	div = ((integer16-rangeMin)*(RANGE_RGB_MAX - RANGE_RGB_MIN)) % (rangeMax - rangeMin); 
	// Division result must be integer. Substract remainder of division	
  out_val = ((integer16-rangeMin)*(RANGE_RGB_MAX - RANGE_RGB_MIN) - div ) / (rangeMax - rangeMin) +  RANGE_RGB_MIN; 
  return  out_val;       
}
// E.g. Average value out from 3-bytes sum 3
uint8_t GetAverageValueFrom_3uint8(unsigned char i1, unsigned char i2,unsigned char i3){
	uint8_t rVal;
	rVal=(i1+i2+i3)/3;
	return rVal;
}
		
// Translate a single hex (0-F) into an ascii symbol 
uint8_t halfbyte_to_hexascii(uint8_t _halfbyte){
	uint8_t _ascii;
	_halfbyte &= 0x0F;
	if(_halfbyte >= 10){
		_ascii=('A' + _halfbyte -10);
	} 
	else{ 
		_ascii=('0' + _halfbyte); 
	}
	return _ascii;	
}


// Translate an ascii symbol into a hex (0-F)
uint8_t hexascii_to_halfbyte(uint8_t _ascii){
	uint8_t _halfbyte;
	if((_ascii >= '0') && (_ascii <= '9')){
	_halfbyte=(_ascii - '0');}
	else if((_ascii >= 'a') && (_ascii <= 'f')){
//	_halfbyte=(_ascii - 'a');}
	_halfbyte=(_ascii - 87);}
	else if((_ascii >= 'A') && (_ascii <= 'F')){
//	_halfbyte=(_ascii - 'A');}
	_halfbyte=(_ascii - 55);}
	else{
		return(0xFF);
	}	
	return _halfbyte;
}











/**
* brief  Read a string (pstring) from position (uint32_t pos) and look for a word (uint8_t *word) sized (uint8_t WordSize)
* param  
* param  
* retval Match found/not found
*/



uint32_t CompareStringtoaWord(uint8_t *pstring, uint32_t pos, uint8_t *word, uint8_t WordSize) {
	uint32_t i=pos;
	uint32_t w=0;
	for(i=pos; i<(WordSize+pos); i++){
		if(pstring[i]!=word[w]) return 1;
			w++;
	}	
	return 0;
}


/**
* brief  Read a string (pstring) sized (uint8_t StringSize) and look for a word (uint8_t *word) sized (uint8_t WordSize)
* param  
* param  
* retval location in a string, if found. PARSE_STRING_NO_RESULTS_FOUND - if no match found
*/

uint32_t find_first_Word_in_a_String(uint8_t *pstring, uint8_t *word, uint8_t StringSize, uint8_t WordSize) {
	uint32_t i=0;
	uint32_t parseResult;
	for(i=0; i<StringSize; i++){
		if(pstring[i]==word[0]){  
			parseResult=CompareStringtoaWord(pstring,i,word,WordSize);
			if(parseResult==0) return i+1;
		}		
	}
	return PARSE_STRING_NO_RESULTS_FOUND;
}




