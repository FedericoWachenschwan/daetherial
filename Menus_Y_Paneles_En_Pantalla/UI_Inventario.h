#pragma once
#include <SFML/Graphics.hpp>
#include "Inventario.h"
#include <string>

///=================================================================///
///   UI_INVENTARIO - Dibuja la mochila del jugador en pantalla y
///   detecta clics sobre los casilleros
///=================================================================///
class UI_Inventario {
private:

    static const int CANTIDAD_DE_CASILLEROS_VISIBLES = 5; // Cuantos casilleros se ven en pantalla

    sf::RectangleShape _fondo_del_casillero; // Rectangulo reutilizable para dibujar cada casillero
    float _tamano_de_cada_casillero; // Ancho y alto de un casillero en pixeles
    float _margen_entre_casilleros; // Espacio entre un casillero y el siguiente
    sf::Font _fuente_del_inventario; // Tipografia para mostrar las cantidades
    sf::Text _texto_de_la_cantidad; // Texto reutilizable para el numero de items

 ///=============================================================///
 ///   AJUSTES EN CALIENTE - Para centrar la grilla a mano
 ///=============================================================///
    float _desplazamiento_x; // Offset horizontal del panel (ajustable con debug)
    float _desplazamiento_y; // Offset vertical del panel (ajustable con debug)
    float _origen_x = 0.f; // Punto de origen del casillero en X (ajustable con debug)
    float _origen_y = 0.f; // Punto de origen del casillero en Y (ajustable con debug)

 ///=============================================================///
 ///   AREA REAL DE LA GRILLA
 ///=============================================================///
    bool _el_inventario_esta_abierto = false; // Si es falso, no se dibuja ni responde a clics

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    UI_Inventario();

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #2
    bool getEsta_abierto() const { return _el_inventario_esta_abierto; } // Devuelve si el inventario esta visible

 ///=============================================================///
 ///   SETTERS
 ///=============================================================///

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
 // #4
    void dibujar(sf::RenderWindow& ventana_del_juego, const Inventario& mochila);
 // #5
    void ajustar_posicion(float desplazamiento_x, float desplazamiento_y); // Mueve el panel de posicion
 // #6
    void ajustar_origen(float origen_x, float origen_y); // Ajusta el origen de los casilleros
 // #7
    void detectar_clic_en_un_casillero(sf::Vector2i posicion_del_mouse, Inventario& mochila, const sf::RenderWindow& ventana_del_juego);
 // #8
    void alternar_abierto_y_cerrado() { _el_inventario_esta_abierto = !_el_inventario_esta_abierto; } // Cambia entre abierto y cerrado
};
