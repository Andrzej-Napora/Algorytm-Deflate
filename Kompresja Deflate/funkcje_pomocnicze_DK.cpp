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

//funkcja przesuwajaca indexy zmiennej decoded, oraz zapisujaca do pliku zawartosc zmiennej decoded
//jesli doszlo do jej przepelnienia
void decodedSwap(std::string& dekoded, const unsigned int oneBajt, bool& first, std::ofstream& ofile)
{
	if (dekoded.size() > 2 * oneBajt)
	{
		int size = dekoded.size();
		//przy pierwszym przejsciu czytamy wartosci od indexu 0
		if (first)
		{
			first = false;
			ofile.write(&dekoded[0], size);
		}
		//przy nastepnych przejsciach czytamy wartosci od indexu oneBajt
		else
			ofile.write(&dekoded[oneBajt], size - oneBajt);

		//przenosimy zawartosc zmiennej decoded z drugiej polowki do pierwszej,
		//z ktorej wartosci odczytywac bedzie tablica hashujaca,
		//nastepnie zmiejszamy jej rozmiar o polowe 
		for (int i = 0; i < oneBajt; i++)
			dekoded[i] = dekoded[size - oneBajt + i];
		dekoded.resize(oneBajt);
	}
}

//funkcja wczytujaca i zwracajaca pelny bajt danych
unsigned short bitLoadUp(char& bit_count, std::string& data_chunk,
	int& index, Node* node, unsigned short& just_decoded, int& bytes_to_read, std::ifstream& ifile)
{
	while (true)
	{
		//przy kazdym przejsciu sprawdzam, czy nie skonczyly sie dane do odczytu, jesli tak, wczytuje nowe
		readSwap(index, data_chunk, ifile, bytes_to_read);

		while (bit_count < 8)
		{
			//wczytuje bajt danych
			unsigned char bajt = static_cast<unsigned char>(data_chunk[index]);

			//sprawdzam bity od najstarszego do najmlodszego
			int bit = (bajt >> (7 - bit_count)) & 1;

			bit_count++;

			//na podstawie opdczytanego bitu, schodze w drzewie binarnym w lewo lub prawo,
			//powtarzam az dojde do liscia
			if (bit == 1)
				node = node->right;
			else
				node = node->left;
			
			//gdy dojde do liscia zwracam odkodowany znak
			if (node->left == nullptr && node->right == nullptr)
			{
				just_decoded = node->data;
				return just_decoded;
			}
		}

		// Koniec bajtu
		index++;
		bit_count = 0;
	}
}

//funkcja obliczajaca i zwracajaca wartosci dodatkowe dlugosci lub dystansu wg RFC 1951
unsigned short extraBitLoadUp(char& bit_count, std::string& data_chunk,
	int& index, unsigned short& extraBits, int& bytes_to_read, std::ifstream& ifile)
{
	unsigned short extra{};
	//przechodze petla, po wszystkich dodatkowych bitach danej dlugosci lub dystansu
	for (int i = 0; i < extraBits; i++)
	{

		//jesli doszedlem do konca bajtu, musze pobrac nowy bajt
		if (bit_count == 8)
		{
			index++;
			readSwap(index, data_chunk, ifile, bytes_to_read); //jesli dane dosly do konca, wczytuje nowe
			bit_count = 0;
		}
		//pobieram bajt z danego indeksu
		unsigned char bajt = static_cast<unsigned char>(data_chunk[index]);

		//pobieram bit
		int bit = (bajt >> 7 - bit_count) & 1;
		bit_count++;

		//wprowadzam bit od prawej strony do zmiennej extra
		if (bit == 1)
		{
			extra <<= 1;
			extra |= 1;
		}
		else
			extra <<= 1;
	}

	return extra;
}

//funkcja sprawdzajaca czy nie skonczyly sie dane do odczytu i wczytujaca nowe
void readSwap(int& index, std::string& data_chunk, std::ifstream& ifile, int& bytes_to_read)
{

	if (index >= bytes_to_read)
	{
		ifile.read(&data_chunk[0], data_chunk.size());
		bytes_to_read = ifile.gcount();
		index = 0;
	}
}

