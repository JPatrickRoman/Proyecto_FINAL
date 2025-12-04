// -------- src/nucleo/Mapa.cpp --------
#include "Mapa.h"
#include "../entidades/Terreno.h"
#include "../entidades/Unidad.h"
#include "../entidades/Edificio.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

Celda::Celda() : terreno(nullptr), unidad(nullptr), edificio(nullptr), propietario(0) {}

Celda::~Celda() {
    // Solo borrar si los punteros no son nulos
    if (terreno != nullptr) delete terreno;
    if (unidad != nullptr) delete unidad;
    if (edificio != nullptr) delete edificio;
}

void Celda::setTerreno(Terreno* t) {
    delete terreno;
    terreno = t;
}

void Celda::setUnidad(Unidad* u) {
    // NO borrar la unidad anterior - dejar que quien llame maneje la memoria
    // Solo actualizar el puntero
    unidad = u;
}

void Celda::setEdificio(Edificio* e) {
    // NO borrar el edificio anterior - dejar que quien llame maneje la memoria
    // Solo actualizar el puntero
    edificio = e;
}

std::string Celda::obtenerCodigo() const {
    std::string codigo = "";

    // PRIORIDAD: Mostrar unidad primero, luego edificio
    if (unidad) {
        codigo = unidad->getCodigo();
    } else if (edificio) {
        codigo = edificio->getCodigo();
    } else {
        codigo = "..";
    }

    codigo += "/" + (terreno ? terreno->getCodigo() : "??");

    return codigo;
}

Mapa::Mapa() {
    celdas.resize(TAMANO);
    for (int i = 0; i < TAMANO; i++) {
        celdas[i].resize(TAMANO);
        for (int j = 0; j < TAMANO; j++) {
            celdas[i][j] = new Celda();
        }
    }
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Mapa::~Mapa() {
    for (int i = 0; i < TAMANO; i++) {
        for (int j = 0; j < TAMANO; j++) {
            delete celdas[i][j];
        }
    }
}

void Mapa::inicializar() {
    generarMapaInicial();
}

void Mapa::generarMapaInicial() {
    // 1. GENERAR TERRENO BASE (mayoría llanuras)
    for (int i = 0; i < TAMANO; i++) {
        for (int j = 0; j < TAMANO; j++) {
            celdas[i][j]->setTerreno(new Llanura());
        }
    }

    // 2. GENERAR ZONAS DE BOSQUE (clusters)
    int numBosques = 3 + std::rand() % 3; // 3-5 zonas de bosque
    for (int b = 0; b < numBosques; b++) {
        int centroX = 2 + std::rand() % (TAMANO - 4);
        int centroY = 2 + std::rand() % (TAMANO - 4);
        int radioBosque = 1 + std::rand() % 2; // radio 1-2

        for (int i = -radioBosque; i <= radioBosque; i++) {
            for (int j = -radioBosque; j <= radioBosque; j++) {
                int x = centroX + i;
                int y = centroY + j;
                if (coordenadaValida(x, y) && std::rand() % 100 < 70) {
                    celdas[x][y]->setTerreno(new Bosque());
                }
            }
        }
    }

    // 3. GENERAR MONTAÑAS (zonas elevadas)
    int numMontanas = 2 + std::rand() % 2; // 2-3 zonas montañosas
    for (int m = 0; m < numMontanas; m++) {
        int centroX = 1 + std::rand() % (TAMANO - 2);
        int centroY = 1 + std::rand() % (TAMANO - 2);

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int x = centroX + i;
                int y = centroY + j;
                if (coordenadaValida(x, y) && std::rand() % 100 < 50) {
                    celdas[x][y]->setTerreno(new Montana());
                }
            }
        }
    }

    // 4. GENERAR RÍO O LAGO (agua en línea o cluster)
    if (std::rand() % 2 == 0) {
        // Río vertical u horizontal
        bool vertical = std::rand() % 2;
        int posicion = 3 + std::rand() % (TAMANO - 6);
        int inicio = 2 + std::rand() % 3;
        int fin = TAMANO - 2 - std::rand() % 3;

        for (int i = inicio; i < fin; i++) {
            if (vertical) {
                celdas[i][posicion]->setTerreno(new Agua());
            } else {
                celdas[posicion][i]->setTerreno(new Agua());
            }
        }
    } else {
        // Lago pequeño
        int centroX = 4 + std::rand() % (TAMANO - 8);
        int centroY = 4 + std::rand() % (TAMANO - 8);
        celdas[centroX][centroY]->setTerreno(new Agua());
        celdas[centroX+1][centroY]->setTerreno(new Agua());
        celdas[centroX][centroY+1]->setTerreno(new Agua());
        celdas[centroX+1][centroY+1]->setTerreno(new Agua());
    }

    // 5. GENERAR PANTANOS (cerca del agua)
    for (int i = 0; i < TAMANO; i++) {
        for (int j = 0; j < TAMANO; j++) {
            if (esCercaDeAgua(i, j) && std::rand() % 100 < 30) {
                // Solo convertir llanuras en pantanos
                if (dynamic_cast<Llanura*>(celdas[i][j]->getTerreno())) {
                    celdas[i][j]->setTerreno(new Pantano());
                }
            }
        }
    }

    // 6. COLOCAR BASE JUGADOR 1 (esquina inferior izquierda)
    int baseJ1X = 0;
    int baseJ1Y = 0;

    // Asegurar que la base esté en terreno transitable
    celdas[baseJ1X][baseJ1Y]->setTerreno(new Llanura());
    celdas[baseJ1X][baseJ1Y]->setEdificio(new Cuartel(1));
    celdas[baseJ1X][baseJ1Y]->setPropietario(1);

    // Colocar unidades iniciales J1 cerca de la base
    colocarUnidadesIniciales(1, baseJ1X, baseJ1Y);

    // 7. COLOCAR BASE JUGADOR 2 (esquina superior derecha)
    int baseJ2X = TAMANO - 1;
    int baseJ2Y = TAMANO - 1;

    celdas[baseJ2X][baseJ2Y]->setTerreno(new Llanura());
    celdas[baseJ2X][baseJ2Y]->setEdificio(new Cuartel(2));
    celdas[baseJ2X][baseJ2Y]->setPropietario(2);

    // Colocar unidades iniciales J2
    colocarUnidadesIniciales(2, baseJ2X, baseJ2Y);

    // 8. COLOCAR EDIFICIOS NEUTRALES estratégicamente
    colocarEdificiosNeutrales();

    // 9. COLOCAR EDIFICIOS ADICIONALES PARA CADA JUGADOR
    colocarEdificiosAdicionales(1, baseJ1X, baseJ1Y);
    colocarEdificiosAdicionales(2, baseJ2X, baseJ2Y);
}

