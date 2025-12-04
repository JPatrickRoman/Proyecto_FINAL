// -------- Coordenada.h --------
#ifndef COORDENADA_H
#define COORDENADA_H

struct Coordenada {
    int x;
    int y;
    
    Coordenada(int x = 0, int y = 0) : x(x), y(y) {}
    
    bool operator==(const Coordenada& otra) const {
        return x == otra.x && y == otra.y;
    }
    
    bool operator!=(const Coordenada& otra) const {
        return !(*this == otra);
    }
};

#endif
