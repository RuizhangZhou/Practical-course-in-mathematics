/**************************************************************
 *  Name       : unit.h                                        *
 *  Verwendung : Schnittstelle zu Praktikumsumgebung (MAPRA),  *
 *               Anzeigen von Grauwertbildern                  *
 *  Autor      : V. Reichelt, IGPM RWTH Aachen                 *
 *  Datum      : Jun '00                                       *
 **************************************************************/

#ifndef __UNIT_H_
#define __UNIT_H_

// Datentyp der Laenge 1 Byte (kann Zahlen im Bereich 0 bis 255 darstellen).
// Da es sich aber - streng genommen - nicht um einen Zahlentyp, sondern um
// einen Zeichentyp handelt, muss man den Datentyp fuer eine Ausgabe mittels
// "<<" nach "short" oder "int" konvertieren (engl. casten) um eine Zahl als
// Ausgabe zu bekommen, z.B.:
//   byte_t x=76; cout << short(x) << endl;

using byte_t = unsigned char;

// Die Funktion "showImage" zeigt ein Graustufenbild der Breite "width"
// und Hoehe "height" an. Die Grauwerte der einzelnen Bildpunkte werden
// (zeilenweise von links oben nach rechts unten) im Feld "pixel" uebergeben.
// (Der Programmierer ist dafuer verantwortlich, dass das Feld mindestens
// die Dimension "width*height" hat.) Ferner bekommt das Fenster die
// Ueberschrift "text".
// Der Parameter "id" hat folgende Bedeutung: Hat keins der angezeigten
// Bilder dieselbe Identifikationsnummer "id", so wird ein neues Fenster
// geoeffnet, andernfalls wird das Bild, das diese Nummer traegt, uebermalt.
// Man kann ein Fenster schliessen, indem man mindestens einen (am besten alle)
// der drei Parameter "width", "height" oder "pixel" auf 0 bzw. nullptr setzt.

void showImage(int width, int height, const byte_t pixel[], int id, const char text[]);

#endif
