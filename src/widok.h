#pragma once

#include <ctime>
#include <string>
#include <vector>

#include "osoba.h"
#include "recepta.h"
#include "slot_czasowy.h"
#include "wizyta.h"

// Funkcje do wypisywania i wczytywania danych (menu i dane startowe).

std::time_t utworzCzas(int rok, int miesiac, int dzien, int godzina,
                       int minuta);

void wypiszNaglowek(const std::string& tytul);
void wypiszWynik(const std::string& operacja, bool udalo_sie);
void wypiszWizyty(const std::vector<Wizyta*>& wizyty);
void wypiszSloty(const std::vector<SlotCzasowy*>& sloty);
void wypiszRecepty(const std::vector<Recepta*>& recepty);
void wypiszPowiadomienia(const Osoba* osoba);

int wczytajLiczbe(const std::string& pytanie);
std::string wczytajTekst(const std::string& pytanie);
