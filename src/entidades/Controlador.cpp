// -------- src/entidades/Controlador.cpp --------
#include "Controlador.h"
#include "../nucleo/Contexto.h"
#include "../nucleo/Mapa.h"
#include "../sistemas/Recursos.h"
#include "../utilidades/Persistencia.h"
#include "../entidades/Unidad.h"
#include "../entidades/Edificio.h"
#include "../utilidades/Validaciones.h"
#include <iostream>
#include <vector>

Controlador::Controlador(int id) : jugadorId(id) {}

void Controlador::aplicar_rutina(const Evento& evento, Contexto& ctx) {
    std::cout << "Controlador aplica rutina." << std::endl;
}

JugadorHumano::JugadorHumano() : Controlador(1) {}

void JugadorHumano::resolver_fase(Contexto& ctx) {
    while (ctx.getPuntosAccion() > 0) {
        mostrarMenu(ctx);
    }
}

void JugadorHumano::mostrarMenu(Contexto& ctx) {
    std::cout << "\n=== MENU JUGADOR ===" << std::endl;
    std::cout << "Acciones restantes: " << ctx.getPuntosAccion() << std::endl;
    std::cout << "1. Mover unidad" << std::endl;
    std::cout << "2. Atacar" << std::endl;
    std::cout << "3. Construir edificio" << std::endl;
    std::cout << "4. Reclutar unidad" << std::endl;
    std::cout << "5. Usar habilidad especial" << std::endl;
    std::cout << "6. Ver reportes" << std::endl;
    std::cout << "7. Guardar partida" << std::endl;
    std::cout << "8. Cargar partida" << std::endl;
    std::cout << "9. Terminar turno" << std::endl;
    std::cout << "Opcion: ";

    int opcion;
    std::cin >> opcion;

    switch(opcion) {
        case 1: moverUnidad(ctx); break;
        case 2: atacarConUnidad(ctx); break;
        case 3: construirEdificio(ctx); break;
        case 4: reclutarUnidad(ctx); break;
        case 5: usarHabilidadEspecial(ctx); break;
        case 6: verReportes(ctx); break;
        case 7: guardarPartida(ctx); break;
        case 8: cargarPartida(ctx); break;
        case 9:
            while(ctx.getPuntosAccion() > 0) ctx.consumirAccion();
            break;
        default: std::cout << "Opción inválida." << std::endl;
    }
}

void JugadorHumano::moverUnidad(Contexto& ctx) {
    std::cout << "\n=== MOVER UNIDAD ===" << std::endl;
    std::cout << "Coordenada de origen (x y): ";
    int origenX, origenY;
    std::cin >> origenX >> origenY;

    Mapa* mapa = ctx.getMapa();
    if (!mapa || !mapa->coordenadaValida(origenX, origenY)) {
        std::cout << "Coordenada origen invalida." << std::endl;
        return;
    }

    Celda* celdaOrigen = mapa->obtenerCelda(origenX, origenY);
    if (!celdaOrigen || !celdaOrigen->getUnidad()) {
        std::cout << "No hay unidad en esa posicion." << std::endl;
        return;
    }

    Unidad* unidad = celdaOrigen->getUnidad();
    if (unidad->getPropietario() != 1) {
        std::cout << "Esa unidad no te pertenece." << std::endl;
        return;
    }

    std::cout << "Coordenada de destino (x y): ";
    int destinoX, destinoY;
    std::cin >> destinoX >> destinoY;

    Coordenada destino(destinoX, destinoY);

    if (!Validaciones::puedeMoverse(unidad, destino, ctx)) {
        std::cout << "Movimiento no valido." << std::endl;
        return;
    }

    Celda* celdaDestino = mapa->obtenerCelda(destino);

    // NUEVO: Capturar edificio neutral
    if (celdaDestino->tieneEdificio()) {
        Edificio* edificio = celdaDestino->getEdificio();
        if (edificio && edificio->getPropietario() == 0) {
            std::cout << "¡Edificio neutral capturado! " << edificio->getNombre()
                     << " ahora es tuyo." << std::endl;
            edificio->setPropietario(1);

            // Actualizar código del edificio
            std::string nuevoCodigo = edificio->getCodigo();
            if (nuevoCodigo.substr(0, 2) == "Ne") {
                nuevoCodigo = "J1" + nuevoCodigo.substr(2);
            }

            celdaDestino->setPropietario(1);

            // La unidad NO se mueve a la celda con edificio
            std::cout << "La unidad permanece en su posicion original." << std::endl;
            ctx.consumirAccion();
            return;
        }
    }

    Unidad* unidadAMover = unidad;
    celdaOrigen->setUnidad(nullptr);
    celdaDestino->setUnidad(unidadAMover);

    if (celdaDestino->getPropietario() == 0) {
        celdaDestino->setPropietario(1);
    }

    std::cout << "Unidad movida exitosamente." << std::endl;
    ctx.consumirAccion();
}

