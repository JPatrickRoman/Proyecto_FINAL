// -------- Terreno.h --------
#ifndef TERRENO_H
#define TERRENO_H

#include <string>

class Unidad;

class Terreno {
protected:
    std::string nombre;
    std::string codigo;
    
public:
    Terreno(const std::string& nombre, const std::string& codigo);
    virtual ~Terreno() = default;
    
    virtual int costo_movimiento(const Unidad& unidad) const = 0;
    virtual int bono_defensa(const Unidad& unidad) const = 0;
    
    std::string getNombre() const { return nombre; }
    std::string getCodigo() const { return codigo; }
};

class Llanura : public Terreno {
public:
    Llanura();
    int costo_movimiento(const Unidad& unidad) const override;
    int bono_defensa(const Unidad& unidad) const override;
};

class Bosque : public Terreno {
public:
    Bosque();
    int costo_movimiento(const Unidad& unidad) const override;
    int bono_defensa(const Unidad& unidad) const override;
};

class Montana : public Terreno {
public:
    Montana();
    int costo_movimiento(const Unidad& unidad) const override;
    int bono_defensa(const Unidad& unidad) const override;
};

class Agua : public Terreno {
public:
    Agua();
    int costo_movimiento(const Unidad& unidad) const override;
    int bono_defensa(const Unidad& unidad) const override;
};

class Pantano : public Terreno {
public:
    Pantano();
    int costo_movimiento(const Unidad& unidad) const override;
    int bono_defensa(const Unidad& unidad) const override;
};

#endif
