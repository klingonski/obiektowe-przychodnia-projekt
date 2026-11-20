#include "system_przychodni.h"

#include <utility>

void SystemPrzychodni::dodajOsobe(std::unique_ptr<Osoba> osoba) {
  if (!osoba) {
    return;
  }
  for (const auto& istniejaca : osoby_) {
    if (istniejaca->pobierzId() == osoba->pobierzId()) {
      return;  // id musi być unikalne
    }
  }
  osoby_.push_back(std::move(osoba));
}

std::vector<Lekarz*> SystemPrzychodni::pobierzLekarzy() const {
  std::vector<Lekarz*> lekarze;
  for (const auto& osoba : osoby_) {
    Lekarz* lekarz = dynamic_cast<Lekarz*>(osoba.get());
    if (lekarz != nullptr) {
      lekarze.push_back(lekarz);
    }
  }
  return lekarze;
}

std::vector<Lekarz*> SystemPrzychodni::wyszukajLekarzy(
    const std::string& specjalizacja) const {
  std::vector<Lekarz*> wynik;
  for (Lekarz* lekarz : pobierzLekarzy()) {
    if (lekarz->pobierzSpecjalizacje() == specjalizacja) {
      wynik.push_back(lekarz);
    }
  }
  return wynik;
}

Lekarz* SystemPrzychodni::znajdzLekarza(int id) const {
  for (Lekarz* lekarz : pobierzLekarzy()) {
    if (lekarz->pobierzId() == id) {
      return lekarz;
    }
  }
  return nullptr;
}

Wizyta* SystemPrzychodni::zarezerwujWizyte(int pacjent_id, int lekarz_id,
                                           int slot_id) {
  Pacjent* pacjent = znajdzPacjenta(pacjent_id);
  Lekarz* lekarz = znajdzLekarza(lekarz_id);
  if (pacjent == nullptr || lekarz == nullptr) {
    return nullptr;
  }
  SlotCzasowy* slot = lekarz->znajdzSlot(slot_id);
  if (slot == nullptr || !slot->czyWolny()) {
    return nullptr;
  }
  slot->zablokuj();
  wizyty_.push_back(std::make_unique<Wizyta>(nastepne_id_wizyty_, pacjent,
                                             lekarz, slot));
  ++nastepne_id_wizyty_;
  Wizyta* wizyta = wizyty_.back().get();
  lekarz->powiadom("Nowa rezerwacja: " + wizyta->pobierzOpis());
  return wizyta;
}

std::vector<Wizyta*> SystemPrzychodni::pobierzWizytyPacjenta(
    int pacjent_id) const {
  std::vector<Wizyta*> wynik;
  for (const auto& wizyta : wizyty_) {
    if (wizyta->pobierzPacjenta()->pobierzId() == pacjent_id) {
      wynik.push_back(wizyta.get());
    }
  }
  return wynik;
}

bool SystemPrzychodni::odwolajRezerwacje(int pacjent_id, int wizyta_id,
                                         std::time_t teraz) {
  Wizyta* wizyta = znajdzWizyte(wizyta_id);
  if (wizyta == nullptr ||
      wizyta->pobierzPacjenta()->pobierzId() != pacjent_id) {
    return false;
  }
  if (!wizyta->czyMoznaOdwolac(teraz, limit_odwolania_godz_)) {
    return false;
  }
  wizyta->odwolaj();
  wizyta->pobierzLekarza()->powiadom("Pacjent odwolal wizyte: " +
                                     wizyta->pobierzOpis());
  return true;
}

std::vector<Recepta*> SystemPrzychodni::pobierzReceptyPacjenta(
    int pacjent_id) const {
  std::vector<Recepta*> recepty;
  for (Wizyta* wizyta : pobierzWizytyPacjenta(pacjent_id)) {
    if (wizyta->pobierzRecepte() != nullptr) {
      recepty.push_back(wizyta->pobierzRecepte());
    }
  }
  return recepty;
}

std::vector<Wizyta*> SystemPrzychodni::pobierzWizytyLekarza(
    int lekarz_id) const {
  std::vector<Wizyta*> wynik;
  for (const auto& wizyta : wizyty_) {
    if (wizyta->pobierzLekarza()->pobierzId() == lekarz_id) {
      wynik.push_back(wizyta.get());
    }
  }
  return wynik;
}

std::vector<Pacjent*> SystemPrzychodni::pobierzPacjentowLekarza(
    int lekarz_id) const {
  std::vector<Pacjent*> pacjenci;
  for (Wizyta* wizyta : pobierzWizytyLekarza(lekarz_id)) {
    if (wizyta->pobierzStatus() == StatusWizyty::ODWOLANA) {
      continue;
    }
    Pacjent* pacjent = wizyta->pobierzPacjenta();
    bool juz_na_liscie = false;
    for (Pacjent* dodany : pacjenci) {
      if (dodany == pacjent) {
        juz_na_liscie = true;
      }
    }
    if (!juz_na_liscie) {
      pacjenci.push_back(pacjent);
    }
  }
  return pacjenci;
}

bool SystemPrzychodni::przesunWizyte(int lekarz_id, int wizyta_id,
                                     int nowy_slot_id) {
  Wizyta* wizyta = znajdzAktywnaWizyteLekarza(lekarz_id, wizyta_id);
  if (wizyta == nullptr) {
    return false;
  }
  SlotCzasowy* nowy_slot = wizyta->pobierzLekarza()->znajdzSlot(nowy_slot_id);
  if (nowy_slot == nullptr || !nowy_slot->czyWolny()) {
    return false;
  }
  wizyta->przenies(nowy_slot);
  wizyta->pobierzPacjenta()->powiadom("Lekarz przesunal wizyte: " + wizyta->pobierzOpis());
  return true;
}

bool SystemPrzychodni::anulujWizyte(int lekarz_id, int wizyta_id) {
  Wizyta* wizyta = znajdzAktywnaWizyteLekarza(lekarz_id, wizyta_id);
  if (wizyta == nullptr) {
    return false;
  }
  wizyta->odwolaj();
  wizyta->pobierzPacjenta()->powiadom("Lekarz anulowal wizyte: " + wizyta->pobierzOpis());
  return true;
}

Pacjent* SystemPrzychodni::znajdzPacjenta(int id) const {
  for (const auto& osoba : osoby_) {
    Pacjent* pacjent = dynamic_cast<Pacjent*>(osoba.get());
    if (pacjent != nullptr && pacjent->pobierzId() == id) {
      return pacjent;
    }
  }
  return nullptr;
}

Wizyta* SystemPrzychodni::znajdzWizyte(int id) const {
  for (const auto& wizyta : wizyty_) {
    if (wizyta->pobierzId() == id) {
      return wizyta.get();
    }
  }
  return nullptr;
}

Wizyta* SystemPrzychodni::znajdzAktywnaWizyteLekarza(int lekarz_id,
                                                     int wizyta_id) const {
  Wizyta* wizyta = znajdzWizyte(wizyta_id);
  if (wizyta == nullptr ||
      wizyta->pobierzLekarza()->pobierzId() != lekarz_id ||
      wizyta->pobierzStatus() != StatusWizyty::POTWIERDZONA) {
    return nullptr;
  }
  return wizyta;
}
