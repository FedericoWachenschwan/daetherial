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
    DESCONOCIDO = 0,
    CONSUMIBLE_DE_VIDA = 1,
    EQUIPAMIENTO = 2,
    RECURSO = 3,
    MUEBLE = 4,
    CONSUMIBLE_DE_MANA = 5
};

///=================================================================///
///   RAREZA DEL ITEM
///=================================================================///
enum class RarezaDelItem {
    COMUN = 0,
    RARO = 1,
    EPICO = 2,
    LEGENDARIO = 3
};

///=================================================================///
///   REGISTRO_DE_ITEM - Lo que se guarda y se lee del archivo .dat
///=================================================================///
struct RegistroDeItem {
    int id;
    int tipo_de_item;
    char nombre[30];
    int valor_del_efecto;
    int precio;
    int rareza;
    int id_de_la_textura;
    bool esta_activo;
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
    int _id_del_item;
    std::string _nombre_del_item;
    TipoDeItem _tipo_del_item;
    int _precio_del_item;

    ///=============================================================///
    ///   CANTIDAD
    ///=============================================================///
    int _cantidad_del_item;
    int _cantidad_maxima_en_el_stack;

    ///=============================================================///
    ///   EFECTOS AL USARSE
    ///=============================================================///
    int _puntos_de_curacion_del_item;
    int _puntos_de_mana_del_item;
    int _bonus_de_ataque_del_item;
    int _bonus_de_defensa_del_item;

    ///=============================================================///
    ///   ESTADO EN EL MAPA
    ///=============================================================///
    sf::Sprite _sprite_del_item;
    sf::FloatRect _hitbox_del_item;
    bool _el_item_esta_tirado_en_el_mapa;
    bool _el_jugador_puede_agarrar_el_item;

public:

    ///=============================================================///
    ///   CONSTRUCTORES
    ///=============================================================///

    // Representa "ningun item", para los casilleros vacios de los arrays
    Item();

    Item(int id, const std::string& nombre, TipoDeItem tipo, int precio, int cantidad, int cantidad_maxima, bool es_agarrable, int puntos_de_curacion = 0, int bonus_de_ataque = 0, int bonus_de_defensa = 0, int puntos_de_mana = 0);

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    int getId() const { return _id_del_item; }
    const std::string& getNombre() const { return _nombre_del_item; }
    TipoDeItem getTipo() const { return _tipo_del_item; }
    int getPrecio() const { return _precio_del_item; }
    int getCantidad() const { return _cantidad_del_item; }
    int getCantidad_maxima_en_el_stack() const { return _cantidad_maxima_en_el_stack; }
    int getPuntos_de_curacion() const { return _puntos_de_curacion_del_item; }
    int getPuntos_de_mana() const { return _puntos_de_mana_del_item; }
    bool getEsta_vacio() const { return _id_del_item == -1; }
    bool getEsta_tirado_en_el_mapa() const { return _el_item_esta_tirado_en_el_mapa; }
    bool getEs_agarrable() const { return _el_jugador_puede_agarrar_el_item; }
    sf::FloatRect getCaja_de_colision() const { return _hitbox_del_item; }
    sf::Sprite& getSprite() { return _sprite_del_item; }
    const sf::Sprite& getSprite() const { return _sprite_del_item; }

    ///=============================================================///
    ///   SETTERS
    ///=============================================================///
    void setCantidad(int nueva_cantidad) { _cantidad_del_item = nueva_cantidad; }
    void setPrecio(int nuevo_precio) { _precio_del_item = nuevo_precio; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    void usar(Personaje& jugador);
    void colocar_en_el_mundo(float posicion_x, float posicion_y, sf::FloatRect hitbox_personalizada = sf::FloatRect());

    // Mueve el item ya tirado a una posicion nueva. Hace mas de una
    // cosa a la vez (mueve el sprite, la hitbox, y marca que esta
    // tirado), por eso no es un simple "set" de un solo atributo
    void reposicionar_en_el_mundo(sf::Vector2f posicion_nueva);

    void dibujar(sf::RenderWindow& ventana_del_juego) const;
};