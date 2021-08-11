#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2020
#include "General.h"

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
void	printDate(const Date* pDate);
char*   createDateString(const Date* pDate);
void	compressedDate(BYTE* pBuffer, Date* pDate);
void	unCompressedDate(BYTE* pBuffer, Date* pDate);

int		 equalDate(const Date* pDate1, const Date* pDate2);
#endif
