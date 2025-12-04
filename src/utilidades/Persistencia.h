// -------- Persistencia.h --------
#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include <fstream>

// Forward declarations
class Contexto;

class GestorArchivos {
public:

    static bool guardarPartida(const Contexto& ctx, const std::string& nombreArchivo);
    static bool cargarPartida(Contexto& ctx, const std::string& nombreArchivo);
    static bool exportarReporte(const Contexto& ctx, const std::string& nombreArchivo);
    static bool exportarTelemetria(const Contexto& ctx, const std::string& nombreArchivo);

private:

    static void escribirEstadoMapa(std::ofstream& archivo, const Contexto& ctx);
    static void escribirRecursos(std::ofstream& archivo, const Contexto& ctx);
    static void escribirUnidades(std::ofstream& archivo, const Contexto& ctx);
};

//Sistema de replay para grabar acciones y eventos de la partida

class SistemaReplay {
private:
    std::string archivoLog;      //< Nombre del archivo de log
    std::ofstream log;           //< Stream del archivo de log

public:

    explicit SistemaReplay(const std::string& nombreArchivo);
    ~SistemaReplay();

    void registrarAccion(const std::string& accion);
    void registrarTurno(int turno);
    void registrarEvento(const std::string& evento);
    void cerrar();
};

#endif // PERSISTENCIA_H