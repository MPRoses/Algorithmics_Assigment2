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
#include <unordered_map>
#include <deque>
#include <iomanip>
#include <climits>

using namespace std;

Stand::Stand() {}

Stand::Stand(int waardeM, int waardeN) {
  hoogte = waardeM;
  breedte = waardeN;
}

Stand::~Stand() {} // ~Stand

// Geeft het nummer van de steen die vakje (i,j) van het bord bedekt,
// of 0 als het vakje nog leeg is.
int Stand::getVakje(int i, int j) {
  if (i < 0 || i >= hoogte || j < 0 || j >= breedte) {
    cout << "Ongeldige positie (" << i << ", " << j << ")." << endl;
    return -1;
  }

  return bord[i][j];
} // getVakje

// Drukt huidige status van bord af inclusief de beschikbare stenen
void Stand::drukAf() {
  // Print het bord
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      cout << std::setw(2) << bord[i][j] << ' ';
    }
    cout << '\n';
  }

  // Print de stenen
  if (aantalStenen != 0) {
    cout << "Beschikbare stenen:\n";
  }

  for (int i = (0 + aantalVerwijderd); i < (aantalStenen + aantalVerwijderd); ++i) {
    // als steen al gelegd, ga verder
    if (stenen[i][0][0] == '-') continue;
    // anders log de steen
    cout << "Steen " << i + 1 << ":\n";
    for (int j = 0; j < 5; ++j) {
      if (stenen[i][j][0] != '\0') { // Zolang rij niet leeg is
        cout << stenen[i][j] << '\n';
      }
    }
  }
} // drukAf

// Leest het spel in op basis van gegeven bestandsnaam
// daarnaast wordt ook zoekGelijkStenen() en getSteenSizes() aangeroepen
// returned true als juist ingelezen, anders false
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

  // voor elke steen, prepend naar member var stenen[20][5][5]
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

  // zie beschrijving bij deze functies
  zoekGelijkStenen();
  getSteenSizes();

  fin.close();

  return true;
} // leesInStenen

// checkt of stenen dupli van elkaar zijn en zo ja
// slaat dat op in vector<pair<int, int>> gelijkStenenParen
// e.g. in stenen1.in zijn 1,2 en 4,5 gelijk dus paar <1,2> en <4,5>
void Stand::zoekGelijkStenen() {
  // Voor elke steen ( steen1)
  for (int steennr1 = 1; steennr1 <= aantalStenen; steennr1++) {
    // Vergelijk met elke andere steen ( steen2)
    for (int steennr2 = steennr1 + 1; steennr2 <= aantalStenen; steennr2++) {
      // Voor elke orientatie van steen 2
      for (int orient = 0; orient < 8; ++orient) {
        // init steen
        char georienteerdeSteen[5][5];

        for (int i = 0; i < 5; ++i) {
          for (int j = 0; j < 5; ++j) {
            georienteerdeSteen[i][j] = stenen[steennr2 - 1][i][j];
          }
        }
        // veranderd steen op basis van gegeven orientatie
        krijgGeorienteerdeSteen(georienteerdeSteen, orient);

        // kijk of stenen gelijk zijn
        bool gelijk = true;
        for (int i = 0; i < 5 && gelijk; i++) {
          for (int j = 0; j < 5 && gelijk; j++) {
            if (stenen[steennr1 - 1][i][j] != georienteerdeSteen[i][j]) {
              gelijk = false;
            }
          }
        }

        // zoja, prepend naar gelijkestenen
        if (gelijk) {
          gelijkStenenParen.push_back(make_pair(steennr1, steennr2));
          break;
        }
      }
    }
  }
} // zoekGelijkStenen

// neemt in steennr ( steen nummer ) en geeft in width en height de 
// breedte en hoogte van een steenrij terug ( DUS NIET e.g. als een steen
// XX.
// .XX , hier is de breedte en hoogte 2, aangezien er 2 achtereenlopende
// vakjes zijn)
void Stand::getSteenDimensies(int steennr, int & width, int & height) {
  // init
  width = 0;
  height = 0;

  // for x en y in steen
  for (int i = 0; i < 5; ++i) {
    int currentWidth = 0;
    int currentHeight = 0;
    for (int j = 0; j < 5; ++j) {
      // telt achtereenlopende vakjes
      if (stenen[steennr - 1][i][j] == 'X') {
        currentWidth++;
      } else {
        currentWidth = 0;
      }

      if (currentWidth > width) {
        width = currentWidth;
      }

      // telt achtereenlopende vakjes
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
} // getSteenDimensies

// Legt steen neer, vereist locatie (rij, kolom), welke steen en orientatie
// Returned true als de zet valide is en gelukt is, anders false
bool Stand::legSteenNeer(int rij, int kolom, int steennr, int orient) {
  // valideer valide zet
  if (steennr < 1 || steennr > (aantalStenen + aantalVerwijderd + 1) || orient < 0 || orient > 7) {
    return false;
  }
  // is de steen al gelegd?
  if (isSteennrInStack(steennr)) {
    return false;
  }

  char steen[5][5];
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      steen[i][j] = stenen[steennr - 1][i][j];
    }
  }

  // veranderd steen op basis van gegeven orientatie
  krijgGeorienteerdeSteen(steen, orient);

  // kijk of gevraagde zet buiten het bord valt
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

  // Leg de steen neer
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (steen[i][j] == 'X') {
        bord[rij + i][kolom + j] = steennr;
      }
    }
  }

  // sla de zojuist gemaakte zet op in een stack
  steennrStack.push_back(steennr); // nummer van gelegde steen
  kolomStack.push_back(kolom); // plek van gelegde steen
  rijStack.push_back(rij); // plek van gelegde steen

  // verwijder steen uit de pot
  verwijderSteen(steennr);

  return true;
} // legSteenNeer