void Mapa::colocarUnidadesIniciales(int jugador, int baseX, int baseY) {
    // Buscar posiciones adyacentes libres a la base
    int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
    int dy[] = {0, 0, -1, 1, -1, -1, 1, 1};

    int unidadesColocadas = 0;
    int maxUnidades = 3; // 3 unidades iniciales por jugador

    for (int i = 0; i < 8 && unidadesColocadas < maxUnidades; i++) {
        int nx = baseX + dx[i];
        int ny = baseY + dy[i];

        if (!coordenadaValida(nx, ny)) continue;

        Celda* celda = celdas[nx][ny];

        // Solo colocar en terreno transitable y vacío
        if (celda->estaLibre() && !celda->tieneEdificio() &&
            !dynamic_cast<Agua*>(celda->getTerreno())) {

            // Asegurar que sea llanura
            if (!dynamic_cast<Llanura*>(celda->getTerreno())) {
                celda->setTerreno(new Llanura());
            }

            // Variedad de unidades
            Unidad* nuevaUnidad = nullptr;
            switch (unidadesColocadas) {
                case 0: nuevaUnidad = new Soldado(jugador); break;
                case 1: nuevaUnidad = new Arquero(jugador); break;
                case 2: nuevaUnidad = new Soldado(jugador); break;
            }

            celda->setUnidad(nuevaUnidad);
            celda->setPropietario(jugador);
            unidadesColocadas++;
        }
    }
}

void Mapa::colocarEdificiosNeutrales() {
    int numEdificiosNeutrales = 3 + std::rand() % 3; // 3-5 edificios neutrales

    for (int e = 0; e < numEdificiosNeutrales; e++) {
        // Buscar posición en el centro del mapa (zona neutral)
        int x = (TAMANO / 4) + std::rand() % (TAMANO / 2);
        int y = (TAMANO / 4) + std::rand() % (TAMANO / 2);

        // Intentar varias posiciones si la primera no es válida
        for (int intento = 0; intento < 10; intento++) {
            if (coordenadaValida(x, y)) {
                Celda* celda = celdas[x][y];

                if (celda->estaLibre() && !celda->tieneEdificio() &&
                    !dynamic_cast<Agua*>(celda->getTerreno())) {

                    // Asegurar terreno llanura
                    if (!dynamic_cast<Llanura*>(celda->getTerreno())) {
                        celda->setTerreno(new Llanura());
                    }

                    // Tipo de edificio neutral aleatorio
                    int tipo = std::rand() % 3;
                    switch (tipo) {
                        case 0: celda->setEdificio(new Granja(0)); break;
                        case 1: celda->setEdificio(new Cuartel(0)); break;
                        case 2: celda->setEdificio(new Torre(0)); break;
                    }
                    celda->setPropietario(0);
                    break;
                }
            }

            // Reintentar en otra posición
            x = (TAMANO / 4) + std::rand() % (TAMANO / 2);
            y = (TAMANO / 4) + std::rand() % (TAMANO / 2);
        }
    }
}

