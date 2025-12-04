// -------- src/nucleo/Contexto.cpp --------
#include "Contexto.h"
#include "../sistemas/Eventos.h"

Contexto::Contexto() 
    : mapa(nullptr), recursosJ1(nullptr), recursosJ2(nullptr), 
      turnoActual(1), puntosAccionRestantes(2) {}

Contexto::~Contexto() {
    limpiarEventos();
}

void Contexto::procesarEventos() {
    for (auto evento : eventosActivos) {
        if (evento) {
            evento->ejecutar(*this);
        }
    }
}

void Contexto::limpiarEventos() {
    for (auto evento : eventosActivos) {
        delete evento;
    }
    eventosActivos.clear();
}