// Checkt of steen al gelegd is
bool Stand::isSteennrInStack(int steennr) {
  for (size_t i = 0; i < steennrStack.size(); ++i) {
    if (steennrStack[i] == steennr) {
      return true;
    }
  }
  return false;
}

// Verwijderd een steen uit de pool ( member var stenen[20][5][5])
// pre: validiteit steennr is al gecheckt in legSteenNr
void Stand::verwijderSteen(int steennr) {
  // Slaat de eerste value op van de steen
  origineleWaarde.push_back(stenen[steennr - 1][0][0]);

  // Eerste value van de steen wordt op inactief gezet
  stenen[steennr - 1][0][0] = '-';

  aantalStenen--;
  aantalVerwijderd++;
}

// Maakt de zet ongedaan
void Stand::maakZetOngedaan() {
  // Als de stack empty is zijn er nog geen zetten gemaakt
  if (steennrStack.empty()) {
    cout << "Geen zetten gedaan\n";
    return;
  }

  // Krijg terug meest vars afgelopen zet
  int steennr = steennrStack.back();
  int rij = rijStack.back();
  int kolom = kolomStack.back();
  char origWaarde = origineleWaarde.back();

  // Verwijder vars afgelopen zet van de stack
  steennrStack.pop_back();
  kolomStack.pop_back();
  rijStack.pop_back();
  origineleWaarde.pop_back();

  // Herstel de eerste val van de steen
  stenen[steennr - 1][0][0] = origWaarde;

  aantalStenen++;
  aantalVerwijderd--;

  // Verwijder steen van bord
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (bord[rij + i][kolom + j] == steennr) {
        bord[rij + i][kolom + j] = 0;
      }
    }
  }
}

