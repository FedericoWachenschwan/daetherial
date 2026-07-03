#include "Item.h"
#include "Personaje.h"
#include <iostream>

///=============================================================///
///   #1 - CONSTRUCTOR VACIO
///=============================================================///
// #1
Item::Item() {
    _id_del_item = -1; // -1 indica que este slot esta vacio
    _nombre_del_item = ""; // Sin nombre
    _tipo_del_item = TipoDeItem::DESCONOCIDO; // Sin tipo definido
    _precio_del_item = 0; // Sin precio
    _cantidad_del_item = 0; // Sin cantidad
    _cantidad_maxima_en_el_stack = 0; // Sin limite de stack
    _puntos_de_curacion_del_item = 0; // No cura nada
    _puntos_de_mana_del_item = 0; // No da mana
    _el_item_esta_tirado_en_el_mapa = false; // No esta en el mundo
    _el_jugador_puede_agarrar_el_item = false; // No se puede agarrar
}

///=============================================================///
///   #2 - CONSTRUCTOR CON DATOS
///=============================================================///
// #2
Item::Item(int id, const std::string& nombre, TipoDeItem tipo, int precio, int cantidad, int cantidad_maxima, bool es_agarrable, int puntos_de_curacion, int puntos_de_mana) {

    _id_del_item = id; // Guarda el identificador unico
    _nombre_del_item = nombre; // Guarda el nombre visible
    _tipo_del_item = tipo; // Guarda que clase de objeto es
    _precio_del_item = precio; // Guarda cuanto cuesta
    _cantidad_del_item = cantidad; // Guarda cuantos hay en el stack
    _cantidad_maxima_en_el_stack = cantidad_maxima; // Guarda el tope del stack
    _el_jugador_puede_agarrar_el_item = es_agarrable; // Guarda si se puede recoger
    _puntos_de_curacion_del_item = puntos_de_curacion; // Guarda cuanto cura
    _puntos_de_mana_del_item = puntos_de_mana; // Guarda cuanto mana da
    _el_item_esta_tirado_en_el_mapa = false; // Empieza sin estar en el mundo

    if (_tipo_del_item == TipoDeItem::GEMA_ARCANA) {
        if (_textura_del_item.loadFromFile("assets/gema_arcana.png")) {
            float escala = 32.f / _textura_del_item.getSize().x; // Ajusta al tamaño de un tile
            _sprite_del_item.setTexture(_textura_del_item);
            _sprite_del_item.setScale(escala, escala);
        }
    }

    if (_tipo_del_item == TipoDeItem::BACULO_ARCANO) {
        if (_textura_del_item.loadFromFile("assets/baculo_arcano.png")) {
            float escala = 96.f / _textura_del_item.getSize().x; // triple de grande
            _sprite_del_item.setTexture(_textura_del_item);
            _sprite_del_item.setScale(escala, escala);
        }
    }

    if (_tipo_del_item == TipoDeItem::CONSUMIBLE_DE_VIDA) {
        if (_textura_del_item.loadFromFile("assets/pocion_vida.png")) {
            float escala = 40.f / _textura_del_item.getSize().x; // 25% mas grande que un tile (32 * 1.25 = 40)
            _sprite_del_item.setTexture(_textura_del_item);
            _sprite_del_item.setScale(escala, escala);
        }
    }

    if (_tipo_del_item == TipoDeItem::CONSUMIBLE_DE_MANA) {
        if (_textura_del_item.loadFromFile("assets/pocion_mana.png")) {
            float escala = 40.f / _textura_del_item.getSize().x; // 25% mas grande que un tile
            _sprite_del_item.setTexture(_textura_del_item);
            _sprite_del_item.setScale(escala, escala);
        }
    }

    if (_tipo_del_item == TipoDeItem::CONSUMIBLE_DE_VIDA || _tipo_del_item == TipoDeItem::CONSUMIBLE_DE_MANA) {
        _hitbox_del_item.width = 16.f; // Las pociones son chicas
        _hitbox_del_item.height = 16.f;
    }
    else {
        _hitbox_del_item.width = 32.f; // El resto ocupa un tile completo
        _hitbox_del_item.height = 32.f;
    }
}

///=============================================================///
///   #3 - CONSTRUCTOR DE COPIA
///=============================================================///
// #3
Item::Item(const Item& otro) {
    _id_del_item                    = otro._id_del_item;
    _nombre_del_item                = otro._nombre_del_item;
    _tipo_del_item                  = otro._tipo_del_item;
    _precio_del_item                = otro._precio_del_item;
    _cantidad_del_item              = otro._cantidad_del_item;
    _cantidad_maxima_en_el_stack    = otro._cantidad_maxima_en_el_stack;
    _puntos_de_curacion_del_item    = otro._puntos_de_curacion_del_item;
    _puntos_de_mana_del_item        = otro._puntos_de_mana_del_item;
    _el_item_esta_tirado_en_el_mapa = otro._el_item_esta_tirado_en_el_mapa;
    _el_jugador_puede_agarrar_el_item = otro._el_jugador_puede_agarrar_el_item;
    _hitbox_del_item                = otro._hitbox_del_item;
    _textura_del_item               = otro._textura_del_item; // copia la textura a nuestra propia memoria
    _sprite_del_item                = otro._sprite_del_item;
    if (otro._sprite_del_item.getTexture() != nullptr) {
        _sprite_del_item.setTexture(_textura_del_item); // apunta al nuestro, no al del original
    }
}

