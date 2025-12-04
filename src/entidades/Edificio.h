// -------- Edificio.h --------
#ifndef EDIFICIO_H
#define EDIFICIO_H

#include <string>

class Contexto;
class Recursos;
class Evento;

class Edificio {
protected:
    std::string nombre;
    std::string codigo;
    int propietario;
    int vida;
    int vidaMaxima;
    
public:
    Edificio(const std::string& nombre, const std::string& codigo, int propietario, int vida);
    virtual ~Edificio() = default;
    
    virtual void efecto_turno(Recursos& recursos) = 0;
    virtual void reaccion_evento(const Evento& evento);
    
    std::string getNombre() const { return nombre; }
    std::string getCodigo() const { return codigo; }
    int getPropietario() const { return propietario; }
    
    void setPropietario(int p);  // MODIFICADO: ahora actualiza código también
    void setCodigo(const std::string& c) { codigo = c; }  // NUEVO
    
    int getVida() const { return vida; }
    
    void recibirDanio(int cantidad);
    bool destruido() const { return vida <= 0; }
};

class Granja : public Edificio {
public:
    Granja(int propietario);
    void efecto_turno(Recursos& recursos) override;
};

class Cuartel : public Edificio {
public:
    Cuartel(int propietario);
    void efecto_turno(Recursos& recursos) override;
};

class Torre : public Edificio {
public:
    Torre(int propietario);
    void efecto_turno(Recursos& recursos) override;
};

class Forja : public Edificio {
public:
    Forja(int propietario);
    void efecto_turno(Recursos& recursos) override;
};

#endif