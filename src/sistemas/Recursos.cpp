// -------- src/sistemas/Recursos.cpp --------
#include "Recursos.h"
#include <iostream>

Recursos::Recursos(int comida, int metal, int energia)
    : comida(comida), metal(metal), energia(energia) {}

void Recursos::agregar(int c, int m, int e) {
    comida += c;
    metal += m;
    energia += e;
}

bool Recursos::consumir(int c, int m, int e) {
    if (!tieneSuficiente(c, m, e)) {
        std::cout << "Recursos insuficientes." << std::endl;
        return false;
    }
    
    comida -= c;
    metal -= m;
    energia -= e;
    return true;
}

bool Recursos::tieneSuficiente(int c, int m, int e) const {
    return comida >= c && metal >= m && energia >= e;
}

void Recursos::operator+=(const Recursos& otros) {
    comida += otros.comida;
    metal += otros.metal;
    energia += otros.energia;
}
