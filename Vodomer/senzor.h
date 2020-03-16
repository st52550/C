#ifndef SENZOR_H
#define SENZOR_H

#include "datetime.h"

typedef struct senzor {
	int id;
	tDateTime timestamp;
	float m3;
	struct senzor* dalsi;
} tSenzor;

// vrací adresu dynamické alokace senzoru, ukazatel dalsi se nastaví na NULL
tSenzor* vytvorSenzor(int id, tDateTime ts, float m3);
//vypíše senzor na obrazovku
void vypisSenzor(tSenzor* senzor);
// formátuje výpis data a èasu
char* formatujCas(tDateTime cas);

#endif // !SENZOR_H

