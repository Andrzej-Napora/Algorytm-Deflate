#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../Kompresja Deflate/structs.h"

unsigned short bitLoadUp(char& bit_count, std::string& data_chunk,
	int& index, Node* node, unsigned short& just_decoded, int& bytes_to_read, std::ifstream& ifile);

unsigned short extraBitLoadUp(char& bit_count, std::string& data_chunk,
	int& index, unsigned short& extraBits, int& bytes_to_read, std::ifstream& ifile);

void decodedSwap(std::string& dekoded, const unsigned int oneBajt, std::ofstream& ofile);


void readSwap(int& index, std::string& data_chunk, std::ifstream& ifile, int& bytes_to_read);