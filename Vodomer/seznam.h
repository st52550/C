#ifndef SEZNAM_H
#define SEZNAM_H

#include "datetime.h"
#include "senzor.h"

typedef struct {
	tDateTime odkdy;
	tDateTime dokdy;
	tSenzor* seznam;
} tSeznam;

// vrací pole id všech senzorù v souboru
int* dejSeznamSenzoru();
// vypíše id všech senzorù
void vypisSeznamSenzoru();
// oveøí zda se èíslo senzorù nachází v seznamu senzorù
bool overCisloSenzoru(int cisloSenzoru);
// naète všechna mìøení daného senzoru, využívá vytvorSenzor
tSeznam* nactiMereni(int id);
// vypíše všechna mìøení, využívá vypisSenzor
void vypisMereni();
// odebere a vrátí požadované mìøení ze seznamu
tSenzor* odeberMereni(tDateTime timestamp);
// smaž (dealokuj) všechny záznamy a tSeznam
void dealokujSeznam();
// Výpoèet prùmìru pro všechny kombinace den v týdnu, který vrátí 
// dynamické 2D pole 7x24 „den v týdnu“ x „hodina dne“(v litrech)
float** analyzuj(tDateTime odkdy, tDateTime dokdy);
// uloží analýzu prùmìrù do souboru CSV
void ulozAnalyzuDoCSV();
// smaž (dealokuj) matici
void dealokujMatici();
// vrátí odchylku v litrech daného záznamu(mìøení v dle timestamp) 
// oproti správnému prùmìru z matice prùmìrù
float dejOdchylku(tDateTime timestamp);

#endif // !SEZNAM_H

