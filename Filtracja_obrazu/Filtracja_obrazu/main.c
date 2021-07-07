///Projekt Filtracja obrazu
//Anna Wołoszyn
#define _CRT_SECURE_NO_WARNINGS//dla Visual Studio
#include<stdio.h>
#include<wingdi.h>
#include<stdbool.h>
#include<stdlib.h>
#include"funkcje.h"



int main(int argc, char**argv) {
	BITMAPINFOHEADER* infonaglowek = malloc(sizeof(BITMAPINFOHEADER));
	unsigned char* obraz = wczytajBMP(argv[1], infonaglowek);//Wczytanie pliku BMP o nazwie podanej jako pierwszy argument w konsoli
	int wiersze = infonaglowek->biHeight, kolumny = infonaglowek->biWidth;
	int** macierz = NULL;
	int rozmiar_mac = 0;
	macierz = wczytanie_Pliku_txt(argv[2], macierz, &rozmiar_mac);//Wczytanie macierzy filtracyjnej z pliku txt.
	if (macierz) {
		//Alokacja tablic na poszczególne składowe pikseli
		unsigned char** b = (unsigned char**)malloc(sizeof(unsigned char*) * wiersze);
		for (int i = 0; i < wiersze; i++)
			b[i] = (unsigned char*)malloc(sizeof(unsigned char) * kolumny);
		unsigned char** g = (unsigned char**)malloc(sizeof(unsigned char*) * wiersze);
		for (int i = 0; i < wiersze; i++)
			g[i] = (unsigned char*)malloc(sizeof(unsigned char) * kolumny);
		unsigned char** r = (unsigned char**)malloc(sizeof(unsigned char*) * wiersze);
		for (int i = 0; i < wiersze; i++)
			r[i] = (unsigned char*)malloc(sizeof(unsigned char) * kolumny);

		podzial_na_skladowe(wiersze, kolumny, obraz, b, g, r);
		filtracja(b, wiersze, kolumny, macierz, rozmiar_mac);
		filtracja(g, wiersze, kolumny, macierz, rozmiar_mac);
		filtracja(r, wiersze, kolumny, macierz, rozmiar_mac);
		obraz = polaczenie_skladowych(kolumny, wiersze, b, g, r);
		if (zapis_obrazu(obraz, "obraz_wyjsciowy.bmp", argv[1]))//Zapis obrazu do pliku wyjściowego o nazwie podanej jako drugi argument w konsoli 
			printf("Udalo sie zapisac przefiltrowany obraz.\n");
		//Zwalnianie pamięci:
		for (int w = 0; w < rozmiar_mac; w++)
			free(macierz[w]);
		free(macierz);
		free(obraz);
		free(infonaglowek);

		for (int i = 0; i < wiersze; i++) {
			free(b[i]);
			free(g[i]);
			free(r[i]);
		}
		free(b);
		free(g);
		free(r);
	}
	else printf("Macierzy nie znaleziono lub jest bledna.\n");

}