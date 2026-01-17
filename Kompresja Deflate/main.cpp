#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include <vector>
#include "../Kompresja Deflate/Header.h"

using namespace std;

int main()
{
	//rozne sciezki testowe
	string ipath2 = "../resources/proba.txt";
	string ipath1 = "../resources/skrotCity.csv";
	string ipath = "../resources/TesktLiteracki.txt";
	string binOpath = "../resources/Binary.txt";
	string binOTpath = "../resources/test.txt";

	//tworze kontener tokenow i wypelniam go za pomoca algorytmu kodujacego LZ77
	std::vector<Token> token_chain;
	try
	{
		token_chain = AlgorytmKodujacyLZ77(ipath2);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Wystapil blad: " << e.what() << std::endl;
		return 1;
	}
	
	//mapy sa przekazywane przez referencje do funkcji zliczajacej, gdzie wypelniane sa
	//czestotliwosciami wystapien kodow wg RFC 1951 dla literalow, dlugosci i osobno dystansow
	std::map<unsigned short, unsigned int> letter_count;
	std::map<unsigned short, unsigned int> distance_count;
	zliczanieLiter(token_chain, letter_count, distance_count);

	//tworze korzen, nastepnie drzewo binarne i nastepnie kod przedrostkowy za pomoca tego drzewa binarnego
	//dla literalow i dlugosci
	Node* letter_root = creatingRoot(letter_count);
	std::map<unsigned short, std::string> HuffmanLetterCode = HuffmanTree(letter_root);

	//tak jak wyzej dla dystansow
	Node* distance_root = creatingRoot(distance_count);
	std::map<unsigned short, std::string> HuffmanDistanceCode = HuffmanTree(distance_root);

	//w naglowku zapisuje mapy zawierajace czestotliwosci wystapien znakow
	savingHeader(binOpath, letter_count, distance_count);

	//upycham kod przedrostkowy w formie bitowej, nastepnie zapisuje dane w nowym pliku
	savingBinaryFile(token_chain, binOpath, HuffmanLetterCode, HuffmanDistanceCode);

	//dekompresja i zapis danych do nowego pliku
	deflateDecompression(binOpath, binOTpath);

	//usuwam drzewa binarne zapisane na stercie
	nodeDelete(letter_root);
	nodeDelete(distance_root);
}