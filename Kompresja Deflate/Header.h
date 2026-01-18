#pragma once

#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include <vector>
#include <utility>
#include "../Kompresja Deflate/structs.h"


std::vector<Token> AlgorytmKodujacyLZ77(const std::string& ipath);

Token hashSearch(long long cursor1, const std::string& fullText,
	std::vector<long long>& head, std::vector<long long>& prev,
	int bytes_to_read);

unsigned short hashCalculation(const unsigned char c1, const unsigned char c2, const unsigned char c3);

void prevSwap(std::vector<long long>& prev);

void headSwap(std::vector<long long>& head);

void headPrevUpdate(long long cursor1, const std::string& fullText,
	std::vector<long long>& head, std::vector<long long>& prev, int bytes_to_read);

void zliczanieLiter(const std::vector<Token>& token_chain, std::map<unsigned short, unsigned int>& letter_count,
	std::map<unsigned short, unsigned int>& distance_count);

LengthCode lengthConversion(unsigned short length);

DistanceCode distanceConversion(unsigned int distance);

Node* creatingRoot(const std::map<unsigned short, unsigned int>& mapa);

std::map<unsigned short, std::string> HuffmanTree(Node* root);

void recursiveAssign(Node* node, std::string& code, std::map<unsigned short, std::string>& codes);

void savingHeader(const std::string& bin_path, const std::map<unsigned short, unsigned int>& letter_count,
	const std::map<unsigned short, unsigned int>& distance_count);

void savingBinaryFile(const std::vector<Token> token_chain, const std::string& bin_path,
	std::map<unsigned short, std::string>& HuffmanLetterCode, std::map<unsigned short, std::string>& HuffmanDistanceCode);

void bufferFlush(std::ofstream& ofile, char& bit_count, unsigned char& buffer);

void lengthDistanceUpdate(LengthCode length, std::map<unsigned short, std::string>& HuffmanLetterCode
	, std::ofstream& ofile, char& bit_count, unsigned char& buffer);

void lengthDistanceUpdate(DistanceCode distance, std::map<unsigned short, std::string>& HuffmandistanceCode
	, std::ofstream& ofile, char& bit_count, unsigned char& buffer);

void headerReader(std::ifstream& ifile, std::map<unsigned short, unsigned int>& letter_count,
	std::map<unsigned short, unsigned int>& distance_count);

void deflateDecompression(const std::string& source_path, const std::string& final_path);

unsigned short bitLoadUp(char& bit_count, std::string& data_chunk,
	int& index, Node* node, unsigned short& just_decoded, int& bytes_to_read, std::ifstream& ifile);

unsigned short extraBitLoadUp(char& bit_count, std::string& data_chunk,
	int& index, unsigned short& extraBits, int& bytes_to_read, std::ifstream& ifile);

void decodedSwap(std::string& dekoded, const unsigned int oneBajt, std::ofstream& ofile);


void readSwap(int& index, std::string& data_chunk, std::ifstream& ifile, int& bytes_to_read);

LengthCode lengthConversion(unsigned short length);

DistanceCode distanceConversion(unsigned int distance);

LengthCode decodeLength(unsigned short symbol);

DistanceCode decodeDistance(unsigned short symbol);

void nodeDelete(Node* node);

void wyswietlLogo();

void Interface();

void wyborMenu(int n, const std::string& iPath, const std::string& binPath, const std::string& finalPath);

void KompresujPlik(const std::string& iPath, const std::string& binPath);

void DekompresujPlik(const std::string& binPath, const std::string& finalPath);

void KompresujDekompresuj(const std::string& iPath, const std::string& binPath, const std::string& finalPath);