#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../Kompresja Deflate/structs.h"

void wyswietlLogo();

void Interface();

void wyborMenu(int n, std::string& iPath, std::string& binPath, std::string& finalPath, std::string& path, bool& menu1);

void KompresujPlik(const std::string& iPath, const std::string& binPath);

void DekompresujPlik(const std::string& binPath, const std::string& finalPath);

void KompresujDekompresuj(const std::string& iPath, const std::string& binPath, const std::string& finalPath);

void zmianaSciezki(std::string& path, std::string& iPath, std::string& binPath, std::string& finalPath);

std::string wyciecieNazwyPliku(std::string& path);