#include"funkcje.h"
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<wingdi.h>
#include<math.h>
#include<stdbool.h>
#include<stdlib.h>

void podzial_na_skladowe(int wier, int kol, unsigned char* obraz, unsigned char** b, unsigned char** g, unsigned char** r) {
	int licznik = 0;
	//Rozdzielenie obrazu wejœciowego na 3 tablice sk³adowych jego pikseli w kolejnoœci BGR z uwzglêdnieniem wymiarów obrazu
	for (int i = 0; i < wier; i++) {
		for (int j = 0; j < kol; j++) {
			b[i][j] = obraz[licznik];
			licznik++;
			g[i][j] = obraz[licznik];
			licznik++;
			r[i][j] = obraz[licznik];
			licznik++;
		}
	}
}

void filtracja(unsigned char** obraz_skladowej, int wi, int ko, int** macierz, int rozm) {
	unsigned char** temp = (unsigned char**)malloc(sizeof(unsigned char*) * wi);//Alokacja pomocniczej tablicy do zapisu przefiltrowanych sk³adowych
	for (int i = 0; i < wi; i++)
		temp[i] = (unsigned char*)malloc(sizeof(unsigned char) * ko);
	int margines = (rozm - 1) / 2;//"Margines" macierzy mówi o tym ile brzegowych pikseli nie mo¿e byæ poprawnie przefiltrowanych.
	for (int i = margines; i < wi - margines; i++) {//Dwie pierwsze pêtle iteruj¹ tylko po pikselach spoza marginesu.
		for (int j = margines; j < ko - margines; j++) {
			int suma_wazona = 0;
			int sumaWag = 0;
			for (int k = -margines; k <= margines; k++) {//W tych pêtlach odbywa siê sumowanie mno¿onych po odpowiednich s¹siadach pikselach i odpowiadaj¹cych im w macierzy wagom
				for (int l = -margines; l <= margines; l++) {//a tak¿e sumowanie wag maski
					suma_wazona += (int)macierz[k + margines][l + margines] * (int)(obraz_skladowej[i + k][j + l]);//Rzutowanie elementów obrazu na int
					sumaWag += (int)macierz[k + margines][l + margines];
				}
			}
			int zaokraglenie = (round(suma_wazona / sumaWag));//Podzielenie odpowiednich sum, aby otrzymaæ now¹ wartoœæ piksela
			zaokraglenie = zaokraglenie < 0 ? 0 : zaokraglenie;//Zabezpieczenie przed wyjœciem z granic
			zaokraglenie = zaokraglenie > 255 ? 255 : zaokraglenie;
			temp[i][j] = (unsigned char)zaokraglenie;//Przypisanie obliczonej wartoœci do pomocniczej tablicy
		}
	}
	for (int i = 0; i < wi; i++) {
		for (int j = 0; j < ko; j++) {
			obraz_skladowej[i][j] = temp[i][j];//Przepisanie wartoœci z tymczasowej tablicy do tablicy sk³adowej.
		}
	}
}


unsigned char* polaczenie_skladowych(int ko, int wi, unsigned char** b, unsigned char** g, unsigned char** r) {
	unsigned char* obraz = (unsigned char*)malloc(sizeof(unsigned char) * 3 * wi * ko);//Alokacja jednowymiarowej tablicy na ca³oœæ danych o obrazie
	int licznik = 0;
	//Scalenie trzech tablic B, G i R w jedn¹ z pikselami o odpowiedniej kolejnoœci.
	for (int i = 0; i < wi; i++) {
		for (int j = 0; j < ko; j++) {
			obraz[licznik] = b[i][j];
			licznik++;
			obraz[licznik] = g[i][j];
			licznik++;
			obraz[licznik] = r[i][j];
			licznik++;
		}
	}
	if (!obraz) printf("Niepoprawna alokacja obrazu zwrotnego.\n");
	return obraz;
}

int wyrownanie_bajtow(int szerokosc) {
	int wyrownanie = 0;
	//Sprawdzenie czy wartoœæ szerokoœci pomno¿onej przez 3 (3 sk³adowe) jest niepodzielna przez 4
	if ((szerokosc * 3) % 4 != 0)
		wyrownanie = 4 - ((szerokosc * 3) % 4);//Je¿eli niepodzielna obliczenie iloœci bajtów do wyrównania do 4
	return wyrownanie;
}

bool zapis_obrazu(unsigned char* obraz, char* nazwa_pliku_wy, char* nazwa_pliku_we) {
	BITMAPFILEHEADER bitmapFileHeader_we;//Miejsce na pierwszy nag³owek pliku
	FILE* plik_wejsciowy = fopen(nazwa_pliku_we, "rb");//Otwarcie pliku wejœciowego w celu przepisania nag³ówków.
	if (plik_wejsciowy == NULL) return false;
	fread(&bitmapFileHeader_we, sizeof(BITMAPFILEHEADER), 1, plik_wejsciowy);
	BITMAPINFOHEADER bitmapinfoheader_we;//Miejsce na drugi nag³ówek pliku
	fread(&bitmapinfoheader_we, sizeof(BITMAPINFOHEADER), 1, plik_wejsciowy);
	int roznica = bitmapFileHeader_we.bfOffBits - sizeof(BITMAPINFOHEADER);//bfOffBits informuje o liczbie bajtów od pocz¹tku pierwszego nag³ówka
	roznica = roznica - sizeof(BITMAPFILEHEADER);//Odejmuj¹c od tej wartoœci rozmiar 2 nag³ówków otrzymujemy liczbê bajtów do nadpisania.
	fclose(plik_wejsciowy);
	FILE* plik_wyjsciowy = fopen(nazwa_pliku_wy, "wb");//Utworzenie pliku wyjœciowego
	if (plik_wyjsciowy == NULL) return false;
	fwrite(&bitmapFileHeader_we, sizeof(BITMAPFILEHEADER), 1, plik_wyjsciowy);//Przepisanie nag³ówka 1.
	fwrite(&bitmapinfoheader_we, sizeof(BITMAPINFOHEADER), 1, plik_wyjsciowy);//Przepisanie nag³ówka 2.
	byte null = 0;
	for (int i = 0; i < roznica; i++)//Nadpisanie bajtów
		fwrite(&null, sizeof(byte), 1, plik_wyjsciowy);
	int padding = wyrownanie_bajtow(bitmapinfoheader_we.biWidth);
	for (int w = 0; w < bitmapinfoheader_we.biHeight; w++) {
		//Zapis pikseli, jeden wiersz na raz
		fwrite(&obraz[w * (bitmapinfoheader_we.biWidth * 3)], sizeof(byte), bitmapinfoheader_we.biWidth * 3, plik_wyjsciowy);
		//Dodanie wyrównuj¹cych bajtów do ka¿dego wiersza
		for (int i = 0; i < padding; i++)
			fwrite(&null, sizeof(byte), 1, plik_wyjsciowy);
	}
	fclose(plik_wyjsciowy);
	return true;
}