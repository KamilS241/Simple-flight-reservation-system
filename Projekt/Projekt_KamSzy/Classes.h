#pragma once
#include <iostream>
#include <vector>
#include <map>

class Pasazer
{
private:
	std::string imie;
	std::string nazwisko;
	std::string miejsce;
public:
	void zbierz_dane_pasaz(int i);
	void pokaz_dane_pasaz();
	void przypisz_miejsce(std::string);
	void zniszcz_pasaz(Pasazer*);
	void zapisz_do_pliku(std::string nazwa_pliku);
};

class Lot
{
private:
	std::string numer_lotu;
	std::string data_lotu;
	std::string godzina_lotu;
	std::string trasa_lotu;
	std::string model_samolotu;
public:
	void ustal_dane_lotu(std::map<std::string, std::string>mapa_lotu);
	void pokaz_samolot(std::string model);
	std::string wybierz_miejsca();
	std::map<std::string,std::string> przekaz_dane();
	void ustal_model_samolotu();
};

class Rezerwacja: Pasazer, Lot
{
private:
	int numer_rez=0;
	std::vector<Pasazer*> parazerowie;
	int liczba_pasaz;
public:
	Rezerwacja(std::map < std::string, std::string>mapa_lotu);
	void wyswietl_dane();
	void stworz_numer_rez();
	void zwolnij_pamiec();
	void zapisz_do_pliku();
};