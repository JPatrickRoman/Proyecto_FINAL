cmake_minimum_required(VERSION 3.10)
project(Proyecto_FINAL)

# Configurar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definir archivos fuente
set(NUCLEO_SOURCES
    src/nucleo/Contexto.cpp
    src/nucleo/Mapa.cpp
    src/nucleo/Juego.cpp
)

set(ENTIDADES_SOURCES
    src/entidades/Terreno.cpp
    src/entidades/Unidad.cpp
    src/entidades/Edificio.cpp
    src/entidades/Controlador.cpp
)

set(SISTEMAS_SOURCES
    src/sistemas/Recursos.cpp
    src/sistemas/Eventos.cpp
    src/sistemas/Combate.cpp
)

set(INTERFAZ_SOURCES
    src/interfaz/UI.cpp
)

set(UTILIDADES_SOURCES
    src/utilidades/Validaciones.cpp
    src/utilidades/Persistencia.cpp
)

# Agrupar todos los archivos fuente
set(ALL_SOURCES
    src/main.cpp
    ${NUCLEO_SOURCES}
    ${ENTIDADES_SOURCES}
    ${SISTEMAS_SOURCES}
    ${INTERFAZ_SOURCES}
    ${UTILIDADES_SOURCES}
)

# Crear el ejecutable
add_executable(ZZZ ${ALL_SOURCES})

# Incluir directorios de headers
target_include_directories(ZZZ PRIVATE src)
