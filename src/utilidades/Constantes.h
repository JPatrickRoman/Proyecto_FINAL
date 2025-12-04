// -------- Constantes.h --------
#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <iostream>
#include <string>
#include <iomanip>
#include "../nucleo/Mapa.h"
#include "../sistemas/Recursos.h"

namespace Constantes {
    const int TAMANO_MAPA = 12;
    const int TURNOS_MAXIMOS = 20;
    const int OBJETIVO_DOMINIO = 60;
    const int PUNTOS_ACCION_POR_TURNO = 2;
    
    const int RECURSOS_INICIALES_COMIDA = 18;
    const int RECURSOS_INICIALES_METAL = 12;
    const int RECURSOS_INICIALES_ENERGIA = 7;
    
    const int COSTO_RECLUTAR_COMIDA = 1;
    const int COSTO_RECLUTAR_METAL = 1;
    
    const int COSTO_GRANJA_METAL = 1;
    const int COSTO_GRANJA_ENERGIA = 1;
    
    const int COSTO_TORRE_METAL = 2;
    const int COSTO_TORRE_ENERGIA = 1;
    
    const int COSTO_INGENIERO_COMIDA = 1;
    const int COSTO_INGENIERO_ENERGIA = 1;
    
    const int COSTO_HABILIDAD_ENERGIA = 1;
}

class Mapa;
class Recursos;

std::ostream& operator<<(std::ostream& os, const Mapa& mapa);
std::ostream& operator<<(std::ostream& os, const Recursos& recursos);

bool operator>(const Recursos& r1, const Recursos& r2);

std::ostream& operator<<(std::ostream& os, const Mapa& mapa) {
    os << "\n    ";
    for (int j = 0; j < mapa.getTamano(); j++) {
        os << std::setw(4) << j << "       ";
    }
    os << "\n";

    for (int i = 0; i < mapa.getTamano(); i++) {
        os << std::setw(2) << i << " ";
        for (int j = 0; j < mapa.getTamano(); j++) {
            os << "[" << std::setw(8) << std::left
               << mapa.obtenerEstadoCelda(i, j) << "] ";
        }
        os << "\n";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Recursos& recursos) {
    os << "Comida=" << recursos.getComida()
       << " | Metal=" << recursos.getMetal()
       << " | Energia=" << recursos.getEnergia();
    return os;
}

bool operator>(const Recursos& r1, const Recursos& r2) {
    int total1 = r1.getComida() + r1.getMetal() + r1.getEnergia();
    int total2 = r2.getComida() + r2.getMetal() + r2.getEnergia();
    return total1 > total2;
}

#endif
