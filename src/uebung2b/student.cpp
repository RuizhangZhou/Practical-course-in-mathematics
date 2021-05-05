#include "student.h"

// Eingabeoperator ">>"
std::istream &operator>>(std::istream &s, Student &a) {
    s >> a.first_name >> a.last_name >> a.matr_nr >> a.grade;
    return s;
}

// Ausgabeoperator "<<"
std::ostream &operator<<(std::ostream &s, const Student &a) {
    s << a.first_name << " " << a.last_name << " " << a.matr_nr << " " << a.grade;
    return s;
}

// Vergleichsoperator "<"
bool operator<(const Student &s1, const Student &s2) {
    return s1.last_name < s2.last_name
           or (s1.last_name == s2.last_name and s1.first_name < s2.first_name);
}

// Vergleichsoperatoren "==" bzw. "!="
bool operator==(const Student &s1, const Student &s2) {
    return s1.first_name == s2.first_name
           and s1.last_name == s2.last_name
           and s1.matr_nr == s2.matr_nr
           and s1.grade == s2.grade;
}

bool operator!=(const Student &s1, const Student &s2) {
    return !(s1 == s2);
}
