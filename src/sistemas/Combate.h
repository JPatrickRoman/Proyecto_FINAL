// -------- Combate.h --------
#ifndef COMBATE_H
#define COMBATE_H

class Unidad;
class Contexto;
class Coordenada;

class ResolutorCombate {
public:
    static void resolverAtaque(Unidad* atacante, Unidad* defensor, Contexto& ctx);
    static int calcularDanio(Unidad* atacante, Unidad* defensor, Contexto& ctx);
    static bool verificarRango(const Coordenada& pos1, const Coordenada& pos2);
    
private:
    static int obtenerBonusTerreno(Unidad* unidad, Contexto& ctx);
};

class ContextoCombate {
private:
    Unidad* atacante;
    Unidad* defensor;
    int danioBase;
    int modificadorTerreno;
    int modificadorMoral;

public:
    ContextoCombate(Unidad* atk, Unidad* def);
    
    void setDanioBase(int d) { danioBase = d; }
    void setModificadorTerreno(int m) { modificadorTerreno = m; }
    void setModificadorMoral(int m) { modificadorMoral = m; }
    
    int getDanioTotal() const;
    
    Unidad* getAtacante() const { return atacante; }
    Unidad* getDefensor() const { return defensor; }
};

#endif
