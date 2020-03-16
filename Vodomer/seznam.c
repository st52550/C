#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "seznam.h"

int* seznamSenzoru = NULL;
unsigned int velikostPole;
tSeznam* seznam;
int radky = 7;
int sloupce = 24;
float** polePrumeru;

// vrací pole id všech senzorù v souboru
int* dejSeznamSenzoru() {
	FILE* file = fopen("data.csv", "r");
	int citac = 0;
	char* buffer[100];
	char* token;
	int id;
	int* novaPamet;
	int novaVelikostPole;
	bool duplicita;

	if (file != NULL) {
		fgets(buffer, 100, file);
		
		while (fgets(buffer, 100, file) != NULL) {
			token = strtok(buffer, ";");
			id = atoi(strtok(NULL, ";"));
			duplicita = false;

			if (citac != 0) {
				for (size_t i = 0; i < citac; i++) {
					if (seznamSenzoru[i] == id) {
						duplicita = true;
						break;
					}
				}

				if (duplicita == false) {
					int novaVelikostPole = citac + 1;					
					novaPamet = realloc(seznamSenzoru, sizeof(int) * novaVelikostPole);

					if (novaPamet != NULL) {
						seznamSenzoru = novaPamet;
						seznamSenzoru[citac] = id;
						citac++;
					} else {
						printf("Nedostatek pameti pro realokaci pole senzoru");
					}
				}
			} else {
				seznamSenzoru = calloc(1, sizeof(int));

				if (seznamSenzoru != NULL) {
					seznamSenzoru[citac] = id;
					citac++;
				} else {
					printf("Nedostatek pameti pro alokaci pole senzoru");
				}
			}
		}

		fclose(file);
	}
	else {
		printf("Soubor se nepodarilo otevrit.");
	}

	velikostPole = citac;
	return seznamSenzoru;
}

// vypíše id všech senzorù
void vypisSeznamSenzoru() {
	printf("Senzory: ");

	for (size_t i = 0; i < velikostPole; i++) {
		if (i != (velikostPole - 1))
		{
			printf("%d, ", seznamSenzoru[i]);
		}
		else {
			printf("%d", seznamSenzoru[i]);
		}
	}

	printf("\n");
}

// oveøí zda se èíslo senzorù nachází v seznamu senzorù
bool overCisloSenzoru(int cisloSenzoru) {
	for (size_t i = 0; i < velikostPole; i++) {
		if (seznamSenzoru[i] == cisloSenzoru) {
			return true;
		}
	}

	printf("Zadane cislo nebylo v seznamu senzoru nalezeno.\n");
	printf("Opakujte akci.\n");
	return false;
}

// naète všechna mìøení daného senzoru, využívá vytvorSenzor
tSeznam* nactiMereni(int id) {
	FILE* file = fopen("data.csv", "r");
	char* buffer[100];
	int idSenzoru;
	char* cas;
	float m3;
	tDateTime structDatum = { 0, 0, 0, 0, 0, 0, 0 };
	tDateTime odkdy = { 0, 0, 0, 0, 0, 0, 0 };
	tDateTime dokdy = { 0, 0, 0, 0, 0, 0, 0 };
	bool prvniZaznam = true;
	tSenzor* prvni = NULL;
	tSenzor* aktualni = NULL;

	if (file != NULL) {
		fgets(buffer, 100, file);

		while (fgets(buffer, 100, file) != NULL) {
			idSenzoru = atoi(strtok(buffer, ";"));

			if ((atoi(strtok(NULL, ";")) == id)) {
				cas = strtok(NULL, ";");
				m3 = (float)atof(strtok(NULL, ""));

				structDatum = dejDateTime(cas);

				if (prvniZaznam) {
					odkdy = structDatum;
					prvniZaznam = false;
				}

				if (prvni != NULL) {
					aktualni->dalsi = vytvorSenzor(idSenzoru, structDatum, m3);
					aktualni = aktualni->dalsi;
				} else {
					prvni = vytvorSenzor(idSenzoru, structDatum, m3);
					aktualni = prvni;
				}
			}
		}

		dokdy = structDatum;
		fclose(file);
	} else {
		printf("Soubor se nepodarilo otevrit.");
	}

	seznam = malloc(sizeof(tSeznam));

	if (seznam != NULL) {
		seznam->odkdy = odkdy;
		seznam->dokdy = dokdy;
		seznam->seznam = prvni;
	} else {
		printf("Nedostatek pameti pro alokaci seznamu mereni.");
	}

	return seznam;
}

// vypíše všechna mìøení, využívá vypisSenzor
void vypisMereni() {
	tDateTime odkdy = seznam->odkdy;
	tDateTime dokdy = seznam->dokdy;

	char* formatOdkdy = formatujCas(odkdy);
	char* formatDokdy = formatujCas(dokdy);

	printf("Vypis mereni od %s do %s\n", formatOdkdy, formatDokdy);
	printf("ID\tCas\t\t\tObjem (m3)\n");

	tSenzor* aktualni = seznam->seznam;

	while (aktualni != NULL) {
		vypisSenzor(aktualni);
		aktualni = aktualni->dalsi;
	}

	free(formatOdkdy);
	free(formatDokdy);
}

// odebere a vrátí požadované mìøení ze seznamu
tSenzor* odeberMereni(tDateTime timestamp) {
	tSenzor* aktualni = seznam->seznam;
	tSenzor* predchozi = NULL;
	tDateTime ts;

	while (aktualni != NULL) {
		ts = aktualni->timestamp;
		if (ts.year == timestamp.year && ts.month == timestamp.month && ts.day == timestamp.day
			&& ts.hour == timestamp.hour && ts.min == timestamp.min && ts.sec == timestamp.sec) {
			if (predchozi == NULL) {
				predchozi = aktualni;
				seznam->seznam = aktualni->dalsi;
				return predchozi;
			} else {
				predchozi->dalsi = aktualni->dalsi;
				return aktualni;
			}			
		}

		predchozi = aktualni;
		aktualni = aktualni->dalsi;
	}

	return NULL;
}

