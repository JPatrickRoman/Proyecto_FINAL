// -------- Contexto.h --------
#ifndef CONTEXTO_H
#define CONTEXTO_H

#include <vector>
#include "Coordenada.h"

class Mapa;
class Recursos;
class Evento;

class Contexto {
private:
    Mapa* mapa;
    Recursos* recursosJ1;
    Recursos* recursosJ2;
    int turnoActual;
    int puntosAccionRestantes;
    std::vector<Evento*> eventosActivos;

public:
    Contexto();
    ~Contexto();
    
    Mapa* getMapa() const { return mapa; }
    Recursos* getRecursosJ1() const { return recursosJ1; }
    Recursos* getRecursosJ2() const { return recursosJ2; }
    int getTurnoActual() const { return turnoActual; }
    int getPuntosAccion() const { return puntosAccionRestantes; }
    
    void setMapa(Mapa* m) { mapa = m; }
    void setRecursosJ1(Recursos* r) { recursosJ1 = r; }
    void setRecursosJ2(Recursos* r) { recursosJ2 = r; }
    void incrementarTurno() { turnoActual++; puntosAccionRestantes = 2; }
    void consumirAccion() { if (puntosAccionRestantes > 0) puntosAccionRestantes--; }
    void resetearAcciones() { puntosAccionRestantes = 2; }
    
    void agregarEvento(Evento* e) { eventosActivos.push_back(e); }
    void procesarEventos();
    void limpiarEventos();
};

#endif
