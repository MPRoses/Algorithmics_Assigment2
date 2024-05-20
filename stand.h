#ifndef StandHVa
#define StandHVar

#include <fstream>
#include "constantes.h"
#include <vector>

class Stand {
  public:
    // default constructor
    Stand();

    // constructor voor een bord met nwHoogte rijen en nwBreedte kolommen
    Stand(int nwHoogte, int nwBreedte);

    // destructor
    ~Stand();

    // Geeft waarde van hoogte
    int getHoogte() {
      return hoogte;
    }

    // Geeft waarde van breedte
    int getBreedte() {
      return breedte;
    }

    // Geef het nummer van de steen die vakje (i,j) van het bord bedekt,
    // of 0 als het vakje nog leeg is
    int getVakje(int i, int j);

    // Druk het bord en de nog beschikbare stenen (met hun nummers, in
    // de oorspronkelijke orientatie) af op het scherm.
    void drukAf();

    // Lees stenen in vanuit het tekstbestand met naam invoernaam.
    // Controleer daarbij:
    // * of het bestand wel te openen is,
    // * of het aantal stenen en de dimensies van deze stenen wel passen
    //   binnen de grenzen van de opdracht
    // Retourneer:
    // * true, als het invoerbestand te openen was en het aantal stenen
    //   en de dimensies van deze stenen passen binnen de grenzen van
    //   de opdracht
    // * false, anders
    //
    // Pre:
    // * de bedekte vakjes van een steen vormen een samenhangend geheel,
    //   dat wil zeggen: ze sluiten horizontaal of verticaal op elkaar aan
    // * de beschrijving van een steen bevat geen overbodige rijen en
    //   kolommen
    // Post:
    // * Als de returnwaarde true is, zijn de stenen opgeslagen
    //   in membervariabelen
    bool leesInStenen(const char * invoernaam);

    // Leg indien mogelijk steen `steennr' in orientatie orient op het bord,
    // in een bounding box met linker boven vakje (rij, kolom).
    // Retourneer:
    // * true, als het lukt om de steen neer te leggen
    // * false, als het niet lukt om de steen neer te leggen
    bool legSteenNeer(int rij, int kolom, int steennr, int orient);

    // Bepaal zo mogelijk 1 oplossing, uitgaande van de huidige stand.
    // Als slim==true, controleer dan na iedere zet of het aantal vakjes
    // in elk gebied nog een veelvoud van de grootste gemene deler is.
    // Als een oplossing gevonden is, stop dan met zoeken.
    // Retourneer:
    // * true, als er minstens 1 oplossing is
    // * false, anders
    // 
    // Post:
    // * De stand (de inhoud van het bord) is hetzelfde als voor aanroep
    //   van deze functie.
    // * Parameter aantalStanden is gelijk aan het aantal standen dat we
    //   hebben bekeken bij deze zoektocht
    // * Als er een oplossing is gevonden, bevat parameter oplossing deze
    //   oplossing (de complete bedekking van het bord met nummers van
    //   de stenen)
    bool bepaalOplossing(long long & aantalStanden, bool slim,
      int oplossing[MaxDimBord][MaxDimBord]);

    // Druk oplossing af op het scherm.
    // Pre:
    // * Parameter oplossing bevat een oplossing voor het huidige bord,
    //   met geldige dimensies en geldige steennummers.
    void drukAfOplossing(int oplossing[MaxDimBord][MaxDimBord]);

    // Tel het aantal oplossingen, uitgaande van de huidige stand.
    // Als slim==true, controleer dan na iedere zet of het aantal vakjes
    // in elk gebied nog een veelvoud van de grootste gemene deler is.
    // Retourneer:
    // * het gevonden aantal oplossingen
    // Post:
    // * Parameter aantalStanden is gelijk aan het aantal standen dat we
    //   hebben bekeken bij deze zoektocht
    long long telOplossingen(long long & aantalStanden, bool slim);
  private: 
    int hoogte,
    breedte, // aantal rijen/kolommen van het bord
    bord[MaxDimBord][MaxDimBord];
    int aantalStenen; // aantal stenen
    int aantalVerwijderd = 0; // aantal verwijderde stenen
    char stenen[20][5][5]; // representatie van de stenen
    int minSteenSize; // size (b*h) van kleinste steen

