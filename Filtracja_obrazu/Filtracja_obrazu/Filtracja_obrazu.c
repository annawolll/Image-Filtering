///Projekt Filtracja obrazu
//Anna Wołoszyn
#define _CRT_SECURE_NO_WARNINGS//dla Visual Studio
#include<stdio.h>
#include<windows.h>//?czy potrzebne
#include<wingdi.h>
#include<math.h>
#include<stdbool.h>
#include<stdlib.h>//niekonieczne


int** wczytanie_Pliku_txt(char* nazwa_txt, int** macierz, int* rozmiar) {
	FILE* pliktxt = fopen(nazwa_txt, "r");
	if (pliktxt) {
		fscanf(pliktxt, "%d", rozmiar);//pobranie liczby wierszy i zarazem kolumn(macierz kwadratowa) zapisanej na początku pliku txt
		if (*rozmiar % 2 == 0) {
			printf("Niepoprawna postac filtru, macierz powinna miec nieparzysta liczbe kolumn i wierszy.");
			return NULL;
		}
		else {
			macierz = (int**)malloc(sizeof(int*) * *rozmiar);
			for (int i = 0; i < *rozmiar; i++)
				macierz[i] = (int*)malloc(sizeof(int) * (*rozmiar));
			for (int i = 0; i < *rozmiar; i++) {
				for (int j = 0; j < *rozmiar; j++) {
					fscanf(pliktxt, "%d ", &macierz[i][j]);
				}
			}
			printf("Macierz maski sie wczytala\n");
			return macierz;
		}
	}
	else printf("Nie znaleziono pliku.");
	return NULL;
}

typedef unsigned char byte;//,,pusty bajt"

unsigned char* wczytajBMP(char* nazwaPliku, BITMAPINFOHEADER* bitmapInfoHeader) {

	FILE* filePtr;
	BITMAPFILEHEADER bitmapFileHeader;
	unsigned char* bitmapImage;  //przechowuje obraz
	int imageIdx = 0;  //licznik indeksow obrazu

	filePtr = fopen(nazwaPliku, "rb");//otwarcie w trybie do czytania, binarnym
	if (filePtr == NULL) {
		printf("Nie znaleziono pliku.");
		return NULL;
	}
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);//sczytanie nagłówka pliku

	if (bitmapFileHeader.bfType != 0x4D42) {//sprawdzenie czy jest to plik bmp (sprawdzając 'bitmap ID')
		fclose(filePtr);
		printf("Nie jest to bmp.");
		return NULL;
	}
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);//sczytanie infonagłówka
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);//przesuniecie wskaznika na poczatek danych o obrazie (bfoffbits informuje)
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);//alokacja pamieci na dane o obrazie

	if (!bitmapImage) {//sprawdzenie czy alokacja sie udala
		free(bitmapImage);
		fclose(filePtr);
		printf("Blad przy alokacji pamieci wejsciowego obrazu.");
		return NULL;
	}

	for (int wiersz = 0; wiersz < bitmapInfoHeader->biHeight; wiersz++) {//wczytywanie z uwzgl.bajtow wyrownujacych
		// Sczytanie po jednym wierszu
		fread(&bitmapImage[wiersz * (bitmapInfoHeader->biWidth * 3)], sizeof(byte) * 3, bitmapInfoHeader->biWidth, filePtr);
		byte wyrownujacy;// Sczytanie i odrzucenie bajtow wyrownujacych
		int wyrownanie = wyrownanie_bajtow(bitmapInfoHeader->biWidth);
		for (int p = 0; p < wyrownanie; p++) {
			fread(&wyrownujacy, sizeof(byte), 1, filePtr);
		}
	}
	if (bitmapImage == NULL) {//sprawdzenie poprawnosci wczytania danych
		printf("Niepoprawne przeczytanie danych bitmapy");
		fclose(filePtr);
		return NULL;
	}

	printf("Poprawne zadzialanie wczytania bmp.\n");//test dzialania
	fclose(filePtr);//zamkniecie pliku i zwrocenie obrazu w postaci tablicy unsigned char
	return bitmapImage;
}


void podzial_na_skladowe(int wier, int kol, unsigned char* obraz, unsigned char** b, unsigned char** g, unsigned char** r) {
	int licznik = 0;
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
	//próby:
	printf("piksel G o przypadkowym numerze: %c\n", g[3][4]);
	printf("piksel z 'obraz': %c\n", obraz[10]);
	printf("Wartosc piksela B(0,0): %c\n", b[0][0]);
}

