#include "lekarz.h"

Lekarz::Lekarz(int id, const std::string& imie, const std::string& nazwisko,
               const std::string& specjalizacja)
    : Osoba(id, imie, nazwisko), specjalizacja_(specjalizacja) {}

std::string Lekarz::pobierzOpis() const {
  return "dr " + imie_ + " " + nazwisko_ + " (" + specjalizacja_ + ")";
}

std::string Lekarz::pobierzSpecjalizacje() const {
  return specjalizacja_;
}

void Lekarz::dodajSlot(std::time_t poczatek) {
  for (const auto& slot : grafik_) {
    if (slot->pobierzPoczatek() == poczatek) {
      return;  // taki termin już jest w grafiku
    }
  }
  // sloty nie są usuwane, więc kolejne id = liczba slotów + 1
  const int nowe_id = static_cast<int>(grafik_.size()) + 1;
  grafik_.push_back(std::make_unique<SlotCzasowy>(nowe_id, poczatek));
}

std::vector<SlotCzasowy*> Lekarz::pobierzGrafik() const {
  std::vector<SlotCzasowy*> grafik;
  for (const auto& slot : grafik_) {
    grafik.push_back(slot.get());
  }
  return grafik;
}

std::vector<SlotCzasowy*> Lekarz::pobierzWolneSloty() const {
  std::vector<SlotCzasowy*> wolne;
  for (SlotCzasowy* slot : pobierzGrafik()) {
    if (slot->czyWolny()) {
      wolne.push_back(slot);
    }
  }
  return wolne;
}

SlotCzasowy* Lekarz::znajdzSlot(int slot_id) const {
  for (const auto& slot : grafik_) {
    if (slot->pobierzId() == slot_id) {
      return slot.get();
    }
  }
  return nullptr;
}
