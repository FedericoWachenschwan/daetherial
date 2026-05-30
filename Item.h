#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Colisionable.h" 

// Le avisamos al compilador que la clase Personaje existe
class Personaje;

// Enumerador para saber qué categoría de objeto es
enum class TipoItem {
    Consumible,   // Pociones, comida
    Equipamiento, // Espadas, escudos, armaduras
    Recurso,      // Oro, madera, piedra
    Mueble        // Caldero, cofres (para construir después)
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
    Item(int id, std::string nombre, TipoItem tipo, int cantidad, int maxStack, bool esAgarrable);

    virtual ~Item() = default;

    virtual void usar(Personaje& jugador) = 0;

    void colocarEnMundo(sf::Texture& textura, float x, float y, sf::FloatRect hitboxCustom = sf::FloatRect());
    void dibujar(sf::RenderWindow& ventana);
    bool estaEnElMundo() const { return _estaEnElMundo; }
    bool esAgarrable() const { return _esAgarrable; }

    sf::FloatRect getBounds() const override { return _hitbox; }

    int getId() const { return _id; }
    std::string getNombre() const { return _nombre; }
    TipoItem getTipo() const { return _tipo; }
    int getCantidad() const { return _cantidad; }
    int getMaxStack() const { return _maxStack; }
    void setCantidad(int cantidad) { _cantidad = cantidad; }
	sf::Sprite getSprite() const { return _sprite; } // Devuelve el sprite para renderizarlo en el inventario UI

}; // 🌟 ACÁ TERMINA LA CLASE ITEM


// ========================================================
// 🌟 Las clases hijas AFUERA de Item
// ========================================================

// 1. LA CLASE CONSUMIBLE (Pociones, comida)
class Consumible : public Item {
private:
    float _curacion;
public:
    // 🌟 Corregido: Ahora se llama Consumible, no Pocion
    Consumible(int id, std::string nombre, float cura, int cantidad = 1);
    void usar(Personaje& jugador) override;
};

// 2. LA CLASE MUEBLE (Horno, Caldero)
class Mueble : public Item {
private:
    int _tipoMueble;
public:
    Mueble(int id, std::string nombre, int tipoMueble);
    void usar(Personaje& jugador) override;
};

// 3. LA CLASE RECURSO (Oro, madera, piedra)
class Recurso : public Item {
private:
    int _tipoRecurso;
public:
    Recurso(int id, std::string nombre, int tipoRecurso, int cantidad);
    void usar(Personaje& jugador) override;
}; // 🌟 Corregido: Se agregó el cierre de la clase Recurso


// 4. LA CLASE EQUIPAMIENTO (Espadas, escudos, armaduras)
class Equipamiento : public Item {
private:
    int _tipoEquipamiento;
    int _bonusAtaque;
    int _bonusDefensa;
public:
    // 🌟 Corregido: Se eliminó el paréntesis sobrante al final
    Equipamiento(int id, std::string nombre, int tipoEquipamiento, int bonusAtaque, int bonusDefensa);
    void usar(Personaje& jugador) override;
}; // 🌟 Corregido: Se agregó el cierre de la clase Equipamiento