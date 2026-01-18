#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../Kompresja Deflate/structs.h"

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