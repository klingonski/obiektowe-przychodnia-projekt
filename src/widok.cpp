#include "widok.h"

#include <iostream>

std::time_t utworzCzas(int rok, int miesiac, int dzien, int godzina,
                       int minuta) {
  std::tm czas{};
  czas.tm_year = rok - 1900;
  czas.tm_mon = miesiac - 1;
  czas.tm_mday = dzien;
  czas.tm_hour = godzina;
  czas.tm_min = minuta;
  czas.tm_isdst = -1;
  return std::mktime(&czas);
}

void wypiszNaglowek(const std::string& tytul) {
  std::cout << "\n===== " << tytul << " =====\n";
}

void wypiszWynik(const std::string& operacja, bool udalo_sie) {
  if (udalo_sie) {
    std::cout << "  " << operacja << ": OK\n";
  } else {
    std::cout << "  " << operacja << ": ODRZUCONO\n";
  }
}

void wypiszWizyty(const std::vector<Wizyta*>& wizyty) {
  if (wizyty.empty()) {
    std::cout << "  (brak wizyt)\n";
  }
  for (Wizyta* wizyta : wizyty) {
    std::cout << "  - " << wizyta->pobierzOpis() << "\n";
  }
}

void wypiszSloty(const std::vector<SlotCzasowy*>& sloty) {
  if (sloty.empty()) {
    std::cout << "    (brak slotow)\n";
  }
  for (SlotCzasowy* slot : sloty) {
    std::cout << "    slot " << slot->pobierzId() << ": "
              << slot->pobierzTermin();
    if (slot->czyWolny()) {
      std::cout << " (wolny)\n";
    } else {
      std::cout << " (zajety)\n";
    }
  }
}

void wypiszRecepty(const std::vector<Recepta*>& recepty) {
  if (recepty.empty()) {
    std::cout << "  (brak recept)\n";
  }
  for (Recepta* recepta : recepty) {
    std::cout << "  " << recepta->pobierzOpis() << "\n";
    for (const PozycjaRecepty& pozycja : recepta->pobierzPozycje()) {
      std::cout << "    - " << pozycja.pobierzOpis() << "\n";
    }
  }
}

void wypiszPowiadomienia(const Osoba* osoba) {
  std::cout << "  " << osoba->pobierzOpis() << ":\n";
  if (osoba->pobierzPowiadomienia().empty()) {
    std::cout << "    (brak powiadomien)\n";
  }
  for (const std::string& tresc : osoba->pobierzPowiadomienia()) {
    std::cout << "    * " << tresc << "\n";
  }
}

int wczytajLiczbe(const std::string& pytanie) {
  std::cout << pytanie;
  int liczba = 0;
  while (!(std::cin >> liczba)) {
    if (std::cin.eof()) {
      return 0;
    }
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cout << "To nie jest liczba, sprobuj jeszcze raz: ";
  }
  std::cin.ignore(1000, '\n');
  return liczba;
}

std::string wczytajTekst(const std::string& pytanie) {
  std::cout << pytanie;
  std::string tekst;
  std::getline(std::cin, tekst);
  return tekst;
}
