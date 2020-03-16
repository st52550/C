#define _CRT_SECURE_NO_WARNINGS

#include "senzor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// vrací adresu dynamické alokace senzoru, ukazatel dalsi se nastaví na NULL
tSenzor* vytvorSenzor(int id, tDateTime ts, float m3) {
	tSenzor* senzor = calloc(1, sizeof(tSenzor));

	if (senzor != NULL) {
		senzor->id = id;
		senzor->timestamp = ts;
		senzor->m3 = m3;
		senzor->dalsi = NULL;
	} else {
		printf("Nedostatek pameti pro alokaci senzoru.");
	}
}

//vypíše senzor na obrazovku
void vypisSenzor(tSenzor* senzor) {
	tDateTime cas = senzor->timestamp;
	char* formatCasu = formatujCas(cas);

	printf("%d\t%s\t%f\n", senzor->id, formatCasu, senzor->m3);
	free(formatCasu);
}

// formátuje výpis data a èasu
char* formatujCas(tDateTime cas) {
	char* formatCasu = malloc(sizeof(char) * 20);
	char token[3];

	if (formatCasu != NULL) {
		strcpy(formatCasu, _itoa(cas.year, formatCasu, 10));
		strcat(formatCasu, "-");
		if (cas.month < 10) { strcat(formatCasu, "0"); };
		_itoa(cas.month, token, 10);
		strcat(formatCasu, token);
		strcat(formatCasu, "-");
		if (cas.day < 10) { strcat(formatCasu, "0"); };
		_itoa(cas.day, token, 10);
		strcat(formatCasu, token);
		strcat(formatCasu, " ");
		if (cas.hour < 10) { strcat(formatCasu, "0"); };
		_itoa(cas.hour, token, 10);
		strcat(formatCasu, token);
		strcat(formatCasu, ":");
		if (cas.min < 10) { strcat(formatCasu, "0"); };
		_itoa(cas.min, token, 10);
		strcat(formatCasu, token);
		strcat(formatCasu, ":");
		if (cas.sec < 10) { strcat(formatCasu, "0"); };
		_itoa(cas.sec, token, 10);
		strcat(formatCasu, token);
	} else {
		printf("Nedostatek pameti pro alokaci formatu casu.");
	}

	return formatCasu;
}