// -------- src/entidades/Edificio.cpp --------
#include "Edificio.h"
#include "../sistemas/Recursos.h"
#include "../sistemas/Eventos.h"
#include <iostream>

Edificio::Edificio(const std::string& nombre, const std::string& codigo, 
                   int propietario, int vida)
    : nombre(nombre), codigo(codigo), propietario(propietario), 
      vida(vida), vidaMaxima(vida) {}

void Edificio::reaccion_evento(const Evento& evento) {
    std::cout << nombre << " reacciona al evento." << std::endl;
}

void Edificio::recibirDanio(int cantidad) {
    vida -= cantidad;
    if (vida < 0) vida = 0;
}

void Edificio::setPropietario(int p) {
    propietario = p;
    
    // Actualizar código según nuevo propietario
    std::string sufijo = codigo.substr(2); // Obtener "Gr", "Cu", "To", "Fo"
    
    if (propietario == 0) {
        codigo = "Ne" + sufijo;
    } else if (propietario == 1) {
        codigo = "J1" + sufijo;
    } else if (propietario == 2) {
        codigo = "J2" + sufijo;
    }
}

Granja::Granja(int propietario) 
    : Edificio("Granja", (propietario == 0 ? "NeGr" : (propietario == 1 ? "J1Gr" : "J2Gr")), 
               propietario, 20) {}

void Granja::efecto_turno(Recursos& recursos) {
    if (propietario > 0) {
        recursos.agregar(2, 0, 0);
        std::cout << "Granja produce +2 comida." << std::endl;
    }
}

Cuartel::Cuartel(int propietario)
    : Edificio("Cuartel", (propietario == 0 ? "NeCu" : (propietario == 1 ? "J1Cu" : "J2Cu")), 
               propietario, 30) {}

void Cuartel::efecto_turno(Recursos& recursos) {
    if (propietario > 0) {
        recursos.agregar(0, 1, 0);
        std::cout << "Cuartel produce +1 metal." << std::endl;
    }
}

Torre::Torre(int propietario)
    : Edificio("Torre", (propietario == 0 ? "NeTo" : (propietario == 1 ? "J1To" : "J2To")), 
               propietario, 25) {}

void Torre::efecto_turno(Recursos& recursos) {
    if (propietario > 0) {
        recursos.agregar(0, 0, 1);
        std::cout << "Torre produce +1 energia." << std::endl;
    }
}

Forja::Forja(int propietario)
    : Edificio("Forja", (propietario == 1 ? "J1Fo" : "J2Fo"), propietario, 20) {}

void Forja::efecto_turno(Recursos& recursos) {
    std::cout << "Forja mejora unidades." << std::endl;
}