void JugadorHumano::atacarConUnidad(Contexto& ctx) {
    std::cout << "\n=== ATACAR ===" << std::endl;
    std::cout << "Coordenada de tu unidad (x y): ";
    int atacanteX, atacanteY;
    std::cin >> atacanteX >> atacanteY;

    Mapa* mapa = ctx.getMapa();
    if (!mapa || !mapa->coordenadaValida(atacanteX, atacanteY)) {
        std::cout << "Coordenada invalida." << std::endl;
        return;
    }

    Celda* celdaAtacante = mapa->obtenerCelda(atacanteX, atacanteY);
    if (!celdaAtacante || !celdaAtacante->getUnidad()) {
        std::cout << "No hay unidad en esa posicion." << std::endl;
        return;
    }

    Unidad* atacante = celdaAtacante->getUnidad();
    if (atacante->getPropietario() != 1) {
        std::cout << "Esa unidad no te pertenece." << std::endl;
        return;
    }

    std::cout << "Coordenada del objetivo (x y): ";
    int objetivoX, objetivoY;
    std::cin >> objetivoX >> objetivoY;

    Coordenada objetivo(objetivoX, objetivoY);

    if (!Validaciones::puedeAtacar(atacante, objetivo, ctx)) {
        std::cout << "No puedes atacar esa posicion." << std::endl;
        return;
    }

    Celda* celdaObjetivo = mapa->obtenerCelda(objetivo);
    Unidad* defensor = celdaObjetivo->getUnidad();

    atacante->atacar(*defensor, ctx);

    if (!defensor->estaViva()) {
        celdaObjetivo->setUnidad(nullptr);
    }

    ctx.consumirAccion();
}

void JugadorHumano::construirEdificio(Contexto& ctx) {
    std::cout << "\n=== CONSTRUIR EDIFICIO ===" << std::endl;
    std::cout << "Tipo: 1.Granja 2.Cuartel 3.Torre 4.Forja" << std::endl;
    std::cout << "Selecciona: ";
    int tipo;
    std::cin >> tipo;

    std::cout << "Coordenada (x y): ";
    int x, y;
    std::cin >> x >> y;

    Coordenada pos(x, y);
    Mapa* mapa = ctx.getMapa();

    if (!mapa || !mapa->coordenadaValida(pos)) {
        std::cout << "Coordenada invalida." << std::endl;
        return;
    }

    Celda* celda = mapa->obtenerCelda(pos);

    if (celda->tieneEdificio()) {
        std::cout << "Ya hay un edificio en esa posicion." << std::endl;
        return;
    }

    if (!celda->estaLibre()) {
        std::cout << "Hay una unidad en esa posicion. Debes moverla primero." << std::endl;
        return;
    }

    if (!Validaciones::puedeConstruir(pos, tipo, ctx)) {
        std::cout << "No se puede construir ahi." << std::endl;
        return;
    }

    Recursos* rec = ctx.getRecursosJ1();
    bool puedeConstruir = false;
    Edificio* nuevoEdificio = nullptr;

    switch(tipo) {
        case 1:
            if (rec->consumir(0, 1, 1)) {
                nuevoEdificio = new Granja(1);
                puedeConstruir = true;
            }
            break;
        case 2:
            if (rec->consumir(0, 2, 0)) {
                nuevoEdificio = new Cuartel(1);
                puedeConstruir = true;
            }
            break;
        case 3:
            if (rec->consumir(0, 2, 1)) {
                nuevoEdificio = new Torre(1);
                puedeConstruir = true;
            }
            break;
        case 4:
            if (rec->consumir(0, 3, 2)) {
                nuevoEdificio = new Forja(1);
                puedeConstruir = true;
            }
            break;
    }

    if (puedeConstruir && nuevoEdificio) {
        celda->setEdificio(nuevoEdificio);
        celda->setPropietario(1);

        std::cout << "Edificio construido exitosamente." << std::endl;
        ctx.consumirAccion();
    } else {
        std::cout << "Recursos insuficientes o construccion invalida." << std::endl;
    }
}

