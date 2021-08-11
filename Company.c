#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Company.h"
#include "Airport.h"
#include "General.h"
#include "fileHelper.h"

static const char* sortOptStr[eNofSortOpt] = {
	"None","Hour", "Date", "Airport takeoff code", "Airport landing code" };


int	initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName)
{
	L_init(&pComp->flighDateList);
	if (loadCompanyFromFile(pComp, pManaer, fileName))
	{
		initDateList(pComp);
		return 1;
	}
	return 0;
}

void	initCompany(Company* pComp,AirportManager* pManaer)
{
	printf("-----------  Init Airline Company\n");
	L_init(&pComp->flighDateList);
	
	pComp->name = getStrExactName("Enter company name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
}

void	initDateList(Company* pComp)
{	
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if(isUniqueDate(pComp,i))
		{
			char* sDate = createDateString(&pComp->flightArr[i]->date);
			L_insert(&(pComp->flighDateList.head), sDate);
		}
	}
}

int		isUniqueDate(const Company* pComp, int index)
{
	Date* pCheck = &pComp->flightArr[index]->date;
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (i == index)
			continue;
		if (equalDate(&pComp->flightArr[i]->date,pCheck))
			return 0;
	}
	return 1;
}

int		addFlight(Company* pComp, const AirportManager* pManager)
{

	if (pManager->count < 2)
	{
		printf("There are not enoght airport to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	CHECK_RETURN_0(pComp->flightArr);

	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1, sizeof(Flight));
	CHECK_RETURN_0(pComp->flightArr[pComp->flightCount]);

	initFlight(pComp->flightArr[pComp->flightCount], pManager);
	if (isUniqueDate(pComp, pComp->flightCount))
	{
		char* sDate = createDateString(&pComp->flightArr[pComp->flightCount]->date);
		L_insert(&(pComp->flighDateList.head), sDate);
	}
	pComp->flightCount++;
	return 1;
}

void	printCompany(const Company* pComp)
{
	printVarCompanyName(pComp->name, "Hachi", "Babit", "Ba", "Olam", NULL);
	printf("Has %d flights\n", pComp->flightCount);
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), printFlightV);
	printf("\nFlight Date List:");
	L_print(&pComp->flighDateList, printStr);
}
void	printVarCompanyName(char* compName, ...)
{
	va_list list;
	char* temp;
	va_start(list, compName);
	temp = compName;
	while (temp) {
		printf("%s", temp);
		temp = va_arg(list, char*);
		if (temp)
			printf("_");
	}
	printf("\n");
}

void	printFlightsCount(const Company* pComp)
{
	char codeOrigin[CODE_LENGTH + 1];
	char codeDestination[CODE_LENGTH + 1];

	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}

	printf("Origin Airport\n");
	getAirportCode(codeOrigin);
	printf("Destination Airport\n");
	getAirportCode(codeDestination);

	int count = countFlightsInRoute(pComp->flightArr, pComp->flightCount, codeOrigin, codeDestination);
	if (count != 0)
		printf("There are %d flights ", count);
	else
		printf("There are No flights ");

	printf("from %s to %s\n", codeOrigin, codeDestination);
}

int		saveCompanyToFile(const Company* pComp, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if(!fp)	{ //if(!CHECK_MSG_RETURN_0(fp,"Error open company file")){
		printf("Error open copmpany file to write\n");
		return 0;
	}
	
	if (!writeStringToFile(pComp->name, fp, "Error write comapny name\n"))
		return 0;

	if (!writeIntToFile(pComp->flightCount, fp, "Error write flight count\n"))
		return 0;

	if (!writeIntToFile((int)pComp->sortOpt, fp, "Error write sort option\n"))
		return 0;

	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (!saveFlightToFile(pComp->flightArr[i], fp))
			return 0;
	}

	fclose(fp);
	return 1;
}

int loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp, "Error open company file\n");


	pComp->flightArr = NULL;


	pComp->name = readStringFromFile(fp, "Error reading company name\n");
	CHECK_RETURN_0(pComp->name);


	if (!readIntFromFile(&pComp->flightCount, fp, "Error reading flight count name\n"))
		return 0;

	int opt;
	if (!readIntFromFile(&opt, fp,"Error reading sort option\n"))
		return 0;

	pComp->sortOpt = (eSortOption)opt;

	if (pComp->flightCount > 0)
	{
		pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
		CHECK_NULL__MSG_CLOSE_FILE(pComp->flightArr, "Alocation error", fp);

	}
	else
		pComp->flightArr = NULL;

	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
		CHECK_NULL__MSG_CLOSE_FILE(pComp->flightArr[i], "Alocation error", fp);

		if (!loadFlightFromFile(pComp->flightArr[i], pManager, fp))
			return 0;
	}

	fclose(fp);
	return 1;
}