// smaž (dealokuj) všechny záznamy a tSeznam
void dealokujSeznam() {
	tSenzor* aktualni = seznam->seznam;
	tSenzor* temp = NULL;

	while (aktualni != NULL) {
		temp = aktualni->dalsi;
		free(aktualni);
		aktualni = temp;
	}

	free(temp);
	free(seznam);
}

// Výpoèet prùmìru pro všechny kombinace den v týdnu, který vrátí 
// dynamické 2D pole 7x24 „den v týdnu“ x „hodina dne“(v litrech)
float** analyzuj(tDateTime odkdy, tDateTime dokdy) {
	tSenzor* aktualni;
	char* denVTydnu[7] = { "pondeli   ", "utery     ", "streda    ", "ctvrtek   ",
		"patek     ", "sobota    ", "nedele    " };

	polePrumeru = calloc(radky, sizeof(float*));
	float* poleSouctu = malloc(sizeof(float) * sloupce);
	int* polePoctu = malloc(sizeof(int) * sloupce);

	if (polePrumeru != NULL && poleSouctu != NULL && polePoctu != NULL) {
		for (size_t i = 0; i < radky; i++) {
			polePrumeru[i] = calloc(sloupce, sizeof(float));
		}

		for (size_t i = 0; i < radky; i++) {
			if (polePrumeru[i] != NULL) {
				aktualni = seznam->seznam;
				memset(poleSouctu, 0, sizeof(float) * sloupce);
				memset(polePoctu, 0, sizeof(int) * sloupce);

				while (aktualni != NULL) {
					if (aktualni->timestamp.dayInMonth == i) {
						for (size_t j = 0; j < sloupce; j++) {
							if (aktualni->timestamp.hour == j) {
								poleSouctu[j] += (aktualni->m3 * 1000.0f);
								polePoctu[j]++;
							}
						}
					}

					aktualni = aktualni->dalsi;
				}

				for (size_t j = 0; j < sloupce; j++) {
					if (polePoctu[j] != 0) {
						polePrumeru[i][j] = poleSouctu[j] / (float)polePoctu[j];
					}
				}
			}
		}

		printf("Den [Hodina]\n");

		for (size_t i = 0; i < radky; i++) {
			printf("%s ", denVTydnu[i]);

			if (polePrumeru[i] != NULL) {
				for (size_t j = 0; j < sloupce; j++) {
					printf(" [%d:00] %.1f", j, polePrumeru[i][j]);
				}
			}

			printf("\n");
		}

		free(poleSouctu);
		free(polePoctu);
	} else {
		printf("Nedostatek pameti pro alokaci zdroju pro analyzu prumeru.");
	}

	return polePrumeru;
}

// uloží analýzu prùmìrù do souboru CSV
void ulozAnalyzuDoCSV() {
	char* denVTydnu[7] = { "pondìlí", "úterý", "støeda", "ètvrtek", "pátek", "sobota", "nedìle" };
	char* formatOdkdy = formatujCas(seznam->odkdy);
	char* formatDokdy = formatujCas(seznam->dokdy);
	char* nazevSouboru[30];
	strcpy(nazevSouboru, strtok(formatOdkdy, " "));
	strcat(nazevSouboru, " – ");
	strcat(nazevSouboru, strtok(formatDokdy, " "));
	strcat(nazevSouboru, ".csv");

	FILE* file = fopen(nazevSouboru, "w");

	if (file != NULL) {
		fprintf(file, "Den/Hodina;");
		
		for (int i = 0; i < sloupce; i++) {
			fprintf(file, "%d;", i);
		}

		fprintf(file, "\n");

		for (size_t i = 0; i < radky; i++) {
			fprintf(file, "%s;", denVTydnu[i]);

			for (size_t j = 0; j < sloupce; j++) {
				fprintf(file, "%.1f;", polePrumeru[i][j]);
			}
			fprintf(file, "\n");
		}

		fclose(file);
	}

	free(formatOdkdy);
	free(formatDokdy);
}

// smaž (dealokuj) matici
void dealokujMatici() {
	for (size_t i = 0; i < radky; i++) {
		free(polePrumeru[i]);
	}

	free(polePrumeru);
}

// vrátí odchylku v litrech daného záznamu(mìøení v dle timestamp) 
// oproti správnému prùmìru z matice prùmìrù
float dejOdchylku(tDateTime timestamp) {
	tSenzor* aktualni = seznam->seznam;
	tDateTime ts = { 0, 0, 0, 0, 0, 0, 0 };
	float odchylka = 0.0f;
	float hodnota = 0.0f;

	while (aktualni != NULL) {
		ts = aktualni->timestamp;
		if (ts.year == timestamp.year && ts.month == timestamp.month && ts.day == timestamp.day
			&& ts.hour == timestamp.hour && ts.min == timestamp.min && ts.sec == timestamp.sec) {
			hodnota = (aktualni->m3 * 1000.0f);
		}

		aktualni = aktualni->dalsi;
	}

	for (size_t i = 0; i < radky; i++) {
		for (size_t j = 0; j < sloupce; j++) {
			if (i == ts.dayInMonth && j == ts.hour)
			{
				odchylka = polePrumeru[i][j] - hodnota;
			}
		}
	}

	return odchylka;
}
