// -------- Unidad.h --------
#ifndef UNIDAD_H
#define UNIDAD_H

#include <string>
#include "../nucleo/Coordenada.h"

class Contexto;

class Unidad {
protected:
    std::string nombre;
    std::string codigo;
    int propietario;
    int vida;
    int vidaMaxima;
    int ataque;
    int defensa;
    bool activa;
    
    // Nuevos atributos para habilidades especiales
    int turnosDefensaExtra;
    int defensaExtraActual;

public:
    Unidad(const std::string& nombre, const std::string& codigo, int propietario,
           int vida, int ataque, int defensa);
    virtual ~Unidad() = default;

    virtual bool mover(const Coordenada& destino, Contexto& ctx);
    virtual void atacar(Unidad& objetivo, Contexto& ctx);
    virtual void habilidad_especial(Contexto& ctx);
    virtual void actualizarTurno(); // Nueva función para actualizar efectos temporales

    void recibirDanio(int cantidad);
    bool estaViva() const { return vida > 0; }

    std::string getNombre() const { return nombre; }
    std::string getCodigo() const { return codigo; }
    int getPropietario() const { return propietario; }
    int getVida() const { return vida; }
    int getAtaque() const { return ataque; }
    int getDefensa() const { return defensa + defensaExtraActual; }
    int getDefensaBase() const { return defensa; }
    bool estaActiva() const { return activa; }

    void setActiva(bool a) { activa = a; }
};

class Soldado : public Unidad {
public:
    Soldado(int propietario);
    void habilidad_especial(Contexto& ctx) override;
    void actualizarTurno() override;
};

class Arquero : public Unidad {
public:
    Arquero(int propietario);
    void atacar(Unidad& objetivo, Contexto& ctx) override;
    void habilidad_especial(Contexto& ctx) override;
};

//
class Caballero : public Unidad {
private:
    // Atributo para controlar si la Carga Devastadora está activa
    bool cargaLista;

public:
    Caballero(int propietario);
    bool mover(const Coordenada& destino, Contexto& ctx) override;
    // Sobrescribir atacar para aplicar el daño doble
    void atacar(Unidad& objetivo, Contexto& ctx) override;
    void habilidad_especial(Contexto& ctx) override;
};

class Mago : public Unidad {
public:
    Mago(int propietario);
    void habilidad_especial(Contexto& ctx) override;
};

class Ingeniero : public Unidad {
public:
    Ingeniero(int propietario);
    void habilidad_especial(Contexto& ctx) override;
};

#endif