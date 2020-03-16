#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <crtdbg.h>
#include "seznamStanic.h"

int main() {
	int choice;
	char stanice[50];
	tStanice *novaStanice;

	printf("Aplikace sledovani vlaku\n");
	printf("------------------------\n");

	do
	{
		printf("Menu:\n\n");
		printf("1. Nacti data\n");
		printf("2. Vloz zaznam na prvni pozici\n");
		printf("3. Vloz zaznam na posledni pozici\n");
		printf("4. Odeber zaznam\n");
		printf("5. Vypis zaznamy\n");
		printf("6. Vypis odebrane zaznamy\n");
		printf("7. Trasuj vlak\n");
		printf("8. Zrus seznam\n");
		printf("9. Konec programu\n");
		printf("\nVyberte cislo pozadovane akce: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1: nactiData("stanice.csv");
				break;
		case 2: novaStanice = zadejStanici();
				vlozNaPrvni(novaStanice);
				break;
		case 3: novaStanice = zadejStanici();
				vlozNaPosledni(novaStanice);
				break;
		case 4: printf("Zadejte nazev stanice: ");
				scanf("%s", stanice);
				odeber(stanice);
				break;
		case 5: vypis();
				break;
		case 6: vypisOdebrane();
				break;
		case 7: trasovaniVlaku("vlak.csv");
				break;
		case 8: zrusData();
				break;
		case 9: printf("Stisknutim libovolne klavesy ukoncite program.\n");
				break;
		default: printf("Nespravna volba, zkusete to znovu.\n");
				break;
		}
	} while (choice != 9);

	_CrtDumpMemoryLeaks();
	system("pause");
	return 0;
}