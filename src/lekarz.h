#pragma once

#include <ctime>
#include <memory>
#include <string>
#include <vector>

#include "osoba.h"
#include "slot_czasowy.h"

class Lekarz : public Osoba {
public:
  Lekarz(int id, const std::string& imie, const std::string& nazwisko,
         const std::string& specjalizacja);

  std::string pobierzOpis() const override;
  std::string pobierzSpecjalizacje() const;
  void dodajSlot(std::time_t poczatek);
  std::vector<SlotCzasowy*> pobierzGrafik() const;
  std::vector<SlotCzasowy*> pobierzWolneSloty() const;
  SlotCzasowy* znajdzSlot(int slot_id) const;

private:
  std::string specjalizacja_;
  std::vector<std::unique_ptr<SlotCzasowy>> grafik_;
};
