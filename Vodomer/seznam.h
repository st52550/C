#ifndef SEZNAM_H
#define SEZNAM_H

#include "datetime.h"
#include "senzor.h"

typedef struct {
	tDateTime odkdy;
	tDateTime dokdy;
	tSenzor* seznam;
} tSeznam;

// vrac� pole id v�ech senzor� v souboru
int* dejSeznamSenzoru();
// vyp�e id v�ech senzor�
void vypisSeznamSenzoru();
// ove�� zda se ��slo senzor� nach�z� v seznamu senzor�
bool overCisloSenzoru(int cisloSenzoru);
// na�te v�echna m��en� dan�ho senzoru, vyu��v� vytvorSenzor
tSeznam* nactiMereni(int id);
// vyp�e v�echna m��en�, vyu��v� vypisSenzor
void vypisMereni();
// odebere a vr�t� po�adovan� m��en� ze seznamu
tSenzor* odeberMereni(tDateTime timestamp);
// sma� (dealokuj) v�echny z�znamy a tSeznam
void dealokujSeznam();
// V�po�et pr�m�ru pro v�echny kombinace den v t�dnu, kter� vr�t� 
// dynamick� 2D pole 7x24 �den v t�dnu� x �hodina dne�(v litrech)
float** analyzuj(tDateTime odkdy, tDateTime dokdy);
// ulo�� anal�zu pr�m�r� do souboru CSV
void ulozAnalyzuDoCSV();
// sma� (dealokuj) matici
void dealokujMatici();
// vr�t� odchylku v litrech dan�ho z�znamu(m��en� v dle timestamp) 
// oproti spr�vn�mu pr�m�ru z matice pr�m�r�
float dejOdchylku(tDateTime timestamp);

#endif // !SEZNAM_H

