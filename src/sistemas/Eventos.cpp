// -------- src/sistemas/Eventos.cpp --------
#include "Eventos.h"
#include "../nucleo/Contexto.h"
#include "../sistemas/Recursos.h"
#include <iostream>

Evento::Evento(const std::string& nombre, int turno)
    : nombre(nombre), turnoActivacion(turno), activo(false) {}

EventoRefuerzo::EventoRefuerzo(int turno, int jugador, int cantidad)
    : Evento("Refuerzo", turno), jugadorObjetivo(jugador), cantidadUnidades(cantidad) {}

void EventoRefuerzo::ejecutar(Contexto& ctx) {
    std::cout << "Evento: Llegan " << cantidadUnidades 
              << " refuerzos para el jugador " << jugadorObjetivo << std::endl;
}

EventoClima::EventoClima(int turno, const std::string& tipo, int duracion)
    : Evento("Clima", turno), tipoClima(tipo), duracion(duracion) {}

void EventoClima::ejecutar(Contexto& ctx) {
    std::cout << "Evento: Clima " << tipoClima 
              << " afecta el tablero durante " << duracion << " turnos." << std::endl;
}

EventoRutaComercial::EventoRutaComercial(int turno, int bonus)
    : Evento("Ruta Comercial", turno), bonusRecursos(bonus) {}

void EventoRutaComercial::ejecutar(Contexto& ctx) {
    std::cout << "Evento: Ruta comercial activa. Bonus de recursos: +" 
              << bonusRecursos << std::endl;
    
    Recursos* recJ1 = ctx.getRecursosJ1();
    if (recJ1) {
        recJ1->agregar(bonusRecursos, bonusRecursos, 0);
    }
}

GestorEventos::GestorEventos() {}

GestorEventos::~GestorEventos() {
    for (auto evento : eventosProgamados) {
        delete evento;
    }
    for (auto evento : eventosActivos) {
        delete evento;
    }
}

void GestorEventos::programarEvento(Evento* evento) {
    if (evento) {
        eventosProgamados.push_back(evento);
    }
}

void GestorEventos::procesarTurno(int turnoActual, Contexto& ctx) {
    for (auto it = eventosProgamados.begin(); it != eventosProgamados.end(); ) {
        if ((*it)->getTurnoActivacion() <= turnoActual) {
            (*it)->activar();
            (*it)->ejecutar(ctx);
            eventosActivos.push_back(*it);
            it = eventosProgamados.erase(it);
        } else {
            ++it;
        }
    }
}

void GestorEventos::limpiarEventosCompletados() {
    eventosActivos.clear();
}
