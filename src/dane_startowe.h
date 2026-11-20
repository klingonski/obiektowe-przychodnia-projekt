#pragma once

#include <vector>

#include "osoba.h"
#include "system_przychodni.h"

// Dodaje do systemu przykładowych lekarzy, pacjentów i sloty. Zwraca zwykłe
// wskaźniki do osób (właścicielem jest system), menu szuka w nich pacjentów.
std::vector<Osoba*> przygotujDane(SystemPrzychodni& system);
