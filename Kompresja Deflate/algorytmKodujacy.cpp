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
#include "../Kompresja Deflate/Header.h"

//algorytm kodujacy LZ77
std::vector<Token> AlgorytmKodujacyLZ77(const std::string& ipath)
{
	std::vector<Token> token_chain;	//deklaracja kontenera do przechowywania tokenow - literalow i par
	std::string fullText;	//zmienna przechowywujaca bloki pliku tekstowego
	std::ifstream ifile(ipath, std::ios::binary | std::ios::ate);
	if (ifile.good())
	{
		auto stream_size = ifile.tellg();	//konwersja ostatniego indeksu pliku na ilosc znakow
		if (stream_size == 0)		//obsluga pustego pliku
		{
			throw std::runtime_error("Pusty plik");
		}
		long long size = (long long)stream_size;
		ifile.seekg(0);		//powrot na pierwszy indeks pliku

		//maksymalny dostepny dystans wg dokumentu RFC 1951 to 32768, jednak poslugiwac
		//bede sie przedzialem 2 razy wiekszym
		unsigned int oneBajt = 32768;
		unsigned int twoBajt = 2 * oneBajt;
		long long cursor1 = twoBajt;
		fullText.resize(2 * twoBajt, 0);	//przygotowuja rozmiar stringa przed wczytaniem bloku danych

		//deklaracja kontenerow do tabliy hashujacej
		std::vector<long long> head(twoBajt, -1);
		std::vector<long long> prev(2 * twoBajt, -1);

		//wczytuje dane blokami po 65535 znakow, lub jesli w pliku juz nie ma tyle danych, to sprawdzam czy jest cokolwiek
		while (ifile.read(&fullText[twoBajt], twoBajt) || ifile.gcount() > 0)
		{
			//sprawdzam ile poprawnych danych zostalo wczytanych, jest to zabezpieczenie przed koncowka pliku
			int bytes_to_read = ifile.gcount() + twoBajt;
			while (cursor1 < bytes_to_read)
			{	
				//przeszukuja tablice hashujaca w celu znalezienia najlepszego dopasowania
				Token token = hashSearch(cursor1, fullText, head, prev, bytes_to_read);

				//jesli dlugosc jest mniejsza niz 3 przypisuje jako literal i ide do nastepnego znaku
				if (token.dlugosc < 3)
				{
					//fullText[cursor1] ma typ char, potrzebuje unsigned char, do obslugi wszystkich mozliwych znakow
					Token temp(static_cast<unsigned char>(fullText[cursor1]), true);
					headPrevUpdate(cursor1, fullText, head, prev, bytes_to_read);
					token_chain.push_back(temp);
					cursor1++;
				}
				//jesli jest wieksze niz 3 przypisuje jako para dlugosc dystans
				else if (token.dlugosc >= 3)
				{
					for (int i = 0; i < token.dlugosc; i++)
						headPrevUpdate(cursor1 + i, fullText, head, prev, bytes_to_read);
					token_chain.push_back(token);
					cursor1 += token.dlugosc;
				}
			}
			//doszedlem do konca okna danych, musze przesunac wszystkie indexy o wartosc twoBajt(65536)
			std::memcpy(&fullText[0], &fullText[twoBajt], twoBajt);
			prevSwap(prev);
			headSwap(head);
			cursor1 = twoBajt;
		}
	}
	else
	{
		std::cout << "Blad odczytu pliku przy kodowaniu" << std::endl;
	}
	Token eof(true);	//konstruktor znaku konca pliku
	token_chain.push_back(eof);
	return token_chain;
}