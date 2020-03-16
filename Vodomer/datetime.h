#ifndef DATETIME_H
#define DATETIME_H

typedef struct {
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
	int dayInMonth;
} tDateTime;

// p�ev�d� vstup (2018-05-01 01:00:00) na strukturu, vyu��v� dejDenVTydnu
tDateTime dejDateTime(char* datetime);
//vrac� den v t�dnu (0-PO,�,6-NE)
int dejDenVTydnu(int r, int m, int d);

#endif // !DATETIME_H

