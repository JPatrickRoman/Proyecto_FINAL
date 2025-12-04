// -------- src/interfaz/UI.cpp --------
#include "UI.h"
#include "../nucleo/Contexto.h"
#include "../nucleo/Mapa.h"
#include "../sistemas/Recursos.h"
#include <iostream>
#include <iomanip>

UI::UI() {}

void UI::mostrarPanel(const Contexto& ctx) {
    std::cout << "\n";
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "          RIVAL FRONTIERS" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Turno: #" << ctx.getTurnoActual() << std::endl;
    std::cout << "Objetivo: Alcanzar 60% de dominio antes del turno 20" << std::endl;

    Recursos* recJ1 = ctx.getRecursosJ1();
    if (recJ1) {
        std::cout << "Recursos: Comida=" << recJ1->getComida()
                  << " | Metal=" << recJ1->getMetal()
                  << " | Energia=" << recJ1->getEnergia() << std::endl;
    }

    Mapa* mapa = ctx.getMapa();
    if (mapa) {
        int dominioJ1 = mapa->calcularDominio(1);
        int dominioJ2 = mapa->calcularDominio(2);
        std::cout << "Jugador: Dominio=" << dominioJ1 << "% | Sistema: Dominio="
                  << dominioJ2 << "%" << std::endl;
    }

    std::cout << "----------------------------------------------------------" << std::endl;
}

void UI::mostrarMenu() {
    std::cout << "\n=== MENU PRINCIPAL ===" << std::endl;
    std::cout << "1. Mover unidad" << std::endl;
    std::cout << "2. Atacar" << std::endl;
    std::cout << "3. Construir edificio" << std::endl;
    std::cout << "4. Reclutar unidad" << std::endl;
    std::cout << "5. Ver estado" << std::endl;
    std::cout << "6. Ayuda" << std::endl;
    std::cout << "7. Terminar turno" << std::endl;
    std::cout << "8. Guardar partida" << std::endl;
    std::cout << "9. Salir" << std::endl;
}

void UI::mostrarAyuda() {
    std::cout << "\n=== AYUDA ===" << std::endl;
    std::cout << "CODIGOS DEL MAPA:" << std::endl;
    std::cout << "  Formato: [UNIDAD_O_EDIFICIO/TERRENO]" << std::endl;
    std::cout << "\n  Terrenos:" << std::endl;
    std::cout << "    LL = Llanura (mov normal)" << std::endl;
    std::cout << "    BO = Bosque (mov +1, def +2)" << std::endl;
    std::cout << "    MO = Montana (mov +2, def +3)" << std::endl;
    std::cout << "    AG = Agua (no transitable)" << std::endl;
    std::cout << "    PA = Pantano (mov +2, def +1)" << std::endl;
    std::cout << "\n  Unidades:" << std::endl;
    std::cout << "    J1S = Soldado Jugador 1" << std::endl;
    std::cout << "    J1A = Arquero Jugador 1" << std::endl;
    std::cout << "    J1C = Caballero Jugador 1" << std::endl;
    std::cout << "    J1M = Mago Jugador 1" << std::endl;
    std::cout << "    J1I = Ingeniero Jugador 1" << std::endl;
    std::cout << "    J2X = Unidades del Sistema (J2)" << std::endl;
    std::cout << "\n  Edificios:" << std::endl;
    std::cout << "    J1Gr = Granja (produce comida)" << std::endl;
    std::cout << "    J1Cu = Cuartel (produce metal)" << std::endl;
    std::cout << "    J1To = Torre (produce energia)" << std::endl;
    std::cout << "    J1Fo = Forja (mejora unidades)" << std::endl;
    std::cout << "    NeXX = Edificio neutral" << std::endl;
    std::cout << "\n  Ejemplo: [J1S/LL] = Soldado J1 en Llanura" << std::endl;
    std::cout << "           [../BO ] = Celda vacia, terreno Bosque" << std::endl;
    std::cout << "\nOBJETIVO:" << std::endl;
    std::cout << "  Alcanzar 60% de dominio territorial antes del turno 20" << std::endl;
    std::cout << "\nRECURSOS:" << std::endl;
    std::cout << "  Comida: mantiene unidades" << std::endl;
    std::cout << "  Metal: construir edificios y reclutar" << std::endl;
    std::cout << "  Energia: habilidades especiales" << std::endl;
    std::cout << "\nCOSTOS:" << std::endl;
    std::cout << "  Soldado/Arquero: 1 comida, 1 metal" << std::endl;
    std::cout << "  Caballero: 2 comida, 2 metal" << std::endl;
    std::cout << "  Mago: 1 comida, 1 metal, 2 energia" << std::endl;
    std::cout << "  Ingeniero: 1 comida, 1 energia" << std::endl;
    std::cout << "  Granja: 1 metal, 1 energia" << std::endl;
    std::cout << "  Cuartel: 2 metal" << std::endl;
    std::cout << "  Torre: 2 metal, 1 energia" << std::endl;
    std::cout << "  Forja: 3 metal, 2 energia" << std::endl;
    std::cout << "=====================================" << std::endl;
}

void UI::agregarLog(const std::string& mensaje) {
    bitacora.push_back(mensaje);
}

void UI::mostrarBitacora() const {
    std::cout << "\n=== BITACORA ===" << std::endl;
    for (const auto& msg : bitacora) {
        std::cout << "  " << msg << std::endl;
    }
}

void UI::limpiarBitacora() {
    bitacora.clear();
}

void UI::mostrarMensaje(const std::string& mensaje) {
    std::cout << mensaje << std::endl;
}

void UI::mostrarError(const std::string& error) {
    std::cout << "ERROR: " << error << std::endl;
}

void UI::mostrarExito(const std::string& mensaje) {
    std::cout << "✓ " << mensaje << std::endl;
}

int UI::solicitarOpcion(int min, int max) {
    int opcion;
    do {
        std::cout << "Ingrese opcion (" << min << "-" << max << "): ";
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Entrada invalida." << std::endl;
            opcion = min - 1;
        }
    } while (opcion < min || opcion > max);

    return opcion;
}

std::string UI::solicitarTexto(const std::string& prompt) {
    std::string texto;
    std::cout << prompt;
    std::cin.ignore();
    std::getline(std::cin, texto);
    return texto;
}

void UI::mostrarEstadoConMapa(const Contexto& ctx, const std::string& tituloEstado) {
    std::cout << "\n";
    std::cout << "============================================================" << std::endl;
    std::cout << "-> " << tituloEstado << std::endl;
    std::cout << "============================================================" << std::endl;

    mostrarPanel(ctx);

    Mapa* mapa = ctx.getMapa();
    if (mapa) {
        mapa->renderizar();
    }

    std::cout << std::endl;
}