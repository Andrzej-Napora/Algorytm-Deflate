#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../Kompresja Deflate/structs.h"

void wyswietlLogo();

void Interface();

void wyborMenu(int n, const std::string& iPath, const std::string& binPath, const std::string& finalPath);

void KompresujPlik(const std::string& iPath, const std::string& binPath);

void DekompresujPlik(const std::string& binPath, const std::string& finalPath);

void KompresujDekompresuj(const std::string& iPath, const std::string& binPath, const std::string& finalPath);