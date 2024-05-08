// Implementatie van klasse Stand.

#include <iostream>
#include <iomanip>
#include "stand.h"
#include "standaard.h"
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

//*************************************************************************

Stand::Stand() {
  // TODO: implementeer zo nodig deze constructor

}

//*************************************************************************

Stand::Stand(int waardeM, int waardeN) {
  hoogte = waardeM;
  breedte = waardeN;
}

//*************************************************************************

Stand::~Stand() {
  // TODO: implementeer zo nodig deze destructor

} // ~Stand

//*************************************************************************

int Stand::getVakje(int i, int j) {
  // TODO: implementeer deze memberfunctie

  return 0;

} // getVakje

//*************************************************************************
void Stand::drukAf() {
  // Print the board
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      cout << bord[i][j] << ' ';
    }
    cout << '\n';
  }

  // Print the available stones
  cout << "Beschikbare stenen:\n";
  for (int i = 0; i < aantalTegels; ++i) {
    cout << "Steen " << i + 1 << ":\n";
    for (int j = 0; j < 5; ++j) {
      if (stenen[i][j][0] != '\0') {  // If the row is not empty
        cout << stenen[i][j] << '\n';
      }
    }
  }
} // drukAf

//*************************************************************************

bool Stand::leesInStenen(const char * invoernaam) {
  ifstream fin(invoernaam);

  if (!fin.is_open()) {
    cout << "Ongeldige bestandsnaam." << endl;
    return false;
  }

  fin >> aantalTegels;

  if (aantalTegels < 1 || aantalTegels > 20) {
    cout << "Ongeldig aantal stenen." << endl;
    return false;
  }

  string line;
  getline(fin, line);

  for (int i = 0; i < aantalTegels; ++i) {
    int mi, ni;
    fin >> mi >> ni;

    if (mi < 1 || mi > 5 || ni < 1 || ni > 5) {
      cout << "Ongeldige afmetingen voor steen " << i + 1 << "." << endl;
      return false;
    }

    getline(fin, line);

    // Lees de representatie van de steen
    for (int j = 0; j < mi; ++j) {
      getline(fin, line);
      strcpy(stenen[i][j], line.c_str());
    }
  }

  fin.close();

  return true;
} // leesInStenen


//*************************************************************************

bool Stand::legSteenNeer(int rij, int kolom, int steennr, int orient) {

  // TODO: implementeer deze memberfunctie

  return true;

} // legSteenNeer

//*************************************************************************

bool Stand::bepaalOplossing(long long & aantalStanden, bool slim,
  int oplossing[MaxDimBord][MaxDimBord]) {

  // TODO: implementeer deze memberfunctie

  return true;

} // bepaalOplossing

//*************************************************************************

void Stand::drukAfOplossing(int oplossing[MaxDimBord][MaxDimBord]) {

  // TODO: implementeer deze memberfunctie

} // drukAfOplossing

//*************************************************************************

long long Stand::telOplossingen(long long & aantalStanden, bool slim) {

  // TODO: implementeer deze memberfunctie

  return 0;

} // telOplossingen

//*************************************************************************