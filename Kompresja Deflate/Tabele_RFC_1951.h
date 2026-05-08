#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../Kompresja Deflate/structs.h"

LengthCode lengthConversion(unsigned short length);

DistanceCode distanceConversion(unsigned int distance);

LengthCode decodeLength(unsigned short symbol);

DistanceCode decodeDistance(unsigned short symbol);