// -------- Validaciones.h --------
#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include "../nucleo/Coordenada.h"

class Contexto;
class Unidad;
class Edificio;

class Validaciones {
public:
    static bool puedeMoverse(const Unidad* unidad, const Coordenada& destino, Contexto& ctx);
    
    static bool puedeAtacar(const Unidad* atacante, const Coordenada& objetivo, Contexto& ctx);
    
    static bool puedeConstruir(const Coordenada& posicion, int tipoEdificio, Contexto& ctx);
    
    static bool tieneRecursosSuficientes(int comida, int metal, int energia, Contexto& ctx);
    
    static bool esMovimientoValido(const Coordenada& origen, const Coordenada& destino);
    
    static bool esCasillaNeutralOPropia(const Coordenada& pos, int jugador, Contexto& ctx);
    
    static int calcularDistancia(const Coordenada& pos1, const Coordenada& pos2);
};

#endif
