#define _CRT_SECURE_NO_WARNINGS

#include "datetime.h"
#include <string.h>

// pøevádí vstup (2018-05-01 01:00:00) na strukturu, využívá dejDenVTydnu
tDateTime dejDateTime(char* datetime) {
	tDateTime dateTime;

	dateTime.year = atoi(strtok(datetime, "-"));
	dateTime.month = atoi(strtok(NULL, "-"));
	dateTime.day= atoi(strtok(NULL, " "));
	dateTime.hour = atoi(strtok(NULL, ":"));
	dateTime.min = atoi(strtok(NULL, ":"));
	dateTime.sec = atoi(strtok(NULL, ""));
	dateTime.dayInMonth = dejDenVTydnu(dateTime.year, dateTime.month, dateTime.day);

	return dateTime;
}

//vrací den v týdnu (0-PO,…,6-NE)
int dejDenVTydnu(int r, int m, int d) {
	int hodnoty[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	int denVTydnu[] = { 6, 0, 1, 2, 3, 4, 5 };
	int den;

	if (m < 3) { r -= 1; };

	den = ((r + r / 4 - r / 100 + r / 400 + hodnoty[m - 1] + d) % 7);

	return denVTydnu[den];
}