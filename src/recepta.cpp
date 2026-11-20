#include "recepta.h"

#include <iomanip>
#include <sstream>

Recepta::Recepta(std::time_t data_wystawienia)
    : data_wystawienia_(data_wystawienia) {}

void Recepta::dodajPozycje(const std::string& lek,
                           const std::string& dawkowanie) {
  pozycje_.push_back(PozycjaRecepty(lek, dawkowanie));
}

const std::vector<PozycjaRecepty>& Recepta::pobierzPozycje() const {
  return pozycje_;
}

std::string Recepta::pobierzOpis() const {
  std::ostringstream opis;
  opis << "Recepta z dnia ";
  const std::tm* data = std::localtime(&data_wystawienia_);
  if (data != nullptr) {
    opis << std::put_time(data, "%Y-%m-%d");
  } else {
    opis << "(nieznana data)";
  }
  opis << ", liczba pozycji: " << pozycje_.size();
  return opis.str();
}
