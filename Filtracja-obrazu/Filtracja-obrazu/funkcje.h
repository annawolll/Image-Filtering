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

/** Zmienna wykorzystywana do sczytywania i nadpisywania bajt�w wyr�wnuj�cych wiersze w pliku BMP.*/
typedef unsigned char byte;

/**Funkcja wczytuj�ca macierz maski z pliku txt, w jego pierwszej linijce zawarty jest rozmiar macierzy.
* @param nazwa_txt Nazwa pliku txt.
* @param macierz Pusta macierz, do kt�rej zapisana b�dzie ta z pliku.
* @param rozmiar Rozmiar macierzy, kt�ry b�dzie sczytany z pierwszej linijki pliku.
* @return Adres zwracanej macierzy.
*/
int** wczytanie_Pliku_txt(char* nazwa_txt, int** macierz, int* rozmiar);

/**Funkcja otwieraj�ca plik BMP, wczytuj�ca z niego nag��wek oraz tablic� danych o pikselach, zwracaj�ca t� tablic�.
* @param nazwaPliku Nazwa pliku do wczytania.
* @param bitmapInfoHeader Puste miejsce do zapisu nag��wka pliku.
* @return Zwracana tablica sk�adowych RGB pikseli.
*/
unsigned char* wczytajBMP(char* nazwaPliku, BITMAPINFOHEADER* bitmapInfoHeader);

/**Funkcja przyjmuj�ca jednowymiarow� tablic� sk�adowych pikseli RGB (w kolejno�ci BGR)
i dziel�ca j� na dwuwymiarowe tablice poszczeg�lnych sk�adowych o wymiarach obrazu pobranych z nag��wka.
* @param wier Liczba wierszy obrazu (wysoko��).
* @param kol Liczba kolumn obrazu (szeroko��).
* @param obraz Wczytany z pliku bmp obraz jako jednowymiarowa tablica pikseli.
* @param b Pusta dwuwymiarowa tablica, do kt�rej zapisane b�d� dane o sk�adowej B (blue) pikseli obrazu.
* @param g Pusta dwuwymiarowa tablica, do kt�rej zapisane b�d� dane o sk�adowej G (green) pikseli obrazu.
* @param r Pusta dwuwymiarowa tablica, do kt�rej zapisane b�d� dane o sk�adowej R (red) pikseli obrazu.
*/
void podzial_na_skladowe(int wier, int kol, unsigned char* obraz, unsigned char** b, unsigned char** g, unsigned char** r);

/**Funkcja przeprowadzaj�ca filtracj� przekazanej do niej tablicy sk�adowej poprzez
obliczanie sumy wag maski (macierzy) oraz sumy wa�onej s�siednich do rozpatrywanego miejsca pikseli
oraz podzielenie sumy wa�onej pzez sum� wag.
* @param obraz_skladowej Przekazana do funkcji dwuwymiarowa tablica jednej ze sk�adowych.
* @param wi Liczba wierszy obrazu.
* @param ko Liczba kolumn obrazu.
* @param macierz Wczytana z pliku txt macierz okre�laj�ca filtracj�.
* @param rozm Rozmiar macierzy filtracyjnej.
*/
void filtracja(unsigned char** obraz_skladowej, int wi, int ko, int** macierz, int rozm);

/**Funkcja przyjmuj�ca dwuwymiarowe tablice ka�dej ze sk�adowych
i ��cz�ca je w jednowymiarow� tablic� obrazu o kolejno�ci sk�adowych BGR (dogodnie do zapisu do pliku BMP).
* @param ko Liczba kolumn obrazu.
* @param wi Liczba wiersz�w obrazu.
* @param b Dwuwymiarowa tablica sk�adowej B (blue) obrazu.
* @param g Dwuwymiarowa tablica sk�adowej G (green) obrazu.
* @param r Dwuwymiarowa tablica sk�adowej R (red) obrazu.
* @return Zwracany obraz wszystkich sk�adowych (BGR) gotowy do zapisu do pliku.
*/
unsigned char* polaczenie_skladowych(int ko, int wi, unsigned char** b, unsigned char** g, unsigned char** r);

/**Funkcja obliczaj�ca liczb� bajt�w wyr�wnuj�cych szeroko�� obrazu do 4.
Informacja ta jest potrzebna przy wczytywaniu obrazu jak i przy jego zapisywaniu.
* @param szerokosc Szeroko�� obrazu (liczba kolumn).
*/
int wyrownanie_bajtow(int szerokosc);

/**Funkcja zapisuj�ca przefiltrowany obraz do nowego pliku BMP
wykorzystuj�ca nag��wki tego, kt�ry zosta� przekazany do filtracji (dane w nag��wkach nie zmieniaj� si�).
* @param obraz Przefiltrowany ca�kowity obraz do zapisu do pliku.
* @param nazwa_pliku_wy Nazwa tworzonego pliku wyj�ciowego, do kt�rego b�dzie zapisywany przefiltrowany obraz.
* @param nazwa_pliku_we Nazwa przekazywanego do programu pliku z obrazem do przefiltrowania.
* @return True je�eli powiedzie si� zapis tablicy obrazu do pliku, False je�eli nie powiedzie si�.
*/
bool zapis_obrazu(unsigned char* obraz, char* nazwa_pliku_wy, char* nazwa_pliku_we);

#endif /* FUNKCJE_H */