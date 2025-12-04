// -------- Juego.h --------
#ifndef JUEGO_H
#define JUEGO_H

#include "Contexto.h"

class Mapa;
class UI;
class Controlador;

class Juego {
private:
    Contexto contexto;
    Mapa* mapa;
    UI* interfaz;
    Controlador* jugador;
    Controlador* sistema;

    bool juegoActivo;
    int turnoMaximo;
    int objetivoDominio;

public:
    Juego();
    ~Juego();

    void iniciar();
    void ejecutarCiclo();

private:
    void turnoJugador();
    void faseDelMundo();
    void verificarCondiciones();

    bool verificarVictoria();
    bool verificarDerrota();

    void mostrarEstado();
    void generarRecursosJugador();
    void actualizarUnidadesJugador();  // NUEVA
    void evaluarResultadoFinal();
};

#endif