// Definitie van klasse Steen.

#ifndef SteenHVar  // om te voorkomen dat dit .h bestand meerdere keren
#define SteenHVar  // wordt ge-include 

#include "constantes.h"

class Steen
{ public:
    // constructor
    Steen ();

    // Lees zo mogelijk een steen in vanuit fin.
    // Pre:
    // * fin bevat achtereenvolgens:
    //   - een regel met twee integers m0 en n0, gescheiden door een spatie
    //   - m0 regels met n0 karakters . of X
    // Post:
    // * als m0<=MaxDim en n0<=MaxDim is de ingelezen steen opgeslagen in
    //   array inhoud
    // Retourneer:
    // * true, als 0<=m0<=MaxDim en 0<=n0<=MaxDim
    // * false, anders
    bool leesIn (ifstream &fin);

    // Geef waarde van m.
    int getM ()
    {
      return m;

    }  // getM

    // Geef waarde van n.
    int getN ()
    {
      return n;

    }  // getN

    // Is vakje (i,j) van de steen gevuld?
    bool gevuld (int i, int j);

    // Geef het aantal vakjes dat de steen bedekt.
    int grootte ();

    // Druk steen af op het scherm.
    void drukAf ();

    // Roteer steen een aantal keer 90 graden met de klok mee.
    void roteer (int aantalkeer);

    // Spiegel steen in horizontale as.
    void spiegelHorizontaleAs ();

    // TODO: eventueel uw eigen public memberfuncties

  private:
    char inhoud[MaxDimSteen][MaxDimSteen];
      // Bovenste rij is rij 0, onderste rij is rij MaxDim-1.
      // Meest linker kolom is kolom 0, meest rechter kolom is kolom MaxDim-1.
    int m, n;  // Feitelijk aantal rijen (vanaf boven) / kolommen (vanaf links)
               // van steen dat in gebruik (deels gevuld) is.
               // Deze waardes worden gezet bij het inlezen van een steen.

    // TODO: uw eigen private memberfuncties en -variabelen

};

#endif