// Checkt of het bord is opgelost ( ofwel vol zit)
// Returned true als vol is, anders false
bool Stand::isOpgelost() {
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      // Als een cel 0 is, is het niet opgelost
      if (bord[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

// Sorteert de stenen van grootte groot naar klein
// Slaat het resultaat op in steenSizes : pair<steennr, size(: width * height)
void Stand::getSteenSizes() {
  // Initialize minSteenSize to a large value
  minSteenSize = INT_MAX;

  // Sla alle sizes op
  for (int steennr = 1; steennr <= (aantalStenen + aantalVerwijderd); ++steennr) {
    int width, height;
    getSteenDimensies(steennr, width, height);
    int size = width * height;
    steenSizes.push_back(std::make_pair(steennr, size));

    // Update minSteenSize
    if (size < minSteenSize) {
      minSteenSize = size;
    }
  }

  // Sorteer de sizes ( bubble sort)
  for (size_t i = 0; i < steenSizes.size() - 1; ++i) {
    for (size_t j = 0; j < steenSizes.size() - i - 1; ++j) {
      if (steenSizes[j].second < steenSizes[j + 1].second) {
        std::swap(steenSizes[j], steenSizes[j + 1]);
      }
    }
  }
}

//main functie voor bepaalOplossing
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
  if (isOpgelost()) {
    return true;
  }

  for (int steennr = 1; steennr <= (aantalStenen + aantalVerwijderd); ++steennr) {
    if (isSteennrInStack(steennr)) {
      continue;
    }

    vector<tuple<int, int, int>> opties = getOptiesSlim(steennr);

    for (const auto& optie : opties) {
      // Place the stone
      if (legSteenNeer(get<0>(optie), get<1>(optie), steennr, get<2>(optie))) {
        aantalStanden++;
        // Recurse to place the next stone
        if (bepaalOplossingDom(aantalStanden)) {
          return true;
        }

        // If placing the stone did not lead to a solution, remove it
        maakZetOngedaan();
      }
    }
  }
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

  for (const auto& steen : steenSizes) {
    int steennr = steen.first;
    if (isSteennrInStack(steennr)) {
      continue;
    }

    vector<tuple<int, int, int>> opties = getOptiesSlim(steennr);

    for (const auto& optie : opties) {
      if (legSteenNeer(get<0>(optie), get<1>(optie), steennr, get<2>(optie))) {
        if (isEncased() && static_cast<int>(steennrStack.size()) < (aantalStenen + aantalVerwijderd - 1)) {
          // Remove the stone before continuing to the next iteration
          maakZetOngedaan();
          continue;
        }
        aantalStanden++;
        // als bovenstaande if-loop waar is houdt in dat een nieuwe positie bereikt is
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
  // Create a map for equivalent stones
  std::unordered_map<int, int> eqStones;
  for (const auto& pair : gelijkStenenParen) {
    eqStones[pair.first] = pair.second;
    eqStones[pair.second] = pair.second;
  }

  std::size_t seed = 0;
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      int stone = bord[i][j];
      // Check if the stone is part of a pair
      if (eqStones.count(stone)) {
        stone = eqStones[stone];
      }
      seed ^= stone + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  }
  return seed;
}



void Stand::drukAfOplossing(int oplossing[MaxDimBord][MaxDimBord]) {
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      // gebruik van setw zodat bij getal > 10 whitespace juist verloopt
      cout << std::setw(2) << oplossing[i][j] << ' ';
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
      cout << "Unique solution seed found: " << key << '\n';
      for (int i = 0; i < hoogte; ++i) {
        for (int j = 0; j < breedte; ++j) {
          // gebruik van setw zodat bij getal > 10 whitespace juist verloopt
          cout << std::setw(2) << bord[i][j] << ' ';
        }
        cout << '\n';
      }

      aantalOplossingen++;
    }
  }

  for (int steennr = 1; steennr <= (aantalStenen + aantalVerwijderd); ++steennr) {
    if (isSteennrInStack(steennr)) {
      continue;
    }
    vector<tuple<int, int, int>> opties = getOptiesSlim(steennr);

    for (const auto& optie : opties) {
      if (legSteenNeer(get<0>(optie), get<1>(optie), steennr, get<2>(optie))) {
        aantalStanden++;
        // als bovenstaande if-loop waar is houdt in dat een nieuwe positie bereikt is
        // Recurse to place the next stone
        telOplossingenDom(aantalStanden, aantalOplossingen);

        maakZetOngedaan();
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
      cout << "Unique solution seed found: " << key << '\n';
      for (int i = 0; i < hoogte; ++i) {
        for (int j = 0; j < breedte; ++j) {
          // gebruik van setw zodat bij getal > 10 whitespace juist verloopt
          cout << std::setw(2) << bord[i][j] << ' ';
        }
        cout << '\n';
      }

      aantalOplossingen++;
    }
    return aantalOplossingen;
  }

  // For each stone
    for (const auto& steen : steenSizes) {
        int steennr = steen.first;
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
        if (isEncased() && static_cast<int>(steennrStack.size()) < (aantalStenen + aantalVerwijderd - 1)) {
          // Remove the stone before continuing to the next iteration
          maakZetOngedaan();
          continue;
        }
          aantalStanden++;
          // Recurse to place the next stone
          telOplossingenSlim(aantalStanden, aantalOplossingen);

          // If placing the stone did not lead to a solution, remove it
          maakZetOngedaan();
      }
    }
  }

  return aantalOplossingen;
}

void Stand::dfs(int i, int j, vector<vector<bool>>& visited, int& zeroCount) {
  // If out of bounds or not a zero, return
  if (i < 0 || i >= hoogte || j < 0 || j >= breedte || bord[i][j] != 0 || visited[i][j]) {
    return;
  }

  // Mark this cell as visited
  visited[i][j] = true;

  // Increment the count of zeros in this connected area
  zeroCount++;

  // Visit all four directions
  dfs(i - 1, j, visited, zeroCount);
  dfs(i + 1, j, visited, zeroCount);
  dfs(i, j - 1, visited, zeroCount);
  dfs(i, j + 1, visited, zeroCount);
}

bool Stand::isEncased() {
  // Create a visited array
  vector<vector<bool>> visited(hoogte, vector<bool>(breedte, false));

  // Iterate over the bord
  for (int i = 0; i < hoogte; ++i) {
    for (int j = 0; j < breedte; ++j) {
      // If we find a zero
      if (bord[i][j] == 0 && !visited[i][j]) {
        int zeroCount = 0;

        // Perform DFS to find the size of this connected area of zeros
        dfs(i, j, visited, zeroCount);

       // cout << "\nZEROCOUNT " << zeroCount << '\n';
        //drukAf();
        //cout << '\n';

        // If the size of this connected area is less than or equal to minSteenSize, return true
        if (zeroCount <= minSteenSize) {
          return true;
        }
      }
    }
  }

  // If no connected area of zeros is smaller than or equal to minSteenSize, return false
  return false;
}




// Draait steen 90 graden, gegeven dat het draait om top left val
// ofwel vakje [0][0] in steen
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