void Mapa::colocarEdificiosAdicionales(int jugador, int baseX, int baseY) {
    // Colocar 1-2 edificios adicionales cerca de la base
    int numEdificios = 1 + std::rand() % 2;

    int dx[] = {-2, -1, 0, 1, 2, -2, 2, -1, 1};
    int dy[] = {0, -1, -2, -1, 0, 2, 2, 2, 2};

    int edificiosColocados = 0;

    for (int i = 0; i < 9 && edificiosColocados < numEdificios; i++) {
        int nx = baseX + dx[i];
        int ny = baseY + dy[i];

        if (!coordenadaValida(nx, ny)) continue;

        Celda* celda = celdas[nx][ny];

        if (celda->estaLibre() && !celda->tieneEdificio() &&
            !dynamic_cast<Agua*>(celda->getTerreno())) {

            // Asegurar terreno llanura
            if (!dynamic_cast<Llanura*>(celda->getTerreno())) {
                celda->setTerreno(new Llanura());
            }

            // Edificio de recursos
            if (edificiosColocados == 0) {
                celda->setEdificio(new Granja(jugador));
            } else {
                celda->setEdificio(new Torre(jugador));
            }
            celda->setPropietario(jugador);
            edificiosColocados++;
        }
    }
}

bool Mapa::esCercaDeAgua(int x, int y) const {
    int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
    int dy[] = {0, 0, -1, 1, -1, -1, 1, 1};

    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (coordenadaValida(nx, ny)) {
            if (dynamic_cast<Agua*>(celdas[nx][ny]->getTerreno())) {
                return true;
            }
        }
    }

    return false;
}

Celda* Mapa::obtenerCelda(const Coordenada& coord) {
    return obtenerCelda(coord.x, coord.y);
}

Celda* Mapa::obtenerCelda(int x, int y) {
    if (!coordenadaValida(x, y)) return nullptr;
    return celdas[x][y];
}

bool Mapa::coordenadaValida(const Coordenada& coord) const {
    return coordenadaValida(coord.x, coord.y);
}

bool Mapa::coordenadaValida(int x, int y) const {
    return x >= 0 && x < TAMANO && y >= 0 && y < TAMANO;
}

void Mapa::renderizar() const {
    std::cout << "\n    ";
    for (int j = 0; j < TAMANO; j++) {
        std::cout << std::setw(4) << j << "     ";
    }
    std::cout << "\n";

    for (int i = 0; i < TAMANO; i++) {
        std::cout << std::setw(2) << i << " ";
        for (int j = 0; j < TAMANO; j++) {
            Celda* celda = celdas[i][j];
            std::string codigo = celda->obtenerCodigo();

            // DEBUG solo para celdas de jugador 1 con unidades/edificios
            if ((i == 1 && (j == 10 || j == 11)) || (i == 0 && j == 0)) {
                std::cout << "[" << std::setw(7) << std::left << codigo << "]";
                /*if (j == 11) {
                    std::cout << " <-- (1,10):" << (celda->getUnidad() ? "U" : "-")
                              << (celda->tieneEdificio() ? "E" : "-");
                }
                */
            } else {
                std::cout << "[" << std::setw(7) << std::left << codigo << "]";
            }
        }
        std::cout << "\n";
    }
}

std::string Mapa::obtenerEstadoCelda(int x, int y) const {
    if (!coordenadaValida(x, y)) return "INVALIDO";
    return celdas[x][y]->obtenerCodigo();
}

int Mapa::calcularDominio(int jugador) const {
    int celdas_controladas = 0;
    int total_celdas = TAMANO * TAMANO;

    for (int i = 0; i < TAMANO; i++) {
        for (int j = 0; j < TAMANO; j++) {
            if (celdas[i][j]->getPropietario() == jugador) {
                celdas_controladas++;
            }
        }
    }

    return (celdas_controladas * 100) / total_celdas;
}