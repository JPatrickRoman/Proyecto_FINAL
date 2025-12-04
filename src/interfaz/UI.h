// -------- UI.h --------
#ifndef UI_H
#define UI_H

#include <string>
#include <vector>

class Contexto;

class UI {
private:
    std::vector<std::string> bitacora;

public:
    UI();
    
    void mostrarPanel(const Contexto& ctx);
    void mostrarMenu();
    void mostrarAyuda();
    
    void agregarLog(const std::string& mensaje);
    void mostrarBitacora() const;
    void limpiarBitacora();
    
    void mostrarMensaje(const std::string& mensaje);
    void mostrarError(const std::string& error);
    void mostrarExito(const std::string& mensaje);
    
    int solicitarOpcion(int min, int max);
    std::string solicitarTexto(const std::string& prompt);

    // Nueva función para mostrar estado completo con bitácora
    void mostrarEstadoConMapa(const Contexto& ctx, const std::string& tituloEstado);
};

#endif