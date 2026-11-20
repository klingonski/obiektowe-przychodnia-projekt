#include "dane_startowe.h"

#include <iostream>
#include <memory>
#include <utility>

#include "lekarz.h"
#include "pacjent.h"
#include "widok.h"

const int KARDIOLOG_ID = 1;
const int INTERNISTA_ID = 2;
const int KOWALSKI_ID = 101;
const int WISNIEWSKA_ID = 102;

std::vector<Osoba*> dodajOsoby(SystemPrzychodni& system) {
  wypiszNaglowek("Osoby w systemie");
  std::vector<std::unique_ptr<Osoba>> nowe;
  nowe.push_back(std::make_unique<Lekarz>(KARDIOLOG_ID, "Anna", "Nowak",
                                          "kardiologia"));
  nowe.push_back(std::make_unique<Lekarz>(INTERNISTA_ID, "Piotr", "Zielinski",
                                          "interna"));
  nowe.push_back(std::make_unique<Pacjent>(KOWALSKI_ID, "Jan", "Kowalski",
                                           "90010112345", "600100200",
                                           "jan.kowalski@poczta.pl"));
  nowe.push_back(std::make_unique<Pacjent>(WISNIEWSKA_ID, "Maria",
                                           "Wisniewska", "85050554321",
                                           "700300400",
                                           "m.wisniewska@poczta.pl"));

  std::vector<Osoba*> osoby;
  for (std::unique_ptr<Osoba>& osoba : nowe) {
    std::cout << "  [id " << osoba->pobierzId() << "] "
              << osoba->pobierzOpis() << "\n"; 
    osoby.push_back(osoba.get());
    system.dodajOsobe(std::move(osoba));
  }
  return osoby;
}

// US #4
void dodajSloty(SystemPrzychodni& system) {
  wypiszNaglowek("Grafiki lekarzy");
  Lekarz* kardiolog = system.znajdzLekarza(KARDIOLOG_ID);
  kardiolog->dodajSlot(utworzCzas(2026, 11, 20, 14, 0));
  kardiolog->dodajSlot(utworzCzas(2026, 11, 23, 9, 0));
  kardiolog->dodajSlot(utworzCzas(2026, 11, 23, 9, 30));
  kardiolog->dodajSlot(utworzCzas(2026, 11, 24, 10, 0));
  kardiolog->dodajSlot(utworzCzas(2026, 11, 23, 9, 0)); 

  Lekarz* internista = system.znajdzLekarza(INTERNISTA_ID);
  internista->dodajSlot(utworzCzas(2026, 11, 21, 12, 0));
  internista->dodajSlot(utworzCzas(2026, 11, 25, 8, 0));

  for (Lekarz* lekarz : system.pobierzLekarzy()) {
    std::cout << "  " << lekarz->pobierzOpis() << "\n";
    wypiszSloty(lekarz->pobierzGrafik());
  }
}

std::vector<Osoba*> przygotujDane(SystemPrzychodni& system) {
  std::vector<Osoba*> osoby = dodajOsoby(system);
  dodajSloty(system);
  return osoby;
}
