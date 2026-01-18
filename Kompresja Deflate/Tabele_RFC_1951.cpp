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
#include "../Kompresja Deflate/Tabele_RFC_1951.h"

//Konwersja dlugosci zgodna z dokumentem RFC 1951
LengthCode lengthConversion(unsigned short length)
{
	if (length == 3) return { 257, 0, 0 };
	if (length == 4) return { 258, 0, 0 };
	if (length == 5) return { 259, 0, 0 };
	if (length == 6) return { 260, 0, 0 };
	if (length == 7) return { 261, 0, 0 };
	if (length == 8) return { 262, 0, 0 };
	if (length == 9) return { 263, 0, 0 };
	if (length == 10) return { 264, 0, 0 };

	if (length >= 11 && length <= 12) return { 265, 1, (unsigned int)(length - 11) };
	if (length >= 13 && length <= 14) return { 266, 1, (unsigned int)(length - 13) };
	if (length >= 15 && length <= 16) return { 267, 1, (unsigned int)(length - 15) };
	if (length >= 17 && length <= 18) return { 268, 1, (unsigned int)(length - 17) };

	if (length >= 19 && length <= 22) return { 269, 2, (unsigned int)(length - 19) };
	if (length >= 23 && length <= 26) return { 270, 2, (unsigned int)(length - 23) };
	if (length >= 27 && length <= 30) return { 271, 2, (unsigned int)(length - 27) };
	if (length >= 31 && length <= 34) return { 272, 2, (unsigned int)(length - 31) };

	if (length >= 35 && length <= 42) return { 273, 3, (unsigned int)(length - 35) };
	if (length >= 43 && length <= 50) return { 274, 3, (unsigned int)(length - 43) };
	if (length >= 51 && length <= 58) return { 275, 3, (unsigned int)(length - 51) };
	if (length >= 59 && length <= 66) return { 276, 3, (unsigned int)(length - 59) };

	if (length >= 67 && length <= 82) return { 277, 4, (unsigned int)(length - 67) };
	if (length >= 83 && length <= 98) return { 278, 4, (unsigned int)(length - 83) };
	if (length >= 99 && length <= 114) return { 279, 4, (unsigned int)(length - 99) };
	if (length >= 115 && length <= 130) return { 280, 4, (unsigned int)(length - 115) };

	if (length >= 131 && length <= 162) return { 281, 5, (unsigned int)(length - 131) };
	if (length >= 163 && length <= 194) return { 282, 5, (unsigned int)(length - 163) };
	if (length >= 195 && length <= 226) return { 283, 5, (unsigned int)(length - 195) };
	if (length >= 227 && length <= 257) return { 284, 5, (unsigned int)(length - 227) };

	if (length == 258) return { 285, 0, 0 };

	return { 0, 0, 0 };		//blad
}

//Konwersja dystansu zgodna z dokumentem RFC 1951
DistanceCode distanceConversion(unsigned int distance)
{
	if (distance == 1) return { 0, 0, 0 };
	if (distance == 2) return { 1, 0, 0 };
	if (distance == 3) return { 2, 0, 0 };
	if (distance == 4) return { 3, 0, 0 };

	if (distance >= 5 && distance <= 6)   return { 4, 1, (unsigned int)(distance - 5) };
	if (distance >= 7 && distance <= 8)   return { 5, 1, (unsigned int)(distance - 7) };

	if (distance >= 9 && distance <= 12)  return { 6, 2, (unsigned int)(distance - 9) };
	if (distance >= 13 && distance <= 16) return { 7, 2, (unsigned int)(distance - 13) };

	if (distance >= 17 && distance <= 24) return { 8, 3, (unsigned int)(distance - 17) };
	if (distance >= 25 && distance <= 32) return { 9, 3, (unsigned int)(distance - 25) };

	if (distance >= 33 && distance <= 48) return { 10, 4, (unsigned int)(distance - 33) };
	if (distance >= 49 && distance <= 64) return { 11, 4, (unsigned int)(distance - 49) };

	if (distance >= 65 && distance <= 96)  return { 12, 5, (unsigned int)(distance - 65) };
	if (distance >= 97 && distance <= 128) return { 13, 5, (unsigned int)(distance - 97) };

	if (distance >= 129 && distance <= 192) return { 14, 6, (unsigned int)(distance - 129) };
	if (distance >= 193 && distance <= 256) return { 15, 6, (unsigned int)(distance - 193) };

	if (distance >= 257 && distance <= 384) return { 16, 7, (unsigned int)(distance - 257) };
	if (distance >= 385 && distance <= 512) return { 17, 7, (unsigned int)(distance - 385) };

	if (distance >= 513 && distance <= 768)  return { 18, 8, (unsigned int)(distance - 513) };
	if (distance >= 769 && distance <= 1024) return { 19, 8, (unsigned int)(distance - 769) };

	if (distance >= 1025 && distance <= 1536) return { 20, 9, (unsigned int)(distance - 1025) };
	if (distance >= 1537 && distance <= 2048) return { 21, 9, (unsigned int)(distance - 1537) };

	if (distance >= 2049 && distance <= 3072) return { 22, 10, (unsigned int)(distance - 2049) };
	if (distance >= 3073 && distance <= 4096) return { 23, 10, (unsigned int)(distance - 3073) };

	if (distance >= 4097 && distance <= 6144) return { 24, 11, (unsigned int)(distance - 4097) };
	if (distance >= 6145 && distance <= 8192) return { 25, 11, (unsigned int)(distance - 6145) };

	if (distance >= 8193 && distance <= 12288)  return { 26, 12, (unsigned int)(distance - 8193) };
	if (distance >= 12289 && distance <= 16384) return { 27, 12, (unsigned int)(distance - 12289) };

	if (distance >= 16385 && distance <= 24576) return { 28, 13, (unsigned int)(distance - 16385) };
	if (distance >= 24577 && distance <= 32768) return { 29, 13, (unsigned int)(distance - 24577) };

	return { 0, 0, 0 };		//blad
}

