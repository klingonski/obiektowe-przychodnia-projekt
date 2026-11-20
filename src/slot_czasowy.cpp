#include "slot_czasowy.h"

#include <iomanip>
#include <sstream>

SlotCzasowy::SlotCzasowy(int id, std::time_t poczatek)
    : id_(id), poczatek_(poczatek) {}

int SlotCzasowy::pobierzId() const {
  return id_;
}

std::time_t SlotCzasowy::pobierzPoczatek() const {
  return poczatek_;
}

std::string SlotCzasowy::pobierzTermin() const {
  const std::tm* czas = std::localtime(&poczatek_);
  if (czas == nullptr) {
    return "(nieznany termin)";
  }
  std::ostringstream termin;
  termin << std::put_time(czas, "%Y-%m-%d %H:%M");
  return termin.str();
}

bool SlotCzasowy::czyWolny() const {
  return !zajety_;
}

void SlotCzasowy::zablokuj() {
  zajety_ = true;
}

void SlotCzasowy::zwolnij() {
  zajety_ = false;
}
