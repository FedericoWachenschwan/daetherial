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
    sf::Texture _textura_de_la_tienda; // Imagen del edificio de la tienda
    sf::Sprite _sprite_de_la_tienda; // Lo que se dibuja en el mapa

 ///=============================================================///
 ///   ZONA DE DETECCION
 ///=============================================================///
    sf::FloatRect _zona_de_interaccion_de_la_tienda; // Rectangulo invisible donde se puede abrir la tienda
    bool _el_jugador_esta_cerca_de_la_tienda; // Verdadero si el jugador esta dentro de la zona

 ///=============================================================///
 ///   ITEMS EN VENTA
 ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA = 3; // Cuantos items puede vender la tienda
    Item _items_en_venta[CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA]; // Array de items disponibles para comprar
    int _cantidad_de_items_cargados_en_la_tienda; // Cuantos items hay actualmente en venta
    int _indice_del_item_seleccionado_en_la_tienda; // Cual item esta resaltado en la interfaz
    int _cantidad_que_quiere_comprar_el_jugador; // Cuantas unidades quiere comprar

 ///=============================================================///
 ///   ESTADO DE LA TIENDA
 ///=============================================================///
    bool _la_tienda_esta_abierta; // Verdadero cuando el panel de compra esta visible

 ///=============================================================///
 ///   FUENTE Y CARTEL
 ///=============================================================///
    sf::Font _fuente_del_cartel_de_la_tienda; // Tipografia decorativa del cartel de aproximacion
    sf::Font _fuente_del_panel_de_la_tienda;  // Tipografia legible para el panel de compra
    sf::Text _cartel_de_la_tienda; // Texto que aparece cuando el jugador se acerca

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa);

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #2
    bool getEl_jugador_esta_cerca_de_la_tienda() const { return _el_jugador_esta_cerca_de_la_tienda; } // Verdadero si se puede abrir
 // #3
    bool getLa_tienda_esta_abierta() const { return _la_tienda_esta_abierta; } // Verdadero si el panel esta visible

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
 // #4
    void cargar_fuente_y_cartel(); // Carga la tipografia y configura el cartel
 // #5
    void agregar_item_en_venta(const Item& item_para_agregar); // Agrega un item al catalogo de la tienda
 // #6
    void actualizar_tienda(sf::Vector2f posicion_actual_del_jugador); // Chequea si el jugador esta en la zona
 // #7
    void dibujar_sprite_en_el_mapa(sf::RenderWindow& ventana_del_juego, bool mostrar_zona_de_deteccion); // Dibuja el edificio
 // #8
    void dibujar_interfaz_de_compra(sf::RenderWindow& ventana_del_juego, sf::Font& fuente_del_hud); // Dibuja el panel de compra
 // #9
    void abrir_tienda(); // Muestra el panel de compra
 // #10
    void cerrar_tienda(); // Oculta el panel de compra
 // #11
    void seleccionar_item_siguiente(); // Mueve el cursor al item de la derecha
 // #12
    void seleccionar_item_anterior(); // Mueve el cursor al item de la izquierda
 // #13
    void aumentar_cantidad_a_comprar(); // Suma una unidad a la cantidad
 // #14
    void disminuir_cantidad_a_comprar(); // Resta una unidad a la cantidad
 // #15
    void intentar_comprar_item_seleccionado(Personaje& jugador); // Cobra el oro y da el item
};
