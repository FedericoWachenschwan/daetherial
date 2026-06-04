#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Colisionable.h" 

// Le avisamos al compilador que la clase Personaje existe
class Personaje;

#pragma once

enum class TipoItem {
    Desconocido = 0,
    Consumible = 1,
    Equipamiento = 2,
    Recurso = 3,
    Mueble = 4
};

enum class RarezaItem {
    Comun = 0,
    Raro = 1,
    Epico = 2,
    Legendario = 3
};

// 🌟 El registro plano que viajará directo al archivo .dat
struct ItemReg {
	int id;             // ID único para cada tipo de item (1, 2, 3...)
    int tipoItem;       // Mapea con TipoItem (int)
    char nombre[30];    // Strings fijos para evitar punteros rotos en el archivo
    int valorEfecto;    // Cuánto cura, cuánto daño suma, etc.
	int precio;         // Para la tienda (futura)
    int rareza;         // Mapea con RarezaItem (int)
    int idTextura;      // El índice de la grilla del Spritesheet (0, 1, 2, 3...)
    bool activo;        // Para la baja lógica del ABML
};


// ========================================================
// 1. LA CLASE BASE (El molde principal)
// ========================================================
class Item : public Colisionable {
protected:
    int _id;
    std::string _nombre;
    TipoItem _tipo;
    int _cantidad;
    int _maxStack;

    // Datos físicos para cuando vive en el mapa
    sf::Sprite _sprite;
    sf::FloatRect _hitbox;
    bool _estaEnElMundo = false;
    bool _esAgarrable = false;

public:
    Item(int id, const std::string& nombre, TipoItem tipo, int cantidad, int maxStack, bool esAgarrable);

    virtual ~Item() = default;

    virtual void usar(Personaje& jugador) = 0;

    void colocarEnMundo(float x, float y, sf::FloatRect hitboxCustom = sf::FloatRect());
    void setPosicion(sf::Vector2f nuevaPosicion);
    void dibujar(sf::RenderWindow& ventana) const;
    bool estaEnElMundo() const { return _estaEnElMundo; }
    bool esAgarrable() const { return _esAgarrable; }

    sf::FloatRect getBounds() const override { return _hitbox; }

    int getId() const { return _id; }
    const std::string& getNombre() const { return _nombre; }
    TipoItem getTipo() const { return _tipo; }
    int getCantidad() const { return _cantidad; }
    int getMaxStack() const { return _maxStack; }
    void setCantidad(int cantidad) { _cantidad = cantidad; }
	sf::Sprite& getSprite() { return _sprite; } // Devuelve el sprite para renderizarlo en el inventario UI


};


// ========================================================
// 🌟 Las clases hijas AFUERA de Item
// ========================================================

// 1. LA CLASE CONSUMIBLE (Pociones, comida)
class Consumible : public Item {
private:
    float _curacion;
public:
    // 🌟 Corregido: Ahora se llama Consumible, no Pocion
    Consumible(int id, const std::string& nombre, float cura, int cantidad = 1);
    void usar(Personaje& jugador) override;
};

// 2. LA CLASE MUEBLE (Horno, Caldero)
class Mueble : public Item {
private:
    int _tipoMueble;
public:
    Mueble(int id, const std::string& nombre, int tipoMueble);
    void usar(Personaje& jugador) override;
};

// 3. LA CLASE RECURSO (Oro, madera, piedra)
class Recurso : public Item {
private:
    int _tipoRecurso;
public:
    Recurso(int id, const std::string& nombre, int tipoRecurso, int cantidad);
    void usar(Personaje& jugador) override;
};


// 4. LA CLASE EQUIPAMIENTO (Espadas, escudos, armaduras)
class Equipamiento : public Item {
private:
    int _tipoEquipamiento;
    int _bonusAtaque;
    int _bonusDefensa;
public:
    Equipamiento(int id, const std::string& nombre, int tipoEquipamiento, int bonusAtaque, int bonusDefensa);
    void usar(Personaje& jugador) override;
};