void	sortFlight(Company* pComp)
{
	pComp->sortOpt = showSortMenu();
	int(*compare)(const void* air1, const void* air2) = NULL;

	switch (pComp->sortOpt)
	{
	case eHour:
		compare = compareByHour;
		break;
	case eDate:
		compare = compareByDate;
		break;
	case eSorceCode:
		compare = compareByCodeOrig;
		break;
	case eDestCode:
		compare = compareByCodeDest;
		break;
	
	}

	if (compare != NULL)
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);

}

void	findFlight(const Company* pComp)
{
	int(*compare)(const void* air1, const void* air2) = NULL;
	Flight f = { 0 };
	Flight* pFlight = &f;


	switch (pComp->sortOpt)
	{
	case eHour:
		f.hour = getFlightHour();
		compare = compareByHour;
		break;
	case eDate:
		getchar();
		getCorrectDate(&f.date);
		compare = compareByDate;
		break;
	case eSorceCode:
		getchar();
		getAirportCode(f.originCode);
		compare = compareByCodeOrig;
		break;
	case eDestCode:
		getchar();
		getAirportCode(f.destCode);
		compare = compareByCodeDest;
		break;
	}

	if (compare != NULL)
	{
		Flight** pF = bsearch(&pFlight, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
		if (!CHECK_RETURN_0(pF))
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(*pF);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}

}

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >eNofSortOpt);

	return (eSortOption)opt;
}

void printNameAndNumOfFlightsOnly(Company* pComp) {
	printf("%s\t%d", pComp->name, pComp->flightCount);
}

void	freeCompany(Company* pComp)
{
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), freeFlight);
	free(pComp->flightArr);
	free(pComp->name);
	L_free(&pComp->flighDateList, freePtr);
}


int saveCompressedCompany(const Company* pComp, const char* fileName) {
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!CHECK_RETURN_0(fp))
		return 0;

	BYTE pBuffer[BIN_AIRLINE_BYTE_SIZE];

	pBuffer[0] = (pComp->flightCount & 0x01) | (pComp->sortOpt) << 1 | strlen(pComp->name) << 4;
	pBuffer[1] = (pComp->flightCount&0x1FE) >> 1;
	if (!writeToBinFile(fp, pBuffer, BIN_AIRLINE_BYTE_SIZE))
		return 0;
	if (!writeToBinFile(fp, pComp->name, (strlen(pComp->name)+1)))
		return 0;
	for (int i = 0;i < pComp->flightCount;i++)
		if (!saveCompressedFlight(pComp->flightArr[i], fp))
			return 0;

	fclose(fp);
	return 1;
}

int loadCompressedCompany( Company* pComp, AirportManager* pManager, const char* fileName) {
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!CHECK_RETURN_0(fp))
		return 0;

	BYTE* pBuffer=(BYTE*)malloc(BIN_AIRLINE_BYTE_SIZE*sizeof(BYTE));
	if(!readFromBinFile(fp, pBuffer, BIN_AIRLINE_BYTE_SIZE))
		return 0;
	pComp->flightCount = pBuffer[1] << 1 | (pBuffer[0] & 0x01);
	pComp->sortOpt = (pBuffer[0] & 0xE) >> 1;
	int nameLen = ((pBuffer[0] & 0xF0) >> 4)+1;

	pComp->name = (char*)malloc(nameLen * sizeof(char));
	if (fread(pComp->name, sizeof(char), nameLen, fp) != nameLen)
	{
		fclose(fp);
		return 0;
	}
	pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
	for (int i = 0;i < pComp->flightCount;i++) {
		BYTE* pFlightBuffer = (BYTE*)malloc(BIN_FLIGHT_BYTE_SIZE * sizeof(BYTE));
		if (!readFromBinFile(fp, pFlightBuffer, BIN_FLIGHT_BYTE_SIZE)) {
			free(pFlightBuffer);
			return 0;
		}
		pComp->flightArr[i] = (Flight*)malloc(sizeof(Flight));
		if (!unCompressedFlight(pFlightBuffer, pComp->flightArr[i], pManager))
		{
			free(pFlightBuffer);
			fclose(fp);
			return 0;
		}
		free(pFlightBuffer);
	}
	//initiating the date-list.
	L_init(&pComp->flighDateList);
	initDateList(pComp);
	free(pBuffer);
	fclose(fp);
	return 1;



}
