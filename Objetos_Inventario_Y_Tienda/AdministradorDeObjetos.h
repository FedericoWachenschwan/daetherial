#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Personaje.h"
#include "InputManager.h"

///=================================================================///
///   ORO_EN_EL_PISO - Una monedita tirada en el mapa
///=================================================================///
struct OroEnElPiso {
    sf::Vector2f posicion_de_la_moneda;
    int valor_de_la_moneda;
    bool esta_ocupado_este_casillero = false;
};

///=================================================================///
///   ADMINISTRADOR_DE_OBJETOS - Maneja todos los items y el oro
///   que estan tirados en el mapa, usando arrays de tamaño fijo
///=================================================================///
class AdministradorDeObjetos {
private:

    static const int CANTIDAD_MAXIMA_DE_ITEMS_EN_EL_MUNDO = 50;
    Item _items_en_el_mundo[CANTIDAD_MAXIMA_DE_ITEMS_EN_EL_MUNDO];
    int _cantidad_de_items_en_el_mundo = 0;

    static const int CANTIDAD_MAXIMA_DE_MONEDAS_EN_EL_PISO = 50;
    OroEnElPiso _oro_en_el_piso[CANTIDAD_MAXIMA_DE_MONEDAS_EN_EL_PISO];
    int _cantidad_de_monedas_en_el_piso = 0;

    sf::CircleShape _circulo_de_la_moneda;

    ///=============================================================///
    ///   QUITAR UN ITEM DEL ARRAY Y CORRER LOS SIGUIENTES
    ///=============================================================///
    void quitar_item_y_correr_los_siguientes(int indice_a_quitar);
    void quitar_moneda_y_correr_las_siguientes(int indice_a_quitar);

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    AdministradorDeObjetos();

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    int getCantidad_de_items_en_el_mundo() const { return _cantidad_de_items_en_el_mundo; }
    const Item& getItem_en_el_mundo(int indice) const { return _items_en_el_mundo[indice]; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    void agregar_item_al_mundo(const Item& nuevo_item, float posicion_x, float posicion_y, sf::FloatRect hitbox_personalizada = sf::FloatRect());
    void dibujar_items(sf::RenderWindow& ventana_del_juego) const;
    void chequear_interacciones(Personaje& jugador, const InputManager& entrada_del_jugador);
    void recibir_item_soltado(const Item& item_soltado);

    void soltar_oro_en_el_piso(sf::Vector2f posicion, int valor);
    void dibujar_oro(sf::RenderWindow& ventana_del_juego);
    void chequear_recoger_oro(Personaje& jugador, const InputManager& entrada_del_jugador);
};