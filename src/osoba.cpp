#include "osoba.h"

Osoba::Osoba(int id, const std::string& imie, const std::string& nazwisko)
    : id_(id), imie_(imie), nazwisko_(nazwisko) {}

int Osoba::pobierzId() const {
  return id_;
}

void Osoba::powiadom(const std::string& tresc) {
  powiadomienia_.push_back(tresc);
}

const std::vector<std::string>& Osoba::pobierzPowiadomienia() const {
  return powiadomienia_;
}
