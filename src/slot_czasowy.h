#pragma once

#include <ctime>
#include <string>

class SlotCzasowy {
public:
  SlotCzasowy(int id, std::time_t poczatek);

  int pobierzId() const;
  std::time_t pobierzPoczatek() const;
  std::string pobierzTermin() const;
  bool czyWolny() const;
  void zablokuj();
  void zwolnij();

private:
  int id_;
  std::time_t poczatek_;
  bool zajety_ = false;
};
