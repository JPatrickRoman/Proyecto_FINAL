// -------- Mapa.h --------
#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <string>
#include "Coordenada.h"

class Terreno;
class Unidad;
class Edificio;

class Celda {
private:
    Terreno* terreno;
    Unidad* unidad;
    Edificio* edificio;
    int propietario;

public:
    Celda();
    ~Celda();
    
    Terreno* getTerreno() const { return terreno; }
    Unidad* getUnidad() const { return unidad; }
    Edificio* getEdificio() const { return edificio; }
    int getPropietario() const { return propietario; }
    
    void setTerreno(Terreno* t);
    void setUnidad(Unidad* u);
    void setEdificio(Edificio* e);
    void setPropietario(int p) { propietario = p; }
    
    bool estaLibre() const { return unidad == nullptr; }
    bool tieneEdificio() const { return edificio != nullptr; }
    std::string obtenerCodigo() const;
};

class Mapa {
private:
    static const int TAMANO = 12;
    std::vector<std::vector<Celda*>> celdas;

    // Funciones auxiliares para generación procedural
    void colocarUnidadesIniciales(int jugador, int baseX, int baseY);
    void colocarEdificiosNeutrales();
    void colocarEdificiosAdicionales(int jugador, int baseX, int baseY);
    bool esCercaDeAgua(int x, int y) const;

public:
    Mapa();
    ~Mapa();

    void inicializar();
    void generarMapaInicial();

    Celda* obtenerCelda(const Coordenada& coord);
    Celda* obtenerCelda(int x, int y);

    bool coordenadaValida(const Coordenada& coord) const;
    bool coordenadaValida(int x, int y) const;

    void renderizar() const;
    std::string obtenerEstadoCelda(int x, int y) const;

    int getTamano() const { return TAMANO; }
    int calcularDominio(int jugador) const;
};

#endif