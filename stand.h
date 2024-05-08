// Definitie van klasse Stand.

#ifndef StandHVar // om te voorkomen dat dit .h bestand meerdere keren
#define StandHVar // wordt ge-include 

#include <fstream>
#include "constantes.h"
#include "steen.h"

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
    // of 0 als het vakje nog leeg is.
    // Nodig voor automatische test bij het nakijken.
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
    char stenen[20][5][5]; // representatie van de stenen
    // Bovenste rij is rij 0, onderste rij is rij hoogte-1.
    // Meest linker kolom is kolom 0, meest rechter kolom is
    // kolom breedte-1.

    // TODO: uw eigen private memberfuncties en -variabelen

    // Rotate a stone 90 degrees clockwise
    void draai90(char steen[5][5]);

    // Mirror a stone in x
    void spiegelX(char steen[5][5]);

    // Get the rotated/mirrored stone based on the orientation number
    void krijgGeorienteerdeSteen(char steen[5][5], int orient);

};

#endif