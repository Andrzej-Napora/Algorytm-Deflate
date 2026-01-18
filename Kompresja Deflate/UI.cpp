#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include "../Kompresja Deflate/Header.h"

using namespace std;

void wyswietlLogo() 
{
	cout << "==========================================" << endl;
	cout << "             KOMPRESOR DEFLATE			   " << endl;
	cout << "==========================================" << endl;
	cout << endl;
}

void Interface()
{

	//rozne sciezki testowe
	string iPath2 = "../resources/proba.txt";
	string iPath1 = "../resources/skrotCity.csv";
	string iPath = "../resources/TesktLiteracki.txt";
	string binPath = "../resources/Binary.txt";
	string finalPath = "../resources/test.txt";

	while (true)
	{
		wyswietlLogo();
		int wybor;
		cout << " [1] Kompresuj plik" << endl;
		cout << " [2] Dekompresuj plik" << endl;
		cout << " [3] Kompresuj i dekompresuj plik" << endl;
		cout << " [4] Zakoncz" << endl;
		cout << endl;
		cout << " Wybierz opcje: ";
		if(cin >> wybor)
		{
			//obsluga nieprawidlowego wyboru
			if (wybor < 1 || wybor > 4)
			{
				system("cls");
				cout << "Podales nieprawidlowa liczbe, prosze wybrac ponownie" << endl;
				cout << endl;
				cout << endl;
			}
			//koniec programu
			else if (wybor == 4)
				return;
			//wybor miedzy 1 a 4
			else
			{
				system("cls");
				wyborMenu(wybor, iPath, binPath, finalPath);
			}
		}
		else
		{
			//sytuacja gdy podano znak inny niz liczba
			system("cls");
			cout << "Podales nieprawidlowa liczbe, prosze wybrac ponownie" << endl;
			cout << endl;
			cout << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<int>::max(),'\n');
		}
	}
}

void wyborMenu(int wybor, const string& iPath, const string& binPath, const string& finalPath)
{
	switch (wybor)
	{
		case 1:
		{
			try
			{
				cout << "Trwa kompresja pliku" << endl;
				KompresujPlik(iPath, binPath);
				system("cls");
			}
			catch (const std::exception& e)
			{
				std::cerr << "Wystapil blad przy kompresji: " << e.what() << std::endl;
				cout << endl;
				cout << endl;
				return;
			}
			cout << "Plik zostal skompresowany" << endl;
			cout << endl;
			cout << endl;
			break;
		}
		case 2:
		{
			try
			{
				cout << "Trwa dekompresja pliku" << endl;
				DekompresujPlik(binPath, finalPath);
				system("cls");
			}
			catch (const std::exception& e)
			{
				std::cerr << "Wystapil blad przy dekompresji: " << e.what() << std::endl;
				cout << endl;
				cout << endl;
				return;
			}
			cout << "Plik zostal zdekompresowany" << endl;
			cout << endl;
			cout << endl;
			break;
		}
		case 3:
		{
			try
			{
				cout << "Trwa kompresja i dekompresja pliku" << endl;
				KompresujDekompresuj(iPath, binPath, finalPath);
				system("cls");
			}
			catch (const std::exception& e)
			{
				std::cerr << "Wystapil blad przy dekompresji: " << e.what() << std::endl;
				cout << endl;
				cout << endl;
				return;
			}
			cout << "Plik zostal skompresowany i zdekompresowany" << endl;
			cout << endl;
			cout << endl;
			break;
		}
	}
}

void KompresujPlik(const string& iPath, const string& binPath)
{
	//tworze kontener tokenow i wypelniam go za pomoca algorytmu kodujacego LZ77
	std::vector<Token> token_chain;
	try
	{
		token_chain = AlgorytmKodujacyLZ77(iPath);
	}
	catch (const std::exception& e)
	{
		throw;
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
	savingHeader(binPath, letter_count, distance_count);

	//upycham kod przedrostkowy w formie bitowej, nastepnie zapisuje dane w nowym pliku
	savingBinaryFile(token_chain, binPath, HuffmanLetterCode, HuffmanDistanceCode);

	//usuwam drzewa binarne zapisane na stercie
	nodeDelete(letter_root);
	nodeDelete(distance_root);
}

void DekompresujPlik(const string& binPath, const string& finalPath)
{
	//dekompresja i zapis danych do nowego pliku
	try
	{
		deflateDecompression(binPath, finalPath);
	}
	catch (const std::exception& e)
	{
		throw;
	}
}

void KompresujDekompresuj(const string& iPath, const string& binPath, const string& finalPath)
{
	//tworze kontener tokenow i wypelniam go za pomoca algorytmu kodujacego LZ77
	std::vector<Token> token_chain;
	try
	{
		token_chain = AlgorytmKodujacyLZ77(iPath);
	}
	catch (const std::exception& e)
	{
		throw;
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
	savingHeader(binPath, letter_count, distance_count);

	//upycham kod przedrostkowy w formie bitowej, nastepnie zapisuje dane w nowym pliku
	savingBinaryFile(token_chain, binPath, HuffmanLetterCode, HuffmanDistanceCode);

	//dekompresja i zapis danych do nowego pliku
	try
	{
		deflateDecompression(binPath, finalPath);
	}
	catch (const std::exception& e)
	{
		throw;
	}

	//usuwam drzewa binarne zapisane na stercie
	nodeDelete(letter_root);
	nodeDelete(distance_root);
}