
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<wingdi.h>
#include<stdlib.h>
#include"funkcje.h"


int** wczytanie_Pliku_txt(char* nazwa_txt, int** macierz, int* rozmiar) {
	FILE* pliktxt = fopen(nazwa_txt, "r");//Otwarcie pliku txt w trybie do czytania
	if (pliktxt) {
		fscanf(pliktxt, "%d", rozmiar);//Pobranie liczby wierszy i zarazem kolumn(macierz musi by� kwadratowa)zapisanej na pocz�tku pliku txt.
		if (*rozmiar % 2 == 0) {//Sprawdzenie czy warto�� rozmiaru jest nieparzysta.
			return NULL;
		}
		else {
			macierz = (int**)malloc(sizeof(int*) * *rozmiar);//Alokacja pami�ci na macierz.
			for (int i = 0; i < *rozmiar; i++)
				macierz[i] = (int*)malloc(sizeof(int) * (*rozmiar));
			for (int i = 0; i < *rozmiar; i++) {
				for (int j = 0; j < *rozmiar; j++) {
					fscanf(pliktxt, "%d ", &macierz[i][j]);//Pobranie warto�ci z pliku
				}
			}
			return macierz;
		}
	}
	else return NULL;
}

unsigned char* wczytajBMP(char* nazwaPliku, BITMAPINFOHEADER* bitmapInfoHeader) {

	FILE* plik;
	BITMAPFILEHEADER bitmapFileHeader;//Do przechowywania pierwszego nag��wka
	unsigned char* bitmapObraz;  //Do przechowywania obrazu

	plik = fopen(nazwaPliku, "rb");//Otwarcie w trybie do czytania, binarnym
	if (plik) {
		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, plik);//Sczytanie pierwszego nag��wka pliku

		if (bitmapFileHeader.bfType != 0x4D42) {//Sprawdzenie czy jest to plik bmp (sprawdzaj�c 'bitmap ID')
			fclose(plik);
			return NULL;
		}
		fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, plik);//Sczytanie drugiego nag��wka
		fseek(plik, bitmapFileHeader.bfOffBits, SEEK_SET);//Przesuni�cie wska�nika na poczatek danych o obrazie (bfOffbits informuje)
		bitmapObraz = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);//Alokacja pami�ci na dane o obrazie

		if (!bitmapObraz){//Sprawdzenie czy alokacja si� uda�a
			free(bitmapObraz);
			fclose(plik);
			return NULL;
			}
		for (int wiersz = 0; wiersz < bitmapInfoHeader->biHeight; wiersz++) {//Wczytywanie z uwzgl�dnieniem bajt�w wyr�wnujacych
			// Sczytanie po jednym wierszu:
			fread(&bitmapObraz[wiersz * (bitmapInfoHeader->biWidth * 3)], sizeof(byte) * 3, bitmapInfoHeader->biWidth, plik);
				byte wyrownujacy;//Utworzenie zmiennej do przechowania i odrzucenia bajt�w wyr�wnuj�cych
				int wyrownanie = wyrownanie_bajtow(bitmapInfoHeader->biWidth);//Wyznaczenie liczby bajt�w wyr�wnuj�cych
				for (int p = 0; p < wyrownanie; p++) {
					fread(&wyrownujacy, sizeof(byte), 1, plik);//Sczytanie ewentualnych bajt�w wyr�wnuj�cych z ko�ca wiersza
				}
		}
		if (bitmapObraz == NULL) {//Sprawdzenie poprawno�ci wczytania danych
			fclose(plik);
			return NULL;
		}
		fclose(plik);//Zamkni�cie pliku 
		return bitmapObraz;//Zwr�cenie obrazu w postaci tablicy unsigned char
	}
	else return NULL;
}
