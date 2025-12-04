// -------- Eventos.h --------
#ifndef EVENTOS_H
#define EVENTOS_H

#include <string>
#include <vector>

class Contexto;

class Evento {
protected:
    std::string nombre;
    int turnoActivacion;
    bool activo;

public:
    Evento(const std::string& nombre, int turno);
    virtual ~Evento() = default;
    
    virtual void ejecutar(Contexto& ctx) = 0;
    
    std::string getNombre() const { return nombre; }
    int getTurnoActivacion() const { return turnoActivacion; }
    bool estaActivo() const { return activo; }
    void activar() { activo = true; }
    void desactivar() { activo = false; }
};

class EventoRefuerzo : public Evento {
private:
    int jugadorObjetivo;
    int cantidadUnidades;

public:
    EventoRefuerzo(int turno, int jugador, int cantidad);
    void ejecutar(Contexto& ctx) override;
};

class EventoClima : public Evento {
private:
    std::string tipoClima;
    int duracion;

public:
    EventoClima(int turno, const std::string& tipo, int duracion);
    void ejecutar(Contexto& ctx) override;
};

class EventoRutaComercial : public Evento {
private:
    int bonusRecursos;

public:
    EventoRutaComercial(int turno, int bonus);
    void ejecutar(Contexto& ctx) override;
};

class GestorEventos {
private:
    std::vector<Evento*> eventosProgamados;
    std::vector<Evento*> eventosActivos;

public:
    GestorEventos();
    ~GestorEventos();
    
    void programarEvento(Evento* evento);
    void procesarTurno(int turnoActual, Contexto& ctx);
    void limpiarEventosCompletados();
    
    int cantidadEventosActivos() const { return eventosActivos.size(); }
};

#endif
