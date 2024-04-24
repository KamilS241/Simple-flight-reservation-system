#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <filesystem>
#include "Classes.h"
#include "Functions.h"

Rezerwacja::Rezerwacja(std::map<std::string, std::string>mapa_lotu)
{
	ustal_dane_lotu(mapa_lotu);
	ustal_model_samolotu();
	std::cout << "Podaj liczbe pasazerow: " << std::endl;
	std::cin >> liczba_pasaz;
	bool blad=false;
	do
	{
		if (liczba_pasaz <0||liczba_pasaz>186)
		{
			blad = true;
		}
		else
		{
			for (int i = 1; i != liczba_pasaz + 1; i++)
			{
				Pasazer* p = new Pasazer;
				p->zbierz_dane_pasaz(i);
				std::cout << "Wybierz miejsce: " << std::endl;
				p->przypisz_miejsce(wybierz_miejsca());
				parazerowie.push_back(p);
			}
			stworz_numer_rez();
		}
	} while (blad);
}

void Rezerwacja:: stworz_numer_rez()
{
	bool decyzja = false;
	int numer;
	std::string str_num;
	do
	{
		numer = rand_liczba(10000, 99999);
		str_num = std::to_string(numer);
		decyzja = czy_nie_ma("numery_rezerwacji.txt", str_num);
	} while (decyzja != true);
	numer_rez = numer;
	dodaj_do_pliku("numery_rezerwacji.txt", str_num);
}

void Rezerwacja:: wyswietl_dane()
{
	std::cout << std::endl;
	std::cout << "Twoja rezerwacja:" << std::endl<<"Numer rezerwacji: "<<numer_rez<<std::endl;
	std::map < std::string, std::string > mapa = przekaz_dane();
	std::map<std::string, std::string>::iterator it = mapa.begin();
	while (it != mapa.end())
	{
		std::cout<< it->first<<": "<< it->second << std::endl;
		++it;
	}
	std::cout << "Liczba pasazerow: " << liczba_pasaz << std::endl;
	std::cout << "Dane pasazerow: " << std::endl;
	for (auto pax : parazerowie)
	{
		pax->pokaz_dane_pasaz();
	}
}

void Rezerwacja::zwolnij_pamiec()
{
	for (Pasazer* pasaz : parazerowie)
	{
		zniszcz_pasaz(pasaz);
	}
}

void Rezerwacja::zapisz_do_pliku()
{
	std::string numer_rez_str = std::to_string(numer_rez);
	std::ofstream do_pliku(numer_rez_str + ".txt");
	do_pliku << numer_rez_str << std::endl << liczba_pasaz << std::endl;
	std::map < std::string, std::string > mapa = przekaz_dane();
	std::map<std::string, std::string>::iterator it = mapa.begin();
	while (it!=mapa.end())
	{
		do_pliku << it->second << std::endl;
		++it;
	}
	do_pliku.close();
	for (Pasazer* pasaz : parazerowie)
	{
		pasaz->zapisz_do_pliku(numer_rez_str + ".txt");
	}
}

void Pasazer::zbierz_dane_pasaz(int i)
{
	std::cout << "Podaj imie pasazera " << i << ":" << std::endl;
	std::cin >> imie;
	std::cout << "Podaj nazwisko pasazera " << i << ":" << std::endl;
	std::cin >> nazwisko;
}

void Pasazer::przypisz_miejsce(std::string wybrane_miejsce)
{
	miejsce = wybrane_miejsce;
}

void Pasazer::pokaz_dane_pasaz()
{
	std::cout << nazwisko << " " << imie << ", Miejsce: " << miejsce;;
	std::cout << std::endl;
}

void Pasazer::zniszcz_pasaz(Pasazer* p)
{
	delete p;
}

void Pasazer::zapisz_do_pliku(std::string nazwa_pliku)
{
	std::string pasaz_do_dod = imie +" "+ nazwisko+" "+miejsce;
	dodaj_do_pliku(nazwa_pliku,pasaz_do_dod);
}

void Lot::ustal_dane_lotu(std::map<std::string, std::string>mapa_lotu)
{
	for (auto element : mapa_lotu)
	{
		if (element.first == "Data lotu") { data_lotu = element.second; }
		else if (element.first == "Godzina lotu") { godzina_lotu = element.second; }
		else if (element.first == "Trasa lotu") { trasa_lotu = element.second; }
		else if (element.first == "Numer lotu") { numer_lotu = element.second; }
	}
}

std::string Lot::wybierz_miejsca()
{
	std::string miejsce;
	std::string numer_miejsca;
	int rzad=0;
	std::string rzad_str;
	pokaz_samolot(model_samolotu);
	std::cout << "Obecnie zajete miejsca: ";
	std::ifstream lot(numer_lotu + ".txt");
	do
	{
		std::getline(lot, miejsce);
		if (miejsce == "Zajete")
		{
			do
			{
				std::getline(lot, miejsce);
				std::cout << miejsce<<", ";
			} while (!lot.eof());
		}
	} while (!lot.eof());
	std::cout << std::endl;
	lot.close();
	bool wolne;
	bool blad=false;
	do
	{
		do
		{
			std::cout << "Podaj rzad(1-31): ";
			std::cin >> rzad;
			if (rzad > 31||rzad<1)
			{
				std::cout << "Bledny rzad, podaj jeszcze raz" << std::endl;
				blad = true;
			}
			else
			{
				blad = false;
			}
		} while (blad == true);
		rzad_str = std::to_string(rzad);
		do
		{
			std::cout << "Podaj miejsce(A-F): ";
			std::cin >> numer_miejsca;
			if (numer_miejsca!="A"&& numer_miejsca != "B" && numer_miejsca!= "C" && numer_miejsca != "D" && numer_miejsca != "E" && numer_miejsca != "F")
			{
				std::cout << "Bledny numer miejsca, podaj jeszcze raz" << std::endl;
				blad = true;
			}
			else
			{
				blad = false;
			}
		} while (blad == true);
		miejsce = rzad_str + numer_miejsca;
		wolne=czy_nie_ma(numer_lotu + ".txt", miejsce);
		if (!wolne)
		{
			std::cout << "To miejsce jest zajete, wybierz inne" << std::endl;;
		}
	} while (!wolne);
	dodaj_do_pliku(numer_lotu + ".txt", miejsce);
	return miejsce;
}

void Lot::ustal_model_samolotu()
{
	std::ifstream numer(numer_lotu + ".txt");
	std::string model;
	do
	{
		std::getline(numer, model);
		if (model == "Model")
		{
			std::getline(numer, model);
			model_samolotu=model;
			return;
		}
	} while (!numer.eof());
	numer.close();
}

void Lot::pokaz_samolot(std::string model)
{
	std::ifstream samolot(model+".txt");
	std::string linia;
	do
	{
		std::getline(samolot, linia);
		std::cout << linia << std::endl;
	} while (!samolot.eof());
	samolot.close();
}

std::map <std::string, std::string> Lot:: przekaz_dane()
{
	std::map <std::string, std::string> mapa;
	mapa["Numer lotu"] = numer_lotu;
	mapa["Data lotu"] = data_lotu;
	mapa["Godzina lotu"] = godzina_lotu;
	mapa["Trasa lotu"] = trasa_lotu;
	mapa["Model samolotu"] = model_samolotu;
	return mapa;
}