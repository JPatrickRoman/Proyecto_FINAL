// -------- src/nucleo/Juego.cpp --------
#include "Juego.h"
#include "Mapa.h"
#include "../interfaz/UI.h"
#include "../entidades/Controlador.h"
#include "../sistemas/Recursos.h"
#include "../utilidades/Persistencia.h"
#include <iostream>
#include <entidades/Edificio.h>
#include <entidades/Unidad.h>

Juego::Juego()
    : mapa(nullptr), interfaz(nullptr), jugador(nullptr), sistema(nullptr),
      juegoActivo(false), turnoMaximo(20), objetivoDominio(60) {}

Juego::~Juego() {
    delete mapa;
    delete interfaz;
    delete jugador;
    delete sistema;
}

void Juego::iniciar() {
    std::cout << "\n";
    std::cout << "============================================================" << std::endl;
    std::cout << "          RIVAL FRONTIERS" << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << "Inicializando juego..." << std::endl;

    mapa = new Mapa();
    mapa->inicializar();
    contexto.setMapa(mapa);

    Recursos* recJ1 = new Recursos(18, 12, 8);
    Recursos* recJ2 = new Recursos(15, 10, 5);
    contexto.setRecursosJ1(recJ1);
    contexto.setRecursosJ2(recJ2);

    jugador = new JugadorHumano();
    sistema = new SistemaIA();

    interfaz = new UI();

    juegoActivo = true;

    interfaz->mostrarPanel(contexto);
    mapa->renderizar();
    interfaz->agregarLog("Juego iniciado. Turno del Jugador 1.");

    std::cout << "\n¡Juego iniciado! Presiona Enter para comenzar...";
    std::cin.ignore();
    std::cin.get();
}

void Juego::ejecutarCiclo() {
    while (juegoActivo) {
        // Estado N: Turno del Jugador
        std::cout << "\n============================================================" << std::endl;
        std::cout << "  Estado " << (contexto.getTurnoActual() * 2 - 1)
                 << " - Turno " << contexto.getTurnoActual() << " (jugador)" << std::endl;
        std::cout << "============================================================" << std::endl;
        interfaz->mostrarPanel(contexto);
        mapa->renderizar();

        turnoJugador();

        // Estado N+1: Fase del Mundo
        std::cout << "\n============================================================" << std::endl;
        std::cout << "  Estado " << (contexto.getTurnoActual() * 2)
                 << " - Fase del mundo" << std::endl;
        std::cout << "============================================================" << std::endl;
        interfaz->mostrarPanel(contexto);
        mapa->renderizar();

        faseDelMundo();

        verificarCondiciones();

        // REGISTRAR TELEMETRÍA AL FINAL DE CADA TURNO
        GestorArchivos::exportarTelemetria(contexto, "telemetria.csv");

        contexto.incrementarTurno();

        if (contexto.getTurnoActual() > turnoMaximo) {
            std::cout << "\n¡Tiempo agotado! Evaluando resultado final..." << std::endl;
            evaluarResultadoFinal();
            juegoActivo = false;
        }
    }

    std::cout << "\n=== FIN DEL JUEGO ===" << std::endl;
    interfaz->mostrarBitacora();

    // EXPORTAR REPORTE FINAL
    std::cout << "\nGenerando reporte final..." << std::endl;
    if (GestorArchivos::exportarReporte(contexto, "reporte_final.txt")) {
        std::cout << "Reporte final guardado en 'reporte_final.txt'" << std::endl;
    }
}

void Juego::turnoJugador() {
    std::cout << "\n--- FASE DEL JUGADOR ---" << std::endl;
    contexto.resetearAcciones();

    // Generar recursos de edificios del jugador
    generarRecursosJugador();

    // Actualizar efectos temporales de unidades
    actualizarUnidadesJugador();

    interfaz->agregarLog("Turno " + std::to_string(contexto.getTurnoActual()) + " - Fase del Jugador iniciada");

    jugador->resolver_fase(contexto);

    interfaz->agregarLog("Jugador termino su turno");
}

