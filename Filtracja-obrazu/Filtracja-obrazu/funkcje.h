//#pragma once
#ifndef FUNKCJE_H
#define FUNKCJE_H
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<wingdi.h>
#include<math.h>
#include<stdbool.h>
#include<stdlib.h>

/** Zmienna wykorzystywana do sczytywania i nadpisywania bajtów wyrównuj¹cych wiersze w pliku BMP.*/
typedef unsigned char byte;

/**Funkcja wczytuj¹ca macierz maski z pliku txt, w jego pierwszej linijce zawarty jest rozmiar macierzy.
* @param nazwa_txt Nazwa pliku txt.
* @param macierz Pusta macierz, do której zapisana bêdzie ta z pliku.
* @param rozmiar Rozmiar macierzy, który bêdzie sczytany z pierwszej linijki pliku.
* @return Adres zwracanej macierzy.
*/
int** wczytanie_Pliku_txt(char* nazwa_txt, int** macierz, int* rozmiar);

/**Funkcja otwieraj¹ca plik BMP, wczytuj¹ca z niego nag³ówek oraz tablicê danych o pikselach, zwracaj¹ca tê tablicê.
* @param nazwaPliku Nazwa pliku do wczytania.
* @param bitmapInfoHeader Puste miejsce do zapisu nag³ówka pliku.
* @return Zwracana tablica sk³adowych RGB pikseli.
*/
unsigned char* wczytajBMP(char* nazwaPliku, BITMAPINFOHEADER* bitmapInfoHeader);

/**Funkcja przyjmuj¹ca jednowymiarow¹ tablicê sk³adowych pikseli RGB (w kolejnoœci BGR)
i dziel¹ca j¹ na dwuwymiarowe tablice poszczególnych sk³adowych o wymiarach obrazu pobranych z nag³ówka.
* @param wier Liczba wierszy obrazu (wysokoœæ).
* @param kol Liczba kolumn obrazu (szerokoœæ).
* @param obraz Wczytany z pliku bmp obraz jako jednowymiarowa tablica pikseli.
* @param b Pusta dwuwymiarowa tablica, do której zapisane bêd¹ dane o sk³adowej B (blue) pikseli obrazu.
* @param g Pusta dwuwymiarowa tablica, do której zapisane bêd¹ dane o sk³adowej G (green) pikseli obrazu.
* @param r Pusta dwuwymiarowa tablica, do której zapisane bêd¹ dane o sk³adowej R (red) pikseli obrazu.
*/
void podzial_na_skladowe(int wier, int kol, unsigned char* obraz, unsigned char** b, unsigned char** g, unsigned char** r);

/**Funkcja przeprowadzaj¹ca filtracjê przekazanej do niej tablicy sk³adowej poprzez
obliczanie sumy wag maski (macierzy) oraz sumy wa¿onej s¹siednich do rozpatrywanego miejsca pikseli
oraz podzielenie sumy wa¿onej pzez sumê wag.
* @param obraz_skladowej Przekazana do funkcji dwuwymiarowa tablica jednej ze sk³adowych.
* @param wi Liczba wierszy obrazu.
* @param ko Liczba kolumn obrazu.
* @param macierz Wczytana z pliku txt macierz okreœlaj¹ca filtracjê.
* @param rozm Rozmiar macierzy filtracyjnej.
*/
void filtracja(unsigned char** obraz_skladowej, int wi, int ko, int** macierz, int rozm);

/**Funkcja przyjmuj¹ca dwuwymiarowe tablice ka¿dej ze sk³adowych
i ³¹cz¹ca je w jednowymiarow¹ tablicê obrazu o kolejnoœci sk³adowych BGR (dogodnie do zapisu do pliku BMP).
* @param ko Liczba kolumn obrazu.
* @param wi Liczba wierszów obrazu.
* @param b Dwuwymiarowa tablica sk³adowej B (blue) obrazu.
* @param g Dwuwymiarowa tablica sk³adowej G (green) obrazu.
* @param r Dwuwymiarowa tablica sk³adowej R (red) obrazu.
* @return Zwracany obraz wszystkich sk³adowych (BGR) gotowy do zapisu do pliku.
*/
unsigned char* polaczenie_skladowych(int ko, int wi, unsigned char** b, unsigned char** g, unsigned char** r);

/**Funkcja obliczaj¹ca liczbê bajtów wyrównuj¹cych szerokoœæ obrazu do 4.
Informacja ta jest potrzebna przy wczytywaniu obrazu jak i przy jego zapisywaniu.
* @param szerokosc Szerokoœæ obrazu (liczba kolumn).
*/
int wyrownanie_bajtow(int szerokosc);

/**Funkcja zapisuj¹ca przefiltrowany obraz do nowego pliku BMP
wykorzystuj¹ca nag³ówki tego, który zosta³ przekazany do filtracji (dane w nag³ówkach nie zmieniaj¹ siê).
* @param obraz Przefiltrowany ca³kowity obraz do zapisu do pliku.
* @param nazwa_pliku_wy Nazwa tworzonego pliku wyjœciowego, do którego bêdzie zapisywany przefiltrowany obraz.
* @param nazwa_pliku_we Nazwa przekazywanego do programu pliku z obrazem do przefiltrowania.
* @return True je¿eli powiedzie siê zapis tablicy obrazu do pliku, False je¿eli nie powiedzie siê.
*/
bool zapis_obrazu(unsigned char* obraz, char* nazwa_pliku_wy, char* nazwa_pliku_we);

#endif /* FUNKCJE_H */