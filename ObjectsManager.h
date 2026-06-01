#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Item.h"

// Declaraciones anticipadas para romper dependencias circulares
class Personaje;
class InputManager; // 🌟 Avisamos que vamos a usar el InputManager

class ObjectsManager {
private:
    std::vector<Item*> _itemsEnMundo;

public:
    ~ObjectsManager();

    void agregarItemAlMundo(Item* nuevoItem, const sf::Texture& textura, float x, float y, sf::FloatRect hitboxCustom = sf::FloatRect());
    void dibujarItems(sf::RenderWindow& ventana) const;

    // 🌟 ACTUALIZADO: Ahora la firma acepta la referencia al InputManager
    void chequearInteracciones(Personaje& jugador, const InputManager& input);

    void recibirItemSoltado(Item* itemSoltado);

    const std::vector<Item*>& getItemsEnMundo() const { return _itemsEnMundo; }
};