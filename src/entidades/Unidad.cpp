// -------- src/entidades/Unidad.cpp --------
#include "Unidad.h"
#include "../nucleo/Contexto.h"
#include "../nucleo/Mapa.h"
#include "../entidades/Edificio.h"
#include <iostream>

Unidad::Unidad(const std::string& nombre, const std::string& codigo, int propietario,
               int vida, int ataque, int defensa)
    : nombre(nombre), codigo(codigo), propietario(propietario),
      vida(vida), vidaMaxima(vida), ataque(ataque), defensa(defensa), activa(true),
      turnosDefensaExtra(0), defensaExtraActual(0) {}

bool Unidad::mover(const Coordenada& destino, Contexto& ctx) {
    Mapa* mapa = ctx.getMapa();
    Celda* celdaDestino = mapa->obtenerCelda(destino);

    if (!celdaDestino || !celdaDestino->estaLibre()) {
        std::cout << "No se puede mover a esa posicion." << std::endl;
        return false;
    }

    return true;
}

void Unidad::atacar(Unidad& objetivo, Contexto& ctx) {
    if (!activa || !estaViva()) return;

    int danio = ataque - objetivo.getDefensa();
    if (danio < 1) danio = 1;

    objetivo.recibirDanio(danio);
    std::cout << nombre << " ataca a " << objetivo.getNombre()
              << " causando " << danio << " de danio." << std::endl;
}

void Unidad::habilidad_especial(Contexto& ctx) {
    std::cout << nombre << " no tiene habilidad especial." << std::endl;
}

void Unidad::actualizarTurno() {
    // Reducir turnos de efectos temporales
    if (turnosDefensaExtra > 0) {
        turnosDefensaExtra--;
        if (turnosDefensaExtra == 0) {
            defensaExtraActual = 0;
        }
    }
}

void Unidad::recibirDanio(int cantidad) {
    vida -= cantidad;
    if (vida < 0) vida = 0;
    std::cout << nombre << " recibe " << cantidad << " de danio. Vida restante: "
              << vida << "/" << vidaMaxima << std::endl;
}

// ========== SOLDADO ==========
Soldado::Soldado(int propietario)
    : Unidad("Soldado", (propietario == 1 ? "J1S" : "J2S"), propietario, 10, 3, 2) {}

void Soldado::habilidad_especial(Contexto& ctx) {
    defensaExtraActual = 2;
    turnosDefensaExtra = 2;
    std::cout << "Soldado: Defensa +2 durante 2 turnos (Defensa total: " 
              << getDefensa() << ")" << std::endl;
}

void Soldado::actualizarTurno() {
    Unidad::actualizarTurno();
}

// ========== ARQUERO ==========
Arquero::Arquero(int propietario)
    : Unidad("Arquero", (propietario == 1 ? "J1A" : "J2A"), propietario, 7, 4, 1) {}

void Arquero::atacar(Unidad& objetivo, Contexto& ctx) {
    std::cout << "Arquero ataca a distancia con precision." << std::endl;
    
    // Ataque especial: daño *1.5 e ignora 1 de defensa
    int defensaEfectiva = objetivo.getDefensa() - 1;
    if (defensaEfectiva < 0) defensaEfectiva = 0;
    
    int danio = static_cast<int>(ataque * 1.5) - defensaEfectiva;
    if (danio < 1) danio = 1;
    
    objetivo.recibirDanio(danio);
    std::cout << nombre << " causa " << danio << " de danio (ignora 1 defensa)." << std::endl;
}

void Arquero::habilidad_especial(Contexto& ctx) {
    std::cout << "Arquero: Ataque de precision activado (siguiente ataque: danio *1.5, ignora 1 defensa)." << std::endl;
}

// ========== CABALLERO ==========
Caballero::Caballero(int propietario)
    : Unidad("Caballero", (propietario == 1 ? "J1C" : "J2C"), propietario, 12, 5, 2),
      cargaLista(false) {} // Inicializar cargaLista

bool Caballero::mover(const Coordenada& destino, Contexto& ctx) {
    std::cout << "Caballero se mueve con mayor alcance." << std::endl;
    return Unidad::mover(destino, ctx);
}

