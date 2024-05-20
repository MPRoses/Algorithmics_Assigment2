// Hoofdprogramma voor oplossing voor tweede programmeeropdracht Algoritmiek,
// voorjaar 2024: Stenen Leggen.
//
// Biedt de gebruiker een menustructuur om
// * een voor een zelf stenen te leggen op een bord
// * te vragen
//   - om een oplossing te bepalen vanaf de huidige stand
//   - of om het aantal oplossingen vanaf de huidige stand te tellen.
//
// Jens van der Sloot (s4018494), Arthur van der Sterren (s4097769)
// 20/05/24

#include <iostream>
#include <ctime>  // voor clock() en clock_t
#include "standaard.h"
#include "stand.h"

using namespace std;
const int MaxBestandsNaamLengte = 30; // maximale lengte van een bestandsnaam


// Speel het spel vanaf stand s1.
// Hierbij krijgt de gebruiker herhaaldelijk de kans om
// * een steen in een bepaalde orientatie op het bord neer te leggen.
//   Als dat een toegestane zet is, wordt die uitgevoerd.
//   Na iedere zet wordt de stand afgedrukt.
// * te vragen
//   - om een oplossing te bepalen vanaf de huidige stand
//   - of om het aantal oplossingen vanaf de huidige stand te tellen.
//
// Dit alles gaat door totdat de gebruiker aangeeft dat hij wil stoppen
// met het huidige spel.
void doeSpel(Stand * s1) {
  int hoogte, breedte,
  keuze,
  rij, kolom, steennr, orient,
  oplossing[MaxDimBord][MaxDimBord];
  long long aantalStanden, // aantal bekeken standen bij aanroep
  // van bepaalOplossing of telOplossingen
  aantalOpl; // aantal oplossingen gevonden met telOplossingen
  clock_t t1, t2;
  bool gelukt; // oplossing gevonden

  hoogte = s1 -> getHoogte();
  breedte = s1 -> getBreedte();

  do {
    s1 -> drukAf();
    cout << endl;
    cout << "1. Een zet doen" << endl;
    cout << "2. Bepaal een oplossing (dom)" << endl;
    cout << "3. Bepaal een oplossing (slim)" << endl;
    cout << "4. Tel oplossingen (dom)" << endl;
    cout << "5. Tel oplossingen (slim)" << endl;
    cout << "6. Stoppen met dit spel" << endl;
    cout << endl;
    cout << "Maak een keuze: ";
    cin >> keuze;

    switch (keuze) {
    case 1:
      cout << endl;
      cout << "Geef het nummer van een rij (0.." << hoogte - 1 <<
        " vanaf boven): ";
      cin >> rij;
      cout << "Geef het nummer van een kolom (0.." << breedte - 1 <<
        " vanaf links): ";
      cin >> kolom;
      cout << "Geef het nummer van een steen: ";
      cin >> steennr;
      cout << "Geef een orientatie van deze steen: ";
      cin >> orient;

      if (!s1 -> legSteenNeer(rij, kolom, steennr, orient)) {
        cout << endl;
        cout << "De opgegeven zet was niet mogelijk." << endl;
      }
      break;
    case 2:
    case 3:
      t1 = clock();
      if (keuze == 2)
        gelukt =
        s1 -> bepaalOplossing(aantalStanden, false, oplossing);
      else
        gelukt =
        s1 -> bepaalOplossing(aantalStanden, true, oplossing);
      t2 = clock();
      if (gelukt)
        s1 -> drukAfOplossing(oplossing);
      cout << endl;
      cout << "We hebben " << aantalStanden << " standen bekeken." <<
        endl;
      cout << "Dit kostte " << (t2 - t1) << " clock ticks, ofwel " <<
        (((double)(t2 - t1)) / CLOCKS_PER_SEC) << " seconden." <<
        endl;

      break;
    case 4:
    case 5:
      t1 = clock();
      if (keuze == 4)
        aantalOpl = s1 -> telOplossingen(aantalStanden, false);
      else
        aantalOpl = s1 -> telOplossingen(aantalStanden, true);
      t2 = clock();
      cout << endl;
      cout << "We hebben " << aantalStanden << " standen bekeken." <<
        endl;
      cout << "We vonden " << aantalOpl << " oplossingen." << endl;
      cout << "Dit kostte " << (t2 - t1) << " clock ticks, ofwel " <<
        (((double)(t2 - t1)) / CLOCKS_PER_SEC) << " seconden." <<
        endl;
      break;
    case 6:
      break;
    default:
      cout << endl;
      cout << "Voer een goede keuze in!" << endl;
    } // switch

  } while (keuze != 6);

} // doeSpel

void hoofdmenu() {
  Stand * s1; // pointer, om makkeijk nieuwe objecten te kunnen maken
  // en weer weg te gooien
  int keuze,
  hoogte, breedte;
  char invoernaam[MaxBestandsNaamLengte + 1];

  do {
    cout << endl;
    cout << "Geef een waarde voor het aantal rijen van het bord (hoogte): ";
    cin >> hoogte;
    cout << "Geef een waarde voor het aantal kolommen van het bord (breedte): ";
    cin >> breedte;
    if (integerInBereik("hoogte", hoogte, 1, MaxDimBord) &&
      integerInBereik("breedte", breedte, 2, MaxDimBord)) {
      s1 = new Stand(hoogte, breedte);

      cout << endl;
      cout << "Geef de naam van het tekstbestand met de stenen: ";
      cin >> invoernaam;
      if (s1 -> leesInStenen(invoernaam))
        doeSpel(s1);

      delete s1; // netjes opruimen

      do {
        cout << endl;
        cout << "1. Een nieuw spel spelen" << endl;
        cout << "2. Stoppen" << endl;
        cout << endl;
        cout << "Maak een keuze: ";
        cin >> keuze;

        if (keuze < 1 || keuze > 2) {
          cout << endl;
          cout << "Voer een goede keuze in!" << endl;
        }
      } while (keuze < 1 || keuze > 2);

    } // if integerInBereik

  } while (keuze != 2);

} // hoofdmenu

int main() {
  hoofdmenu();
  return 0;

}