#include "menu.h"

#include <ctime>
#include <iostream>
#include <string>

#include "lekarz.h"
#include "pacjent.h"
#include "widok.h"
#include "wizyta.h"

// menu pacjenta

void menuWyszukajLekarzy(SystemPrzychodni& system) {
  std::string specjalizacja =
      wczytajTekst("Specjalizacja (np. kardiologia, interna): ");
  std::vector<Lekarz*> lekarze = system.wyszukajLekarzy(specjalizacja);
  if (lekarze.empty()) {
    std::cout << "  Brak lekarzy o tej specjalizacji.\n";
  }
  for (Lekarz* lekarz : lekarze) {
    std::cout << "  [id " << lekarz->pobierzId() << "] "
              << lekarz->pobierzOpis() << ", wolne terminy:\n";
    wypiszSloty(lekarz->pobierzWolneSloty());
  }
}

void menuZarezerwuj(SystemPrzychodni& system, int pacjent_id) {
  int lekarz_id = wczytajLiczbe("Id lekarza: ");
  int slot_id = wczytajLiczbe("Nr slotu: ");
  Wizyta* wizyta = system.zarezerwujWizyte(pacjent_id, lekarz_id, slot_id);
  if (wizyta != nullptr) {
    std::cout << "  Zarezerwowano: " << wizyta->pobierzOpis() << "\n";
  } else {
    std::cout << "  Nie udalo sie: nie ma takiego lekarza lub slotu, "
                 "albo slot jest zajety.\n";
  }
}

void menuOdwolaj(SystemPrzychodni& system, int pacjent_id) {
  int wizyta_id = wczytajLiczbe("Nr wizyty do odwolania: ");
  bool udalo_sie =
      system.odwolajRezerwacje(pacjent_id, wizyta_id, std::time(nullptr));
  wypiszWynik("Odwolanie wizyty nr " + std::to_string(wizyta_id), udalo_sie);
  if (!udalo_sie) {
    std::cout << "  Mozna odwolac tylko swoja potwierdzona wizyte, "
                 "najpozniej 24 h przed terminem.\n";
  }
}

void menuPacjenta(SystemPrzychodni& system, Pacjent* pacjent) {
  int wybor = 0;
  do {
    wypiszNaglowek("Pacjent: " + pacjent->pobierzOpis());
    std::cout << "  1 - wyszukaj lekarzy\n"
                 "  2 - zarezerwuj wizyte\n"
                 "  3 - moje wizyty\n"
                 "  4 - odwolaj wizyte\n"
                 "  5 - moje recepty\n"
                 "  6 - powiadomienia\n"
                 "  0 - wyloguj\n";
    wybor = wczytajLiczbe("> ");
    switch (wybor) {
      case 1:
        menuWyszukajLekarzy(system);
        break;
      case 2:
        menuZarezerwuj(system, pacjent->pobierzId());
        break;
      case 3:
        wypiszWizyty(system.pobierzWizytyPacjenta(pacjent->pobierzId()));
        break;
      case 4:
        menuOdwolaj(system, pacjent->pobierzId());
        break;
      case 5:
        wypiszRecepty(system.pobierzReceptyPacjenta(pacjent->pobierzId()));
        break;
      case 6:
        wypiszPowiadomienia(pacjent);
        break;
      default:
        break;
    }
  } while (wybor != 0);
}

// menu lekarza

Wizyta* menuZnajdzWizyte(SystemPrzychodni& system, Lekarz* lekarz) {
  int wizyta_id = wczytajLiczbe("Nr wizyty: ");
  for (Wizyta* wizyta : system.pobierzWizytyLekarza(lekarz->pobierzId())) {
    if (wizyta->pobierzId() == wizyta_id) {
      return wizyta;
    }
  }
  std::cout << "  Nie masz wizyty o takim numerze.\n";
  return nullptr;
}

void menuDodajSlot(Lekarz* lekarz) {
  std::cout << "  Podaj termin nowego slotu.\n";
  int rok = wczytajLiczbe("Rok: ");
  int miesiac = wczytajLiczbe("Miesiac: ");
  int dzien = wczytajLiczbe("Dzien: ");
  int godzina = wczytajLiczbe("Godzina: ");
  int minuta = wczytajLiczbe("Minuta: ");
  lekarz->dodajSlot(utworzCzas(rok, miesiac, dzien, godzina, minuta));
  std::cout << "  Grafik po zmianie:\n";
  wypiszSloty(lekarz->pobierzGrafik());
}

void menuPacjenciLekarza(SystemPrzychodni& system, Lekarz* lekarz) {
  std::vector<Pacjent*> pacjenci =
      system.pobierzPacjentowLekarza(lekarz->pobierzId());
  if (pacjenci.empty()) {
    std::cout << "  (brak pacjentow)\n";
  }
  for (Pacjent* pacjent : pacjenci) {
    std::cout << "  - " << pacjent->pobierzOpis() << ": "
              << pacjent->pobierzDaneKontaktowe() << "\n";
    wypiszRecepty(system.pobierzReceptyPacjenta(pacjent->pobierzId()));
  }
}

