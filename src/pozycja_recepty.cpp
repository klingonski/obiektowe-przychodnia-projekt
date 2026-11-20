#include "pozycja_recepty.h"

PozycjaRecepty::PozycjaRecepty(const std::string& lek,
                               const std::string& dawkowanie)
    : lek_(lek), dawkowanie_(dawkowanie) {}

std::string PozycjaRecepty::pobierzOpis() const {
  return lek_ + " - " + dawkowanie_;
}
