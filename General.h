#ifndef __GENERAL__
#define __GENERAL__

typedef unsigned char BYTE;
#define MAX_STR_LEN 255
#define BIN_AIRLINE_BYTE_SIZE 2
#define BIN_FLIGHT_BYTE_SIZE 10
#include "myMacros.h"

char*	getStrExactName(const char* msg);
char*	myGets(char* buffer, int size, FILE* fp);
char*	getDynStr(char* str);
char**	splitCharsToWords(char* str,int* pCount,int* pTotalLength);
void	printStr(const void* str);
void	freePtr(void* str);
void	generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element));
int		writeToBinFile(const FILE* fp, void* pBuffer, int size);
int		readFromBinFile(const FILE* fp, void* pBuffer, int size);



#endif

