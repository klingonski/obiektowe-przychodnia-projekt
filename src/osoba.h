#pragma once

#include <string>
#include <vector>

class Osoba {
public:
  virtual ~Osoba() = default;

  int pobierzId() const;
  void powiadom(const std::string& tresc);
  const std::vector<std::string>& pobierzPowiadomienia() const;
  virtual std::string pobierzOpis() const = 0;

protected:
  Osoba(int id, const std::string& imie, const std::string& nazwisko);

  int id_;
  std::string imie_;
  std::string nazwisko_;
  std::vector<std::string> powiadomienia_;
};
