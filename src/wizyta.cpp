#include "wizyta.h"

#include <stdexcept>

Wizyta::Wizyta(int id, Pacjent* pacjent, Lekarz* lekarz, SlotCzasowy* slot)
    : id_(id), pacjent_(pacjent), lekarz_(lekarz), slot_(slot) {}

int Wizyta::pobierzId() const {
  return id_;
}

StatusWizyty Wizyta::pobierzStatus() const {
  return status_;
}

Pacjent* Wizyta::pobierzPacjenta() const {
  return pacjent_;
}

Lekarz* Wizyta::pobierzLekarza() const {
  return lekarz_;
}

Recepta* Wizyta::pobierzRecepte() const {
  return recepta_.get();
}

std::string Wizyta::pobierzOpis() const {
  std::string status;
  if (status_ == StatusWizyty::POTWIERDZONA) {
    status = "Potwierdzona";
  } else if (status_ == StatusWizyty::ZAKONCZONA) {
    status = "Zakonczona";
  } else {
    status = "Odwolana";
  }
  return "Wizyta nr " + std::to_string(id_) + " [" + status + "] " +
         slot_->pobierzTermin() + ", " + lekarz_->pobierzOpis() +
         ", pacjent: " + pacjent_->pobierzOpis();
}

// pacjent może odwołać tylko potwierdzoną wizytę i najpóźniej
// limit_godz godzin przed jej początkiem
bool Wizyta::czyMoznaOdwolac(std::time_t teraz, int limit_godz) const {
  if (status_ != StatusWizyty::POTWIERDZONA) {
    return false;
  }
  const double SEKUND_W_GODZINIE = 3600.0;
  double sekundy_do_wizyty = std::difftime(slot_->pobierzPoczatek(), teraz);
  return sekundy_do_wizyty >= limit_godz * SEKUND_W_GODZINIE;
}

void Wizyta::odwolaj() {
  if (status_ != StatusWizyty::POTWIERDZONA) {
    return;
  }
  status_ = StatusWizyty::ODWOLANA;
  slot_->zwolnij();
}

// tylko na wolny slot z grafiku tego samego lekarza
void Wizyta::przenies(SlotCzasowy* nowy_slot) {
  if (status_ != StatusWizyty::POTWIERDZONA || nowy_slot == nullptr ||
      !nowy_slot->czyWolny() ||
      lekarz_->znajdzSlot(nowy_slot->pobierzId()) != nowy_slot) {
    return;
  }
  slot_->zwolnij();
  nowy_slot->zablokuj();
  slot_ = nowy_slot;
}

Recepta& Wizyta::wystawRecepte(const std::string& lek,
                               const std::string& dawkowanie) {
  if (status_ != StatusWizyty::ZAKONCZONA) {
    throw std::logic_error(
        "Recepte mozna wystawic dopiero po zakonczeniu wizyty");
  }
  if (!recepta_) {
    recepta_ = std::make_unique<Recepta>(slot_->pobierzPoczatek());
    pacjent_->powiadom("Wystawiono recepte po wizycie nr " +
                       std::to_string(id_) + " u " + lekarz_->pobierzOpis());
  }
  recepta_->dodajPozycje(lek, dawkowanie);
  return *recepta_;
}

void Wizyta::zakoncz() {
  if (status_ == StatusWizyty::POTWIERDZONA) {
    status_ = StatusWizyty::ZAKONCZONA;
  }
}
