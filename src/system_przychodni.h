#pragma once

#include <ctime>
#include <memory>
#include <string>
#include <vector>

#include "lekarz.h"
#include "osoba.h"
#include "pacjent.h"
#include "recepta.h"
#include "wizyta.h"

class SystemPrzychodni {
public:
  void dodajOsobe(std::unique_ptr<Osoba> osoba);
  std::vector<Lekarz*> pobierzLekarzy() const;
  std::vector<Lekarz*> wyszukajLekarzy(const std::string& specjalizacja) const;
  Lekarz* znajdzLekarza(int id) const;
  Wizyta* zarezerwujWizyte(int pacjent_id, int lekarz_id, int slot_id);
  std::vector<Wizyta*> pobierzWizytyPacjenta(int pacjent_id) const;
  bool odwolajRezerwacje(int pacjent_id, int wizyta_id, std::time_t teraz);
  std::vector<Recepta*> pobierzReceptyPacjenta(int pacjent_id) const;
  std::vector<Wizyta*> pobierzWizytyLekarza(int lekarz_id) const;
  std::vector<Pacjent*> pobierzPacjentowLekarza(int lekarz_id) const;
  bool przesunWizyte(int lekarz_id, int wizyta_id, int nowy_slot_id);
  bool anulujWizyte(int lekarz_id, int wizyta_id);

private:
  Pacjent* znajdzPacjenta(int id) const;
  Wizyta* znajdzWizyte(int id) const;
  Wizyta* znajdzAktywnaWizyteLekarza(int lekarz_id, int wizyta_id) const;

  std::vector<std::unique_ptr<Osoba>> osoby_;
  std::vector<std::unique_ptr<Wizyta>> wizyty_;
  int nastepne_id_wizyty_ = 1;
  int limit_odwolania_godz_ = 24;
};
