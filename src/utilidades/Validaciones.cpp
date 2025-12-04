// -------- src/utilidades/Validaciones.cpp --------
#include "Validaciones.h"
#include "../nucleo/Contexto.h"
#include "../nucleo/Mapa.h"
#include "../entidades/Unidad.h"
#include "../entidades/Edificio.h"
#include "../sistemas/Recursos.h"
#include <cmath>

bool Validaciones::puedeMoverse(const Unidad* unidad, const Coordenada& destino, Contexto& ctx) {
    if (!unidad || !unidad->estaActiva()) return false;
    
    Mapa* mapa = ctx.getMapa();
    if (!mapa || !mapa->coordenadaValida(destino)) return false;
    
    Celda* celdaDestino = mapa->obtenerCelda(destino);
    if (!celdaDestino || !celdaDestino->estaLibre()) return false;
    
    return true;
}

bool Validaciones::puedeAtacar(const Unidad* atacante, const Coordenada& objetivo, Contexto& ctx) {
    if (!atacante || !atacante->estaActiva()) return false;
    
    Mapa* mapa = ctx.getMapa();
    if (!mapa || !mapa->coordenadaValida(objetivo)) return false;
    
    Celda* celdaObjetivo = mapa->obtenerCelda(objetivo);
    if (!celdaObjetivo) return false;
    
    Unidad* unidadObjetivo = celdaObjetivo->getUnidad();
    if (!unidadObjetivo) return false;
    
    if (unidadObjetivo->getPropietario() == atacante->getPropietario()) return false;
    
    return true;
}

bool Validaciones::puedeConstruir(const Coordenada& posicion, int tipoEdificio, Contexto& ctx) {
    Mapa* mapa = ctx.getMapa();
    if (!mapa || !mapa->coordenadaValida(posicion)) return false;
    
    Celda* celda = mapa->obtenerCelda(posicion);
    if (!celda || celda->tieneEdificio() || !celda->estaLibre()) return false;
    
    return true;
}

bool Validaciones::tieneRecursosSuficientes(int comida, int metal, int energia, Contexto& ctx) {
    Recursos* rec = ctx.getRecursosJ1();
    if (!rec) return false;
    
    return rec->tieneSuficiente(comida, metal, energia);
}

bool Validaciones::esMovimientoValido(const Coordenada& origen, const Coordenada& destino) {
    int distancia = calcularDistancia(origen, destino);
    return distancia == 1;
}

bool Validaciones::esCasillaNeutralOPropia(const Coordenada& pos, int jugador, Contexto& ctx) {
    Mapa* mapa = ctx.getMapa();
    if (!mapa) return false;
    
    Celda* celda = mapa->obtenerCelda(pos);
    if (!celda) return false;
    
    int propietario = celda->getPropietario();
    return propietario == 0 || propietario == jugador;
}

int Validaciones::calcularDistancia(const Coordenada& pos1, const Coordenada& pos2) {
    return std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y);
}
