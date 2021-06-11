/********************************************************************
 *  Name       : mapraview.cpp                                       *
 *  Verwendung : Benutzerschnittstelle zu einer Bildklasse (MAPRA),  *
 *               Bildbearbeitung: PGM-Format,                        *
 *               Punktoperationen und lokale Filter                  *
 *  Autor      : Sven Gross, Volker Reichelt, IGPM RWTH Aachen       *
 *  Datum      : Jun '00                                             *
 ********************************************************************/

#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>

#include "greyscale.h"
#include "unit.h"

//using namespace std;

// globale Konstanten, die die Darstellung des Bildes kontrollieren

int zoomFac = 1;   // Vergroesserungsfaktor
bool show = true;  // Bild anzeigen oder nicht

// Die Funktion "display" dient als Verbindung zwischen der "Greyscale"-Klasse
// und der Funktion "showImage" aus "unit7. Hier wird der Datentyp "float"
// in den Datentyp "byte_t" (inclusive Clamping) umgewandelt. Je nach dem Wert
// von "zoomFac" werden aus einem "float"-Pixel ein oder mehrere "byte_t"-Pixel.

void display(const GreyScale &pic, int id, const char text[]) {
    if (!show) return;  // Bild nicht anzeigen

    std::vector<byte_t> pixel(pic.getWidth() * pic.getHeight() * zoomFac * zoomFac);

    for (int j = 0; j < pic.getHeight(); j++)  // Fuer alle Bildpunkte ...
        for (int i = 0; i < pic.getWidth(); i++) {
            float gr = std::rint(pic(i, j) * 255);  // [0,1] auf [0,255] skalieren
            if (gr > 255)
                gr = 255;
            else if (gr < 0)
                gr = 0;                                      // clampen
            if (zoomFac == 1)                                // behandle den Fall zoomFac==1
                pixel[i + j * pic.getWidth()] = byte_t(gr);  // der Effizienz wegen einzeln
            else
                for (int k = 0; k < zoomFac; k++)
                    for (int l = 0; l < zoomFac; l++)
                        pixel[(i * zoomFac + k) + (j * zoomFac + l) * pic.getWidth() * zoomFac] = byte_t(gr);
        }

    showImage(pic.getWidth() * zoomFac, pic.getHeight() * zoomFac, pixel.data(), id, text);
}

// Das Hauptprogramm besteht im wesentlichen aus einer Schleife, die auf
// die Benutzereingaben wartet und die Kommandos (in der switch-Anweisung)
// interpretiert (z.B. die Filter aus der Greyscale-Klasse aufruft oder die
// globalen Variablen "show" und "zoomFac" setzt und das Ergebnis anzeigt).

