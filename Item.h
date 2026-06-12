#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Colisionable.h"

class Personaje;

///=================================================///
///   TIPOS DE ITEM - Qué clase de objeto es
///=================================================///
enum class TipoItem {
    Desconocido = 0,
    Consumible = 1, // Pociones de vida - cura al jugador al usarse
    Equipamiento = 2, // Espadas, escudos - sube stats del jugador
    Recurso = 3, // Materiales para craftear
    Mueble = 4, // Objetos del entorno, no se pueden agarrar
    PocionMana = 5  // Pociones de maná - restaura maná al jugador al usarse
};

///=================================================///
///   RAREZA DEL ITEM - Qué tan difícil es conseguirlo
///=================================================///
enum class RarezaItem {
    Comun = 0,
    Raro = 1,
    Epico = 2,
    Legendario = 3
};

///=================================================///
///   ITEMREG - El registro que se guarda en el archivo .dat
///=================================================///
struct ItemReg {
    int id;
    int tipoItem;
    char nombre[30];
    int valorEfecto;
    int precio;
    int rareza;
    int idTextura;
    bool activo;
};

///=================================================///
///   CLASE ITEM - Representa cualquier objeto del juego
///=================================================///
class Item : public Colisionable {
private:

    ///=========================================================///
    ///   IDENTIDAD - Qué es y cómo se llama el item
    ///=========================================================///
    int _id_del_item;
    std::string _nombre_del_item;
    TipoItem _tipo_del_item;
    int _precio_del_item;

    ///=========================================================///
    ///   CANTIDAD - Cuántos hay apilados en este slot
    ///=========================================================///
    int _cantidad_del_item;
    int _cantidad_maxima_en_el_stack;

    ///=========================================================///
    ///   EFECTOS - Qué le hace al jugador cuando se usa
    ///=========================================================///
    int _puntos_de_curacion_del_item;  // Cuánta vida recupera (solo si es poción de vida)
    int _puntos_de_mana_del_item;      // Cuánto maná recupera (solo si es poción de maná)
    int _bonus_de_ataque_del_item;     // Cuánto ataque suma (solo si es espada)
    int _bonus_de_defensa_del_item;    // Cuánta defensa suma (solo si es escudo)

    ///=========================================================///
    ///   ESTADO EN EL MAPA - Si está tirado en el suelo
    ///=========================================================///
    sf::Sprite _sprite_del_item;
    sf::FloatRect _hitbox_del_item;
    bool _el_item_esta_tirado_en_el_mapa;
    bool _el_jugador_puede_agarrar_el_item;

public:

    ///=========================================================///
    ///   CONSTRUCTOR Y DESTRUCTOR
    ///=========================================================///
    Item(int id, const std::string& nombre, TipoItem tipo, int precio, int cantidad, int cantidad_maxima, bool es_agarrable, int puntos_de_curacion = 0, int bonus_de_ataque = 0, int bonus_de_defensa = 0, int puntos_de_mana = 0);
    virtual ~Item() = default;

    ///=========================================================///
    ///   ACCIONES - Lo que puede hacer el item
    ///=========================================================///
    void usar(Personaje& jugador);
    void colocarEnMundo(float x, float y, sf::FloatRect hitbox_custom = sf::FloatRect());
    void setPosicion(sf::Vector2f nueva_posicion_del_item);
    void dibujar(sf::RenderWindow& ventana_del_juego) const;
    Item* crearCopia();

    ///=========================================================///
    ///   GETTERS
    ///=========================================================///
    int getId() const;
    const std::string& getNombre() const;
    TipoItem getTipo() const;
    int getPrecio() const;
    int getCantidad() const;
    int getMaxStack() const;
    int getPuntosDeCluracion() const;
    int getPuntosDeMana() const;
    int getBonusDeAtaque() const;
    int getBonusDeDefensa() const;
    bool estaEnElMundo() const;
    bool esAgarrable() const;
    sf::FloatRect getBounds() const override;
    sf::Sprite& getSprite();

    ///=========================================================///
    ///   SETTERS
    ///=========================================================///
    void setCantidad(int nueva_cantidad_del_item);
    void setPrecio(int nuevo_precio_del_item);
};