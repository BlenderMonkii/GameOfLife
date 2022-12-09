#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

#pragma warning (disable: 4996)

#define DURCHGÄNGE 10

//TODO - Füllprozent der ZellenMatrix besser bestimmen
//TODO - neue Datei anlegen und Matrix selbst erstellen
//TODO - auf DAO prüfen

char** GetDateiStartZustand(char* fileName, int zeilen, int reihen);
char** GetRandomStartZustand(int prozent, int reihe, int zeile);
FILE* TextÖffnen(char* fileName, char* modus);
void PrintZelle(char** zellen, int zeilen, int reihen);
void StartZustandErstellen(char fileName[], char* modus);
int GetZeilen(char* dateiName);
int GetReihen(char* dateiName);

int main() {
	srand(time(NULL));
	int prozent, zeilen, reihen;
	char p[1024];
	int eingabe;
	int auswahl;
	char* dateiName = (char*)malloc(100 * sizeof(char));
	char** zellen = 0;
	char** pzellen;
	int count = 0;
	int lebendeNachbarn;
	int zustand = 0;
	char zelle;
	int durchgang = 0;

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
		zellen = GetRandomStartZustand(prozent,zeilen,reihen);
		break;
	case 1:
		dateiName = "Startzustand_1.txt";
		zeilen = GetZeilen(dateiName);
		reihen = GetReihen(dateiName);
		zellen = GetDateiStartZustand(dateiName, zeilen, reihen, "rt");
		break;
	case 2:
		dateiName = "Startzustand_2.txt";
		zeilen = GetZeilen(dateiName);
		reihen = GetReihen(dateiName);
		zellen = GetDateiStartZustand(dateiName, zeilen, reihen, "rt");
		break;
	case 3:
		dateiName = "Startzustand_3.txt";
		zeilen = GetZeilen(dateiName);
		reihen = GetReihen(dateiName);
		zellen = GetDateiStartZustand(dateiName, zeilen, reihen, "rt");
		break;
	case 4:
		printf("dateiname: ");
		scanf("%s", dateiName);
		fgets(p, 1024, stdin);
		StartZustandErstellen(dateiName, "w+t");
		zeilen = GetZeilen(dateiName);
		reihen = GetReihen(dateiName);
		zellen = GetDateiStartZustand(dateiName, zeilen, reihen, "rt");
		break;
	default:
		printf("Falsche Eingabe!");
		break;
	}

	printf("SchrittweiseAbarbeitung: 0\n");
	printf("fliessende Animation: 1\n");
	scanf("%d", &eingabe);

	//Speicherplatz für Zellen generieren
	pzellen = malloc(zeilen * sizeof(int*));
	for (int i = 0; i < zeilen; i++) {
		pzellen[i] = malloc(reihen * sizeof(int));
		if (NULL == pzellen[i]) {
			printf("Kein Speicher mehr fuer Zeile %d\n", i);
			return EXIT_FAILURE;
		}
	}

	//Run:
	while(durchgang <= DURCHGÄNGE){
		Sleep(100);
		system("cls");

		for (int i = 0; i < zeilen; i++)
		{
			for (int k = 0; k < reihen; k++)
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
					if (i > 0 && k < (reihen)-1) {
							if (zelle == zellen[i-1][k + 1]) { lebendeNachbarn++; }
					}
						//obenlinks, gibt es nicht in der ersten Reihe und erster Spalte
					if (i > 0 && k > 0) {
						if (zelle == zellen[i - 1][k - 1]) { lebendeNachbarn++; }
					}
						//untenlinks, gibt es nicht in der letzten Reihe und erster Spalte
					if (i < (zeilen)-1 && k > 0) {
						if (zelle == zellen[i + 1][k - 1]) { lebendeNachbarn++; }
					}
						//untenrechts, gibt es nicht in der letzten Reihe und letztenSpalze Spalte
					if (i < (zeilen)-1 && k < (reihen)-1) {
						if (zelle == zellen[i + 1][k + 1]) { lebendeNachbarn++; }
					}
					if (k < (reihen) - 1) {
						if (zelle == zellen[i][k + 1]) { lebendeNachbarn++; }
					}
					if (i < (zeilen) -1) {
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
					if (i > 0 && k < (reihen)-1) {
						if (zelle != zellen[i - 1][k + 1]) { lebendeNachbarn++; }
					}
					//obenlinks, gibt es nicht in der ersten Reihe und erster Spalte
					if (i > 0 && k > 0) {
						if (zelle != zellen[i - 1][k - 1]) { count++; }
					}
					//untenlinks, gibt es nicht in der letzten Reihe und erster Spalte
					if (i < (zeilen)-1 && k > 0) {
						if (zelle != zellen[i + 1][k - 1]) { count++; }
					}
					//untenrechts, gibt es nicht in der letzten Reihe und letztenSpalze Spalte
					if (i < (zeilen)-1 && k < (reihen)-1) {
						if (zelle != zellen[i + 1][k + 1]) { count++; }
					}
					if (k < (reihen)-1) {
						if (zelle != zellen[i][k + 1]) { count++; }
					}
					if (i < (zeilen)-1) {
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
		for (int i = 0; i < zeilen; i++)
		{
			for (int k = 0; k < reihen; k++)
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
char** GetRandomStartZustand(int prozent, int zeilen, int reihen) {
	char** zellen;
	zellen = malloc(zeilen * sizeof(int*));
	for (int i = 0; i < zeilen; i++) {
		zellen[i] = malloc(reihen * sizeof(int));
		if (NULL == zellen[i]) {
			printf("Kein Speicher mehr fuer Zeile %d\n", i);
			return EXIT_FAILURE;
		}
	}
	int maxAnzahlAnLebendenZelle = ((zeilen) * (reihen)) * ((float)(prozent) / 100);
	int maxToteZellen = ((zeilen) * (reihen)) - maxAnzahlAnLebendenZelle;
	char zustand = ' ';
	int currentLebendeZellen = 0;
	int currentToteZellen = 0;

	for (int i = 0; i < zeilen; i++)
	{
		for (int k = 0; k < reihen; k++)
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
		}
	}
	printf("Startzustand:\n");
	PrintZelle(zellen, zeilen, reihen);
	return zellen;
}
void PrintZelle(char** zellen, int zeilen, int reihen) {

	for (int i = 0; i < zeilen; i++)
	{
		for (int k = 0; k < reihen; k++)
		{
			printf(" %c ", zellen[i][k]);
		}
		printf("\n");
	}
}
char** GetDateiStartZustand(char* fileName, int zeilen, int reihen, char* modus) {
	char** zellen;
	zellen = malloc(zeilen * sizeof(int*));
	for (int i = 0; i < zeilen; i++) {
		zellen[i] = malloc(reihen * sizeof(int));
		if (NULL == zellen[i]) {
			printf("Kein Speicher mehr fuer Zeile %d\n", i);
			return EXIT_FAILURE;
		}
	}
	FILE* file = TextÖffnen(fileName, modus);

	int zeichen;
	for (int i = 0; i < zeilen; i++)
	{
		for (int k = 0; k < reihen; k++)
		{
			if ((zeichen = fgetc(file)) != EOF){
				if (zeichen == 10) {
					k--;
					zellen[i][k] = zeichen;
				}
				else {
					zellen[i][k] = zeichen;
				}
			}
		}
	}
	printf("Startzustand:\n");
	PrintZelle(zellen, zeilen, reihen);
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
		return file;
	}
}
void StartZustandErstellen(char fileName[], char* modus) {
	char** zellen = { 0 };
	char p[1024];
	FILE* file = TextÖffnen(fileName, modus);
	char* zustand = (char*)malloc(100 * sizeof(char));
	printf("Zellen erstellen (q = beenden) : \n");

	while (true) {
		scanf("%[q, ,*]s", zustand);	
		fputs(zustand, file);
		fputs("\n", file);
		fgets(p, 1024, stdin);
		if (*zustand == 113) {
			break;
		}		
	}
	fclose(file);

}
int GetZeilen(char* dateiName) {
	int zeile = 0;
	int spalte = 0;
	FILE* file = TextÖffnen(dateiName, "rt");
	int zeichen;
	while ((zeichen = fgetc(file)) != EOF) {
		if (zeichen == 10) {
			zeile++;
		}
		if (zeichen == 113) {
			zeile--;
			break;
		}
	}
	zeile++;
	fclose(file);
	return zeile;
}
int GetReihen(char* dateiName) {
	int spalte = 0;
	FILE* file = TextÖffnen(dateiName, "rt");
	int zeichen;
	while ((zeichen = fgetc(file)) != EOF) {
		if (zeichen != 10) {
			spalte++;
		}
		else {
			break;
		}
	}
	fclose(file);
	return spalte;
}