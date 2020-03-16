#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <crtdbg.h>
#include <stdbool.h>
#include <string.h>
#include "seznam.h"

bool overFormatCasu(char* cas);
void clearInput();

int main(void) {
	int volba = -1;
	tSeznam* seznamMereni = NULL;
	tSenzor* odebraneMereni = NULL;
	tDateTime casMereni;
	tDateTime odkdy;
	tDateTime dokdy;
	int* seznamSenzoru = NULL;
	float** prumerneSpotreby = NULL;
	int cisloSenzoru;
	char cas[20];
	float odchylka;

	printf("Aplikace pro analyzu spotreby vody\n");
	printf("----------------------------------\n");
	printf("Menu:\n\n");
	printf("[1] Nacti a vypis ID vsech senzoru\n");
	printf("[2] Nacti mereni\n");
	printf("[3] Vypis nactena mereni\n");
	printf("[4] Odeber mereni\n");
	printf("[5] Vypocitej a vypis analyzu prumeru\n");
	printf("[6] Uloz analyzu prumeru do souboru CSV\n");
	printf("[7] Vypis odchylku\n");
	printf("[8] Dealokuj matici\n");
	printf("[9] Vymaz mereni\n");
	printf("[0] Konec programu\n");

	do {		
		printf("\nZadejte [cislo] pozadovane akce: ");

		if (scanf("%d", &volba) != 0) {
			switch (volba) {
			case 1:
				printf("Probiha vypocet...\n");
				seznamSenzoru = dejSeznamSenzoru();
				vypisSeznamSenzoru();
				break;
			case 2:
				if (seznamSenzoru != NULL) {
					printf("Zadejte cislo ze seznamu senzoru: ");
					if (scanf("%d", &cisloSenzoru) != 0) {
						if (overCisloSenzoru(cisloSenzoru)) {
							seznamMereni = nactiMereni(cisloSenzoru);
						}
					}
					else {
						printf("Neplatny vstup.\n");
						clearInput();
					}
				} else {
					printf("Nejdrive nactete seznam senzoru.");
				}
				break;
			case 3:
				if (seznamMereni != NULL) {
					vypisMereni();
				} else {
					printf("Seznam mereni nebyl nacten.");
				}
				break;
			case 4:
				if (seznamMereni != NULL) {
					printf("Zadejte datum a cas mereni ktery chcete odebrat:\n");
					printf("!Pozadovany format casu je yyyy.mm.dd hh:mm:ss!\n");
					clearInput();
					gets(cas);
					if (overFormatCasu(cas)) {
						casMereni = dejDateTime(cas);
						odebraneMereni = odeberMereni(casMereni);

						if (odebraneMereni != NULL) {
							printf("Mereni s ID %d bylo odebrano.", odebraneMereni->id);
						}
						else {
							printf("Zadane mereni nebylo nalezeno.");
						}
					}
					else {
						printf("Zadany format casu neni platny. Opakujte akci.");
					}
				} else {
					printf("Seznam mereni nebyl nacten.");
				}
				break;
			case 5:
				if (seznamMereni != NULL) {
					prumerneSpotreby = analyzuj(seznamMereni->odkdy, seznamMereni->dokdy);
				} else {
					printf("Seznam mereni nebyl nacten.");
				}
				break;
			case 6:
				if (seznamMereni != NULL && prumerneSpotreby != NULL) {
					ulozAnalyzuDoCSV();
					printf("Ulozeni do souboru bylo uspesne.");
				} else {
					printf("Nebyl nacten seznam mereni nebo nebyla provedena analyza prumeru.");
				}
				break;
			case 7:
				if (seznamMereni != NULL) {
					printf("Zadejte datum a cas mereni pro odchylku od prumeru.\n");
					printf("Pozadovany format je yyyy.mm.dd hh:mm:ss\n");
					clearInput();
					gets(cas);

					if (overFormatCasu(cas)) {
						casMereni = dejDateTime(cas);
						odchylka = dejOdchylku(casMereni);
						printf("Odchylka od prumeru: %.1f l\n", odchylka);
					}
					else {
						printf("Zadany format casu neni platny. Opakujte akci.");
					}
				} else {
					printf("Seznam mereni nebyl nacten.");
				}
				break;
			case 8:
				if (prumerneSpotreby != NULL) {
					dealokujMatici();
					prumerneSpotreby = NULL;
				} else {
					printf("Analyza prumeru nebyla vytvorena.");
				}
				break;
			case 9:
				if (seznamMereni != NULL) {
					dealokujSeznam();
					seznamMereni = NULL;
				} else {
					printf("Seznam mereni nebyl nacten.");
				}
				break;
			case 0:
				printf("Konec programu...\n");

				free(seznamSenzoru);
				seznamSenzoru = NULL;
				if (seznamMereni != NULL) { dealokujSeznam(); }
				if (odebraneMereni != NULL) { free(odebraneMereni); }
				if (prumerneSpotreby != NULL) { dealokujMatici(); }

				break;
			default: printf("Nespravna volba, opakujte akci.\n");
				break;
			}
		}
		else {
			printf("Neplatny vstup.\n");
			clearInput();
		}
	} while (volba != 0);

	if (_CrtDumpMemoryLeaks() != 0) {
		printf("Nebyla provedena dealokace");
	}

	return 0;
}

bool overFormatCasu(char* cas) {
	if (strlen(cas) != 19) {
		return false;
	}

	if (cas[4] != '.' && cas[7] != '.' && cas[10] != ' ' 
		&& cas[13] != ':' && cas[16] != ':') {
		return false;
	}

	for (size_t i = 0; i < strlen(cas); i++) {
		if (i != 4 && i != 7 && i != 10 && i != 13 && i != 16) {
			if (cas[i] < 48 || cas[i] > 57) {
				return false;
			}
		}
	}

	return true;
}

void clearInput() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF){}
}