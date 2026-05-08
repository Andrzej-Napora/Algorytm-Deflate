#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../Kompresja Deflate/structs.h"

Token hashSearch(long long cursor1, const std::string& fullText,
	std::vector<long long>& head, std::vector<long long>& prev,
	int bytes_to_read);

unsigned short hashCalculation(const unsigned char c1, const unsigned char c2, const unsigned char c3);

void prevSwap(std::vector<long long>& prev);

void headSwap(std::vector<long long>& head);

void headPrevUpdate(long long cursor1, const std::string& fullText,
	std::vector<long long>& head, std::vector<long long>& prev, int bytes_to_read);