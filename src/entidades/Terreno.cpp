// -------- src/entidades/Terreno.cpp --------
#include "Terreno.h"
#include "../entidades/Unidad.h"

Terreno::Terreno(const std::string& nombre, const std::string& codigo)
    : nombre(nombre), codigo(codigo) {}

// ========== LLANURA ==========
Llanura::Llanura() : Terreno("Llanura", "LL") {}

int Llanura::costo_movimiento(const Unidad& unidad) const {
    return 1; // Costo base de movimiento
}

int Llanura::bono_defensa(const Unidad& unidad) const {
    return 0; // Sin bono de defensa
}

// ========== BOSQUE ==========
Bosque::Bosque() : Terreno("Bosque", "BO") {}

int Bosque::costo_movimiento(const Unidad& unidad) const {
    return 2; // Movimiento más lento en bosque
}

int Bosque::bono_defensa(const Unidad& unidad) const {
    return 2; // Bono de defensa por cobertura
}

// ========== MONTAÑA ==========
Montana::Montana() : Terreno("Montana", "MO") {}

int Montana::costo_movimiento(const Unidad& unidad) const {
    return 3; // Movimiento muy lento en montaña
}

int Montana::bono_defensa(const Unidad& unidad) const {
    return 3; // Alto bono de defensa por elevación
}

// ========== AGUA ==========
Agua::Agua() : Terreno("Agua", "AG") {}

int Agua::costo_movimiento(const Unidad& unidad) const {
    // Solo unidades náuticas pueden atravesar agua
    // Para unidades terrestres, retorna un costo prohibitivo
    return 999; // Efectivamente intransitable para unidades normales
}

int Agua::bono_defensa(const Unidad& unidad) const {
    return 0; // Sin bono de defensa en agua
}

// ========== PANTANO ==========
Pantano::Pantano() : Terreno("Pantano", "PA") {}

int Pantano::costo_movimiento(const Unidad& unidad) const {
    return 3; // Movimiento difícil en terreno pantanoso
}

int Pantano::bono_defensa(const Unidad& unidad) const {
    return 1; // Bono mínimo de defensa
}