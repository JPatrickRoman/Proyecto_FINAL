// -------- Controlador.h --------
#ifndef CONTROLADOR_H
#define CONTROLADOR_H

class Contexto;
class Evento;
class Mapa;
class Recursos;

class Controlador {
protected:
    int jugadorId;

public:
    Controlador(int id);
    virtual ~Controlador() = default;

    virtual void resolver_fase(Contexto& ctx) = 0;
    virtual void aplicar_rutina(const Evento& evento, Contexto& ctx);

    int getId() const { return jugadorId; }
};

class JugadorHumano : public Controlador {
public:
    JugadorHumano();
    void resolver_fase(Contexto& ctx) override;

private:
    void mostrarMenu(Contexto& ctx);
    void moverUnidad(Contexto& ctx);
    void atacarConUnidad(Contexto& ctx);
    void construirEdificio(Contexto& ctx);
    void reclutarUnidad(Contexto& ctx);
    void usarHabilidadEspecial(Contexto& ctx);
    void verReportes(Contexto& ctx);
    void guardarPartida(Contexto& ctx);
    void cargarPartida(Contexto& ctx);
    void mostrarEstado(Contexto& ctx);
    void actualizarUnidades(Contexto& ctx);  // NUEVA FUNCIÓN
};

class SistemaIA : public Controlador {
public:
    SistemaIA();
    void resolver_fase(Contexto& ctx) override;

private:
    void patrullar(Contexto& ctx);
    void reforzarPosiciones(Contexto& ctx);
    void generarRecursos(Contexto& ctx);
    bool encontrarDestinoParaUnidad(int x, int y, int& destX, int& destY, Mapa* mapa);
    void buscarYMoverHaciaEnemigo(int x, int y, Mapa* mapa); // Deprecated - mantener por compatibilidad
    bool intentarReclutarCerca(int x, int y, Mapa* mapa, Recursos* rec);
};

class NeutralControlador : public Controlador {
public:
    NeutralControlador();
    void resolver_fase(Contexto& ctx) override;
};

#endif