void filtracja(unsigned char** obraz_skladowej, int wi, int ko, int** macierz, int rozm) {
	unsigned char** temp = (unsigned char**)malloc(sizeof(unsigned char*) * wi);
	for (int i = 0; i < wi; i++)
		temp[i] = (unsigned char*)malloc(sizeof(unsigned char) * ko);
	int margines = (rozm - 1) / 2;
	//printf("Funkcja filtrujaca tablice skladowych, przed piksel ma wartosc: %c\n", obraz_skladowej[1][1]);
	for (int i = margines; i < wi - margines; i++) {
		for (int j = margines; j < ko - margines; j++) {
			int suma_wazona = 0;
			int sumaWag = 0;
			for (int k = -margines; k <= margines; k++) {
				for (int l = -margines; l <= margines; l++) {
					suma_wazona += (int)macierz[k+margines][l+margines] * (int)(obraz_skladowej[i+k][j+l]);
					sumaWag += (int)macierz[k+margines][l+margines];
				}
			}
			int zaokraglenie = (round(suma_wazona / sumaWag));
			zaokraglenie = zaokraglenie < 0 ? 0 : zaokraglenie;
			zaokraglenie = zaokraglenie > 255 ? 255 : zaokraglenie;
			temp[i][j] = (unsigned char)zaokraglenie;
		}
	}
	for (int i = 0; i < wi; i++) {
		for (int j = 0; j < ko; j++) {
			obraz_skladowej[i][j] = temp[i][j];
		}
	}
	printf("Po piksel ma wartosc: %c\n", obraz_skladowej[1][1]);//sprawdzenie czy nastapila zmiana wartosci
}

unsigned char* polaczenie_skladowych(int ko, int wi, unsigned char** b, unsigned char** g, unsigned char** r) {
	unsigned char* obraz = (unsigned char*)malloc(sizeof(unsigned char) * 3 * wi * ko);
	int licznik = 0;
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
	if (obraz) printf("Poprawna alokacja obrazu zwrotnego.\n");
	return obraz;
}

int wyrownanie_bajtow(int width) {
	int wyrownanie = 0;
	// Jezeli wartosc szerokosci nie jest podzielna przez 4
	if ((width * 3) % 4 != 0)
		wyrownanie = 4 - ((width * 3) % 4);// Obliczenie ilosci bajtow do wyrownania do 4
	return wyrownanie;
}

bool zapis_obrazu(unsigned char* obraz, char* nazwa_pliku_wy, char* nazwa_pliku_we) {
	BITMAPFILEHEADER bitmapFileHeader_we;
	FILE* plik_wejsciowy = fopen(nazwa_pliku_we, "rb");
	if (plik_wejsciowy == NULL) {
		printf("Nie znaleziono pliku.");
		return false;
	}
	fread(&bitmapFileHeader_we, sizeof(BITMAPFILEHEADER), 1, plik_wejsciowy);
	BITMAPINFOHEADER bitmapinfoheader_we;
	fread(&bitmapinfoheader_we, sizeof(BITMAPINFOHEADER), 1, plik_wejsciowy);
	int diff = bitmapFileHeader_we.bfOffBits - sizeof(BITMAPINFOHEADER);
	diff = diff - sizeof(BITMAPFILEHEADER);

	//printf("difference: %d ",diff);
	fclose(plik_wejsciowy);
	FILE* plik_wyjsciowy = fopen(nazwa_pliku_wy, "wb");
	if (plik_wyjsciowy == NULL) return false;
	fwrite(&bitmapFileHeader_we, sizeof(BITMAPFILEHEADER), 1, plik_wyjsciowy);
	fwrite(&bitmapinfoheader_we, sizeof(BITMAPINFOHEADER), 1, plik_wyjsciowy);
	byte null = 0;
	for(int i = 0; i < diff; i++)//84
		fwrite(&null, sizeof(byte), 1, plik_wyjsciowy);
	int padding = wyrownanie_bajtow(bitmapinfoheader_we.biWidth);
	for (int w = 0; w < bitmapinfoheader_we.biHeight; w++) {
		// Zapis pikseli, jeden wiersz na raz
		fwrite(&obraz[w * (bitmapinfoheader_we.biWidth * 3)], sizeof(byte), bitmapinfoheader_we.biWidth*3, plik_wyjsciowy);
		// Dodanie padding bajtów do kazdego wiersza
		for (int i = 0; i < padding; i++)
			fwrite(&null, sizeof(byte), 1, plik_wyjsciowy);
	}
	fclose(plik_wyjsciowy);
	return true;
}
int main() {
	BITMAPINFOHEADER* infonaglowek = malloc(sizeof(BITMAPINFOHEADER));
	unsigned char* obraz = wczytajBMP("lena.bmp", infonaglowek);
	int wiersze = infonaglowek->biWidth, kolumny = infonaglowek->biHeight;
	int** macierz = NULL;
	int rozmiar_mac = 0;
	macierz = wczytanie_Pliku_txt("filtr_usredniajacy.txt", macierz, &rozmiar_mac);
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
	if (zapis_obrazu(obraz, "obraz_wyjsciowy.bmp", "lena.bmp"))
		printf("Udalo sie zapisac obraz.\n");

	//Zwalnianie pamieci
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