void JugadorHumano::reclutarUnidad(Contexto& ctx) {
    std::cout << "\n=== RECLUTAR UNIDAD ===" << std::endl;

    // PASO 1: Encontrar cuarteles del jugador
    Mapa* mapa = ctx.getMapa();
    std::vector<Coordenada> cuarteles;

    for (int i = 0; i < mapa->getTamano(); i++) {
        for (int j = 0; j < mapa->getTamano(); j++) {
            Celda* celda = mapa->obtenerCelda(i, j);
            if (celda && celda->tieneEdificio()) {
                Edificio* edificio = celda->getEdificio();
                if (edificio && edificio->getPropietario() == 1) {
                    Cuartel* cuartel = dynamic_cast<Cuartel*>(edificio);
                    if (cuartel) {
                        cuarteles.push_back(Coordenada(i, j));
                    }
                }
            }
        }
    }

    if (cuarteles.empty()) {
        std::cout << "No tienes cuarteles disponibles para reclutar." << std::endl;
        return;
    }

    // PASO 2: Mostrar cuarteles disponibles
    std::cout << "Cuarteles disponibles:" << std::endl;
    for (size_t i = 0; i < cuarteles.size(); i++) {
        std::cout << i + 1 << ". Cuartel en (" << cuarteles[i].x << ","
                 << cuarteles[i].y << ")" << std::endl;
    }

    std::cout << "Selecciona cuartel (1-" << cuarteles.size() << "): ";
    int seleccion;
    std::cin >> seleccion;

    if (seleccion < 1 || seleccion > static_cast<int>(cuarteles.size())) {
        std::cout << "Seleccion invalida." << std::endl;
        return;
    }

    Coordenada cuartelSeleccionado = cuarteles[seleccion - 1];

    // PASO 3: Seleccionar tipo de unidad
    std::cout << "\nTipo: 1.Soldado 2.Arquero 3.Caballero 4.Mago 5.Ingeniero" << std::endl;
    std::cout << "Selecciona: ";
    int tipo;
    std::cin >> tipo;

    // PASO 4: Buscar celda libre cerca del cuartel
    int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
    int dy[] = {0, 0, -1, 1, -1, -1, 1, 1};

    Celda* celdaLibre = nullptr;
    int posX = -1, posY = -1;

    for (int i = 0; i < 8; i++) {
        int nx = cuartelSeleccionado.x + dx[i];
        int ny = cuartelSeleccionado.y + dy[i];

        if (!mapa->coordenadaValida(nx, ny)) continue;

        Celda* celda = mapa->obtenerCelda(nx, ny);
        if (celda && celda->estaLibre() && !celda->tieneEdificio()) {
            celdaLibre = celda;
            posX = nx;
            posY = ny;
            break;
        }
    }

    if (!celdaLibre) {
        std::cout << "No hay espacio libre cerca del cuartel seleccionado." << std::endl;
        return;
    }

    // PASO 5: Reclutar la unidad
    Recursos* rec = ctx.getRecursosJ1();
    Unidad* nuevaUnidad = nullptr;
    bool puedeReclutar = false;

    switch(tipo) {
        case 1:
            if (rec->consumir(1, 1, 0)) {
                nuevaUnidad = new Soldado(1);
                puedeReclutar = true;
            }
            break;
        case 2:
            if (rec->consumir(1, 1, 0)) {
                nuevaUnidad = new Arquero(1);
                puedeReclutar = true;
            }
            break;
        case 3:
            if (rec->consumir(2, 2, 0)) {
                nuevaUnidad = new Caballero(1);
                puedeReclutar = true;
            }
            break;
        case 4:
            if (rec->consumir(1, 1, 2)) {
                nuevaUnidad = new Mago(1);
                puedeReclutar = true;
            }
            break;
        case 5:
            if (rec->consumir(1, 0, 1)) {
                nuevaUnidad = new Ingeniero(1);
                puedeReclutar = true;
            }
            break;
    }

    if (puedeReclutar && nuevaUnidad) {
        celdaLibre->setUnidad(nuevaUnidad);
        if (celdaLibre->getPropietario() == 0) {
            celdaLibre->setPropietario(1);
        }
        std::cout << "Unidad reclutada en (" << posX << "," << posY
                 << ") cerca del cuartel." << std::endl;
        ctx.consumirAccion();
    } else {
        std::cout << "Recursos insuficientes o reclutamiento invalido." << std::endl;
    }
}

