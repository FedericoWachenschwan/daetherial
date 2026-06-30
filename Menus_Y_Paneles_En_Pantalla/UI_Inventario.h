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

    static const int CANTIDAD_DE_CASILLEROS_VISIBLES = 5;

    sf::RectangleShape _fondo_del_casillero;
    float _tamano_de_cada_casillero;
    float _margen_entre_casilleros;
    sf::Font _fuente_del_inventario;
    sf::Text _texto_de_la_cantidad;

    ///=============================================================///
    ///   AJUSTES EN CALIENTE - Para centrar la grilla a mano
    ///=============================================================///
    float _desplazamiento_x;
    float _desplazamiento_y;
    float _origen_x = 0.f;
    float _origen_y = 0.f;

    ///=============================================================///
    ///   AREA REAL DE LA GRILLA
    ///=============================================================///
    sf::FloatRect _area_total_del_panel;

    bool _el_inventario_esta_abierto = false;

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    UI_Inventario();

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    bool getEsta_abierto() const { return _el_inventario_esta_abierto; }

    ///=============================================================///
    ///   SETTERS
    ///=============================================================///
    void setAbierto(bool abierto) { _el_inventario_esta_abierto = abierto; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    void dibujar(sf::RenderWindow& ventana_del_juego, const Inventario& mochila);
    void ajustar_posicion(float desplazamiento_x, float desplazamiento_y);
    void ajustar_origen(float origen_x, float origen_y);
    void detectar_clic_en_un_casillero(sf::Vector2i posicion_del_mouse, Inventario& mochila, const sf::RenderWindow& ventana_del_juego);
    void alternar_abierto_y_cerrado() { _el_inventario_esta_abierto = !_el_inventario_esta_abierto; }

    // Combina un chequeo de "esta abierto" con un calculo de
    // posicion: no es un simple devolver-un-atributo, por eso no
    // lleva "get" adelante
    bool getEl_mouse_esta_sobre_el_panel(sf::Vector2i posicion_del_mouse) const;
};