//dekodowanie dlugosci zgodne z dokumentem RFC 1951
LengthCode decodeLength(unsigned short symbol)
{
	if (symbol >= 257 && symbol <= 264)
		return LengthCode(0, symbol - 254);

	if (symbol >= 265 && symbol <= 268)
		return LengthCode(1, 11 + (symbol - 265) * 2);

	if (symbol >= 269 && symbol <= 272)
		return LengthCode(2, 19 + (symbol - 269) * 4);

	if (symbol >= 273 && symbol <= 276)
		return LengthCode(3, 35 + (symbol - 273) * 8);

	if (symbol >= 277 && symbol <= 280)
		return LengthCode(4, 67 + (symbol - 277) * 16);

	if (symbol >= 281 && symbol <= 284)
		return LengthCode(5, 131 + (symbol - 281) * 32);

	if (symbol == 285)
		return LengthCode(0, 258);

	return LengthCode(0, 0);
}

//dekodowanie dystansu zgodne z dokumentem RFC 1951
DistanceCode decodeDistance(unsigned short symbol)
{
	if (symbol <= 3)
		return DistanceCode(0, symbol + 1);

	if (symbol >= 4 && symbol <= 5)
		return DistanceCode(1, 5 + (symbol - 4) * 2);

	if (symbol >= 6 && symbol <= 7)
		return DistanceCode(2, 9 + (symbol - 6) * 4);

	if (symbol >= 8 && symbol <= 9)
		return DistanceCode(3, 17 + (symbol - 8) * 8);

	if (symbol >= 10 && symbol <= 11)
		return DistanceCode(4, 33 + (symbol - 10) * 16);

	if (symbol >= 12 && symbol <= 13)
		return DistanceCode(5, 65 + (symbol - 12) * 32);

	if (symbol >= 14 && symbol <= 15)
		return DistanceCode(6, 129 + (symbol - 14) * 64);

	if (symbol >= 16 && symbol <= 17)
		return DistanceCode(7, 257 + (symbol - 16) * 128);

	if (symbol >= 18 && symbol <= 19)
		return DistanceCode(8, 513 + (symbol - 18) * 256);

	if (symbol >= 20 && symbol <= 21)
		return DistanceCode(9, 1025 + (symbol - 20) * 512);

	if (symbol >= 22 && symbol <= 23)
		return DistanceCode(10, 2049 + (symbol - 22) * 1024);

	if (symbol >= 24 && symbol <= 25)
		return DistanceCode(11, 4097 + (symbol - 24) * 2048);

	if (symbol >= 26 && symbol <= 27)
		return DistanceCode(12, 8193 + (symbol - 26) * 4096);

	if (symbol >= 28 && symbol <= 29)
		return DistanceCode(13, 16385 + (symbol - 28) * 8192);

	return DistanceCode(0, 0);
}