///=============================================================///
///   #4 - OPERADOR DE ASIGNACION
///=============================================================///
// #4
Item& Item::operator=(const Item& otro) {
    if (this == &otro) return *this; // evita auto-asignacion
    _id_del_item                    = otro._id_del_item;
    _nombre_del_item                = otro._nombre_del_item;
    _tipo_del_item                  = otro._tipo_del_item;
    _precio_del_item                = otro._precio_del_item;
    _cantidad_del_item              = otro._cantidad_del_item;
    _cantidad_maxima_en_el_stack    = otro._cantidad_maxima_en_el_stack;
    _puntos_de_curacion_del_item    = otro._puntos_de_curacion_del_item;
    _puntos_de_mana_del_item        = otro._puntos_de_mana_del_item;
    _el_item_esta_tirado_en_el_mapa = otro._el_item_esta_tirado_en_el_mapa;
    _el_jugador_puede_agarrar_el_item = otro._el_jugador_puede_agarrar_el_item;
    _hitbox_del_item                = otro._hitbox_del_item;
    _textura_del_item               = otro._textura_del_item;
    _sprite_del_item                = otro._sprite_del_item;
    if (otro._sprite_del_item.getTexture() != nullptr) {
        _sprite_del_item.setTexture(_textura_del_item);
    }
    return *this;
}

///=============================================================///
///   #5 - USAR
///=============================================================///
// #5
void Item::usar(Personaje& jugador) {

    if (_tipo_del_item == TipoDeItem::CONSUMIBLE_DE_VIDA) {
        jugador.curar(_puntos_de_curacion_del_item); // Restaura vida al personaje
        std::cout << "USASTE " << _nombre_del_item << ". TE CURASTE " << _puntos_de_curacion_del_item << " DE VIDA." << std::endl;
    }
    else if (_tipo_del_item == TipoDeItem::CONSUMIBLE_DE_MANA) {
        jugador.restaurar_mana(_puntos_de_mana_del_item); // Restaura mana al personaje
        std::cout << "USASTE " << _nombre_del_item << ". RECUPERASTE " << _puntos_de_mana_del_item << " DE MANA." << std::endl;
    }
    else if (_tipo_del_item == TipoDeItem::BACULO_ARCANO) {
        jugador.setDano(500); // Equipa el baculo: potencia el poder magico del personaje
        std::cout << "EQUIPASTE EL BACULO ARCANO. TU DAÑO AHORA ES 500." << std::endl;
    }
}

///=============================================================///
///   #6 - COLOCAR EN EL MUNDO
///=============================================================///
// #6
void Item::colocar_en_el_mundo(float posicion_x, float posicion_y, sf::FloatRect hitbox_personalizada) {
    _sprite_del_item.setPosition(posicion_x, posicion_y); // Coloca el icono en esa posicion del mapa
    _hitbox_del_item.left = posicion_x; // Alinea la hitbox con el sprite
    _hitbox_del_item.top = posicion_y; // Alinea la hitbox con el sprite
    _el_item_esta_tirado_en_el_mapa = true; // Ahora el item es visible en el mundo

    if (hitbox_personalizada.width != 0.f) {
        _hitbox_del_item.width = hitbox_personalizada.width; // Usa el tamanio de hitbox proporcionado
        _hitbox_del_item.height = hitbox_personalizada.height; // Usa el tamanio de hitbox proporcionado
    }
}

///=============================================================///
///   #7 - REPOSICIONAR EN EL MUNDO / TIRAR ITEM
///=============================================================///
// #7
void Item::reposicionar_en_el_mundo(sf::Vector2f posicion_nueva) {
    _sprite_del_item.setPosition(posicion_nueva); // Mueve el icono a la nueva posicion
    _hitbox_del_item.left = posicion_nueva.x; // Actualiza la hitbox en X
    _hitbox_del_item.top = posicion_nueva.y; // Actualiza la hitbox en Y
    _el_item_esta_tirado_en_el_mapa = true; // El item queda tirado en el suelo
}

///=============================================================///
///   #8 - DIBUJAR
///=============================================================///
// #8
void Item::dibujar(sf::RenderWindow& ventana_del_juego) const {
    if (_el_item_esta_tirado_en_el_mapa == true) {
        ventana_del_juego.draw(_sprite_del_item); // Solo se dibuja si esta tirado en el mundo
    }
}
