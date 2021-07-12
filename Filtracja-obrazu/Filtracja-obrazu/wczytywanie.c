
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<wingdi.h>
#include<stdlib.h>
#include"funkcje.h"


int** wczytanie_Pliku_txt(char* nazwa_txt, int** macierz, int* rozmiar) {
	FILE* pliktxt = fopen(nazwa_txt, "r");//Otwarcie pliku txt w trybie do czytania
	if (pliktxt) {
		fscanf(pliktxt, "%d", rozmiar);//Pobranie liczby wierszy i zarazem kolumn(macierz musi byæ kwadratowa)zapisanej na pocz¹tku pliku txt.
		if (*rozmiar % 2 == 0) {//Sprawdzenie czy wartoœæ rozmiaru jest nieparzysta.
			return NULL;
		}
		else {
			macierz = (int**)malloc(sizeof(int*) * *rozmiar);//Alokacja pamiêci na macierz.
			for (int i = 0; i < *rozmiar; i++)
				macierz[i] = (int*)malloc(sizeof(int) * (*rozmiar));
			for (int i = 0; i < *rozmiar; i++) {
				for (int j = 0; j < *rozmiar; j++) {
					fscanf(pliktxt, "%d ", &macierz[i][j]);//Pobranie wartoœci z pliku
				}
			}
			return macierz;
		}
	}
	else return NULL;
}

unsigned char* wczytajBMP(char* nazwaPliku, BITMAPINFOHEADER* bitmapInfoHeader) {

	FILE* plik;
	BITMAPFILEHEADER bitmapFileHeader;//Do przechowywania pierwszego nag³ówka
	unsigned char* bitmapObraz;  //Do przechowywania obrazu

	plik = fopen(nazwaPliku, "rb");//Otwarcie w trybie do czytania, binarnym
	if (plik) {
		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, plik);//Sczytanie pierwszego nag³ówka pliku

		if (bitmapFileHeader.bfType != 0x4D42) {//Sprawdzenie czy jest to plik bmp (sprawdzaj¹c 'bitmap ID')
			fclose(plik);
			return NULL;
		}
		fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, plik);//Sczytanie drugiego nag³ówka
		fseek(plik, bitmapFileHeader.bfOffBits, SEEK_SET);//Przesuniêcie wskaŸnika na poczatek danych o obrazie (bfOffbits informuje)
		bitmapObraz = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);//Alokacja pamiêci na dane o obrazie

		if (!bitmapObraz){//Sprawdzenie czy alokacja siê uda³a
			free(bitmapObraz);
			fclose(plik);
			return NULL;
			}
		for (int wiersz = 0; wiersz < bitmapInfoHeader->biHeight; wiersz++) {//Wczytywanie z uwzglêdnieniem bajtów wyrównujacych
			// Sczytanie po jednym wierszu:
			fread(&bitmapObraz[wiersz * (bitmapInfoHeader->biWidth * 3)], sizeof(byte) * 3, bitmapInfoHeader->biWidth, plik);
				byte wyrownujacy;//Utworzenie zmiennej do przechowania i odrzucenia bajtów wyrównuj¹cych
				int wyrownanie = wyrownanie_bajtow(bitmapInfoHeader->biWidth);//Wyznaczenie liczby bajtów wyrównuj¹cych
				for (int p = 0; p < wyrownanie; p++) {
					fread(&wyrownujacy, sizeof(byte), 1, plik);//Sczytanie ewentualnych bajtów wyrównuj¹cych z koñca wiersza
				}
		}
		if (bitmapObraz == NULL) {//Sprawdzenie poprawnoœci wczytania danych
			fclose(plik);
			return NULL;
		}
		fclose(plik);//Zamkniêcie pliku 
		return bitmapObraz;//Zwrócenie obrazu w postaci tablicy unsigned char
	}
	else return NULL;
}