// NUEVA FUNCIÓN: Actualizar turnos de todas las unidades
void JugadorHumano::actualizarUnidades(Contexto& ctx) {
    Mapa* mapa = ctx.getMapa();
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

void JugadorHumano::usarHabilidadEspecial(Contexto& ctx) {
    std::cout << "\n=== USAR HABILIDAD ESPECIAL ===" << std::endl;
    std::cout << "Coordenada de la unidad (x y): ";
    int x, y;
    std::cin >> x >> y;

    Mapa* mapa = ctx.getMapa();
    if (!mapa || !mapa->coordenadaValida(x, y)) {
        std::cout << "Coordenada invalida." << std::endl;
        return;
    }

    Celda* celda = mapa->obtenerCelda(x, y);
    if (!celda || !celda->getUnidad()) {
        std::cout << "No hay unidad en esa posicion." << std::endl;
        return;
    }

    Unidad* unidad = celda->getUnidad();
    if (unidad->getPropietario() != 1) {
        std::cout << "Esa unidad no te pertenece." << std::endl;
        return;
    }

    Recursos* rec = ctx.getRecursosJ1();
    if (!rec->tieneSuficiente(0, 0, 1)) {
        std::cout << "Energia insuficiente. Se requiere 1 de energia." << std::endl;
        return;
    }

    rec->consumir(0, 0, 1);
    unidad->habilidad_especial(ctx);
    std::cout << "Habilidad especial activada." << std::endl;
    ctx.consumirAccion();
}

void JugadorHumano::verReportes(Contexto& ctx) {
    std::cout << "\n=== REPORTES ===" << std::endl;
    std::cout << "1. Estado actual" << std::endl;
    std::cout << "2. Exportar reporte completo" << std::endl;
    std::cout << "3. Exportar telemetria" << std::endl;
    std::cout << "Selecciona: ";

    int opcion;
    std::cin >> opcion;

    switch(opcion) {
        case 1:
            mostrarEstado(ctx);
            break;
        case 2:
            if (GestorArchivos::exportarReporte(ctx, "reporte_partida.txt")) {
                std::cout << "Reporte exportado a 'reporte_partida.txt'" << std::endl;
            }
            break;
        case 3:
            if (GestorArchivos::exportarTelemetria(ctx, "telemetria.csv")) {
                std::cout << "Telemetria exportada a 'telemetria.csv'" << std::endl;
            }
            break;
        default:
            std::cout << "Opcion invalida." << std::endl;
    }
}

void JugadorHumano::guardarPartida(Contexto& ctx) {
    std::cout << "\n=== GUARDAR PARTIDA ===" << std::endl;
    std::cout << "Nombre del archivo (sin extension): ";
    std::string nombreArchivo;
    std::cin.ignore();
    std::getline(std::cin, nombreArchivo);
    nombreArchivo += ".sav";

    if (GestorArchivos::guardarPartida(ctx, nombreArchivo)) {
        std::cout << "Partida guardada exitosamente en '" << nombreArchivo << "'" << std::endl;
    } else {
        std::cout << "Error al guardar la partida." << std::endl;
    }
}

void JugadorHumano::cargarPartida(Contexto& ctx) {
    std::cout << "\n=== CARGAR PARTIDA ===" << std::endl;
    std::cout << "Nombre del archivo (sin extension): ";
    std::string nombreArchivo;
    std::cin.ignore();
    std::getline(std::cin, nombreArchivo);
    nombreArchivo += ".sav";

    if (GestorArchivos::cargarPartida(ctx, nombreArchivo)) {
        std::cout << "Partida cargada exitosamente desde '" << nombreArchivo << "'" << std::endl;
    } else {
        std::cout << "Error al cargar la partida." << std::endl;
    }
}

void JugadorHumano::mostrarEstado(Contexto& ctx) {
    Recursos* rec = ctx.getRecursosJ1();
    Mapa* mapa = ctx.getMapa();

    std::cout << "\n=== ESTADO ACTUAL ===" << std::endl;
    std::cout << "Turno: " << ctx.getTurnoActual() << std::endl;
    std::cout << "Comida: " << rec->getComida() << std::endl;
    std::cout << "Metal: " << rec->getMetal() << std::endl;
    std::cout << "Energia: " << rec->getEnergia() << std::endl;
    std::cout << "Dominio J1: " << mapa->calcularDominio(1) << "%" << std::endl;
    std::cout << "Dominio J2: " << mapa->calcularDominio(2) << "%" << std::endl;
}

SistemaIA::SistemaIA() : Controlador(2) {}

void SistemaIA::resolver_fase(Contexto& ctx) {
    std::cout << "Sistema IA ejecutando rutinas..." << std::endl;
    patrullar(ctx);
    reforzarPosiciones(ctx);
    generarRecursos(ctx);
}

void SistemaIA::patrullar(Contexto& ctx) {
    std::cout << "IA: Patrullando territorios." << std::endl;

    Mapa* mapa = ctx.getMapa();
    if (!mapa) return;

    struct MovimientoPendiente {
        int origenX, origenY;
        int destinoX, destinoY;
        Unidad* unidad;
    };

    std::vector<MovimientoPendiente> movimientos;

    for (int i = 0; i < mapa->getTamano(); i++) {
        for (int j = 0; j < mapa->getTamano(); j++) {
            Celda* celda = mapa->obtenerCelda(i, j);
            if (!celda) continue;

            Unidad* unidad = celda->getUnidad();
            if (unidad && unidad->getPropietario() == 2) {
                int destX, destY;
                if (encontrarDestinoParaUnidad(i, j, destX, destY, mapa)) {
                    MovimientoPendiente mov;
                    mov.origenX = i;
                    mov.origenY = j;
                    mov.destinoX = destX;
                    mov.destinoY = destY;
                    mov.unidad = unidad;
                    movimientos.push_back(mov);
                }
            }
        }
    }

    for (const auto& mov : movimientos) {
        Celda* celdaOrigen = mapa->obtenerCelda(mov.origenX, mov.origenY);
        Celda* celdaDestino = mapa->obtenerCelda(mov.destinoX, mov.destinoY);

        if (celdaOrigen && celdaDestino &&
            celdaOrigen->getUnidad() == mov.unidad &&
            celdaDestino->estaLibre()) {

            celdaDestino->setUnidad(mov.unidad);
            celdaOrigen->setUnidad(nullptr);
            celdaDestino->setPropietario(2);

            std::cout << "IA movio unidad de (" << mov.origenX << "," << mov.origenY
                      << ") a (" << mov.destinoX << "," << mov.destinoY << ")" << std::endl;
        }
    }
}

void SistemaIA::reforzarPosiciones(Contexto& ctx) {
    std::cout << "IA: Reforzando posiciones clave." << std::endl;

    Recursos* rec = ctx.getRecursosJ2();
    if (!rec || !rec->tieneSuficiente(1, 1, 0)) return;

    Mapa* mapa = ctx.getMapa();
    if (!mapa) return;

    for (int i = 0; i < mapa->getTamano(); i++) {
        for (int j = 0; j < mapa->getTamano(); j++) {
            Celda* celda = mapa->obtenerCelda(i, j);
            if (!celda) continue;

            if (celda->tieneEdificio() && celda->getPropietario() == 2) {
                if (intentarReclutarCerca(i, j, mapa, rec)) {
                    return;
                }
            }
        }
    }
}

void SistemaIA::generarRecursos(Contexto& ctx) {
    std::cout << "IA: Generando recursos de edificios." << std::endl;

    Mapa* mapa = ctx.getMapa();
    Recursos* rec = ctx.getRecursosJ2();

    if (!mapa || !rec) return;

    for (int i = 0; i < mapa->getTamano(); i++) {
        for (int j = 0; j < mapa->getTamano(); j++) {
            Celda* celda = mapa->obtenerCelda(i, j);
            if (!celda || !celda->tieneEdificio()) continue;

            Edificio* edificio = celda->getEdificio();
            if (edificio && edificio->getPropietario() == 2) {
                edificio->efecto_turno(*rec);
            }
        }
    }
}

bool SistemaIA::encontrarDestinoParaUnidad(int x, int y, int& destX, int& destY, Mapa* mapa) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (!mapa->coordenadaValida(nx, ny)) continue;

        Celda* celdaDestino = mapa->obtenerCelda(nx, ny);
        if (!celdaDestino || !celdaDestino->estaLibre()) continue;

        if (celdaDestino->getPropietario() != 2) {
            destX = nx;
            destY = ny;
            return true;
        }
    }

    return false;
}

void SistemaIA::buscarYMoverHaciaEnemigo(int x, int y, Mapa* mapa) {
    // Deprecated
}

bool SistemaIA::intentarReclutarCerca(int x, int y, Mapa* mapa, Recursos* rec) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (!mapa->coordenadaValida(nx, ny)) continue;

        Celda* celda = mapa->obtenerCelda(nx, ny);
        if (!celda || !celda->estaLibre()) continue;

        if (rec->consumir(1, 1, 0)) {
            celda->setUnidad(new Soldado(2));
            celda->setPropietario(2);
            std::cout << "IA recluto Soldado en (" << nx << "," << ny << ")" << std::endl;
            return true;
        }
    }

    return false;
}

NeutralControlador::NeutralControlador() : Controlador(0) {}

void NeutralControlador::resolver_fase(Contexto& ctx) {
    // Entidades neutrales no toman acciones
}
