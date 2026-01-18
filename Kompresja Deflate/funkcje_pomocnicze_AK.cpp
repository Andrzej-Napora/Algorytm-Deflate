#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <iomanip>
#include <cctype>
#include <vector>
#include <utility>
#include <fstream>
#include <queue>
#include <algorithm>
#include "../Kompresja Deflate/funkcje_pomocnicze_AK.h"

//funkcja szukajaca dopasowan w tablicy hash'ujacej
Token hashSearch(long long cursor1, const std::string& fullText,
	std::vector<long long>& head, std::vector<long long>& prev,
	int bytes_to_read)
{
	unsigned short dystans{};
	unsigned short dlugosc{};
	int max{};
	unsigned int maxDystans = 32768;

	//warunek potrzebny, zeby nie wyjsc poza zakres
	if (cursor1 + 2 < bytes_to_read)
	{
		//obliczam hash dla 3 kolejnych liter
		unsigned short hash = hashCalculation(fullText[cursor1], fullText[cursor1 + 1], fullText[cursor1 + 2]);
		{
			long long cursor2 = head[hash];
			int steps = 256;

			//wlasciwa petla szukajaca dopasowan
			//"cursor2 != -1" -- oznacza, ze dany hash zostal napotkany po raz pierwszy
			//"cursor1 - cursor2 < window" -- edge case, oznacza to, ze cursor 2 wyszedl poza zakres
			while (cursor2 != -1 && cursor1 - cursor2 < maxDystans && steps-- > 0)
			{
				bool match = false;
				//sprawdzanie matchu, jeden hash moze wskazywac na wiecej niz jedna trojke, dlatego zawsze trzeba sprawdzic,
				//czy trojka ktora znalezlismy w tablicy, to ta sama, na ktora wskazuje cursor1
				if (cursor1 + 2 <= bytes_to_read)
				{
					if (fullText[cursor2] == fullText[cursor1] &&
						fullText[cursor2 + 1] == fullText[cursor1 + 1] &&
						fullText[cursor2 + 2] == fullText[cursor1 + 2])
					{
						match = true;
					}
				}
				if (match)
				{
					unsigned short count{};
					int temp_cursor1 = cursor1;
					int temp_cursor2 = cursor2;
					//gdy znaleszlismy dopasowanie, sprawdzamy dlugosc tego dopasowania
					while (temp_cursor1 < bytes_to_read && fullText[temp_cursor1++] == fullText[temp_cursor2++])
						count++;
					if (count > max)
					{
						//przypisywanie znalezionych wielksoci do zmiennych
						if (count >= 258)	//standard deflate opisany w tablicy RFC 1951 przewiduje dlugosc tylko do 258
							count = 258;
						max = count;
						dlugosc = max;
						dystans = cursor1 - cursor2;
						if (dlugosc >= 258)
							break;
					}
				}

				if (cursor2 == prev[cursor2])	//safety check, pozostalosc po debuggowaniu, 
					break;						//zapobiega zapetleniu, w poprawnym kodzie nie potrzebny
				else
					cursor2 = prev[cursor2];	//koniec petli i przypisanie zmiennej cursor2 
			}									//poprzedniego pasujacego indexu dla danego hasha
		}
	}
	//zapisywanie znalezionych wielkosci do tokenu
	Token token(dystans, dlugosc, false);
	return token;
}

//funkcja aktualizujaca tablice hash'ujaca; head[hash] przypisuje aktualna pozycje, 
//prev[cursor1] przypisuje poprzednia wartosc head[hash]
void headPrevUpdate(long long cursor1, const std::string& fullText,
	std::vector<long long>& head, std::vector<long long>& prev, int bytes_to_read)
{
	if (cursor1 + 2 < bytes_to_read)//warunek zapobiegajacy wyjsciu poza zakres
	{
		unsigned short hash = hashCalculation(fullText[cursor1], fullText[cursor1 + 1], fullText[cursor1 + 2]);

		if (head[hash] == -1)
			head[hash] = cursor1;
		else
		{
			prev[cursor1] = head[hash];
			head[hash] = cursor1;
		}
	}
}

//funkcja przesuwajaca tablice prev, zgodnie z przesuwaniem okna danych
void prevSwap(std::vector<long long>& prev)
{
	//indeksy w stringu fullText przesuwaja sie o oneBajt, wiec indeksy w prev, rowniez musza
		//sie przesunac o ta wartosc, te ktore wypadna poza okno sa oznaczone jako nieznane, czyli
		//przypisywana jest im wartosc -1
	unsigned int oneBajt = 32768;
	unsigned int twoBajt = 2*oneBajt;
	for (int i = 0; i < twoBajt; i++)
	{
		prev[i] = prev[twoBajt + i];
		if (prev[i] != -1)
			prev[i] -= twoBajt;
		prev[twoBajt + i] = -1;
	}
}

//funkcja przesuwajaca tablice head, zgodnie z przesuwaniem okna danych
void headSwap(std::vector<long long>& head)
{
	//w przeciwienstwie do prev[], vector head nie przesuwa sie, zamiast tego, poniewaz head[]
	//przyjmuje indeksy jako wartosci swoich elementow, oneBajt odejmowany jest wlasnie od wartosci kazdego elementu
	unsigned int oneBajt = 32768;
	unsigned int twoBajt = 2 * oneBajt;
	for (int i = 0; i < twoBajt; i++)
	{
		if (head[i] != -1)
		{
			head[i] -= twoBajt;
			if (head[i] < 0)
				head[i] = -1;
		}
	}
}

//mieszanie bitowe hash; w mojej implementacji najstarsze 8 bitow to pierwszy znak, najmlodsze 8 bitow to drugi znak,
//a trzeci znak jest umieszczany na pozycjach 4-12 i mieszany z pozostalymi operatorem XOR
unsigned short hashCalculation(const unsigned char c1, const unsigned char c2, const unsigned char c3)
{
	unsigned short twoBajt{};
	twoBajt |= c1;
	twoBajt <<= 8;
	unsigned short temp{};
	temp |= c2;
	twoBajt |= temp;
	temp = 0;
	temp |= c3;
	temp <<= 4;
	twoBajt ^= temp;
	return twoBajt;

}