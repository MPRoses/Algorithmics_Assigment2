// Implementatie van klasse Stand.

#include <iostream>
#include <iomanip>
#include "stand.h"
#include "standaard.h"
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

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

// Geef het nummer van de steen die vakje (i,j) van het bord bedekt,
// of 0 als het vakje nog leeg is.
int Stand::getVakje(int i, int j) {
  if (i < 0 || i >= hoogte || j < 0 || j >= breedte) {
    cout << "Ongeldige positie (" << i << ", " << j << ")." << endl;
    return -1;  // Return -1 for invalid position
  }

  return bord[i][j];
}//getVakje


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
  for (int i = (0 + aantalVerwijderd); i < (aantalStenen + aantalVerwijderd); ++i) {
    // als steen al gelegd, ga verder
    if (stenen[i][0][0] == '-') continue;
    // anders log de steen
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

    if (mi < 1 || mi > breedte || ni < 1 || ni > hoogte) {
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

  if (isSteennrInStack(steennr)) {
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

  steennrStack.push_back(steennr);
  kolomStack.push_back(kolom);
  rijStack.push_back(rij);

  verwijderSteen(steennr);

  return true;
}//legSteenNeer

bool Stand::isSteennrInStack(int steennr) {
  for (size_t i = 0; i < steennrStack.size(); ++i) {
    if (steennrStack[i] == steennr) {
      return true;
    }
  }
  return false;
}


// Remove a stone from the pool
// pre: validiteit steennr is al gecheckt in legSteenNr
void Stand::verwijderSteen(int steennr) {
  // Save the original value of the top-left cell
  origineleWaarde.push_back(stenen[steennr - 1][0][0]);

  stenen[steennr - 1][0][0] = '-';

  aantalStenen--;
  aantalVerwijderd++;
}

void Stand::maakZetOngedaan() {
    // Check if the stacks are empty
    if (steennrStack.empty()) {
        cout << "No moves to undo!" << endl;
        return;
    }

    // Get the most recently added variables
    int steennr = steennrStack.back();
    int rij = rijStack.back();
    int kolom = kolomStack.back();
    char origWaarde = origineleWaarde.back();

    // Remove the most recent move from the stacks
    steennrStack.pop_back();
    kolomStack.pop_back();
    rijStack.pop_back();
    origineleWaarde.pop_back();

    // Restore the original value of the top-left cell of the stone
    stenen[steennr - 1][0][0] = origWaarde;

    aantalStenen++;
    aantalVerwijderd--;

    // Remove the stone from the board
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (bord[rij + i][kolom + j] == steennr) {
                bord[rij + i][kolom + j] = 0;
            }
        }
    }
}

bool Stand::isOpgelost() {
    for (int i = 0; i < hoogte; ++i) {
      for (int j = 0; j < breedte; ++j) {
        // If any cell on the board is empty (0), the game is not solved
        if (bord[i][j] == 0) {
          return false;
        }
      }
    }
    return true;
}

bool Stand::bepaalOplossing(long long & aantalStanden, bool slim, int oplossing[MaxDimBord][MaxDimBord]) {
  aantalStanden = 0;
  // Create a copy of the board
  int tempBord[MaxDimBord][MaxDimBord];
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      tempBord[i][j] = bord[i][j];
    }
  }

  // Create a copy of the stacks
  std::vector<int> tempSteennrStack = steennrStack;
  std::vector<int> tempKolomStack = kolomStack;
  std::vector<int> tempRijStack = rijStack;
  std::vector<char> temporigineleWaarde = origineleWaarde;

  int gevonden = 0;

  if (slim == 0) {
    if (bepaalOplossingDom(aantalStanden, oplossing)) {
      gevonden = 1;
    }
  } else {
    // smart
  }

  if (gevonden) {
    // copy solved bord to solution
    for (int i = 0; i < hoogte; ++i) {
      for (int j = 0; j < breedte; ++j) {
        oplossing[i][j] = bord[i][j];
      }
    }
  }

  // Restore the original board and stacks
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      bord[i][j] = tempBord[i][j];
    }
  }
  steennrStack = tempSteennrStack;
  kolomStack = tempKolomStack;
  rijStack = tempRijStack;
  origineleWaarde = temporigineleWaarde;

  if (!gevonden) {
    return false;
  }

  return true;
} 

bool Stand::bepaalOplossingDom(long long & aantalStanden, int oplossing[MaxDimBord][MaxDimBord]) {
  // Base case: If the board is solved, we have found a solution
  if (isOpgelost()) {
    return true;
  }

  // Try to place each stone on the board
  for (int steennr = 1; steennr <= (aantalStenen + aantalVerwijderd); ++steennr) {
    // If the stone is already in the stack, skip this iteration
    if (isSteennrInStack(steennr)) {
      continue;
    }
    for (int rij = 0; rij < hoogte; ++rij) {
      for (int kolom = 0; kolom < breedte; ++kolom) {
        for (int orient = 0; orient < 8; ++orient) {
          // Try to place the stone
          if (legSteenNeer(rij, kolom, steennr, orient)) {
            aantalStanden++;

            // Recurse to place the next stone
            if (bepaalOplossingDom(aantalStanden, oplossing)) {
              return true;
            }

            // If placing the stone did not lead to a solution, remove it
            maakZetOngedaan();
          }
        }
      }
    }
  }

  // If no solution was found after trying all stones, return false
  return false;
}



//*************************************************************************

void Stand::drukAfOplossing(int oplossing[MaxDimBord][MaxDimBord]) {
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      cout << oplossing[i][j] << ' ';
    }
    cout << '\n';
  }
}

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