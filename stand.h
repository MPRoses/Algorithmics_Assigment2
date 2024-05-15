// Definitie van klasse Stand........
#ifndef StandHVar // om te voorkomen dat dit .h bestand meerdere keren
#define StandHVar // wordt ge-include 

#include <fstream>
#include "constantes.h"
#include "steen.h"
#include <vector>

class Stand {
  public:
    // default constructor
    Stand();

    // constructor voor een bord met nwHoogte rijen en nwBreedte kolommen
    Stand(int nwHoogte, int nwBreedte);

    // destructor
    ~Stand();

    // Geef waarde van hoogte.
    int getHoogte() {
      return hoogte;
    }

    // Geef waarde van breedte.
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
    int performance = 0;
    char opgeslagenSteen[5][5];

    std::vector<std::size_t> foundSolutions;
    
    // stack voor bijhouden zetten
    std::vector<int> steennrStack;
    std::vector<int> kolomStack;
    std::vector<int> rijStack;
    std::vector<char> origineleWaarde; // originele waarde van de top-left cell van elke steen


    // TODO: uw eigen private memberfuncties en -variabelen

    bool isSteennrInStack(int steennr);

    bool isOpgelost();

    bool bepaalOplossingDom(long long & aantalStanden);

    bool bepaalOplossingSlim(long long & aantalStanden);

    int telOplossingenDom(long long & aantalStanden, long long & aantalOplossingen);

    int telOplossingenSlim(long long & aantalStanden, long long & aantalOplossingen);

    
    // Remove a stone from the pool
    void verwijderSteen(int steennr);

    void maakZetOngedaan();

    size_t computePositionKey();

    // Rotate a stone 90 degrees clockwise
    void draai90(char steen[5][5]);

    char getSteen(int steennr);

    // Mirror a stone in x
    void spiegelX(char steen[5][5]);
  
    // Get the rotated/mirrored stone based on the orientation number
    void krijgGeorienteerdeSteen(char steen[5][5], int orient);

    void getSteenDimensies(int steennr, int& width, int& height);

    // tempbord
    int tempBord[MaxDimBord][MaxDimBord];
    int tempAantalStenen;
    int tempAantalVerwijderd; 
    char tempStenen[20][5][5];
    std::vector<int> tempSteennrStack;
    std::vector<int> tempKolomStack;
    std::vector<int> tempRijStack;
    std::vector<char> temporigineleWaarde;
      // sla huidig bord, stenen etc. tijdelijk op
    void kopieerBord();
      // herstel het bord
    void herstelBord();

    void sortStenen();

};

#endif