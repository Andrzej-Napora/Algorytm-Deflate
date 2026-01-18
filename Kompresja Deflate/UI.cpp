#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include "../Kompresja Deflate/UI.h"
#include "../Kompresja Deflate/algorytmKodujacy.h"
#include "../Kompresja Deflate/dekompresja.h"
#include "../Kompresja Deflate/kompresja.h"


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
	string iPath0 = "../resources/skrotCity.csv";
	string iPath3 = "../resources/TesktLiteracki.txt";
	string iPath = "../resources/plik_do_kompresji.txt";
	string binPath = "../resources/plik_skompresowany.txt";
	string finalPath = "../resources/plik_po_dekompresji.txt";

	string path;
	bool menu1 = true;
	while (true)
	{
		int wybor{};
		if(menu1)
		{
			wyswietlLogo();
			cout << " [0] wybor sciezki do pliku(inna niz domyslna)" << endl;
			cout << " [1] Kompresuj plik" << endl;
			cout << " [2] Dekompresuj plik" << endl;
			cout << " [3] Kompresuj i dekompresuj plik" << endl;
			cout << " [4] Zakoncz" << endl;
			cout << endl;
			cout << " domyslna sciezko do kompresji to C:/Users/andrz/source/repos/Kompresja Deflate/resources/plik_do_kompresji.txt" << endl;
			cout << endl;
			cout << " Wybierz opcje: ";
		}
		else
		{
			wyswietlLogo();
			cout << " [0] wybor sciezki do pliku" << endl;
			cout << " [1] Kompresuj plik" << endl;
			cout << " [2] Dekompresuj plik" << endl;
			cout << " [3] Kompresuj i dekompresuj plik" << endl;
			cout << " [4] Zakoncz" << endl;
			cout << endl;
			cout << " sciezko do kompresji to: "<<path << endl;
			cout << endl;
			cout << " Wybierz opcje: ";
		}
		if(cin >> wybor)
		{
			//obsluga nieprawidlowego wyboru
			if (wybor < 0 || wybor > 4)
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
				wyborMenu(wybor, iPath, binPath, finalPath, path, menu1);
			}
		}
		else
		{
			//sytuacja gdy podano znak inny niz liczba
			system("cls");
			cout << "Podales znak nie bedacy liczba, prosze wybrac ponownie" << endl;
			cout << endl;
			cout << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<int>::max(),'\n');
		}
	}
}

//funkcja zwracajaca nazwe plike z pelnej sciezki
string wyciecieNazwyPliku(string& path)
{
	string file_name;
	int index{};
	for (int i = path.size(); i > 0; i--)
	{
		if (path[i] == '/')
		{
			index = i + 1;
			break;
		}
	}
	for (int i = index; i < path.size(); i++)
		file_name.push_back(path[i]);
	return file_name;
}


//funkcja modyfikujaca sciezke pliku do kompresji, zgodnie z instrukcja uzytkownika
//a takze zmieniajace sciezki plikow skompresowanego i po dekompresji, tak aby
//wszystkie pliki byly w jednym folderze
void zmianaSciezki(string& path, string&  iPath, string&  binPath, string&  finalPath)
{
	iPath = path;
	string path_no_name;
	int index;

	for (int i = path.size(); i > 0; i--)
	{
		if (path[i] == '/')
		{
			index = i;
			break;
		}
	}
	for (int i = 0; i <= index; i++)
		path_no_name.push_back(path[i]);

	string bin_name = wyciecieNazwyPliku(binPath);
	string final_name = wyciecieNazwyPliku(finalPath);
	binPath = path_no_name + bin_name;
	finalPath = path_no_name + final_name;
}


//obsluga poszczegolnych wyborow uzytkownika
void wyborMenu(int wybor, string& iPath, string& binPath, string& finalPath, std::string& path, bool& menu1)
{
	switch (wybor)
	{
		case 0:
		{
			//wprowadzenie sciezki do pliku ktory chcemy skompresowac
			cout << "Wprowadz sciezko do pliku, ktory chcesz skompresowac:" << endl;
			cin.ignore();
			getline(cin,path);
			menu1 = false;
			system("cls");

			//po wybraniu scieki pliku do kompresji, sciezki plikow skompresowanego
			//oraz finalnego pliku po dekompresji zmieniaja sie zgodnie ze sciezko uzytkownika, tak
			//aby wszystkie pojawili sie w tym samym folderze
			zmianaSciezki(path, iPath, binPath, finalPath);
			break;
		}
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
				std::cerr << "Wystapil blad: " << e.what() << std::endl;
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
				std::cerr << "Wystapil blad: " << e.what() << std::endl;
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
				std::cerr << "Wystapil blad: " << e.what() << std::endl;
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

	KompresujPlik(iPath, binPath);

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