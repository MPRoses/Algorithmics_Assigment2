// Implementatie van klasse Stand.

#include <iostream>
#include <iomanip>
#include "stand.h"
#include "standaard.h"
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
#include <unordered_set>
#include <deque>



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
  if (aantalStenen != 0) {
    cout << "Beschikbare stenen:\n";
  }

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
void Stand::getSteenDimensies(int steennr, int& width, int& height) {
  // Initialize width and height to 0
  width = 0;
  height = 0;

  // Calculate the width and height of the stone
  for (int i = 0; i < 5; ++i) {
    int currentWidth = 0;
    int currentHeight = 0;
    for (int j = 0; j < 5; ++j) {

      if (stenen[steennr - 1][i][j] == 'X') {
        currentWidth++;
      } else {
        currentWidth = 0;
      }

      if (currentWidth > width) {
        width = currentWidth;
      }

      if (stenen[steennr - 1][j][i] == 'X') {
        currentHeight++;
      } else {
        currentHeight = 0;
      }

      if (currentHeight > height) {
        height = currentHeight;
      }
    }
  }
}


bool Stand::legSteenNeer(int rij, int kolom, int steennr, int orient) {
    if (steennr < 1 || steennr > (aantalStenen + aantalVerwijderd + 1) || orient < 0 || orient > 7) {
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
  kopieerBord();

  // reset vars
  aantalStanden = 0;
  performance = 0;
  int gevonden = 0;

  if (slim == 0) {
    if (bepaalOplossingDom(aantalStanden)) {
      gevonden = 1;
    }
  } else {
    performance = 1;
    if (bepaalOplossingSlim(aantalStanden)) {
      gevonden = 1;
    }
  }

  if (gevonden) {
    // copy solved bord to solution
    for (int i = 0; i < hoogte; ++i) {
      for (int j = 0; j < breedte; ++j) {
        oplossing[i][j] = bord[i][j];
      }
    }
  }
  
  herstelBord();

  if (!gevonden) {
    return false;
  }

  return true;
} 

bool Stand::bepaalOplossingDom(long long & aantalStanden) {
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
          aantalStanden++;
          // Try to place the stone
          if (legSteenNeer(rij, kolom, steennr, orient)) {

            // Recurse to place the next stone
            if (bepaalOplossingDom(aantalStanden)) {
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

vector<tuple<int, int, int>> Stand::getOptiesSlim(int steennr) {
  vector<tuple<int, int, int>> opties;

  int steenWidth, steenHeight;
  getSteenDimensies(steennr, steenWidth, steenHeight);

  for (int orient = 0; orient < 8; ++orient) {
    int width = (orient % 2 == 0) ? steenWidth : steenHeight;
    int height = (orient % 2 == 0) ? steenHeight : steenWidth;

    for (int rij = 0; rij <= hoogte - height; ++rij) {
      for (int kolom = 0; kolom <= breedte - width; ++kolom) {
        if (kanSteenHier(rij, kolom, steennr, orient)) {
          opties.push_back(make_tuple(rij, kolom, orient));
        }
      }
    }
  }

  return opties;
}
bool Stand::kanSteenHier(int rij, int kolom, int steennr, int orient) {
  if (steennr < 1 || steennr > (aantalStenen + aantalVerwijderd + 1) || orient < 0 || orient > 7) {
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
        if (x < 0 || x >= hoogte || y < 0 || y >= breedte || (bord[x][y] != 0 && (i != 0 || j != 0))) {
          return false; 
        }
      }
    }
  }

  return true;
}



bool Stand::bepaalOplossingSlim(long long & aantalStanden) {
  if (isOpgelost()) {
    return true;
  }

  for (int steennr = 1; steennr <= (aantalStenen + aantalVerwijderd); ++steennr) {
    if (isSteennrInStack(steennr)) {
      continue;
    }

    vector<tuple<int, int, int>> opties = getOptiesSlim(steennr);

    for (const auto& optie : opties) {
      if (legSteenNeer(get<0>(optie), get<1>(optie), steennr, get<2>(optie))) {
        if (bepaalOplossingSlim(aantalStanden)) {
          return true;
        }
        maakZetOngedaan();
      }
    }
  }

  return false;
}



//*************************************************************************

std::size_t Stand::computePositionKey() {
  std::size_t seed = 0;
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      seed ^= bord[i][j] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  }
  return seed;
}


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
  kopieerBord();

  // reset vars
  foundSolutions.clear();
  aantalStanden = 0;
  long long aantalOplossingen = 0;

  if (slim == 0) {
    aantalOplossingen = telOplossingenDom(aantalStanden, aantalOplossingen);
  } else {
    aantalOplossingen = telOplossingenSlim(aantalStanden, aantalOplossingen);
  }

  // Restore the original board and stacks
  herstelBord();

  return aantalOplossingen;
} 

