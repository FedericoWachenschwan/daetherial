#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Colisionable.h"

// Le avisamos al compilador que Personaje existe sin incluir su archivo
class Personaje;

///=================================================///
///   TIPOS DE ITEM - Qué clase de objeto es
///=================================================///
enum class TipoItem {
    Desconocido = 0,
    Consumible = 1, // Pociones - cura al jugador al usarse
    Equipamiento = 2, // Espadas, escudos - sube stats del jugador
    Recurso = 3, // Materiales para craftear
    Mueble = 4  // Objetos del entorno, no se pueden agarrar
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
///   Esta estructura tiene que mantenerse igual para que el archivo funcione
///=================================================///
struct ItemReg {
    int id;           // ID único del item
    int tipoItem;     // Tipo del item (mapea con TipoItem)
    char nombre[30];  // Nombre del item (tamaño fijo para el archivo binario)
    int valorEfecto;  // Cuánto cura si es poción, cuánto ataque si es espada
    int precio;       // Cuánto oro cuesta en la tienda
    int rareza;       // Rareza del item (mapea con RarezaItem)
    int idTextura;    // Posición del sprite en el spritesheet
    bool activo;      // Si está activo en la base de datos
};

///=================================================///
///   CLASE ITEM - Representa cualquier objeto del juego
///=================================================///
class Item : public Colisionable {
private:

    ///=========================================================///
    ///   IDENTIDAD - Qué es y cómo se llama el item
    ///=========================================================///
    int _id_del_item;                        // Número único que identifica este tipo de item
    std::string _nombre_del_item;            // Cómo se llama el item
    TipoItem _tipo_del_item;                 // Si es poción, espada, recurso, etc.
    int _precio_del_item;                    // Cuánto oro cuesta comprarlo en la tienda

    ///=========================================================///
    ///   CANTIDAD - Cuántos hay apilados en este slot
    ///=========================================================///
    int _cantidad_del_item;                  // Cuántos hay actualmente en este slot
    int _cantidad_maxima_en_el_stack;        // Cuántos pueden apilarse como máximo

    ///=========================================================///
    ///   EFECTOS - Qué le hace al jugador cuando se usa
    ///=========================================================///
    int _puntos_de_curacion_del_item;        // Cuánta vida recupera (solo si es poción)
    int _bonus_de_ataque_del_item;           // Cuánto ataque suma (solo si es espada)
    int _bonus_de_defensa_del_item;          // Cuánta defensa suma (solo si es escudo)

    ///=========================================================///
    ///   ESTADO EN EL MAPA - Si está tirado en el suelo
    ///=========================================================///
    sf::Sprite _sprite_del_item;             // El dibujito del item en el mapa
    sf::FloatRect _hitbox_del_item;          // El rectángulo de colisión del item
    bool _el_item_esta_tirado_en_el_mapa;    // true si está en el suelo del mapa
    bool _el_jugador_puede_agarrar_el_item;  // true si el jugador puede recogerlo

public:

    ///=========================================================///
    ///   CONSTRUCTOR Y DESTRUCTOR
    ///=========================================================///
    Item(int id, const std::string& nombre, TipoItem tipo, int precio, int cantidad, int cantidad_maxima, bool es_agarrable, int puntos_de_curacion = 0, int bonus_de_ataque = 0, int bonus_de_defensa = 0);
    virtual ~Item() = default;

    ///=========================================================///
    ///   ACCIONES - Lo que puede hacer el item
    ///=========================================================///
    void usar(Personaje& jugador);                                      // Usa el item según su tipo
    void colocarEnMundo(float x, float y, sf::FloatRect hitbox_custom = sf::FloatRect()); // Pone el item en el mapa
    void setPosicion(sf::Vector2f nueva_posicion_del_item);             // Mueve el item a una posición
    void dibujar(sf::RenderWindow& ventana_del_juego) const;            // Dibuja el item si está en el mapa

    ///=========================================================///
    ///   GETTERS - Para leer los datos del item desde afuera
    ///=========================================================///
    int getId() const;
    const std::string& getNombre() const;
    TipoItem getTipo() const;
    int getPrecio() const;
    int getCantidad() const;
    int getMaxStack() const;
    int getPuntosDeCluracion() const;
    int getBonusDeAtaque() const;
    int getBonusDeDefensa() const;
    bool estaEnElMundo() const;
    bool esAgarrable() const;
    sf::FloatRect getBounds() const override;
    sf::Sprite& getSprite();

    ///=========================================================///
    ///   SETTERS - Para cambiar los datos del item desde afuera
    ///=========================================================///
    void setCantidad(int nueva_cantidad_del_item);
    void setPrecio(int nuevo_precio_del_item);
};