#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Item.h"
#include "Personaje.h"

///=================================================================///
///   TIENDA - El edificio que vende items al jugador
///=================================================================///
class Tienda {
private:

    ///=============================================================///
    ///   IMAGEN DE LA TIENDA
    ///=============================================================///
    sf::Texture _textura_de_la_tienda;
    sf::Sprite _sprite_de_la_tienda;

    ///=============================================================///
    ///   ZONA DE DETECCION
    ///=============================================================///
    sf::FloatRect _zona_de_interaccion_de_la_tienda;
    bool _el_jugador_esta_cerca_de_la_tienda;

    ///=============================================================///
    ///   ITEMS EN VENTA
    ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA = 3;
    Item _items_en_venta[CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA];
    int _cantidad_de_items_cargados_en_la_tienda;
    int _indice_del_item_seleccionado_en_la_tienda;
    int _cantidad_que_quiere_comprar_el_jugador;

    ///=============================================================///
    ///   ESTADO DE LA TIENDA
    ///=============================================================///
    bool _la_tienda_esta_abierta;

    ///=============================================================///
    ///   FUENTE Y CARTEL
    ///=============================================================///
    sf::Font _fuente_del_cartel_de_la_tienda;
    sf::Text _cartel_de_la_tienda;

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa);

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    bool getEl_jugador_esta_cerca_de_la_tienda() const { return _el_jugador_esta_cerca_de_la_tienda; }
    bool getLa_tienda_esta_abierta() const { return _la_tienda_esta_abierta; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    void cargar_fuente_y_cartel();
    void agregar_item_en_venta(const Item& item_para_agregar);

    void actualizar_tienda(sf::Vector2f posicion_actual_del_jugador);
    void dibujar_sprite_en_el_mapa(sf::RenderWindow& ventana_del_juego, bool mostrar_zona_de_deteccion);
    void dibujar_interfaz_de_compra(sf::RenderWindow& ventana_del_juego, sf::Font& fuente_del_hud);

    void abrir_tienda();
    void cerrar_tienda();
    void seleccionar_item_siguiente();
    void seleccionar_item_anterior();
    void aumentar_cantidad_a_comprar();
    void disminuir_cantidad_a_comprar();
    void intentar_comprar_item_seleccionado(Personaje& jugador);
};