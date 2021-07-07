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
	//Rozdzielenie obrazu wej�ciowego na 3 tablice sk�adowych jego pikseli w kolejno�ci BGR z uwzgl�dnieniem wymiar�w obrazu
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
	unsigned char** temp = (unsigned char**)malloc(sizeof(unsigned char*) * wi);//Alokacja pomocniczej tablicy do zapisu przefiltrowanych sk�adowych
	for (int i = 0; i < wi; i++)
		temp[i] = (unsigned char*)malloc(sizeof(unsigned char) * ko);
	int margines = (rozm - 1) / 2;//"Margines" macierzy m�wi o tym ile brzegowych pikseli nie mo�e by� poprawnie przefiltrowanych.
	for (int i = margines; i < wi - margines; i++) {//Dwie pierwsze p�tle iteruj� tylko po pikselach spoza marginesu.
		for (int j = margines; j < ko - margines; j++) {
			int suma_wazona = 0;
			int sumaWag = 0;
			for (int k = -margines; k <= margines; k++) {//W tych p�tlach odbywa si� sumowanie mno�onych po odpowiednich s�siadach pikselach i odpowiadaj�cych im w macierzy wagom
				for (int l = -margines; l <= margines; l++) {//a tak�e sumowanie wag maski
					suma_wazona += (int)macierz[k + margines][l + margines] * (int)(obraz_skladowej[i + k][j + l]);//Rzutowanie element�w obrazu na int
					sumaWag += (int)macierz[k + margines][l + margines];
				}
			}
			int zaokraglenie = (round(suma_wazona / sumaWag));//Podzielenie odpowiednich sum, aby otrzyma� now� warto�� piksela
			zaokraglenie = zaokraglenie < 0 ? 0 : zaokraglenie;//Zabezpieczenie przed wyj�ciem z granic
			zaokraglenie = zaokraglenie > 255 ? 255 : zaokraglenie;
			temp[i][j] = (unsigned char)zaokraglenie;//Przypisanie obliczonej warto�ci do pomocniczej tablicy
		}
	}
	for (int i = 0; i < wi; i++) {
		for (int j = 0; j < ko; j++) {
			obraz_skladowej[i][j] = temp[i][j];//Przepisanie warto�ci z tymczasowej tablicy do tablicy sk�adowej.
		}
	}
}


unsigned char* polaczenie_skladowych(int ko, int wi, unsigned char** b, unsigned char** g, unsigned char** r) {
	unsigned char* obraz = (unsigned char*)malloc(sizeof(unsigned char) * 3 * wi * ko);//Alokacja jednowymiarowej tablicy na ca�o�� danych o obrazie
	int licznik = 0;
	//Scalenie trzech tablic B, G i R w jedn� z pikselami o odpowiedniej kolejno�ci.
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
	//Sprawdzenie czy warto�� szeroko�ci pomno�onej przez 3 (3 sk�adowe) jest niepodzielna przez 4
	if ((szerokosc * 3) % 4 != 0)
		wyrownanie = 4 - ((szerokosc * 3) % 4);//Je�eli niepodzielna obliczenie ilo�ci bajt�w do wyr�wnania do 4
	return wyrownanie;
}

bool zapis_obrazu(unsigned char* obraz, char* nazwa_pliku_wy, char* nazwa_pliku_we) {
	BITMAPFILEHEADER bitmapFileHeader_we;//Miejsce na pierwszy nag�owek pliku
	FILE* plik_wejsciowy = fopen(nazwa_pliku_we, "rb");//Otwarcie pliku wej�ciowego w celu przepisania nag��wk�w.
	if (plik_wejsciowy == NULL) return false;
	fread(&bitmapFileHeader_we, sizeof(BITMAPFILEHEADER), 1, plik_wejsciowy);
	BITMAPINFOHEADER bitmapinfoheader_we;//Miejsce na drugi nag��wek pliku
	fread(&bitmapinfoheader_we, sizeof(BITMAPINFOHEADER), 1, plik_wejsciowy);
	int roznica = bitmapFileHeader_we.bfOffBits - sizeof(BITMAPINFOHEADER);//bfOffBits informuje o liczbie bajt�w od pocz�tku pierwszego nag��wka
	roznica = roznica - sizeof(BITMAPFILEHEADER);//Odejmuj�c od tej warto�ci rozmiar 2 nag��wk�w otrzymujemy liczb� bajt�w do nadpisania.
	fclose(plik_wejsciowy);
	FILE* plik_wyjsciowy = fopen(nazwa_pliku_wy, "wb");//Utworzenie pliku wyj�ciowego
	if (plik_wyjsciowy == NULL) return false;
	fwrite(&bitmapFileHeader_we, sizeof(BITMAPFILEHEADER), 1, plik_wyjsciowy);//Przepisanie nag��wka 1.
	fwrite(&bitmapinfoheader_we, sizeof(BITMAPINFOHEADER), 1, plik_wyjsciowy);//Przepisanie nag��wka 2.
	byte null = 0;
	for (int i = 0; i < roznica; i++)//Nadpisanie bajt�w
		fwrite(&null, sizeof(byte), 1, plik_wyjsciowy);
	int padding = wyrownanie_bajtow(bitmapinfoheader_we.biWidth);
	for (int w = 0; w < bitmapinfoheader_we.biHeight; w++) {
		//Zapis pikseli, jeden wiersz na raz
		fwrite(&obraz[w * (bitmapinfoheader_we.biWidth * 3)], sizeof(byte), bitmapinfoheader_we.biWidth * 3, plik_wyjsciowy);
		//Dodanie wyr�wnuj�cych bajt�w do ka�dego wiersza
		for (int i = 0; i < padding; i++)
			fwrite(&null, sizeof(byte), 1, plik_wyjsciowy);
	}
	fclose(plik_wyjsciowy);
	return true;
}