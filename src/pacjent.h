#pragma once

#include <string>

#include "osoba.h"

class Pacjent : public Osoba {
public:
  Pacjent(int id, const std::string& imie, const std::string& nazwisko,
          const std::string& pesel, const std::string& telefon,
          const std::string& email);

  std::string pobierzOpis() const override;
  std::string pobierzDaneKontaktowe() const;

private:
  std::string pesel_;
  std::string telefon_;
  std::string email_;
};
