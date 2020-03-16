#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "seznamStanic.h"

#define PI 3.14159265
#define  R 6372.795

static tStanice *prvni = NULL;
static tStanice *akt = NULL;
static tStanice **odebraneStanice;
static int pocOdebranych = 0;
double vzdalenost;
int id;
char stanice[50];
double km;
tGPS gps;

static double vzdalDvouBodu(tGPS gps1, tGPS gps2) {
	vzdalenost = acos(sin(gps1.latit*PI / 180)*sin(gps2.latit*PI / 180)
				 + (cos(gps1.latit*PI / 180)*cos(gps2.latit*PI / 180)
				 * cos((gps2.longit - gps1.longit)*PI / 180))) * R;
	return vzdalenost;
}

static tStanice * najdiNejblizStanici(tGPS gpsVlak) {
	tStanice *stanice = prvni;
	tStanice *nejblizsiStanice;
	int vzdalenost;
	int minumum = INT_MAX;

	while (stanice != NULL)
	{
		vzdalenost = vzdalDvouBodu(gpsVlak, stanice->gps);
		
		if (vzdalenost < minumum)
		{
			minumum = vzdalenost;
			nejblizsiStanice = stanice;
		}

		stanice = stanice->dalsi;
	}
	return nejblizsiStanice;
}

int dejPocetZaz(char *nazev) {
	FILE *file = fopen(nazev, "r");
	char buffer[250];
	int pocetZaznamu = 0;
	
	while (fgets(buffer, 250, file) != NULL)
	{
		pocetZaznamu++;
	}
	return  pocetZaznamu;
}

void nactiData(char *nazev) {
	FILE *file = fopen(nazev, "r");
	char *token;
	char buffer[250];
	tStanice *novaStanice;

	if (file == NULL)
	{
		puts("Chyba pri nacitani souboru");
		return 0;
	}

	int pocetZaznamu = dejPocetZaz(nazev);

	fgets(buffer, 250, file);		
		
		for (size_t i = 1; i < pocetZaznamu; i++)
		{
			fgets(buffer, 250, file);
			token = strtok(buffer, ";");
			id = atoi(token);
			token = strtok(NULL, ";");
			strcpy(stanice, token);
			token = strtok(NULL, ";");
			km = atof(token);
			token = strtok(NULL, ";");
			gps.longit = atof(token);
			token = strtok(NULL, ";");
			gps.latit = atof(token);
			
			novaStanice = vytvorStanici(id, stanice, km, gps);
			vlozNaPosledni(novaStanice);
		}	
	fclose(file);
	puts("Nacteni souboru probehlo uspesne.");
	printf("Pocet stanic: %d\n", pocetZaznamu);
}

void vypis() {
	tStanice *index = prvni;
	while (index != NULL) {
		vypisStanici(index);
		index = index->dalsi;
	}
}

void vlozNaPosledni(tStanice *stanice) {
	if (prvni == NULL)
	{
		prvni = stanice;
		akt = prvni;
	}
	else {
		akt->dalsi = stanice;
		akt = stanice;
	}
}

void vlozNaPrvni(tStanice *stanice) {
	if (prvni == NULL)
	{
		prvni = stanice;
		akt = prvni;
	}
	else {
		stanice->dalsi = prvni;
		prvni = stanice;
	}
}

void odeber(char *stanice) {
	if (prvni == NULL)
	{
		puts("List je prazdny.");
		return 0;
	}

	tStanice *hledanaStanice = prvni;
	tStanice *predchoziStanice = NULL;

	while (hledanaStanice != NULL)
	{
		if (strcmp(hledanaStanice->stanice, stanice) == 0)
		{
			break;
		}
		predchoziStanice = hledanaStanice;
		hledanaStanice = hledanaStanice->dalsi;
	}

	if (hledanaStanice == prvni) {
		prvni = prvni->dalsi;
	}
	else {
		predchoziStanice->dalsi = hledanaStanice->dalsi;
		hledanaStanice->dalsi = NULL;
	}

	pocOdebranych++;
	poleOdebranych(hledanaStanice, pocOdebranych);
	puts("Prvek byl smazan.");
}

int poleOdebranych(tStanice *stanice, int pocet) {
	int index = pocet - 1;
	odebraneStanice = realloc(odebraneStanice,pocet * sizeof(tStanice*));
	odebraneStanice[pocet] = malloc(2 * sizeof(tStanice*));
	odebraneStanice[index] = stanice;
}

void vypisOdebrane() {	
	for (int i = 0; i < pocOdebranych; i++)
	{
		vypisStanici(odebraneStanice[i]);
	}
}

void zadejStanici() {
	tStanice *nStanice;
	printf("Zadejte ID stanice: ");
	scanf("%d", &id);
	printf("Zadejte nazev stanice: ");
	scanf("%s", stanice);
	printf("Zadejte vzdalenost stanice v km: ");
	scanf("%f", &km);
	printf("Zadejte GPS latitude stanice: ");
	scanf("%f", &gps.latit);
	printf("Zadejte GPS longitude stanice: ");
	scanf("%f", &gps.longit);

	nStanice = vytvorStanici(id, stanice, km, gps);
	return nStanice;
}

void trasovaniVlaku(char *nazev) {
	if (prvni == NULL)
	{
		puts("Nejprve musite nacist data.");
		return 0;
	}

	FILE *file = fopen(nazev, "r");
	char *token;
	char buffer[250];
	int rychlost;
	tStanice *stanice;

	if (file == NULL)
	{
		puts("Chyba pri nacitani souboru");
		return 0;
	}

	int pocetZaznamu = dejPocetZaz(nazev);

	fgets(buffer, 250, file);

	for (size_t i = 1; i < pocetZaznamu; i++)
	{
		fgets(buffer, 250, file);
		token = strtok(buffer, ";");
		id = atoi(token);
		token = strtok(NULL, ";");
		gps.latit = atof(token);
		token = strtok(NULL, ";");
		gps.longit = atof(token);
		token = strtok(NULL, ";");
		rychlost = atoi(token);
		
		stanice = najdiNejblizStanici(gps);
		printf("Vlak c. %d - GPS: N%f E%f - Rychlost: %d km/h\n", id, gps.latit, gps.longit, rychlost);
		printf("Nezjblizsi stanice: %s - Vzdalenost: %f km\n", stanice->stanice, vzdalDvouBodu(gps, stanice->gps));
		printf("---------------------------------------------------------------\n");
	}
	fclose(file);
}

void zrusData() {
	if (prvni == NULL)
	{
		return 0;
	}

	for (size_t i = 0; i < pocOdebranych; i++)
	{
		free(odebraneStanice[i]);
	}

	tStanice *stanice = prvni;
	tStanice *dalsiStanice = prvni->dalsi;
	
	while (dalsiStanice != NULL)
	{
		free(stanice);
		stanice = dalsiStanice;
		dalsiStanice = stanice->dalsi;
	}
	free(akt);
}