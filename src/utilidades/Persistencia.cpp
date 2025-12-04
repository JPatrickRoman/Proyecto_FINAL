// -------- src/utilidades/Persistencia.cpp --------
#include "Persistencia.h"
#include "../nucleo/Contexto.h"
#include "../nucleo/Mapa.h"
#include "../sistemas/Recursos.h"
#include "../entidades/Unidad.h"
#include "../entidades/Edificio.h"
#include <iostream>
#include <iomanip>

bool GestorArchivos::guardarPartida(const Contexto& ctx, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al crear archivo de guardado." << std::endl;
        return false;
    }

    archivo << "RIVAL_FRONTIERS_SAVE_V1" << std::endl;
    archivo << "TURNO=" << ctx.getTurnoActual() << std::endl;

    escribirRecursos(archivo, ctx);
    escribirEstadoMapa(archivo, ctx);

    archivo.close();
    std::cout << "Partida guardada exitosamente." << std::endl;
    return true;
}

bool GestorArchivos::cargarPartida(Contexto& ctx, const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir archivo de guardado." << std::endl;
        return false;
    }

    std::string linea;
    std::getline(archivo, linea);

    if (linea != "RIVAL_FRONTIERS_SAVE_V1") {
        std::cerr << "Formato de archivo invalido." << std::endl;
        return false;
    }

    archivo.close();
    std::cout << "Partida cargada (implementacion basica)." << std::endl;
    return true;
}

bool GestorArchivos::exportarReporte(const Contexto& ctx, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al crear archivo de reporte." << std::endl;
        return false;
    }

    archivo << "============================================================" << std::endl;
    archivo << "          REPORTE DE PARTIDA - RIVAL FRONTIERS" << std::endl;
    archivo << "============================================================" << std::endl;
    archivo << std::endl;

    // INFORMACIÓN GENERAL
    archivo << "--- INFORMACION GENERAL ---" << std::endl;
    archivo << "Turno actual: " << ctx.getTurnoActual() << std::endl;
    archivo << "Acciones restantes: " << ctx.getPuntosAccion() << std::endl;
    archivo << std::endl;

    // RECURSOS JUGADOR 1
    Recursos* recJ1 = ctx.getRecursosJ1();
    if (recJ1) {
        archivo << "--- RECURSOS JUGADOR 1 ---" << std::endl;
        archivo << "Comida:   " << recJ1->getComida() << std::endl;
        archivo << "Metal:    " << recJ1->getMetal() << std::endl;
        archivo << "Energia:  " << recJ1->getEnergia() << std::endl;
        archivo << std::endl;
    }

    // RECURSOS JUGADOR 2 (IA)
    Recursos* recJ2 = ctx.getRecursosJ2();
    if (recJ2) {
        archivo << "--- RECURSOS JUGADOR 2 (IA) ---" << std::endl;
        archivo << "Comida:   " << recJ2->getComida() << std::endl;
        archivo << "Metal:    " << recJ2->getMetal() << std::endl;
        archivo << "Energia:  " << recJ2->getEnergia() << std::endl;
        archivo << std::endl;
    }

    // ANÁLISIS DEL MAPA
    Mapa* mapa = ctx.getMapa();
    if (mapa) {
        archivo << "--- CONTROL TERRITORIAL ---" << std::endl;
        int dominioJ1 = mapa->calcularDominio(1);
        int dominioJ2 = mapa->calcularDominio(2);
        int dominioNeutral = 100 - dominioJ1 - dominioJ2;

        archivo << "Jugador 1:  " << dominioJ1 << "%" << std::endl;
        archivo << "Jugador 2:  " << dominioJ2 << "%" << std::endl;
        archivo << "Neutral:    " << dominioNeutral << "%" << std::endl;
        archivo << std::endl;

        // CONTEO DE UNIDADES
        int unidadesJ1 = 0, unidadesJ2 = 0;
        int edificiosJ1 = 0, edificiosJ2 = 0, edificiosNeutrales = 0;

        for (int i = 0; i < mapa->getTamano(); i++) {
            for (int j = 0; j < mapa->getTamano(); j++) {
                Celda* celda = mapa->obtenerCelda(i, j);
                if (!celda) continue;

                if (celda->getUnidad()) {
                    if (celda->getUnidad()->getPropietario() == 1) unidadesJ1++;
                    else if (celda->getUnidad()->getPropietario() == 2) unidadesJ2++;
                }

                if (celda->tieneEdificio()) {
                    int prop = celda->getEdificio()->getPropietario();
                    if (prop == 1) edificiosJ1++;
                    else if (prop == 2) edificiosJ2++;
                    else if (prop == 0) edificiosNeutrales++;
                }
            }
        }

        archivo << "--- UNIDADES ---" << std::endl;
        archivo << "Jugador 1: " << unidadesJ1 << " unidades" << std::endl;
        archivo << "Jugador 2: " << unidadesJ2 << " unidades" << std::endl;
        archivo << std::endl;

        archivo << "--- EDIFICIOS ---" << std::endl;
        archivo << "Jugador 1:  " << edificiosJ1 << " edificios" << std::endl;
        archivo << "Jugador 2:  " << edificiosJ2 << " edificios" << std::endl;
        archivo << "Neutrales:  " << edificiosNeutrales << " edificios" << std::endl;
        archivo << std::endl;

        // DETALLE DE UNIDADES POR TIPO
        archivo << "--- DETALLE DE UNIDADES JUGADOR 1 ---" << std::endl;
        int soldados = 0, arqueros = 0, caballeros = 0, magos = 0, ingenieros = 0;

        for (int i = 0; i < mapa->getTamano(); i++) {
            for (int j = 0; j < mapa->getTamano(); j++) {
                Celda* celda = mapa->obtenerCelda(i, j);
                if (!celda || !celda->getUnidad()) continue;

                Unidad* unidad = celda->getUnidad();
                if (unidad->getPropietario() != 1) continue;

                std::string codigo = unidad->getCodigo();
                if (codigo.find("So") != std::string::npos) soldados++;
                else if (codigo.find("Ar") != std::string::npos) arqueros++;
                else if (codigo.find("Ca") != std::string::npos) caballeros++;
                else if (codigo.find("Ma") != std::string::npos) magos++;
                else if (codigo.find("In") != std::string::npos) ingenieros++;

                archivo << "  (" << i << "," << j << ") " << unidad->getNombre()
                       << " - Vida: " << unidad->getVida()
                       << " - Ataque: " << unidad->getAtaque() << std::endl;
            }
        }

        archivo << std::endl;
        archivo << "Resumen por tipo:" << std::endl;
        archivo << "  Soldados:    " << soldados << std::endl;
        archivo << "  Arqueros:    " << arqueros << std::endl;
        archivo << "  Caballeros:  " << caballeros << std::endl;
        archivo << "  Magos:       " << magos << std::endl;
        archivo << "  Ingenieros:  " << ingenieros << std::endl;
        archivo << std::endl;

        // DETALLE DE EDIFICIOS
        archivo << "--- DETALLE DE EDIFICIOS JUGADOR 1 ---" << std::endl;
        for (int i = 0; i < mapa->getTamano(); i++) {
            for (int j = 0; j < mapa->getTamano(); j++) {
                Celda* celda = mapa->obtenerCelda(i, j);
                if (!celda || !celda->tieneEdificio()) continue;

                Edificio* edificio = celda->getEdificio();
                if (edificio->getPropietario() == 1) {
                    archivo << "  (" << i << "," << j << ") " << edificio->getNombre()
                           << std::endl;
                }
            }
        }
        archivo << std::endl;
    }

    archivo << "============================================================" << std::endl;
    archivo << "                    FIN DEL REPORTE" << std::endl;
    archivo << "============================================================" << std::endl;

    archivo.close();
    return true;
}