void menuPrzesun(SystemPrzychodni& system, Lekarz* lekarz) {
  int wizyta_id = wczytajLiczbe("Nr wizyty: ");
  int slot_id = wczytajLiczbe("Nr nowego slotu: ");
  wypiszWynik("Przesuniecie wizyty",
              system.przesunWizyte(lekarz->pobierzId(), wizyta_id, slot_id));
}

void menuAnuluj(SystemPrzychodni& system, Lekarz* lekarz) {
  int wizyta_id = wczytajLiczbe("Nr wizyty: ");
  wypiszWynik("Anulowanie wizyty",
              system.anulujWizyte(lekarz->pobierzId(), wizyta_id));
}

void menuZakoncz(SystemPrzychodni& system, Lekarz* lekarz) {
  Wizyta* wizyta = menuZnajdzWizyte(system, lekarz);
  if (wizyta == nullptr) {
    return;
  }
  wizyta->zakoncz();
  std::cout << "  " << wizyta->pobierzOpis() << "\n";
}

void menuWystawRecepte(SystemPrzychodni& system, Lekarz* lekarz) {
  Wizyta* wizyta = menuZnajdzWizyte(system, lekarz);
  if (wizyta == nullptr) {
    return;
  }
  if (wizyta->pobierzStatus() != StatusWizyty::ZAKONCZONA) {
    std::cout << "  Najpierw zakoncz wizyte (opcja 7).\n";
    return;
  }
  std::string lek = wczytajTekst("Lek (pusta linia konczy): ");
  while (!lek.empty()) {
    std::string dawkowanie = wczytajTekst("Dawkowanie: ");
    wizyta->wystawRecepte(lek, dawkowanie);
    lek = wczytajTekst("Lek (pusta linia konczy): ");
  }
  if (wizyta->pobierzRecepte() != nullptr) {
    std::cout << "  " << wizyta->pobierzRecepte()->pobierzOpis() << "\n";
  }
}

void wypiszOpcjeLekarza() {
  std::cout << "  1 - moj grafik\n"
               "  2 - dodaj slot\n"
               "  3 - moje wizyty\n"
               "  4 - moi pacjenci (kontakt i recepty)\n"
               "  5 - przesun wizyte\n"
               "  6 - anuluj wizyte\n"
               "  7 - zakoncz wizyte\n"
               "  8 - wystaw recepte (do zakonczonej wizyty)\n"
               "  9 - powiadomienia\n"
               "  0 - wyloguj\n";
}

void menuLekarza(SystemPrzychodni& system, Lekarz* lekarz) {
  int wybor = 0;
  do {
    wypiszNaglowek("Lekarz: " + lekarz->pobierzOpis());
    wypiszOpcjeLekarza();
    wybor = wczytajLiczbe("> ");
    switch (wybor) {
      case 1:
        wypiszSloty(lekarz->pobierzGrafik());
        break;
      case 2:
        menuDodajSlot(lekarz);
        break;
      case 3:
        wypiszWizyty(system.pobierzWizytyLekarza(lekarz->pobierzId()));
        break;
      case 4:
        menuPacjenciLekarza(system, lekarz);
        break;
      case 5:
        menuPrzesun(system, lekarz);
        break;
      case 6:
        menuAnuluj(system, lekarz);
        break;
      case 7:
        menuZakoncz(system, lekarz);
        break;
      case 8:
        menuWystawRecepte(system, lekarz);
        break;
      case 9:
        wypiszPowiadomienia(lekarz);
        break;
      default:
        break;
    }
  } while (wybor != 0);
}

// logowanie

Pacjent* menuZnajdzPacjenta(const std::vector<Osoba*>& osoby, int id) {
  for (Osoba* osoba : osoby) {
    Pacjent* pacjent = dynamic_cast<Pacjent*>(osoba);
    if (pacjent != nullptr && pacjent->pobierzId() == id) {
      return pacjent;
    }
  }
  return nullptr;
}

void uruchomMenu(SystemPrzychodni& system, const std::vector<Osoba*>& osoby) {
  int wybor = 0;
  do {
    wypiszNaglowek("Logowanie");
    std::cout << "  1 - zaloguj jako pacjent\n"
                 "  2 - zaloguj jako lekarz\n"
                 "  0 - zakoncz program\n";
    wybor = wczytajLiczbe("> ");
    if (wybor == 1) {
      Pacjent* pacjent = menuZnajdzPacjenta(osoby, wczytajLiczbe("Id: "));
      if (pacjent != nullptr) {
        menuPacjenta(system, pacjent);
      } else {
        std::cout << "  Nie ma pacjenta o takim id.\n";
      }
    } else if (wybor == 2) {
      Lekarz* lekarz = system.znajdzLekarza(wczytajLiczbe("Id: "));
      if (lekarz != nullptr) {
        menuLekarza(system, lekarz);
      } else {
        std::cout << "  Nie ma lekarza o takim id.\n";
      }
    }
  } while (wybor != 0);
}
