#include "Item.h"
#include "Personaje.h"
#include <iostream>

///=============================================================///
///   CONSTRUCTOR VACIO
///=============================================================///
Item::Item() {
    _id_del_item = -1;
    _nombre_del_item = "";
    _tipo_del_item = TipoDeItem::DESCONOCIDO;
    _precio_del_item = 0;
    _cantidad_del_item = 0;
    _cantidad_maxima_en_el_stack = 0;
    _puntos_de_curacion_del_item = 0;
    _puntos_de_mana_del_item = 0;
    _bonus_de_ataque_del_item = 0;
    _bonus_de_defensa_del_item = 0;
    _el_item_esta_tirado_en_el_mapa = false;
    _el_jugador_puede_agarrar_el_item = false;
}

///=============================================================///
///   CONSTRUCTOR CON DATOS
///=============================================================///
Item::Item(int id, const std::string& nombre, TipoDeItem tipo, int precio, int cantidad, int cantidad_maxima, bool es_agarrable, int puntos_de_curacion, int bonus_de_ataque, int bonus_de_defensa, int puntos_de_mana) {

    _id_del_item = id;
    _nombre_del_item = nombre;
    _tipo_del_item = tipo;
    _precio_del_item = precio;
    _cantidad_del_item = cantidad;
    _cantidad_maxima_en_el_stack = cantidad_maxima;
    _el_jugador_puede_agarrar_el_item = es_agarrable;
    _puntos_de_curacion_del_item = puntos_de_curacion;
    _puntos_de_mana_del_item = puntos_de_mana;
    _bonus_de_ataque_del_item = bonus_de_ataque;
    _bonus_de_defensa_del_item = bonus_de_defensa;
    _el_item_esta_tirado_en_el_mapa = false;

    if (_tipo_del_item == TipoDeItem::CONSUMIBLE_DE_VIDA || _tipo_del_item == TipoDeItem::CONSUMIBLE_DE_MANA) {
        _hitbox_del_item.width = 16.f;
        _hitbox_del_item.height = 16.f;
    }
    else if (_tipo_del_item == TipoDeItem::EQUIPAMIENTO) {
        _hitbox_del_item.width = 24.f;
        _hitbox_del_item.height = 24.f;
    }
    else {
        _hitbox_del_item.width = 32.f;
        _hitbox_del_item.height = 32.f;
    }
}

///=============================================================///
///   USAR
///=============================================================///
void Item::usar(Personaje& jugador) {

    if (_tipo_del_item == TipoDeItem::CONSUMIBLE_DE_VIDA) {
        jugador.curar(_puntos_de_curacion_del_item);
        std::cout << "USASTE " << _nombre_del_item << ". TE CURASTE " << _puntos_de_curacion_del_item << " DE VIDA." << std::endl;
    }
    else if (_tipo_del_item == TipoDeItem::CONSUMIBLE_DE_MANA) {
        jugador.restaurar_mana(_puntos_de_mana_del_item);
        std::cout << "USASTE " << _nombre_del_item << ". RECUPERASTE " << _puntos_de_mana_del_item << " DE MANA." << std::endl;
    }
    else if (_tipo_del_item == TipoDeItem::EQUIPAMIENTO) {
        std::cout << "EQUIPASTE " << _nombre_del_item << ". +ATAQUE: " << _bonus_de_ataque_del_item << " +DEFENSA: " << _bonus_de_defensa_del_item << std::endl;
    }
    else {
        std::cout << "NO PODES USAR " << _nombre_del_item << " DIRECTAMENTE." << std::endl;
    }
}

///=============================================================///
///   COLOCAR EN EL MUNDO
///=============================================================///
void Item::colocar_en_el_mundo(float posicion_x, float posicion_y, sf::FloatRect hitbox_personalizada) {
    _sprite_del_item.setPosition(posicion_x, posicion_y);
    _hitbox_del_item.left = posicion_x;
    _hitbox_del_item.top = posicion_y;
    _el_item_esta_tirado_en_el_mapa = true;

    if (hitbox_personalizada.width != 0.f) {
        _hitbox_del_item.width = hitbox_personalizada.width;
        _hitbox_del_item.height = hitbox_personalizada.height;
    }
}

///=============================================================///
///   REPOSICIONAR EN EL MUNDO
///=============================================================///
void Item::reposicionar_en_el_mundo(sf::Vector2f posicion_nueva) {
    _sprite_del_item.setPosition(posicion_nueva);
    _hitbox_del_item.left = posicion_nueva.x;
    _hitbox_del_item.top = posicion_nueva.y;
    _el_item_esta_tirado_en_el_mapa = true;
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void Item::dibujar(sf::RenderWindow& ventana_del_juego) const {
    if (_el_item_esta_tirado_en_el_mapa == true) {
        ventana_del_juego.draw(_sprite_del_item);
    }
}