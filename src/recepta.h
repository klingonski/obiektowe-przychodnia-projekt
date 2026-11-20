#pragma once

#include <ctime>
#include <string>
#include <vector>

#include "pozycja_recepty.h"

class Recepta {
public:
  explicit Recepta(std::time_t data_wystawienia);

  void dodajPozycje(const std::string& lek, const std::string& dawkowanie);
  const std::vector<PozycjaRecepty>& pobierzPozycje() const;
  std::string pobierzOpis() const;

private:
  std::time_t data_wystawienia_;
  std::vector<PozycjaRecepty> pozycje_;
};