bool GestorArchivos::exportarTelemetria(const Contexto& ctx, const std::string& nombreArchivo) {
    // Verificar si el archivo existe para saber si escribir encabezados
    bool archivoExiste = false;
    std::ifstream archivoTest(nombreArchivo);
    if (archivoTest.good()) {
        archivoExiste = true;
    }
    archivoTest.close();

    std::ofstream archivo(nombreArchivo, std::ios::app); // Modo append para registro histórico
    if (!archivo.is_open()) {
        std::cerr << "Error al crear archivo de telemetria." << std::endl;
        return false;
    }

    // Si el archivo no existía, escribir encabezados
    if (!archivoExiste) {
        archivo << "turno,comida_j1,metal_j1,energia_j1,comida_j2,metal_j2,energia_j2,"
               << "dominio_j1,dominio_j2,unidades_j1,unidades_j2,edificios_j1,edificios_j2,"
               << "edificios_neutrales" << std::endl;
    }

    // DATOS DEL TURNO ACTUAL
    archivo << ctx.getTurnoActual() << ",";

    // Recursos J1
    Recursos* recJ1 = ctx.getRecursosJ1();
    if (recJ1) {
        archivo << recJ1->getComida() << ","
               << recJ1->getMetal() << ","
               << recJ1->getEnergia() << ",";
    } else {
        archivo << "0,0,0,";
    }

    // Recursos J2
    Recursos* recJ2 = ctx.getRecursosJ2();
    if (recJ2) {
        archivo << recJ2->getComida() << ","
               << recJ2->getMetal() << ","
               << recJ2->getEnergia() << ",";
    } else {
        archivo << "0,0,0,";
    }

    // Datos del mapa
    Mapa* mapa = ctx.getMapa();
    if (mapa) {
        int dominioJ1 = mapa->calcularDominio(1);
        int dominioJ2 = mapa->calcularDominio(2);

        archivo << dominioJ1 << "," << dominioJ2 << ",";

        // Contar unidades y edificios
        int unidadesJ1 = 0, unidadesJ2 = 0;
        int edificiosJ1 = 0, edificiosJ2 = 0, edificiosNeutrales = 0;

        for (int i = 0; i < mapa->getTamano(); i++) {
            for (int j = 0; j < mapa->getTamano(); j++) {
                Celda* celda = mapa->obtenerCelda(i, j);
                if (!celda) continue;

                if (celda->getUnidad()) {
                    if (celda->getUnidad()->getPropietario() == 1) unidadesJ1++;
                    else if (celda->getUnidad()->getPropietario() == 2) unidadesJ2++;
                }

                if (celda->tieneEdificio()) {
                    int prop = celda->getEdificio()->getPropietario();
                    if (prop == 1) edificiosJ1++;
                    else if (prop == 2) edificiosJ2++;
                    else if (prop == 0) edificiosNeutrales++;
                }
            }
        }

        archivo << unidadesJ1 << "," << unidadesJ2 << ","
               << edificiosJ1 << "," << edificiosJ2 << ","
               << edificiosNeutrales;
    } else {
        archivo << "0,0,0,0,0,0,0";
    }

    archivo << std::endl;
    archivo.close();
    return true;
}

