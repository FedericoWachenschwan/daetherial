#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Personaje.h"
#include "InputManager.h"

///=================================================================///
///   ORO_EN_EL_PISO - Una monedita tirada en el mapa
///=================================================================///
struct OroEnElPiso {
    sf::Vector2f posicion_de_la_moneda; // Donde esta la moneda en el mundo
    int valor_de_la_moneda; // Cuanto oro vale al recogerla
    bool esta_ocupado_este_casillero = false; // Si es falso, este slot del array esta libre
};

///=================================================================///
///   ADMINISTRADOR_DE_OBJETOS - Maneja todos los items y el oro
///   que estan tirados en el mapa, usando arrays de tamaño fijo
///=================================================================///
class AdministradorDeObjetos {
private:

    static const int CANTIDAD_MAXIMA_DE_ITEMS_EN_EL_MUNDO = 50; // Maximo de items tirados en el suelo
    Item _items_en_el_mundo[CANTIDAD_MAXIMA_DE_ITEMS_EN_EL_MUNDO]; // Array de todos los items en el suelo
    int _cantidad_de_items_en_el_mundo = 0; // Cuantos items hay tirados ahora

    static const int CANTIDAD_MAXIMA_DE_MONEDAS_EN_EL_PISO = 50; // Maximo de monedas en el suelo
    OroEnElPiso _oro_en_el_piso[CANTIDAD_MAXIMA_DE_MONEDAS_EN_EL_PISO]; // Array de monedas tiradas
    int _cantidad_de_monedas_en_el_piso = 0; // Cuantas monedas hay ahora

    sf::CircleShape _circulo_de_la_moneda; // Figura reutilizable para dibujar las monedas

 ///=============================================================///
 ///   QUITAR UN ITEM DEL ARRAY Y CORRER LOS SIGUIENTES
 ///=============================================================///
 // #1
    void quitar_item_y_correr_los_siguientes(int indice_a_quitar); // Elimina un item y tapa el hueco
 // #2
    void quitar_moneda_y_correr_las_siguientes(int indice_a_quitar); // Elimina una moneda y tapa el hueco

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #3
    AdministradorDeObjetos();

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #4
    int getCantidad_de_items_en_el_mundo() const { return _cantidad_de_items_en_el_mundo; } // Cuantos items hay en el suelo
 // #5
    const Item& getItem_en_el_mundo(int indice) const { return _items_en_el_mundo[indice]; } // Devuelve un item del suelo por indice

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
 // #6
    void agregar_item_al_mundo(const Item& nuevo_item, float posicion_x, float posicion_y, sf::FloatRect hitbox_personalizada = sf::FloatRect());
 // #7
    void dibujar_items(sf::RenderWindow& ventana_del_juego) const;
 // #8
    void chequear_interacciones(Personaje& jugador, const InputManager& entrada_del_jugador);
 // #9
    void recibir_item_soltado(const Item& item_soltado);
 // #10
    void soltar_oro_en_el_piso(sf::Vector2f posicion, int valor); // Tira una moneda en el mapa
 // #11
    void dibujar_oro(sf::RenderWindow& ventana_del_juego); // Dibuja todas las monedas
 // #12
    void chequear_recoger_oro(Personaje& jugador, const InputManager& entrada_del_jugador); // Detecta si el jugador agarra oro
 // #13
    void limpiar() { _cantidad_de_items_en_el_mundo = 0; _cantidad_de_monedas_en_el_piso = 0; } // Borra todos los items y monedas del suelo
};
