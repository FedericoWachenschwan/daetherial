#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Item.h"

// 🌟 ¡El ObjectsManager es el encargado de manejar todos los ítems que están en el mundo!

	// Como todo es un item, el manager solo tiene que tener una lista de punteros a Item, sin importar si son oro, pociones, o un horno gigante.
    // Para el, todo se dibuja y todo colisiona de la misma manera.


class ObjectsManager {
private:
    std::vector<Item*> _itemsEnMundo; // 🌟 ¡Lista unificada de ítems físicos!

public:
    ~ObjectsManager();

    void agregarItemAlMundo(Item* nuevoItem, sf::Texture& textura, float x, float y, sf::FloatRect hitboxCustom = sf::FloatRect());
    void dibujarItems(sf::RenderWindow& ventana);
    void chequearInteracciones(class Personaje& jugador);

    const std::vector<Item*>& getItemsEnMundo() const { return _itemsEnMundo; }
};