void GestorArchivos::escribirEstadoMapa(std::ofstream& archivo, const Contexto& ctx) {
    archivo << "MAPA_INICIO" << std::endl;

    Mapa* mapa = ctx.getMapa();
    if (mapa) {
        for (int i = 0; i < mapa->getTamano(); i++) {
            for (int j = 0; j < mapa->getTamano(); j++) {
                Celda* celda = mapa->obtenerCelda(i, j);
                if (celda) {
                    archivo << "CELDA=" << i << "," << j << ","
                           << celda->getPropietario() << std::endl;
                }
            }
        }
    }

    archivo << "MAPA_FIN" << std::endl;
}

void GestorArchivos::escribirRecursos(std::ofstream& archivo, const Contexto& ctx) {
    Recursos* recJ1 = ctx.getRecursosJ1();
    if (recJ1) {
        archivo << "RECURSOS_J1=" << recJ1->getComida() << ","
                << recJ1->getMetal() << "," << recJ1->getEnergia() << std::endl;
    }

    Recursos* recJ2 = ctx.getRecursosJ2();
    if (recJ2) {
        archivo << "RECURSOS_J2=" << recJ2->getComida() << ","
                << recJ2->getMetal() << "," << recJ2->getEnergia() << std::endl;
    }
}

void GestorArchivos::escribirUnidades(std::ofstream& archivo, const Contexto& ctx) {
    archivo << "UNIDADES_INICIO" << std::endl;

    Mapa* mapa = ctx.getMapa();
    if (mapa) {
        for (int i = 0; i < mapa->getTamano(); i++) {
            for (int j = 0; j < mapa->getTamano(); j++) {
                Celda* celda = mapa->obtenerCelda(i, j);
                if (celda && celda->getUnidad()) {
                    Unidad* unidad = celda->getUnidad();
                    archivo << "UNIDAD=" << i << "," << j << ","
                           << unidad->getPropietario() << ","
                           << unidad->getCodigo() << std::endl;
                }
            }
        }
    }

    archivo << "UNIDADES_FIN" << std::endl;
}

// SISTEMA DE REPLAY
SistemaReplay::SistemaReplay(const std::string& nombreArchivo) : archivoLog(nombreArchivo) {
    log.open(archivoLog);
    if (log.is_open()) {
        log << "=== REPLAY DE PARTIDA ===" << std::endl;
        log << "Inicio: " << std::endl;
    }
}

SistemaReplay::~SistemaReplay() {
    cerrar();
}

void SistemaReplay::registrarAccion(const std::string& accion) {
    if (log.is_open()) {
        log << "ACCION: " << accion << std::endl;
    }
}

void SistemaReplay::registrarTurno(int turno) {
    if (log.is_open()) {
        log << "\n--- TURNO " << turno << " ---" << std::endl;
    }
}

void SistemaReplay::registrarEvento(const std::string& evento) {
    if (log.is_open()) {
        log << "EVENTO: " << evento << std::endl;
    }
}

void SistemaReplay::cerrar() {
    if (log.is_open()) {
        log << "\n=== FIN DEL REPLAY ===" << std::endl;
        log.close();
    }
}