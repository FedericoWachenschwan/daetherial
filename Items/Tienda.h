#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Item.h"

// Le avisamos al compilador que la clase Personaje existe, sin incluir todo su archivo
class Personaje;

///=============================================================///
///         CLASE TIENDA - El edificio que vende items al jugador
///=============================================================///
class Tienda {

private:

    ///=========================================================///
    ///     IMAGEN DE LA TIENDA - Lo que se ve en el mapa
    ///=========================================================///
    sf::Texture _textura_de_la_tienda; // La imagen de la tienda guardada en memoria
    sf::Sprite _sprite_de_la_tienda; // El dibujito que aparece en el mapa

    ///=========================================================///
    ///     POSICIÓN Y RANGO - Dónde está y hasta dónde detecta al jugador
    ///=========================================================///
    sf::Vector2f _posicion_de_la_tienda_en_el_mapa; // Las coordenadas X e Y de la tienda en el mapa
    sf::FloatRect _zona_de_interaccion_de_la_tienda; // El rectángulo invisible que detecta al jugador
    bool _el_jugador_esta_cerca_de_la_tienda; // true si el jugador está dentro de la zona

    ///=========================================================///
    ///     ITEM EN VENTA - El objeto que vende la tienda
    ///=========================================================///
    Item* _item_que_vende_la_tienda; // El item que ofrece la tienda para comprar
    int _precio_del_item_que_vende_la_tienda; // Cuánto oro cuesta comprarlo

    ///=========================================================///
    ///     CARTEL - El texto que aparece cuando el jugador se acerca
    ///=========================================================///
    sf::Font _fuente_del_cartel_de_la_tienda; // El tipo de letra del cartel
    sf::Text _cartel_de_la_tienda; // El texto que se dibuja cuando el jugador está cerca

public:

    ///=========================================================///
    ///     CONSTRUCTOR Y DESTRUCTOR - Crear y destruir la tienda
    ///=========================================================///
    Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa, Item* item_que_vende_la_tienda, int precio_del_item_que_vende_la_tienda);
    ~Tienda();

    ///=========================================================///
    ///     CARGA DEL CARTEL - Se llama una sola vez al crear la tienda
    ///=========================================================///
    void cargar_fuente_y_cartel(); // Carga la fuente y configura el texto del cartel

    ///=========================================================///
    ///     MÉTODOS PRINCIPALES - Lo que hace la tienda cada frame
    ///=========================================================///
    void actualizar_tienda(sf::Vector2f posicion_actual_del_jugador); // Chequea cada frame si el jugador está cerca
    void dibujar_tienda(sf::RenderWindow& ventana_del_juego, bool mostrar_zona_de_deteccion); // Dibuja la tienda, y el rectángulo verde solo si el debug está activo

    ///=========================================================///
    ///     MÉTODO DE COMPRA - Ejecuta la compra del item
    ///=========================================================///
    void intentar_comprar_item(Personaje& jugador_que_quiere_comprar); // Intenta hacer la compra si el jugador puede pagar

    ///=========================================================///
    ///     GETTER - Para saber desde afuera si el jugador está cerca
    ///=========================================================///
    bool getJugadorEstaCercaDeLaTienda(); // Devuelve true si el jugador está cerca de la tienda
};