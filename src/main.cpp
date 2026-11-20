#include <vector>

#include "dane_startowe.h"
#include "menu.h"
#include "osoba.h"
#include "system_przychodni.h"

int main() {
  SystemPrzychodni system;
  std::vector<Osoba*> osoby = przygotujDane(system);
  uruchomMenu(system, osoby);
  return 0;
}
