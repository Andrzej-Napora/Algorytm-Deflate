#pragma once

#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include <vector>
#include <utility>

//struktura pomocnicza dla algorytmu LZ77, przechowuje informacje o literalach, dlugosciach i dystansach
struct Token
{
	bool literal = false;
	unsigned short sign{};
	unsigned short dystans{};
	unsigned short dlugosc{};
	bool eof = false;

	//konstruktor dla literlau
	Token(unsigned short sign, bool literal) : sign(sign), literal(literal) {}

	//konstruktor ma taka postac, zeby nie wykluczal sie z konstruktorem powyzej
	//jest to konstruktor znaku konca pliku
	Token(bool eof)
	{
		this->eof = eof;
		if (eof) {
			this->literal = true;
			this->sign = 256;
		}
	}

	//konstruktor dla pary dlugosc/dystans
	Token(unsigned short dystans, unsigned short dlugosc, bool literal)
	{
		this->dystans = dystans;
		this->dlugosc = dlugosc;
		this->literal = literal;
		if (this->dystans == 0 && this->dlugosc < 3)
			this->literal = true;
	}
};

//struktura przechowujaca informacje o danej dlugosci zgodne z dokumentem RFC 1951
struct LengthCode {
	unsigned short code{};
	unsigned short extraBits{};
	unsigned int extraValue{};
	unsigned short baseLength{};
	LengthCode(unsigned short extraBits, unsigned short baseLength) :
		extraBits(extraBits), baseLength(baseLength) {
	}
	LengthCode(unsigned short code, unsigned short extraBits, unsigned int extraValue)
	{
		this->code = code;
		this->extraBits = extraBits;
		this->extraValue = extraValue;
	}
};

//struktura przechowujaca informacje o danym dystansie zgodne z dokumentem RFC 1951
struct DistanceCode {
	unsigned short code{};
	unsigned short extraBits{};
	unsigned int extraValue{};
	unsigned short baseDistance{};
	DistanceCode(unsigned short extraBits, unsigned short baseDistance) :
		extraBits(extraBits), baseDistance(baseDistance) {
	}
	DistanceCode(unsigned short code, unsigned short extraBits, unsigned int extraValue)
	{
		this->code = code;
		this->extraBits = extraBits;
		this->extraValue = extraValue;
	}
};

//struktura potrzebna do budowy drzewa binarnego
struct Node
{
	unsigned int freq{};
	unsigned short data{};
	Node* left = nullptr;
	Node* right = nullptr;

	Node(unsigned int freq, unsigned short data)
	{
		this->freq = freq;
		this->data = data;
		this->left = nullptr;
		this->right = nullptr;
	}
	Node(unsigned int freq, unsigned short data, Node* left, Node* right)
	{
		this->freq = freq;
		this->data = data;
		this->left = left;
		this->right = right;
	}
};

struct cmp	//komparator do priority_queue
{
	bool operator()(Node* l, Node* r)
	{
		return (l->freq > r->freq);
	}
};