int main() {
    GreyScale pic, newpic;
    char name[200], c = 'h';
#ifdef SET_FORMAT
    int frmt;
#endif

    do {  // Main loop
        switch (c) {
            case 'h':  // Help
#ifdef SET_FORMAT
                std::cout << "(h)elp (r)ead (w)rite (f)ormat (u)ndo (q)uit (d)isplay (1) (2) (3) (4)\n"
#else
                std::cout << "(h)elp (r)ead (w)rite (u)ndo (q)uit (d)isplay on/off (1) (2) (3) (4)\n"
#endif
                          << "(+) (-) (<) (>) (b)inarize (B)lur (c)lamp (C)ontrast (i)nvert\n"
                          << "(k)irsch (K)irsch+ (l)aplace (L)aplace+ (m)edian (s)obel (S)obel+" << std::endl;
                break;

            case 'r':  // Read
                std::cout << "Read image... Enter filename: ";
                std::cin >> name;
                {
                    std::ifstream file(name);
                    if (file.rdstate())
                        std::cerr << "Error: Couldn't open '" << name << "'!\n";
                    else {
                        file >> newpic;
                        display(newpic, 0, name);
                        pic.resize(0, 0);
                        display(pic, 1, "");
                    }
                }
                break;

            case 'w':  // Write
                std::cout << "Write image... Enter filename: ";
                std::cin >> name;
                {
                    std::ofstream file(name);
                    if (file.rdstate())
                        std::cerr << "Error: Couldn't open '" << name << "'!\n";
                    else
                        file << newpic;
                }
                break;

#ifdef SET_FORMAT
            case 'f':  // Output-Format
                std::cout << "Output format... (0) PGM-Ascii (1) PGM-Raw (2) Huffman (3) Huffman-Dist\n"
                          << "Enter format: ";
                std::cin >> frmt;
                GreyScale::setFormat(frmt);
                break;
#endif

            case 'u':  // Undo
                std::cout << "Undo...\n";
                newpic = pic;
                display(newpic, 1, "Undo");
                break;

            case 'd':  // display on/off
                show = !show;
                std::cout << "Display " << (show ? "on" : "off") << " ...\n";
                display(newpic, 1, "Display");
                break;

            case '1':  // Zoom
            case '2':
            case '3':
            case '4':
                zoomFac = c - 48;
                std::cout << "Set zoom factor " << zoomFac << "...\n";
                display(newpic, 1, "Zoom");
                break;

            case '+':  // Lighter
                std::cout << "Lighter...\n";
                pic = newpic;
                newpic = pic.linTrans(1, 0.1);
                display(newpic, 1, "Lighter");
                break;

            case '-':  // Darker
                std::cout << "Darker...\n";
                pic = newpic;
                newpic = pic.linTrans(1, -0.1);
                display(newpic, 1, "Darker");
                break;

            case '<':  // Increase contrast
                std::cout << "Increase contrast...\n";
                pic = newpic;
                newpic = pic.linTrans(1.1, -0.05);
                display(newpic, 1, "Increase contrast");
                break;

            case '>':  // Decrease contrast
                std::cout << "Decrease contrast...\n";
                pic = newpic;
                newpic = pic.linTrans(0.9, 0.05);
                display(newpic, 1, "Decrease contrast");
                break;

            case 'b':  // Binarize
                std::cout << "Binarize...\n";
                pic = newpic;
                newpic = pic.binarize(0.5);
                display(newpic, 1, "Binarize");
                break;

            case 'B':  // Blur
                std::cout << "Blur...\n";
                pic = newpic;
                newpic = pic.blur();
                display(newpic, 1, "Blur");
                break;

            case 'c':  // Clamp
                std::cout << "Clamp...\n";
                pic = newpic;
                newpic = pic.clamp();
                display(newpic, 1, "Clamp");
                break;

            case 'C':  // Contrast
                std::cout << "Maximize contrast...\n";
                pic = newpic;
                newpic = pic.contrast();
                display(newpic, 1, "Maximize contrast");
                break;

            case 'i':  // Invert
                std::cout << "Invert...\n";
                pic = newpic;
                newpic = pic.linTrans(-1, 1);
                display(newpic, 1, "Invert");
                break;

            case 'k':  // Kirsch
                std::cout << "Kirsch filter...\n";
                pic = newpic;
                newpic = pic.kirsch();
                display(newpic, 1, "Kirsch");
                break;

            case 'K':  // Kirsch+
                std::cout << "Add kirsch filter...\n";
                pic = newpic;
                newpic = pic.kirsch().linTrans(0.1, 0);
                newpic += pic;
                display(newpic, 1, "Add kirsch");
                break;

            case 'l':  // Laplace
                std::cout << "Laplace filter...\n";
                pic = newpic;
                newpic = pic.laplace();
                display(newpic, 1, "Laplace");
                break;

            case 'L':  // Laplace+
                std::cout << "Add Laplace filter...\n";
                pic = newpic;
                newpic = pic.laplace().linTrans(0.1, 0);
                newpic += pic;
                display(newpic, 1, "Add Laplace");
                break;

            case 'm':  // median
                std::cout << "Median filter...\n";
                pic = newpic;
                newpic = pic.median();
                display(newpic, 1, "Median");
                break;

            case 's':  // Sobel
                std::cout << "Sobel filter...\n";
                pic = newpic;
                newpic = pic.sobel();
                display(newpic, 1, "Sobel");
                break;

            case 'S':  // Sobel+
                std::cout << "Add Sobel filter...\n";
                pic = newpic;
                newpic = pic.sobel().linTrans(0.1, 0);
                newpic += pic;
                display(newpic, 1, "Add Sobel");
                break;

            default:  // Error
                std::cout << "Warning: Ignored unknown command '" << c << "'\n";
                break;
        }

        std::cout << "Enter Command (h for help): ";
        std::cin >> c;
        if (std::cin.eof()) c = 'q';

    } while (c != 'q');  // Quit


    //1.
    GreyScale gs;
    ifstream  ifs;
    ifs.open("bilder/mapra.pgm", ios::in );
    ifs >> gs;
    ifs.close();

    ofstream ofs;
    ofs.open("bilder/result01.pgm",ios::out);
    ofs << gs;
    ofs.close();
    
    //2.
    ifs.open("bilder/puppenbrunnen.pgm", ios::in );
    ifs >> gs;
    ifs.close();

    ofs.open("bilder/result02.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //3.
    ifs.open("bilder/dom.pgm", ios::in );
    ifs >> gs;
    for(int i =1;i<=5;i++){
       gs.median();
    }
    ifs.close();

    ofs.open("bilder/result03.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //4.
    ifs.open("bilder/dom.pgm", ios::in );
    ifs >> gs;
    for(int i =1;i<=2;i++){
       gs.blur();
    }
    ifs.close();

    ofs.open("bilder/result04.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //5.
    ifs.open("bilder/shuttle.pgm", ios::in );
    ifs >> gs;
    gs.kirsch();
    gs.contrast();
    ifs.close();

    ofs.open("bilder/result05.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //6.
    ifs.open("bilder/shuttle.pgm", ios::in );
    ifs >> gs;
    gs.laplace();
    ifs.close();

    ofs.open("bilder/result06.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //7.
    ifs.open("bilder/shuttle.pgm", ios::in );
    ifs >> gs;
    gs.sobel();
    ifs.close();

    ofs.open("bilder/result07.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //8.
    ifs.open("bilder/bauteil.pgm", ios::in );
    ifs >> gs;
    gs.contrast();
    gs.binarize(0.5);//which value should we choose? 0.5?
    ifs.close();

    ofs.open("bilder/result08.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //9.
    ifs.open("bilder/bauteil.pgm", ios::in );
    ifs >> gs;
    gs.sobel();
    gs.invert();
    ifs.close();

    ofs.open("bilder/result09.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //10. what's the meaning of "+" hier?
    ifs.open("bilder/roentgen.pgm", ios::in );
    ifs >> gs;
    for(int i=1;i<=3;i++){
        gs.sobel();
    }
    gs.contrast();
    ifs.close();

    ofs.open("bilder/result10.pgm",ios::out);
    ofs << gs;
    ofs.close();

    //11. Also hier what's the meaning of "+"?
    ifs.open("bilder/result4.pgm", ios::in );
    ifs >> gs;
    for(int i=1;i<5;i++){
        gs.laplace();
    }
    ifs.close();

    ofs.open("bilder/result11.pgm",ios::out);
    ofs << gs;
    ofs.close();








    return 0;
}
