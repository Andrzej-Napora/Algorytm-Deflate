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
#include "../Kompresja Deflate/kompresja.h"


//funkcja przyjmuje mapy przez referencje i modyfikuje je. Kazdemu znakowi przypisywana jest ilosc jego wystapien,
//dlugosc i dystans sa konwertowane na kod zgodnie z tablica wartosc RFC 1951, a nastepnie tym kodom rowniez
//przypisywana jest czestosc wystapien. Dlugosc i znaki sa przypisywane do jednej mapy, dystanse do drugiej.
void zliczanieLiter(const std::vector<Token>& token_chain,
	std::map<unsigned short, unsigned int>& letter_count,
	std::map<unsigned short, unsigned int>& distance_count)
{
	for (size_t i = 0; i < token_chain.size(); i++)
	{
		if (token_chain[i].literal)
		{
			letter_count[static_cast<unsigned short>(token_chain[i].sign)]++;
		}
		else
		{
			LengthCode tempLenght = lengthConversion(token_chain[i].dlugosc);//konwersja dlugosci na kod RFC 1952
			letter_count[tempLenght.code]++;

			DistanceCode tempDistance = distanceConversion(token_chain[i].dystans);//konwersja dystansu na kod RFC 1952
			distance_count[tempDistance.code]++;
		}
	}
	letter_count[256] = 1;//znak konca pliku
}

//funkcja budujaca korzen drzewa Huffmana z mapy czestotliwosci
Node* creatingRoot(const std::map<unsigned short, unsigned int>& mapa)
{
	//Zabezpieczenie przed bardzo krotkim plikiem, w ktorym nie bedzie zadnego dystansu
	if (mapa.empty())
	{
		Node* root = nullptr;
		return root;
	}
	//Tworzymy kolejke priorytetowa odwrocona(komparator cmp) przyjmujaca obiekty typu Node* i ja zapelniamy
	std::priority_queue<Node*, std::vector<Node*>, cmp> queue;
	for (auto& [data, freq] : mapa)
	{
		//kazdy wezle jest zapisywany na heap, bedzie potrzebny w dalszej czesco programu
		Node* temp = new Node(freq, data);
		queue.push(temp);
	}
	//sprawdzamy czy kolejka jest wieksza niz 1
	if(queue.size() > 1)
	{
		//redukuje wezly, tak dlugo az pozostanie ostatni wezel, czyli korzen
		while (queue.size() > 1)
		{
			Node* temp_left = queue.top();
			queue.pop();

			Node* temp_right = queue.top();
			queue.pop();

			Node* temp_node = new Node(temp_left->freq + temp_right->freq, '0', temp_left, temp_right);
			queue.push(temp_node);
		}
		return queue.top();
	}
	else	//edge case, kolejka rowna 1 np dla pliku "aaaa...", warunek moze byc spelniony tylko dla drzewa dystansow,
			//gdzie nie wystepuje znak EOF
	{
		//potrzebuje stworzyc sztuczny korzen, z ktorego moj pojedynczy lisc bedzie mogl "zejsc" w funkji
		//dekodujacej drzewo
		Node* root = queue.top();

		Node* temp = new Node(root->freq, root->data, root, nullptr);
		return temp;
	}
}

std::map<unsigned short, std::string> HuffmanTree(Node* root)
{
	std::map<unsigned short, std::string> codes;

	//dla malych plikow, gdzie nie ma dystansu
	if (root == nullptr)
	{
		return codes; // Zwracamy pust¹ mapê
	}

	//wprowadzam dwie zmienne string, zeby 0 i 1 nie pomieszaly sie przy wychodzenia z rekurencji
	std::string code0, code1;
	if (root->left != nullptr)
	{
		code0.push_back('0');
		recursiveAssign(root->left, code0, codes);
	}
	if (root->right != nullptr)
	{
		code1.push_back('1');
		recursiveAssign(root->right, code1, codes);
	}
	return codes;
}

//pomocnicza funkcja rekurencyjna
void recursiveAssign(Node* node, std::string& code, std::map<unsigned short, std::string>& codes)
{
	if (node->left == nullptr && node->right == nullptr)
	{
		codes[node->data] = code;
		return;
	}
	if (node->left != nullptr)
	{
		code.push_back('0');
		recursiveAssign(node->left, code, codes);
		code.pop_back();
	}
	if (node->right != nullptr)
	{
		code.push_back('1');
		recursiveAssign(node->right, code, codes);
		code.pop_back();
	}
}

//funkcja zapisujaca naglowek
void savingHeader(const std::string& bin_path,
	const std::map<unsigned short, unsigned int>& letter_count,
	const std::map<unsigned short, unsigned int>& distance_count)
{
	std::ofstream ofile(bin_path, std::ios::binary);
	if (ofile.good())
	{
		//najpierw zapisywane sa frekwencje wystepowania literalow i dlugosci z mapy letter_count
		unsigned short size_letter = letter_count.size();
		ofile.write(reinterpret_cast<const char*>(&size_letter), sizeof(size_letter));
		for (const auto& [data, freq] : letter_count)
		{
			ofile.write(reinterpret_cast<const char*>(&data), sizeof(data));
			ofile.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
		}

		//nastepnie zapisywane sa frekwencje wystepowania dystansu
		unsigned short size_distance = distance_count.size();
		ofile.write(reinterpret_cast<const char*>(&size_distance), sizeof(size_distance));
		for (const auto& [data, freq] : distance_count)
		{
			ofile.write(reinterpret_cast<const char*>(&data), sizeof(data));
			ofile.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
		}
	}
	else
		std::cerr << "Blad zapisu Naglowka" << std::endl;
}

