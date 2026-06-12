#include "Item.h"
#include "Personaje.h"
#include <iostream>

///=============================================================///
///   CONSTRUCTOR - Crea el item con todos sus datos
///=============================================================///
Item::Item(int id, const std::string& nombre, TipoItem tipo, int precio, int cantidad, int cantidad_maxima, bool es_agarrable, int puntos_de_curacion, int bonus_de_ataque, int bonus_de_defensa) {

    _id_del_item = id;              // Guardamos el ID del item
    _nombre_del_item = nombre;          // Guardamos el nombre del item
    _tipo_del_item = tipo;            // Guardamos el tipo del item
    _precio_del_item = precio;          // Guardamos el precio del item
    _cantidad_del_item = cantidad;        // Guardamos cuántos hay
    _cantidad_maxima_en_el_stack = cantidad_maxima; // Guardamos cuántos pueden apilarse
    _el_jugador_puede_agarrar_el_item = es_agarrable;   // Guardamos si se puede agarrar
    _puntos_de_curacion_del_item = puntos_de_curacion; // Guardamos cuánto cura
    _bonus_de_ataque_del_item = bonus_de_ataque;    // Guardamos cuánto ataque da
    _bonus_de_defensa_del_item = bonus_de_defensa;   // Guardamos cuánta defensa da
    _el_item_esta_tirado_en_el_mapa = false;           // Al crearse no está en el mapa todavía

    ///=========================================================///
    ///   TAMAÑO DE LA HITBOX - Según el tipo de item
    ///=========================================================///
    if (_tipo_del_item == TipoItem::Consumible) {
        _hitbox_del_item.width = 16.f; // Las pociones son pequeñas
        _hitbox_del_item.height = 16.f;
    }
    else if (_tipo_del_item == TipoItem::Equipamiento) {
        _hitbox_del_item.width = 24.f; // Las armas son medianas
        _hitbox_del_item.height = 24.f;
    }
    else {
        _hitbox_del_item.width = 32.f; // El resto es grande
        _hitbox_del_item.height = 32.f;
    }
}

///=============================================================///
///   USAR - Aplica el efecto del item al jugador según su tipo
///=============================================================///
void Item::usar(Personaje& jugador) {

    if (_tipo_del_item == TipoItem::Consumible) {
        std::cout << "USASTE " << _nombre_del_item << ". TE CURASTE " << _puntos_de_curacion_del_item << " DE VIDA." << std::endl;
    }
    else if (_tipo_del_item == TipoItem::Equipamiento) {
        std::cout << "EQUIPASTE " << _nombre_del_item << ". +ATAQUE: " << _bonus_de_ataque_del_item << " +DEFENSA: " << _bonus_de_defensa_del_item << std::endl;
    }
    else {
        std::cout << "NO PODES USAR " << _nombre_del_item << " DIRECTAMENTE." << std::endl;
    }
}

///=============================================================///
///   COLOCAR EN EL MUNDO - Pone el item tirado en el mapa
///=============================================================///
void Item::colocarEnMundo(float x, float y, sf::FloatRect hitbox_custom) {

    _sprite_del_item.setPosition(x, y);  // Ponemos el sprite en la posición indicada
    _hitbox_del_item.left = x;           // Actualizamos la hitbox en X
    _hitbox_del_item.top = y;           // Actualizamos la hitbox en Y
    _el_item_esta_tirado_en_el_mapa = true; // Marcamos que está en el mapa

    if (hitbox_custom.width != 0) {            // Si nos pasaron una hitbox personalizada
        _hitbox_del_item.width = hitbox_custom.width;  // Usamos su ancho
        _hitbox_del_item.height = hitbox_custom.height; // Usamos su alto
    }
}

///=============================================================///
///   SET POSICION - Mueve el item a una nueva posición
///=============================================================///
void Item::setPosicion(sf::Vector2f nueva_posicion_del_item) {

    _sprite_del_item.setPosition(nueva_posicion_del_item);  // Movemos el sprite
    _hitbox_del_item.left = nueva_posicion_del_item.x;      // Actualizamos la hitbox en X
    _hitbox_del_item.top = nueva_posicion_del_item.y;      // Actualizamos la hitbox en Y
    _el_item_esta_tirado_en_el_mapa = true;                  // Marcamos que está en el mapa
}

///=============================================================///
///   DIBUJAR - Dibuja el item solo si está tirado en el mapa
///=============================================================///
void Item::dibujar(sf::RenderWindow& ventana_del_juego) const {

    if (_el_item_esta_tirado_en_el_mapa == true) {
        ventana_del_juego.draw(_sprite_del_item); // Solo dibujamos si está en el mapa
    }
}

///=============================================================///
///   GETTERS - Devuelven los datos del item
///=============================================================///
int Item::getId()                       const { return _id_del_item; }
const std::string& Item::getNombre()   const { return _nombre_del_item; }
TipoItem Item::getTipo()               const { return _tipo_del_item; }
int Item::getPrecio()                  const { return _precio_del_item; }
int Item::getCantidad()                const { return _cantidad_del_item; }
int Item::getMaxStack()                const { return _cantidad_maxima_en_el_stack; }
int Item::getPuntosDeCluracion()       const { return _puntos_de_curacion_del_item; }
int Item::getBonusDeAtaque()           const { return _bonus_de_ataque_del_item; }
int Item::getBonusDeDefensa()          const { return _bonus_de_defensa_del_item; }
bool Item::estaEnElMundo()             const { return _el_item_esta_tirado_en_el_mapa; }
bool Item::esAgarrable()               const { return _el_jugador_puede_agarrar_el_item; }
sf::FloatRect Item::getBounds()        const { return _hitbox_del_item; }
sf::Sprite& Item::getSprite() { return _sprite_del_item; }

///=============================================================///
///   SETTERS - Cambian los datos del item
///=============================================================///
void Item::setCantidad(int nueva_cantidad_del_item) { _cantidad_del_item = nueva_cantidad_del_item; }
void Item::setPrecio(int nuevo_precio_del_item) { _precio_del_item = nuevo_precio_del_item; }