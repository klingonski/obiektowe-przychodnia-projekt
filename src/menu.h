#pragma once

#include <vector>

#include "osoba.h"
#include "system_przychodni.h"

// Menu: logowanie jako pacjent albo lekarz i wybór akcji.
void uruchomMenu(SystemPrzychodni& system, const std::vector<Osoba*>& osoby);
