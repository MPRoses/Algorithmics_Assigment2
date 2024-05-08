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
  for (int i = 0; i < aantalStenen; ++i) {
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

  fin >> aantalStenen;

  if (aantalStenen < 1 || aantalStenen > 20) {
    cout << "Ongeldig aantal stenen." << endl;
    return false;
  }

  string line;
  getline(fin, line);

  for (int i = 0; i < aantalStenen; ++i) {
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
  if (steennr < 1 || steennr > (aantalStenen + 1) || orient < 0 || orient > 7) {
    return false;
  }

  char steen[5][5];
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      steen[i][j] = stenen[steennr - 1][i][j];
    }
  }

  // Rotate the stone to the desired orientation
  krijgGeorienteerdeSteen(steen, orient);

   // Check if the stone fits within the board
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (steen[i][j] == 'X') {
        int x = rij + i;
        int y = kolom + j;
        if (x < 0 || x >= hoogte || y < 0 || y >= breedte || bord[x][y] != 0) {
          cout << "De steen past niet op het bord! \n";
          return false; 
        }
      }
    }
  }

  // Place the stone on the board
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (steen[i][j] == 'X') {
        bord[rij + i][kolom + j] = steennr;
      }
    }
  }

  cout << "REACHAEHFCHADA \n";

  // Remove the stone from the pool of available stones
  // TODO: Implement the removal of the stone from the pool

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
void Stand::draai90(char steen[5][5]) {
  char temp[5][5];
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      temp[j][4 - i] = steen[i][j];
    }
  }
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      steen[i][j] = temp[i][j];
    }
  }
}

void Stand::spiegelX(char steen[5][5]) {
  for (int i = 0; i < 2; ++i) {  // Only need to iterate through half the rows
    for (int j = 0; j < 5; ++j) {
      swap(steen[i][j], steen[4 - i][j]);
    }
  }
}

void Stand::krijgGeorienteerdeSteen(char steen[5][5], int orient) {
  if (orient >= 4) {
    spiegelX(steen);
    orient -= 4;
  }

  for (int i = 0; i < orient; ++i) {
    draai90(steen);
  }
// Shift the stone to the top-left corner
  int top = 5, left = 5;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (steen[i][j] == 'X') {
        top = min(top, i);
        left = min(left, j);
      }
    }
  }

  if (top > 0 || left > 0) {
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        int x = i + top;
        int y = j + left;
        steen[i][j] = (x < 5 && y < 5) ? steen[x][y] : '.';
      }
    }
  }
}