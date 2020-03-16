#pragma once
#include "gps.h"

typedef struct stanice {
	int id;
	char stanice[50];
	double km;
	tGPS gps;
	struct stanice *dalsi;
}tStanice;

tStanice *vytvorStanici(int id, char *stanice, double km, tGPS gps);

void vypisStanici(tStanice *sta); 