    // bijhouden gevonden oplossingen bij tel oplossingen
    std::vector<std::size_t> foundSolutions;
  
    // stack voor bijhouden zetten
    std::vector<int> steennrStack;
    std::vector<int> kolomStack;
    std::vector<int> rijStack;
    std::vector<char> origineleWaarde; // originele waarde van de top-left cell van elke steen

    // tempbord
    int tempBord[MaxDimBord][MaxDimBord];
    int tempAantalStenen;
    int tempAantalVerwijderd; 
    char tempStenen[20][5][5];
    std::vector<int> tempSteennrStack;
    std::vector<int> tempKolomStack;
    std::vector<int> tempRijStack;
    std::vector<char> temporigineleWaarde;

    // Houdt pair steennummers bij voor gelijke stenen
    vector<pair<int, int>> gelijkStenenParen;

    // Pair : steennummer, size
    std::vector<std::pair<int, int>> steenSizes;

    // Checkt of steen op basis van steennummer al gelegd is
    // true als gelegd, anders false
    bool isSteennrInStack(int steennr);
    // Checkt of het bord is opgelost ( ofwel vol zit)
    // Returned true als vol is, anders false
    bool isOpgelost();
    // Zoekt oplossing op basis van huidige positie met beschikbare
    // stenen
    // True als gelukt, anders false
    bool bepaalOplossingDom(long long & aantalStanden);
    // Zoekt oplossing op basis van huidige positie met beschikbare
    // stenen. Gebruikt pruning i.v.t. bepaalOplossingDom
    // True als gelukt, anders false
    bool bepaalOplossingSlim(long long & aantalStanden);
    // Telt het aantal mogelijke oplossingen op basis van huidige
    // positie met beschikbare stenen.
    // Returned het aantal oplossingen
    int telOplossingenDom(long long & aantalStanden, long long & aantalOplossingen);
    // Telt het aantal mogelijke oplossingen op basis van huidige
    // positie met beschikbare stenen. Gebruikt pruning i.v.t
    // telOplossingenDom
    // Returned het aantal oplossingen
    int telOplossingenSlim(long long & aantalStanden, long long & aantalOplossingen);

    // Verwijdert een steen van de beschikbare pool
    void verwijderSteen(int steennr);
    // Voegt een steen terug toe
    void maakZetOngedaan();
    // checkt of stenen dupli van elkaar zijn en zo ja
    // slaat dat op in vector<pair<int, int>> gelijkStenenParen
    void zoekGelijkStenen();
    // Genereerd seed op basis van huidige positie
    // Als gebruikte steen voor oplossing onderdeel is van 
    // gelijkStenenParen wordt voor de eerste een seed gegenereerd
    // zodat dupli's vermeden worden 
    size_t computePositionKey();
    // Depth first search die vakgebied met nullen bijhoudt
    void dfs(int i, int j, vector<vector<bool>>& visited, int& zeroCount);
    // Checkt of er een leeg vakgebied bestaat wat kleiner is dan 
    // int minSteenSize, zoja geeft true, anders false
    bool isEncased();
  
    // Krijg de georienteerde steen op basis van meegegeven steen en orientatie
    void krijgGeorienteerdeSteen(char steen[5][5], int orient);
    // Roteerd een steen 90 graden met de klok mee
    void draai90(char steen[5][5]);
    // Spiegelt een steen in de x-as
    void spiegelX(char steen[5][5]);
    // Geeft de achtereenlopende dimensies terug van een steen
    // E.g. XX.\n.XX\..X heeft maximaal twee horizontaal en
    // twee verticaal dus width = 2, height = 2
    void getSteenDimensies(int steennr, int& width, int& height);
    // Sorteert stenen van groot tot klein
    // Slaat het resultaat op in steenSizes : pair<steennr, size(: width * height)
    void getSteenSizes();
    // sla huidig bord, stenen etc. tijdelijk op
    void kopieerBord();
    // herstelt het bord
    void herstelBord();
    // Geeft de mogelijke opties terug in de huidige positie
    // in vorm: rij, kolom, orient
    vector<tuple<int, int, int>> getOpties(int steennr);
    // Checkt of steen geplaatst kan worden op meegegeven plek
    // True als steen hier gelegd kan worden, anders false
    bool kanSteenHier(int rij, int kolom, int steennr, int orient);
};

#endif