//funkcja kompresujaca dane w formie kodu przedrostkowego w formie bitowej
void savingBinaryFile(const std::vector<Token> token_chain, const std::string& bin_path,
	std::map<unsigned short, std::string>& HuffmanLetterCode, std::map<unsigned short, std::string>& HuffmanDistanceCode)
{
	std::ofstream ofile(bin_path, std::ios::binary | std::ios::app);
	if (ofile.good())
	{
		unsigned char buffer{};	//tu bede gromadzil bity przed zapisem do pliku
		char bit_count{};		//licznik bitow zgromadzonych w zmiennej buffer
		for(auto& token:token_chain)
		{
			if(token.literal)
			{
				if (token.eof)	//sprawdzenie znaku konca pliku, ktory jest literalem
				{
					std::string& eof_bits = HuffmanLetterCode[256];	//uzywam referencji dla szybkosci

					//upycham kod przedrostkowy bit po bicie w bajcie
					for (auto& bit : eof_bits) {
						buffer <<= 1;
						if (bit == '1')
							buffer |= 1;
						bit_count++;
						bufferFlush(ofile, bit_count, buffer);//sprawdzam przepelnienie bufforu i go oprozniam
					}
					break;
				}

				//tak jak wyzej, tylko dla pozostalych literalow
				std::string& bit_string = HuffmanLetterCode[token.sign];

				//upycham kod przedrostkowy bit po bicie w bajcie
				for (auto& bit : bit_string)
				{
					buffer <<= 1;
					if (bit == '1')
						buffer |= 1;
					bit_count++;
					bufferFlush(ofile, bit_count, buffer);//sprawdzam przepelnienie bufforu i go oprozniam
				}
			}
			else
			{
				//zapisywanie do pliku kodu dlugosci i dystansu

				//konwersja dlugosci i dystansu na skladowe dla danej dlugosci i dystansu wg tabeli RFC 1951
				LengthCode length = lengthConversion(token.dlugosc);
				DistanceCode distance = distanceConversion(token.dystans);

				//wywolanie funkcji zapisujacej 
				lengthDistanceUpdate(length, HuffmanLetterCode, ofile, bit_count, buffer);
				lengthDistanceUpdate(distance, HuffmanDistanceCode, ofile, bit_count, buffer);
				
			}
		}
		//zapisanie resztki ostatniego bajtu
		if (bit_count != 0)
		{
			buffer <<= 8 - bit_count;
			ofile.put(buffer);
		}
	}
	else
	{
		std::cerr << "Blad w trakcie zapisu do pliku danych binarnych" << std::endl;
	}
}

//funkcja zapisujaca bajt po pelnym wypelnieniu
void bufferFlush(std::ofstream& ofile, char& bit_count, unsigned char& buffer)
{
	if (bit_count == 8)
	{
		ofile.put(buffer);
		buffer = 0;
		bit_count = 0;
	}
}

//funkcja zapisujaca kod dlugosci wraz z dodatkowymi bitami do bajtu
void lengthDistanceUpdate(LengthCode length, std::map<unsigned short, std::string>& HuffmanLetterCode,
	std::ofstream& ofile, char& bit_count, unsigned char& buffer)
{
	//wyciagam kod Huffmana dla danego kodu z tabeli RFC 1951 i zapisuje do bajtu
	const std::string& bit_letter_string = HuffmanLetterCode[length.code];
	for (auto& bit : bit_letter_string)
	{
		buffer <<= 1;
		if (bit == '1')
		{
			buffer |= 1;
		}
		bit_count++;
		bufferFlush(ofile, bit_count, buffer);//sprawdzam przepelnienie bufforu i go oprozniam
	}
	//zapisuje do bajtu dodatkowe bity wg tabeli RFC 1951
	for (int i = 0; i < length.extraBits; i++)
	{
		buffer <<= 1;
		char current_bit = (length.extraValue >> length.extraBits - 1 - i) & 1;
		buffer |= current_bit;
		bit_count++;
		bufferFlush(ofile, bit_count, buffer);//sprawdzam przepelnienie bufforu i go oprozniam
	}
}
//przeladowana funkcja dla dystansu, dziala tak samo jak funkcja wyzej
void lengthDistanceUpdate(DistanceCode distance, std::map<unsigned short, std::string>& HuffmanDistanceCode
	, std::ofstream& ofile, char& bit_count, unsigned char& buffer)
{
	const std::string& bit_distance_string = HuffmanDistanceCode[distance.code];
	for (auto& bit : bit_distance_string)
	{
		if (bit == '1')
		{
			buffer <<= 1;
			buffer |= 1;
		}
		else
			buffer <<= 1;
		bit_count++;
		bufferFlush(ofile, bit_count, buffer);
	}
	for (int i = 0; i < distance.extraBits; i++)
	{
		buffer <<= 1;
		char current_bit = (distance.extraValue >> distance.extraBits - 1 - i) & 1;
		buffer |= current_bit;
		bit_count++;
		bufferFlush(ofile, bit_count, buffer);
	}
}

