#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "stanice.h"

tStanice *vytvorStanici(int id, char *stanice, double km, tGPS gps) {
	tStanice *sta = calloc(1, sizeof(tStanice));
	sta->id = id;
	strcpy(sta->stanice, stanice);
	sta->km = km;
	sta->gps = gps;
	return sta;
}

void vypisStanici(tStanice *sta) {
	printf("%s\n", sta->stanice);
}
