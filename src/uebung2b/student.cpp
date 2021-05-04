#include "student.h"

// Eingabeoperator ">>"
std::istream& operator>>(std::istream& s, Student& a) {
    s >> a.first_name >> a.last_name >> a.matr_nr >> a.grade;
    return s;
}

// Ausgabeoperator "<<"
std::ostream& operator<<(std::ostream& s, const Student& a) {
    // hier den Ausgabeoperator definieren
}

// Vergleichsoperator "<"
bool operator<(const Student&, const Student&) {
    // hier fehlt was
}

// Vergleichsoperatoren "==" bzw. "!="
bool operator==(const Student&, const Student&) {
    // hier fehlt was
}

bool operator!=(const Student&, const Student&) {
    // hier fehlt was
}