void Juego::faseDelMundo() {
    std::cout << "\n--- FASE DEL MUNDO ---" << std::endl;

    interfaz->agregarLog("Fase del Sistema iniciada");

    sistema->resolver_fase(contexto);
    contexto.procesarEventos();

    interfaz->agregarLog("Fase del Sistema completada");
}

void Juego::verificarCondiciones() {
    if (verificarVictoria()) {
        std::cout << "\n¡VICTORIA! Has conquistado el territorio." << std::endl;
        interfaz->agregarLog("¡VICTORIA DEL JUGADOR!");
        juegoActivo = false;
    } else if (verificarDerrota()) {
        std::cout << "\n¡DERROTA! El sistema te ha superado." << std::endl;
        interfaz->agregarLog("¡DERROTA DEL JUGADOR!");
        juegoActivo = false;
    }
}

bool Juego::verificarVictoria() {
    int dominioJ1 = mapa->calcularDominio(1);
    return dominioJ1 >= objetivoDominio;
}

bool Juego::verificarDerrota() {
    // Derrota si el Sistema alcanza el objetivo primero
    int dominioJ2 = mapa->calcularDominio(2);
    if (dominioJ2 >= objetivoDominio) {
        return true;
    }

    // O si el jugador pierde todos sus recursos críticos
    Recursos* recJ1 = contexto.getRecursosJ1();
    return (recJ1->getComida() <= 0 && recJ1->getMetal() <= 0 && recJ1->getEnergia() <= 0);
}

void Juego::mostrarEstado() {
    interfaz->mostrarPanel(contexto);
    mapa->renderizar();
}

void Juego::generarRecursosJugador() {
    Recursos* recJ1 = contexto.getRecursosJ1();
    if (!recJ1 || !mapa) return;

    for (int i = 0; i < mapa->getTamano(); i++) {
        for (int j = 0; j < mapa->getTamano(); j++) {
            Celda* celda = mapa->obtenerCelda(i, j);
            if (!celda || !celda->tieneEdificio()) continue;

            Edificio* edificio = celda->getEdificio();
            if (edificio && edificio->getPropietario() == 1) {
                edificio->efecto_turno(*recJ1);
            }
        }
    }
}

void Juego::actualizarUnidadesJugador() {
    if (!mapa) return;

    for (int i = 0; i < mapa->getTamano(); i++) {
        for (int j = 0; j < mapa->getTamano(); j++) {
            Celda* celda = mapa->obtenerCelda(i, j);
            if (celda && celda->getUnidad()) {
                Unidad* unidad = celda->getUnidad();
                if (unidad->getPropietario() == 1) {
                    unidad->actualizarTurno();
                }
            }
        }
    }
}

void Juego::evaluarResultadoFinal() {
    int dominioJ1 = mapa->calcularDominio(1);
    int dominioJ2 = mapa->calcularDominio(2);

    std::cout << "\n=== EVALUACION FINAL ===" << std::endl;
    std::cout << "Dominio Jugador: " << dominioJ1 << "%" << std::endl;
    std::cout << "Dominio Sistema: " << dominioJ2 << "%" << std::endl;

    if (dominioJ1 > dominioJ2) {
        std::cout << "¡VICTORIA POR DOMINIO TERRITORIAL!" << std::endl;
        interfaz->agregarLog("Victoria del Jugador por dominio territorial");
    } else if (dominioJ2 > dominioJ1) {
        std::cout << "¡DERROTA! El Sistema controlo mas territorio." << std::endl;
        interfaz->agregarLog("Derrota del Jugador - Sistema controlo mas territorio");
    } else {
        std::cout << "¡EMPATE! Ambos controlaron el mismo territorio." << std::endl;
        interfaz->agregarLog("Empate territorial");
    }
}