void Caballero::atacar(Unidad& objetivo, Contexto& ctx) {
    if (!activa || !estaViva()) return;

    int danioBase = ataque - objetivo.getDefensa();
    if (danioBase < 1) danioBase = 1;

    int danioFinal = danioBase;
    std::string mensajeCarga = "";

    if (cargaLista) {
        danioFinal = danioBase * 2; // ¡Doble de daño!
        cargaLista = false; // Desactivar la carga después de usarla
        mensajeCarga = " (¡DANO DOBLE POR CARGA!)";
    }

    objetivo.recibirDanio(danioFinal);
    std::cout << nombre << " ataca a " << objetivo.getNombre()
              << " causando " << danioFinal << " de danio" << mensajeCarga << "." << std::endl;
}

void Caballero::habilidad_especial(Contexto& ctx) {
    cargaLista = true; // Activar el flag
    std::cout << "Caballero: ¡Carga devastadora ACTIVADA! (Siguiente ataque hace daño DOBLE)" << std::endl;
}

// ========== MAGO ==========
Mago::Mago(int propietario)
    : Unidad("Mago", (propietario == 1 ? "J1M" : "J2M"), propietario, 8, 6, 1) {}

void Mago::habilidad_especial(Contexto& ctx) {
    std::cout << "\n=== HECHIZO DE AREA 3x3 ===" << std::endl;
    std::cout << "Ingresa coordenada central (x y): ";
    int x, y;
    std::cin >> x >> y;
    
    Mapa* mapa = ctx.getMapa();
    if (!mapa || !mapa->coordenadaValida(x, y)) {
        std::cout << "Coordenada invalida." << std::endl;
        return;
    }
    
    std::cout << "Mago lanza hechizo de area 3x3 centrado en (" << x << "," << y << ")" << std::endl;
    
    // Aplicar daño 3 en área 3x3
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (!mapa->coordenadaValida(i, j)) continue;
            
            Celda* celda = mapa->obtenerCelda(i, j);
            if (!celda) continue;
            
            Unidad* unidad = celda->getUnidad();
            if (unidad && unidad->getPropietario() != propietario) {
                unidad->recibirDanio(3);
                std::cout << "  -> " << unidad->getNombre() << " en (" 
                         << i << "," << j << ") recibe 3 de danio magico" << std::endl;
                
                if (!unidad->estaViva()) {
                    celda->setUnidad(nullptr);
                }
            }
        }
    }
}

// ========== INGENIERO ==========
Ingeniero::Ingeniero(int propietario)
    : Unidad("Ingeniero", (propietario == 1 ? "J1I" : "J2I"), propietario, 6, 1, 1) {}

void Ingeniero::habilidad_especial(Contexto& ctx) {
    std::cout << "\n=== INGENIERO: CONSTRUCCION RAPIDA ===" << std::endl;
    std::cout << "Ingresa coordenada del ingeniero (x y): ";
    int ingX, ingY;
    std::cin >> ingX >> ingY;
    
    Mapa* mapa = ctx.getMapa();
    if (!mapa || !mapa->coordenadaValida(ingX, ingY)) {
        std::cout << "Coordenada invalida." << std::endl;
        return;
    }
    
    Celda* celdaIngeniero = mapa->obtenerCelda(ingX, ingY);
    if (!celdaIngeniero || celdaIngeniero->getUnidad() != this) {
        std::cout << "El ingeniero no esta en esa posicion." << std::endl;
        return;
    }
    
    std::cout << "Buscando celdas libres cercanas para construir 2 edificios..." << std::endl;
    
    int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
    int dy[] = {0, 0, -1, 1, -1, -1, 1, 1};
    
    int edificiosConstruidos = 0;
    
    for (int i = 0; i < 8 && edificiosConstruidos < 2; i++) {
        int nx = ingX + dx[i];
        int ny = ingY + dy[i];
        
        if (!mapa->coordenadaValida(nx, ny)) continue;
        
        Celda* celda = mapa->obtenerCelda(nx, ny);
        if (!celda || !celda->estaLibre() || celda->tieneEdificio()) continue;
        
        // Construir edificio aleatorio
        Edificio* nuevoEdificio = nullptr;
        if (edificiosConstruidos == 0) {
            nuevoEdificio = new Granja(propietario);
            std::cout << "  -> Granja construida en (" << nx << "," << ny << ")" << std::endl;
        } else {
            nuevoEdificio = new Torre(propietario);
            std::cout << "  -> Torre construida en (" << nx << "," << ny << ")" << std::endl;
        }
        
        celda->setEdificio(nuevoEdificio);
        celda->setPropietario(propietario);
        edificiosConstruidos++;
    }
    
    if (edificiosConstruidos == 0) {
        std::cout << "No hay celdas libres cercanas para construir." << std::endl;
    } else {
        std::cout << "Ingeniero construyo " << edificiosConstruidos << " edificio(s)." << std::endl;
    }
}