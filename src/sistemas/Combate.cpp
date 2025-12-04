// -------- src/sistemas/Combate.cpp --------
#include "Combate.h"
#include "../entidades/Unidad.h"
#include "../entidades/Terreno.h"
#include "../nucleo/Contexto.h"
#include "../nucleo/Mapa.h"
#include "../nucleo/Coordenada.h"
#include <iostream>
#include <cmath>

void ResolutorCombate::resolverAtaque(Unidad* atacante, Unidad* defensor, Contexto& ctx) {
    if (!atacante || !defensor) {
        std::cout << "Error: unidades invalidas." << std::endl;
        return;
    }
    
    if (!atacante->estaViva() || !defensor->estaViva()) {
        std::cout << "Una de las unidades no esta viva." << std::endl;
        return;
    }
    
    int danio = calcularDanio(atacante, defensor, ctx);
    
    std::cout << "\n--- COMBATE ---" << std::endl;
    std::cout << atacante->getNombre() << " (" << atacante->getVida() << " HP) ataca a "
              << defensor->getNombre() << " (" << defensor->getVida() << " HP)" << std::endl;
    
    defensor->recibirDanio(danio);
    
    if (!defensor->estaViva()) {
        std::cout << defensor->getNombre() << " ha sido derrotado." << std::endl;
    }
}

int ResolutorCombate::calcularDanio(Unidad* atacante, Unidad* defensor, Contexto& ctx) {
    int danioBase = atacante->getAtaque();
    int defensa = defensor->getDefensa();
    int bonusTerreno = obtenerBonusTerreno(defensor, ctx);
    
    int danioTotal = danioBase - (defensa + bonusTerreno);
    
    if (danioTotal < 1) danioTotal = 1;
    
    return danioTotal;
}

bool ResolutorCombate::verificarRango(const Coordenada& pos1, const Coordenada& pos2) {
    int distancia = std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y);
    return distancia <= 1;
}

int ResolutorCombate::obtenerBonusTerreno(Unidad* unidad, Contexto& ctx) {
    return 0;
}

ContextoCombate::ContextoCombate(Unidad* atk, Unidad* def)
    : atacante(atk), defensor(def), danioBase(0), 
      modificadorTerreno(0), modificadorMoral(0) {}

int ContextoCombate::getDanioTotal() const {
    return danioBase + modificadorTerreno + modificadorMoral;
}
