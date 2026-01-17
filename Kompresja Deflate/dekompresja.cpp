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

//funkcja odczytujace dane z naglowka i budujace na ich podstawie mape z czestotliwosciami wystapien literalow i dlugosci,
//oraz osobno, z czestotliwosciami wystapien dystnasow
void headerReader(std::ifstream& ifile, std::map<unsigned short, unsigned int>& letter_count,
	std::map<unsigned short, unsigned int>& distance_count)
{
	unsigned short size{};
	unsigned short data{};
	unsigned int freq{};
	//pierwszy z naglowka odczytywany jest rozmiar mapy literalow i dlugosci, a nastepnie cala mape
	ifile.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		ifile.read(reinterpret_cast<char*>(&data), sizeof(data));
		ifile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
		letter_count[data] = freq;
	}
	//po odczytaniu calej pierwszej mapy, odczytujemy rozmiar mapy dystansow, a nastepnie cala mape
	ifile.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		ifile.read(reinterpret_cast<char*>(&data), sizeof(data));
		ifile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
		distance_count[data] = freq;
	}
}



//funkcja dekompresujaca, odczytujaca i zapisujaca w pliku wyjsciowym
void deflateDecompression(const std::string& source_path, const std::string& final_path)
{
	std::ifstream ifile(source_path, std::ios::binary);
	std::ofstream ofile(final_path, std::ios::binary);
	if (ifile.good() && ofile.good())
	{
		//deklaracja map przechowywujacych czestotliwosci wystapien literalow, dlugosci i dystansow
		std::map<unsigned short, unsigned int> letter_count;
		std::map<unsigned short, unsigned int> distance_count;
		//wypelniam mapy danymi z naglowka
		headerReader(ifile, letter_count, distance_count);

		//na podstawie tych map tworze korzen, a nastepnie kod przedrostkowy dla kazdego znaku
		//dla literalow i dlugosci
		Node* letter_root = creatingRoot(letter_count);
		std::map<unsigned short, std::string> HuffmanLetterCode = HuffmanTree(letter_root);
		//dla dystansow
		Node* distance_root = creatingRoot(distance_count);
		std::map<unsigned short, std::string> HuffmanDistanceCode = HuffmanTree(distance_root);

		//deklaracja wymaganych zmiennych pomocniczych
		unsigned int oneBajt = 32768;	//maksymalny obslugiwany dystans wg dokumentu RFC 1951
		std::string data_chunk;

		//do zmiennej decoded wczytywane beda odkodowane dane,
		//i na podstawie tych danych podstawie tablica hashujaca bedzie odczytywac znaki
		std::string decoded;

		data_chunk.resize(2 * oneBajt);
		char bit_count{};
		bool first_pass = true;
		//warunek pobierajacy 2*oneBajt znakow z pliku, lub jesli to sie nie uda, sprawdzajacy,
		//czy pobrane zostalo cokolwiek
		while (ifile.read(&data_chunk[0], 2 * oneBajt) || ifile.gcount() > 0)
		{
			//ilosc dobrych danych, ktore zostaly wczytane do data_chunk; obsluga koncowki pliku
			int bytes_to_read = ifile.gcount(); 

			unsigned short decoded_value{};
			int index{};
			while (index < bytes_to_read)
			{
				//zwracam zdekodowany bajt
				decoded_value = bitLoadUp(bit_count, data_chunk, index, letter_root, decoded_value, bytes_to_read, ifile);

				//oblsluga znaku konca pliku 256
				if (decoded_value == 256)
				{
					//jesli plik skonczyl sie przy pierwszym przejsciu, zapisujemy do pliku wszystko
					//od poczatku do konca zmiennej decoded
					if (first_pass)
						ofile.write(&decoded[0], decoded.size());

					//jesli plik skonczyl sie po pierwszym przejsciu, czyli byl wiekszy niz 65535 znakow,
					//zapisujemy do pliku wszystko, od indexu = oneBajt do konca zmiennej decoded
					else if (decoded.size() > oneBajt)
						ofile.write(&decoded[oneBajt], decoded.size() - oneBajt);
					return;
				}

				//obsluga literalow
				if (decoded_value < 256)
				{
					decoded.push_back(decoded_value);

					//sprawdzam czy decoded nie zostalo przepelnione
					decodedSwap(decoded, oneBajt, first_pass, ofile);
				}

				//obsluga dlugosci i dystansu
				else if (256 < decoded_value && decoded_value <= 285)
				{
					//dlugosc
					//dekoduje kod wg RFC 1951 spowrotem na dlugosc bazowa
					LengthCode length = decodeLength(decoded_value);

					//obliczam dlugosc dodatkowa wg RFC 1951
					unsigned short extraValue = extraBitLoadUp(bit_count, data_chunk, index,
						length.extraBits, bytes_to_read, ifile);

					//sumuje dlugosc bazowa z dlugoscia dodatkowa
					unsigned short dlugosc = length.baseLength + extraValue;

					//po dlugosci zawsze odrazu jest dystans
					//wczytuje nowy bajt z danych, wiem ze jest to dystans
					decoded_value = bitLoadUp(bit_count, data_chunk, index, distance_root,
						decoded_value, bytes_to_read, ifile);

					//dekoduje kod wg RFC 1951 spowrotem na dystans bazowy
					DistanceCode distance = decodeDistance(decoded_value);

					//obliczam dystans dodatkowy wg RFC 1951
					extraValue = extraBitLoadUp(bit_count, data_chunk, index, distance.extraBits, bytes_to_read, ifile);

					//sumuje dystans bazowy i dystans dodatkowy
					unsigned short dystans = distance.baseDistance + extraValue;

					//majac juz dlugosc i dystans moge wczytac do zmiennej decoded odpowiadajace im znaki
					int cursor = decoded.size() - dystans;
					for (int i = 0; i < dlugosc; i++)
					{
						decoded.push_back(decoded[cursor + i]);
					}

					//sprawdzam, czy dekoded nie zostalo przepelnione
					decodedSwap(decoded, oneBajt, first_pass, ofile);
				}
			}
		}
	}
}

//funkcja usuwajace cale drzewo Huffmana zapisane na stercie
void nodeDelete(Node* node)
{
	while(node != nullptr)
	{
		if (node->left == nullptr && node->right == nullptr)
		{
			delete node;
			return;
		}
		if (node->left != nullptr)
		{
			nodeDelete(node->left);
			node->left = nullptr;
		}
		if (node->right != nullptr)
		{
			nodeDelete(node->right);
			node->right = nullptr;
		}
	}
}
