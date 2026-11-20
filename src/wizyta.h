#pragma once

#include <ctime>
#include <memory>
#include <string>

#include "lekarz.h"
#include "pacjent.h"
#include "recepta.h"
#include "slot_czasowy.h"
#include "status_wizyty.h"

class Wizyta {
public:
  Wizyta(int id, Pacjent* pacjent, Lekarz* lekarz, SlotCzasowy* slot);

  int pobierzId() const;
  StatusWizyty pobierzStatus() const;
  Pacjent* pobierzPacjenta() const;
  Lekarz* pobierzLekarza() const;
  Recepta* pobierzRecepte() const;
  std::string pobierzOpis() const;
  bool czyMoznaOdwolac(std::time_t teraz, int limit_godz) const;
  void odwolaj();
  void przenies(SlotCzasowy* nowy_slot);
  Recepta& wystawRecepte(const std::string& lek,
                         const std::string& dawkowanie);
  void zakoncz();

private:
  int id_;
  StatusWizyty status_ = StatusWizyty::POTWIERDZONA;
  Pacjent* pacjent_;
  Lekarz* lekarz_;
  SlotCzasowy* slot_;
  std::unique_ptr<Recepta> recepta_;
};
