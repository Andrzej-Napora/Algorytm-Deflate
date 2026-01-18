#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../Kompresja Deflate/structs.h"

void headerReader(std::ifstream& ifile, std::map<unsigned short, unsigned int>& letter_count,
	std::map<unsigned short, unsigned int>& distance_count);

void deflateDecompression(const std::string& source_path, const std::string& final_path);

void nodeDelete(Node* node);