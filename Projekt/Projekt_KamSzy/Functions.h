#pragma once
#include <map>

void poczatek_programu();

void stworz_rezerwacje();

std::map <std::string, std::string> wybierz_lot();

std::vector <std::pair<std::string, std::string>*>wyszukaj_loty(std::string trasa, std::string data);

bool czy_nie_pelny(std::string nr_lotu);

int rand_liczba(int dolny_zakres, int gorny_zakres);

void dodaj_do_pliku(std::string nazwa_pliku, std::string wartosc);

bool czy_nie_ma(std::string nazwa_pliku, std::string wartosc);

bool petla_wyboru(std::string kom);

void koniec_programu();

