#pragma once

#include <string>

class PozycjaRecepty {
public:
  PozycjaRecepty(const std::string& lek, const std::string& dawkowanie);

  std::string pobierzOpis() const;

private:
  std::string lek_;
  std::string dawkowanie_;
};
