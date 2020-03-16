#ifndef SENZOR_H
#define SENZOR_H

#include "datetime.h"

typedef struct senzor {
	int id;
	tDateTime timestamp;
	float m3;
	struct senzor* dalsi;
} tSenzor;

// vrac� adresu dynamick� alokace senzoru, ukazatel dalsi se nastav� na NULL
tSenzor* vytvorSenzor(int id, tDateTime ts, float m3);
//vyp�e senzor na obrazovku
void vypisSenzor(tSenzor* senzor);
// form�tuje v�pis data a �asu
char* formatujCas(tDateTime cas);

#endif // !SENZOR_H