void Stand::herstelBord() {
  // herstel bord
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      bord[i][j] = tempBord[i][j];
    }
  }
  // herstel vars
  steennrStack = tempSteennrStack;
  kolomStack = tempKolomStack;
  rijStack = tempRijStack;
  origineleWaarde = temporigineleWaarde;
  aantalStenen = tempAantalStenen;
  aantalVerwijderd = tempAantalVerwijderd;
  // herstel stenen
  for(int i = 0; i < 20; i++) {
    for(int j = 0; j < 5; j++) {
        for(int k = 0; k < 5; k++) {
            stenen[i][j][k] = tempStenen[i][j][k];
        }
    }
  }
}

void Stand::kopieerBord() {
  // sla bord op
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      tempBord[i][j] = bord[i][j];
    }
  }
  // sla vars op
  tempAantalStenen = aantalStenen;
  tempAantalVerwijderd = aantalVerwijderd;
  tempSteennrStack = steennrStack;
  tempKolomStack = kolomStack;
  tempRijStack = rijStack;
  temporigineleWaarde = origineleWaarde;
  // sla stenen op
  for(int i = 0; i < 20; i++) {
    for(int j = 0; j < 5; j++) {
      for(int k = 0; k < 5; k++) {
        tempStenen[i][j][k] = stenen[i][j][k];
      }
    }
  }
  cout << '\n';
}

int Stand::telOplossingenDom(long long & aantalStanden, long long & aantalOplossingen) {
  if (isOpgelost()) {
    std::size_t key = computePositionKey();

    bool keyExists = false;
    for (const auto& existingKey : foundSolutions) {
      if (existingKey == key) {
        keyExists = true;
        break;
      }
    }

    if (!keyExists) {
      foundSolutions.push_back(key);
      //cout << "Solved position key: " << key << '\n';

      for (int i = 0; i < hoogte; ++i) {
        for (int j = 0; j < breedte; ++j) {
          cout << bord[i][j] << ' ';
        }
        cout << '\n';
      }
        cout << '\n';
      
      aantalOplossingen++;
    }
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
          aantalStanden++;
          // Try to place the stone
          if (legSteenNeer(rij, kolom, steennr, orient)) {
            // Recurse to place the next stone
            telOplossingenDom(aantalStanden, aantalOplossingen);

            // If placing the stone did not lead to a solution, remove it
            maakZetOngedaan();
          }
        }
      }
    }
  }
  return aantalOplossingen;
}

int Stand::telOplossingenSlim(long long & aantalStanden, long long & aantalOplossingen) {
  // If the puzzle is solved
  if (isOpgelost()) {
    std::size_t key = computePositionKey();

    bool keyExists = false;
    for (const auto& existingKey : foundSolutions) {
      if (existingKey == key) {
        keyExists = true;
        break;
      }
    }

    if (!keyExists) {
      foundSolutions.push_back(key);
      cout << "Key gevonden: " << key << '\n';
      aantalOplossingen++;
    }
    return aantalOplossingen;
  }

  // For each stone
  for (int steennr = 1; steennr <= (aantalStenen + aantalVerwijderd); ++steennr) {
    // If the stone is already placed, go to the next stone
    if (isSteennrInStack(steennr)) {
      continue;
    }

    // Get all possible placements for the current stone
    vector<tuple<int, int, int>> opties = getOptiesSlim(steennr);

    // Try each placement option
    for (const auto& optie : opties) {
      // Place the stone
      if (legSteenNeer(get<0>(optie), get<1>(optie), steennr, get<2>(optie))) {
        // Recurse to place the next stone
        telOplossingenSlim(aantalStanden, aantalOplossingen);

        // If placing the stone did not lead to a solution, remove it
        maakZetOngedaan();
      }
    }
  }

  return aantalOplossingen;
}

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