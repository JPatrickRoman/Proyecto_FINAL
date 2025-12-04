// -------- Recursos.h --------
#ifndef RECURSOS_H
#define RECURSOS_H

class Recursos {
private:
    int comida;
    int metal;
    int energia;

public:
    Recursos(int comida = 0, int metal = 0, int energia = 0);
    
    int getComida() const { return comida; }
    int getMetal() const { return metal; }
    int getEnergia() const { return energia; }
    
    void setComida(int c) { comida = c; }
    void setMetal(int m) { metal = m; }
    void setEnergia(int e) { energia = e; }
    
    void agregar(int c, int m, int e);
    bool consumir(int c, int m, int e);
    
    bool tieneSuficiente(int c, int m, int e) const;
    
    void operator+=(const Recursos& otros);
};

#endif
