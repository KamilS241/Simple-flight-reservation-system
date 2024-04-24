#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <random>
#include <time.h>
#include <map>
#include <filesystem>
#include "Functions.h"
#include "Classes.h"

void stworz_rezerwacje()
{
	std::map <std::string, std::string> wybrany_lot = wybierz_lot();
	if (wybrany_lot.empty()) { return; }
	Rezerwacja p(wybrany_lot);
	p.wyswietl_dane();
	p.zapisz_do_pliku();
	p.zwolnij_pamiec();
}

std::map <std::string,std::string> wybierz_lot()
{
	std::map <std::string, std::string> mapa_lotu;
	std::string data;
	std::string trasa;
	std::string trasa_plik;
	bool czy_istnieje;
	bool czy_nie_istnieje;
	bool koniec = false;
	bool koniec2 = false;
	std::pair<std::string, std::string> wybor_terminu;
	std::vector< std::pair<int*, std::pair<std::string, std::string>*>*> znalezione_godziny;
	int numer_godz = 1;
	int decyzja_godz=-2;
	while (!koniec2)
	{
		try
		{
			koniec = false;
			std::cout << "Podaj trase(kod lotniska wylotu i przylotu, zaraz po sobie): ";
			std::cin >> trasa;
			trasa_plik = "Trasa_" + trasa + ".txt";
			czy_istnieje = std::filesystem::exists(trasa_plik);
			if (!czy_istnieje)
			{
				throw (petla_wyboru("Niestety, nie mamy w ofercie tej trasy(lub zostal poodany zly format), czy chcesz wyszukac ponownie?(1-tak, 0-nie)"));
			}
			std::cout << "Podaj date w formacie DD/MM/RRRR: ";
			std::cin >> data;
			czy_nie_istnieje = czy_nie_ma(trasa_plik, data);
			if (czy_nie_istnieje == true)
			{
				throw petla_wyboru("Niestety, nie mamy w ofercie tej daty (lub zostal podany zly format).Czy chcesz wyszukac ponownie ? (1 - tak, 0 - nie)");
			}
			std::vector <std::pair<std::string, std::string>*>godziny = wyszukaj_loty(trasa_plik, data);
			if (!godziny.empty())
			{
				std::cout << "Godziny odlotow:\n";
				for (auto godzina : godziny)
				{
					std::pair<int*, std::pair<std::string, std::string>*>* p = new std::pair<int*, std::pair<std::string, std::string>*>;
					int* wybrany_lot = new int;
					*wybrany_lot = numer_godz;
					std::cout << "Godzina: " << numer_godz << " " << godzina->first << ", Numer lotu: " << godzina->second << std::endl;
					p->first = wybrany_lot;
					p->second = godzina;
					znalezione_godziny.push_back(p);
					numer_godz++;
				}
				std::cout << "Wybierz godzine (wybierz jeden z podanych numerow np 1,2,3...): " << std::endl;
				std::cout << "Zadna godzina ci nie odpowiada? Wybierz ponownie: 0, zakoncz program: -1" << std::endl;
				do
				{
					koniec = false;
					std::cin >> decyzja_godz;
					if (decyzja_godz == 0)
					{
						throw true;
					}
					else if (decyzja_godz == -1)
					{
						throw false;
					}
					else
					{
						for (auto godzina : znalezione_godziny)
						{
							if (decyzja_godz == *godzina->first)
							{
								std::cout << "Twoj wybor:\n" << trasa << std::endl << "Data: " << data << std::endl << "Godzina: " << godzina->second->first << std::endl;
								std::cout << "Numer lotu: " << godzina->second->second << std::endl;
								mapa_lotu["Numer lotu"] = godzina->second->second;
								mapa_lotu["Godzina lotu"] = godzina->second->first;
								mapa_lotu["Trasa lotu"] = trasa;
								mapa_lotu["Data lotu"] = data;
								for (auto godzina : znalezione_godziny)
								{
									delete godzina->first;
									delete godzina->second;
									delete godzina;
								}
								return mapa_lotu;
							}
						}
						if (koniec == false)
						{
							std::cout << "Zly wybor, wybierz ponownie.";
						}
					}
				} while (koniec == false);
			}
			else
			{
				switch (petla_wyboru("Wszystkie loty tego dnia sa pelne, czy chcesz wybrac ponownie? 1-tak, 0- nie"))
				{
				case true: continue;
				case false: throw false;
				}
			}
		}
		catch (bool wybor)
		{
			switch (wybor)
			{
			case true: continue;
			case false: return mapa_lotu;
			}
		}
	}
	return mapa_lotu;
}

std::vector <std::pair<std::string, std::string>*>wyszukaj_loty(std::string trasa, std::string data)
{
	std::vector <std::pair<std::string, std::string>*> godziny;
	std::string wartosc_z_pliku;
	std::ifstream wartosci_z_pliku(trasa);
	bool czy_jest_miejsce;
	do
	{
		std::getline(wartosci_z_pliku, wartosc_z_pliku);
		if (wartosc_z_pliku == data)
		{
			std::getline(wartosci_z_pliku, wartosc_z_pliku);
			czy_jest_miejsce = czy_nie_pelny(wartosc_z_pliku);
			if (czy_jest_miejsce)
			{
				std::pair<std::string, std::string>* p = new std::pair<std::string, std::string>;
				p->second = wartosc_z_pliku;//numer lotu
				std::getline(wartosci_z_pliku, wartosc_z_pliku);
				p->first = wartosc_z_pliku;//godzina
				godziny.push_back(p);
			}
		}
	} while (!wartosci_z_pliku.eof());
	wartosci_z_pliku.close();
	return godziny;
}

bool czy_nie_pelny(std::string nr_lotu)
{
	std::ifstream lot(nr_lotu + ".txt");
	std::string ilosc;
	do
	{
		std::getline(lot, ilosc);
		if (ilosc == "Dostepne")
		{
			std::getline(lot, ilosc);
			if (ilosc == "0")
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	} while (!lot.eof());
}

int rand_liczba(int dolny_zakres, int gorny_zakres)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(dolny_zakres, gorny_zakres);
	int liczba = distr(gen);
	return liczba;
}

void dodaj_do_pliku(std::string nazwa_pliku, std::string wartosc)
{
	std::ofstream do_pliku;
	do_pliku.open(nazwa_pliku, std::ios_base::app);
	do_pliku << wartosc << std::endl;
	do_pliku.close();
}

bool czy_nie_ma (std::string nazwa_pliku, std::string wartosc)
{
	std::string wartosc_z_pliku;
	std::ifstream wartosci_z_pliku(nazwa_pliku);
	while (!wartosci_z_pliku.eof())
	{
		std::getline(wartosci_z_pliku, wartosc_z_pliku);
		if (wartosc_z_pliku == wartosc)
		{
			wartosci_z_pliku.close();
			return false;
		}
	}
	wartosci_z_pliku.close();
	return true;
}


bool petla_wyboru(std::string kom)
{
	int decyzja;
	std::cout << kom << std::endl;
	std::cin >> decyzja;
	switch (decyzja)
	{
	case 1:return true;
	case 0:return false;
	}
}

void poczatek_programu()
{
	std::cout << "Witamy w lotach Ryanair" << std::endl<<std::endl;
}
void koniec_programu()
{
	std::cout << std::endl << "Dziekujemy za skorzystanie z naszego programu" << std::endl;
}