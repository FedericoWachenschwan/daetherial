#include "Item.h"
#include "Personaje.h"
#include <iostream>

///=============================================================///
///   CONSTRUCTOR - Crea el item con todos sus datos
///=============================================================///
Item::Item(int id, const std::string& nombre, TipoItem tipo, int precio, int cantidad, int cantidad_maxima, bool es_agarrable, int puntos_de_curacion, int bonus_de_ataque, int bonus_de_defensa, int puntos_de_mana) {

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

    if (_tipo_del_item == TipoItem::Consumible || _tipo_del_item == TipoItem::PocionMana) {
        _hitbox_del_item.width = 16.f;
        _hitbox_del_item.height = 16.f;
    }
    else if (_tipo_del_item == TipoItem::Equipamiento) {
        _hitbox_del_item.width = 24.f;
        _hitbox_del_item.height = 24.f;
    }
    else {
        _hitbox_del_item.width = 32.f;
        _hitbox_del_item.height = 32.f;
    }
}

///=============================================================///
///   USAR - Aplica el efecto del item al jugador según su tipo
///=============================================================///
void Item::usar(Personaje& jugador) {

    if (_tipo_del_item == TipoItem::Consumible) {
        int vida_nueva = jugador.getVida() + _puntos_de_curacion_del_item; // Calculamos la vida nueva
        if (vida_nueva > jugador.getVidaMaxima()) {
            vida_nueva = jugador.getVidaMaxima(); // No puede pasar del máximo
        }
        jugador.setVida(vida_nueva); // Le aplicamos la vida nueva al jugador
        std::cout << "USASTE " << _nombre_del_item << ". TE CURASTE " << _puntos_de_curacion_del_item << " DE VIDA." << std::endl;
    }
    else if (_tipo_del_item == TipoItem::PocionMana) {
        int mana_nuevo = jugador.getMana() + _puntos_de_mana_del_item; // Calculamos el maná nuevo
        if (mana_nuevo > jugador.getManaMaXima()) {
            mana_nuevo = jugador.getManaMaXima(); // No puede pasar del máximo
        }
        jugador.setMana(mana_nuevo); // Le aplicamos el maná nuevo al jugador
        std::cout << "USASTE " << _nombre_del_item << ". RECUPERASTE " << _puntos_de_mana_del_item << " DE MANA." << std::endl;
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
    _sprite_del_item.setPosition(x, y);
    _hitbox_del_item.left = x;
    _hitbox_del_item.top = y;
    _el_item_esta_tirado_en_el_mapa = true;
    if (hitbox_custom.width != 0) {
        _hitbox_del_item.width = hitbox_custom.width;
        _hitbox_del_item.height = hitbox_custom.height;
    }
}

///=============================================================///
///   SET POSICION - Mueve el item a una nueva posición
///=============================================================///
void Item::setPosicion(sf::Vector2f nueva_posicion_del_item) {
    _sprite_del_item.setPosition(nueva_posicion_del_item);
    _hitbox_del_item.left = nueva_posicion_del_item.x;
    _hitbox_del_item.top = nueva_posicion_del_item.y;
    _el_item_esta_tirado_en_el_mapa = true;
}

///=============================================================///
///   DIBUJAR - Dibuja el item solo si está tirado en el mapa
///=============================================================///
void Item::dibujar(sf::RenderWindow& ventana_del_juego) const {
    if (_el_item_esta_tirado_en_el_mapa == true) {
        ventana_del_juego.draw(_sprite_del_item);
    }
}

///=============================================================///
///   CREAR COPIA - Devuelve un item nuevo con los mismos datos
///=============================================================///
Item* Item::crearCopia() {
    Item* copia = new Item(
        _id_del_item,
        _nombre_del_item,
        _tipo_del_item,
        _precio_del_item,
        1,
        _cantidad_maxima_en_el_stack,
        _el_jugador_puede_agarrar_el_item,
        _puntos_de_curacion_del_item,
        _bonus_de_ataque_del_item,
        _bonus_de_defensa_del_item,
        _puntos_de_mana_del_item
    );

    const sf::Texture* textura_original = _sprite_del_item.getTexture();
    if (textura_original != nullptr) {
        copia->_sprite_del_item.setTexture(*textura_original);
        copia->_sprite_del_item.setTextureRect(_sprite_del_item.getTextureRect());
    }

    return copia;
}

///=============================================================///
///   GETTERS
///=============================================================///
int Item::getId()                      const { return _id_del_item; }
const std::string& Item::getNombre()   const { return _nombre_del_item; }
TipoItem Item::getTipo()               const { return _tipo_del_item; }
int Item::getPrecio()                  const { return _precio_del_item; }
int Item::getCantidad()                const { return _cantidad_del_item; }
int Item::getMaxStack()                const { return _cantidad_maxima_en_el_stack; }
int Item::getPuntosDeCluracion()       const { return _puntos_de_curacion_del_item; }
int Item::getPuntosDeMana()            const { return _puntos_de_mana_del_item; }
int Item::getBonusDeAtaque()           const { return _bonus_de_ataque_del_item; }
int Item::getBonusDeDefensa()          const { return _bonus_de_defensa_del_item; }
bool Item::estaEnElMundo()             const { return _el_item_esta_tirado_en_el_mapa; }
bool Item::esAgarrable()               const { return _el_jugador_puede_agarrar_el_item; }
sf::FloatRect Item::getBounds()        const { return _hitbox_del_item; }
sf::Sprite& Item::getSprite() { return _sprite_del_item; }

///=============================================================///
///   SETTERS
///=============================================================///
void Item::setCantidad(int nueva_cantidad) { _cantidad_del_item = nueva_cantidad; }
void Item::setPrecio(int nuevo_precio) { _precio_del_item = nuevo_precio; }