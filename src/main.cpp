// -------- src/main.cpp --------
#include <iostream>
#include "nucleo/Juego.h"

int main() {
    try {
        Juego juego;
        juego.iniciar();
        juego.ejecutarCiclo();
    } catch (const std::exception& e) {
        std::cerr << "Error fatal: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
