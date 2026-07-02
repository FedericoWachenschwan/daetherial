#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// CASO ESPECIAL (ver regla #32): no podemos poner "#include "Personaje.h""
// aca porque se forma un ciclo (Item -> Personaje -> Inventario -> Item)
// y no compila. Por eso, solo en este caso puntual, usamos la
// declaracion adelantada en vez del include normal
class Personaje;

///=================================================================///
///   TIPO DE ITEM - Que clase de objeto es
///=================================================================///
enum class TipoDeItem {
    DESCONOCIDO = 0, // Item sin tipo definido (slots vacios del inventario)
    CONSUMIBLE_DE_VIDA = 1, // Pociones de vida
    CONSUMIBLE_DE_MANA = 5 // Pociones de mana
};

///=================================================================///
///   ITEM - Representa cualquier objeto del juego. Es un dato
///   SIMPLE: se copia como cualquier numero, sin punteros ni "new"
///=================================================================///
class Item {
private:

 ///=============================================================///
 ///   IDENTIDAD
 ///=============================================================///
    int _id_del_item; // Numero unico que identifica este tipo de item
    std::string _nombre_del_item; // Nombre que se muestra al jugador
    TipoDeItem _tipo_del_item; // Que clase de objeto es
    int _precio_del_item; // Cuanto cuesta en la tienda

 ///=============================================================///
 ///   CANTIDAD
 ///=============================================================///
    int _cantidad_del_item; // Cuantos hay apilados en este slot
    int _cantidad_maxima_en_el_stack; // Maximo que puede haber en un stack

 ///=============================================================///
 ///   EFECTOS AL USARSE
 ///=============================================================///
    int _puntos_de_curacion_del_item; // Vida que restaura al usarse
    int _puntos_de_mana_del_item; // Mana que restaura al usarse

 ///=============================================================///
 ///   ESTADO EN EL MAPA
 ///=============================================================///
    sf::Sprite _sprite_del_item; // Icono del item en el mundo o en el inventario
    sf::FloatRect _hitbox_del_item; // Zona de colision para agarrarlo del piso
    bool _el_item_esta_tirado_en_el_mapa; // Si es verdadero, se dibuja en el mundo
    bool _el_jugador_puede_agarrar_el_item; // Si es falso, solo se puede usar al tocarlo

public:

 ///=============================================================///
 ///   CONSTRUCTORES
 ///=============================================================///

 // Representa "ningun item", para los casilleros vacios de los arrays
    Item();

    Item(int id, const std::string& nombre, TipoDeItem tipo, int precio, int cantidad, int cantidad_maxima, bool es_agarrable, int puntos_de_curacion = 0, int puntos_de_mana = 0);

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
    int getId() const { return _id_del_item; } // Devuelve el ID del item
    const std::string& getNombre() const { return _nombre_del_item; } // Devuelve el nombre del item
    TipoDeItem getTipo() const { return _tipo_del_item; } // Devuelve el tipo de item
    int getPrecio() const { return _precio_del_item; } // Devuelve cuanto cuesta
    int getCantidad() const { return _cantidad_del_item; } // Devuelve cuantos hay apilados
    int getCantidad_maxima_en_el_stack() const { return _cantidad_maxima_en_el_stack; } // Devuelve el maximo del stack
    int getPuntos_de_curacion() const { return _puntos_de_curacion_del_item; } // Devuelve cuanto cura
    int getPuntos_de_mana() const { return _puntos_de_mana_del_item; } // Devuelve cuanto mana da
    bool getEsta_vacio() const { return _id_del_item == -1; } // Verdadero si es un casillero vacio
    bool getEsta_tirado_en_el_mapa() const { return _el_item_esta_tirado_en_el_mapa; } // Verdadero si esta en el suelo
    bool getEs_agarrable() const { return _el_jugador_puede_agarrar_el_item; } // Verdadero si se puede recoger
    sf::FloatRect getCaja_de_colision() const { return _hitbox_del_item; } // Devuelve la zona de colision
    sf::Sprite& getSprite() { return _sprite_del_item; } // Devuelve el sprite para modificarlo
    const sf::Sprite& getSprite() const { return _sprite_del_item; } // Devuelve el sprite para leerlo

 ///=============================================================///
 ///   SETTERS
 ///=============================================================///
    void setCantidad(int nueva_cantidad) { _cantidad_del_item = nueva_cantidad; } // Cambia la cantidad del stack
    void setPrecio(int nuevo_precio) { _precio_del_item = nuevo_precio; } // Cambia el precio del item

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
    void usar(Personaje& jugador);
    void colocar_en_el_mundo(float posicion_x, float posicion_y, sf::FloatRect hitbox_personalizada = sf::FloatRect());

 // Mueve el item ya tirado a una posicion nueva. Hace mas de una
 // cosa a la vez (mueve el sprite, la hitbox, y marca que esta
 // tirado), por eso no es un simple "set" de un solo atributo
    void reposicionar_en_el_mundo(sf::Vector2f posicion_nueva);

    void dibujar(sf::RenderWindow& ventana_del_juego) const; // Lo dibuja solo si esta en el mundo
};
