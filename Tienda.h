#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Item.h"

class Personaje;

///=============================================================///
///   CLASE TIENDA - El edificio que vende items al jugador
///=============================================================///
class Tienda {
private:

    ///=========================================================///
    ///   IMAGEN DE LA TIENDA - Lo que se ve en el mapa
    ///=========================================================///
    sf::Texture _textura_de_la_tienda;        // La imagen cargada en memoria
    sf::Sprite _sprite_de_la_tienda;          // El dibujito que aparece en el mapa

    ///=========================================================///
    ///   ZONA DE DETECCIÓN - Donde el jugador activa la tienda
    ///=========================================================///
    sf::FloatRect _zona_de_interaccion_de_la_tienda; // El rectángulo invisible de detección
    bool _el_jugador_esta_cerca_de_la_tienda;         // true si el jugador está dentro de la zona

    ///=========================================================///
    ///   ITEMS EN VENTA - Los objetos que vende la tienda
    ///=========================================================///
    static const int CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA = 3; // Cuántos items puede tener la tienda
    Item* _items_en_venta[CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA]; // Array fijo de items
    int _cantidad_de_items_cargados_en_la_tienda;     // Cuántos items hay cargados actualmente
    int _indice_del_item_seleccionado_en_la_tienda;   // Qué item está seleccionado (0, 1, 2...)
    int _cantidad_que_quiere_comprar_el_jugador;       // Cuántos quiere comprar el jugador

    ///=========================================================///
    ///   ESTADO DE LA TIENDA - Si está abierta o no
    ///=========================================================///
    bool _la_tienda_esta_abierta; // true si el jugador abrió el menú de compra

    ///=========================================================///
    ///   FUENTE Y CARTEL - El texto que aparece al acercarse
    ///=========================================================///
    sf::Font _fuente_del_cartel_de_la_tienda; // El tipo de letra del cartel
    sf::Text _cartel_de_la_tienda;            // El texto que se dibuja cuando el jugador está cerca

public:

    ///=========================================================///
    ///   CONSTRUCTOR Y DESTRUCTOR
    ///=========================================================///
    Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa);
    ~Tienda();

    ///=========================================================///
    ///   CARGA - Se llama una sola vez al crear la tienda
    ///=========================================================///
    void cargar_fuente_y_cartel();
    void agregarItemEnVenta(Item* item_para_agregar); // Agrega un item a la lista de la tienda

    ///=========================================================///
    ///   MÉTODOS PRINCIPALES - Lo que hace la tienda cada frame
    ///=========================================================///
    void actualizar_tienda(sf::Vector2f posicion_actual_del_jugador);
    void dibujar_sprite_en_el_mapa(sf::RenderWindow& ventana_del_juego, bool mostrar_zona_de_deteccion);
    void dibujar_interfaz_de_compra(sf::RenderWindow& ventana_del_juego, sf::Font& fuente_del_hud);

    ///=========================================================///
    ///   ACCIONES DE NAVEGACIÓN - Controlan el menú de la tienda
    ///=========================================================///
    void abrir_tienda();
    void cerrar_tienda();
    void seleccionar_item_siguiente();
    void seleccionar_item_anterior();
    void aumentar_cantidad_a_comprar();
    void disminuir_cantidad_a_comprar();
    void intentar_comprar_item_seleccionado(Personaje& jugador);

    ///=========================================================///
    ///   GETTERS
    ///=========================================================///
    bool getJugadorEstaCercaDeLaTienda() const;
    bool getLaTiendaEstaAbierta() const;
};