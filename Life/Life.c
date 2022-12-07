#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#pragma warning (disable: 4996)

#define N 15
#define M 20
#define DURCHGÄNGE 10

//TODO - n und m aus datei auslesen!
//TODO - Füllprozent der ZellenMatrix besser bestimmen


//void gotoXY(int x, int y);
char** GetDateiStartZustand(char* fileName, char** zellen);
char** GetRandomStartZustand(char** zellen, int prozent);
FILE* TextÖffnen(char* fileName, char* modus);
void PrintZelle(char** zellen);
char** StartZustandErstellen(char fileName[], char** zellen);

int main() {
	srand(time(NULL));
	int prozent, zeilen, reihen;
	char p[1024];
	int eingabe;
	int auswahl;
	char* dateiName = (char*)malloc(100 * sizeof(char));
	char** zellen;
	char** pzellen;
	int count = 0;
	int lebendeNachbarn;
	int zustand = 0;
	char zelle;
	int durchgang = 0;
	zellen = malloc(N * sizeof(int*));
	for (int i = 0; i < N; i++) {
		zellen[i] = malloc(M * sizeof(int));
		if (NULL == zellen[i]) {
			printf("Kein Speicher mehr fuer Zeile %d\n", i);
			return EXIT_FAILURE;
		}
	}
	printf("\tGame of Life\n\n");
	printf("Datei Laden: 1\n");
	printf("Zufallsgenerator: 0\n");
	scanf("%d", &auswahl);
	fgets(p, 1024, stdin);

	//Datei Laden:
	if (auswahl == 1) {
		printf("DateiName: \n");
		printf("Startzustand_1: 1\n");
		printf("Startzustand_2: 2\n");
		printf("Startzustand_3: 3\n");
		printf("Startzustand erstellen: 4\n");

		scanf("%d", &auswahl);
		//scanf("%s", &dateiName);
		fgets(p, 1024, stdin);
	}
	
	switch (auswahl)
	{
	case 0:
		//Zufallsgenerator:
		printf("Wie viele Zeilen soll die Zelle besitzen: ");
		scanf("%d", &zeilen);
		printf("Wie viele Reihen soll die Zelle besitzen: ");
		scanf("%d", &reihen);
		printf("Wie viele lebende Zellen soll es geben (Prozent): ");
		scanf("%d", &prozent);

		zellen = GetRandomStartZustand(zellen, prozent);
		break;
	case 1:
		dateiName = "Startzustand_1.txt";
		zellen = GetDateiStartZustand(dateiName, zellen);
		break;
	case 2:
		dateiName = "Startzustand_2.txt";
		zellen = GetDateiStartZustand(dateiName, zellen);
			break;
	case 3:
		dateiName = "Startzustand_3.txt";
		zellen = GetDateiStartZustand(dateiName, zellen);
		break;
	case 4:
		printf("DateiName: ");
		fgets(dateiName, sizeof(dateiName), stdin);
		fgets(p, 1024, stdin);
		zellen = StartZustandErstellen(dateiName, zellen);
		break;
	default:
		printf("Falsche Eingabe!");
		break;
	}

	printf("Startzustand:\n");
	PrintZelle(zellen);

	printf("SchrittweiseAbarbeitung: 0\n");
	printf("fliessende Animation: 1\n");
	scanf("%d", &eingabe);

	//Speicherplatz für Zellen generieren
	pzellen = malloc(N * sizeof(int*));
	for (int i = 0; i < M; i++) {
		pzellen[i] = malloc(M * sizeof(int));
		if (NULL == pzellen[i]) {
			printf("Kein Speicher mehr fuer Zeile %d\n", i);
			return EXIT_FAILURE;
		}
	}

	//Run:
	while(durchgang <= DURCHGÄNGE){
		Sleep(100);
		system("cls");

		for (int i = 0; i < N; i++)
		{
			for (int k = 0; k < M; k++)
			{
				lebendeNachbarn = 0;
				count = 0;
				zelle = zellen[i][k];

				//1.	Besitzt eine lebende Zelle 0, 1 oder mehr als 3 lebende 
				//		Nachbarzellen, stirbt sie an Vereinsamung oder Übervölkerung.
				//2.	Besitzt eine lebende Zelle 2 oder 3 lebende Nachbarzellen, dann
				//		existiert sie in der nächsten Generation weiter.
				if (zelle == '*') {
					//obenrechts, gibt es nicht in der ersten Reihe und letzten Spalte
					if (i > 0 && k < (M)-1) {
							if (zelle == zellen[i-1][k + 1]) { lebendeNachbarn++; }
					}
						//obenlinks, gibt es nicht in der ersten Reihe und erster Spalte
					if (i > 0 && k > 0) {
						if (zelle == zellen[i - 1][k - 1]) { lebendeNachbarn++; }
					}
						//untenlinks, gibt es nicht in der letzten Reihe und erster Spalte
					if (i < (N)-1 && k > 0) {
						if (zelle == zellen[i + 1][k - 1]) { lebendeNachbarn++; }
					}
						//untenrechts, gibt es nicht in der letzten Reihe und letztenSpalze Spalte
					if (i < (N)-1 && k < (M)-1) {
						if (zelle == zellen[i + 1][k + 1]) { lebendeNachbarn++; }
					}
					if (k < (M) - 1) {
						if (zelle == zellen[i][k + 1]) { lebendeNachbarn++; }
					}
					if (i < (N) -1) {
						if (zelle == zellen[i + 1][k]) { lebendeNachbarn++; }
					}
					if (k > 0) {
						if (zelle == zellen[i][k - 1]) { lebendeNachbarn++; }
					}
					if (i > 0) {
						if (zelle == zellen[i - 1][k]) { lebendeNachbarn++; }
					}
				}
				//3.	Ist eine tote Zelle von genau 3 lebenden Zellen umgeben, dann
				//		entsteht aus ihr eine neue lebende Zelle.
				if (zelle == ' ') {
					//obenrechts, gibt es nicht in der ersten Reihe und letzten Spalte
					if (i > 0 && k < (M)-1) {
						if (zelle != zellen[i - 1][k + 1]) { lebendeNachbarn++; }
					}
					//obenlinks, gibt es nicht in der ersten Reihe und erster Spalte
					if (i > 0 && k > 0) {
						if (zelle != zellen[i - 1][k - 1]) { count++; }
					}
					//untenlinks, gibt es nicht in der letzten Reihe und erster Spalte
					if (i < (N)-1 && k > 0) {
						if (zelle != zellen[i + 1][k - 1]) { count++; }
					}
					//untenrechts, gibt es nicht in der letzten Reihe und letztenSpalze Spalte
					if (i < (N)-1 && k < (M)-1) {
						if (zelle != zellen[i + 1][k + 1]) { count++; }
					}
					if (k < (M)-1) {
						if (zelle != zellen[i][k + 1]) { count++; }
					}
					if (i < (N)-1) {
						if (zelle != zellen[i + 1][k]) { count++; }
					}
					if (k > 0) {
						if (zelle != zellen[i][k - 1]) { count++; }
					}
					if (i > 0) {
						if (zelle != zellen[i - 1][k]) { count++; }
					}
				}
				if (count == 3) {
					pzellen[i][k] = '*';
					zustand = 1;
				}
				else if (lebendeNachbarn < 2 || lebendeNachbarn > 3) {
					pzellen[i][k] = ' ';
					zustand = 0;
				}
				else {
					pzellen[i][k] = '*';
					zustand = 1;
				}
			}
			printf("\n");
		}
		//Ausgabe & tauschen
		for (int i = 0; i < N; i++)
		{
			for (int k = 0; k < M; k++)
			{
				printf(" %c ", pzellen[i][k]);
				zellen[i][k] = pzellen[i][k];
			}
			printf("\n");
		}
		if (eingabe == 0) {
			int tmp = eingabe;
			scanf("%d", &tmp);
			fgets(p, 1024, stdin);
		}
		durchgang++;
	}
	return 0;
}
char** GetRandomStartZustand(char** zellen, int prozent) {
	int maxAnzahlAnLebendenZelle = ((N)*(M)) * ((float)(prozent) / 100);
	int maxToteZellen = ((N) * (M)) - maxAnzahlAnLebendenZelle;
	char zustand = ' ';
	int currentLebendeZellen = 0;
	int currentToteZellen = 0;

	for (int i = 0; i < N; i++)
	{
		for (int k = 0; k < M; k++)
		{
			if (maxToteZellen == currentToteZellen) {
				zustand = '*';
				zellen[i][k] = zustand;
			}
			else if (maxAnzahlAnLebendenZelle > currentLebendeZellen) {
				if (rand() % 2 == 1) {
					zustand = '*';
					zellen[i][k] = zustand;
					currentLebendeZellen++;
				}
				else {
					currentToteZellen++;
					zustand = ' ';
					zellen[i][k] = zustand;

				}
			}
			else {
				zustand = ' ';
				zellen[i][k] = zustand;

			}
			//char zustand = ((rand() % 2) == 0 ? ' ' : '*');
		}
	}
	return zellen;
}
void PrintZelle(char** zellen) {

	for (int i = 0; i < N; i++)
	{
		for (int k = 0; k < M; k++)
		{
			printf(" %c ", zellen[i][k]);
		}
		printf("\n");
	}
}
char** GetDateiStartZustand(char* fileName, char** zellen) {
	int i = 0;
	int k = 0;
	FILE* file = TextÖffnen(fileName, "rt");
	int zeichen;
		while ((zeichen = fgetc(file)) != EOF) {
			if (i != N) {
				if (zeichen != 10) {
					zellen[i][k] = zeichen;
					//printf("%d", fputc(zeichen, stdout));
					k++;
					if (k == M) {
						k = 0;
						i++;
					}
				}
			}
		}
	fclose(file);
	return zellen;
}
FILE* TextÖffnen(char* fileName, char* modus) {
	FILE* file;
	if ((file = fopen(fileName, modus)) == NULL) {
		printf("\nÖffnen von %s fehlgeschlagen\n", fileName);
		return EXIT_FAILURE;
	}
	else {
		printf("\n%s geöffnet\n", fileName);
		return file;
	}
}
char** StartZustandErstellen(char fileName[], char** zellen) {
	printf("%s", fileName);
	FILE* file = TextÖffnen(fileName, "wt");
	char zustand;
	for (int i = 0; i < N; i++)
	{
		for (int k = 0; k < M; k++)
		{
			scanf("%c", &zustand);
			fputs(&zustand, file);
		}
		//fputs('\n', file);
	}
	fclose(file);
	return zellen;
}