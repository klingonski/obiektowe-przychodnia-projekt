#include "pacjent.h"

Pacjent::Pacjent(int id, const std::string& imie, const std::string& nazwisko,
                 const std::string& pesel, const std::string& telefon,
                 const std::string& email)
    : Osoba(id, imie, nazwisko),
      pesel_(pesel),
      telefon_(telefon),
      email_(email) {}

std::string Pacjent::pobierzOpis() const {
  return imie_ + " " + nazwisko_ + " (PESEL: " + pesel_ + ")";
}

std::string Pacjent::pobierzDaneKontaktowe() const {
  return "tel. " + telefon_ + ", e-mail: " + email_;
}
