#include "ObjectsManager.h"
#include "Personaje.h"
#include <iostream>

ObjectsManager::~ObjectsManager() {
    for (auto* item : _itemsEnMundo) {
        delete item;
    }
    _itemsEnMundo.clear();
}

void ObjectsManager::agregarItemAlMundo(Item* nuevoItem, sf::Texture& textura, float x, float y, sf::FloatRect hitboxCustom) {
    nuevoItem->colocarEnMundo(textura, x, y, hitboxCustom);
    _itemsEnMundo.push_back(nuevoItem);
}

void ObjectsManager::dibujarItems(sf::RenderWindow& ventana) {
    for (auto* item : _itemsEnMundo) {
        item->dibujar(ventana);
    }
}

void ObjectsManager::chequearInteracciones(Personaje& jugador) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        for (int i = _itemsEnMundo.size() - 1; i >= 0; i--) {

            if (jugador.getBounds().intersects(_itemsEnMundo[i]->getBounds())) {

                Item* itemActual = _itemsEnMundo[i];

                // CASO A: Es un ítem del piso (Poción, Madera, Oro)
                if (itemActual->esAgarrable()) {
                    // Intentamos meterlo en la mochila del personaje
                    if (jugador.getInventario().agarrarItem(itemActual)) {
                        std::cout << "🎒 Guardaste en la mochila: " << itemActual->getNombre() << std::endl;
                        _itemsEnMundo.erase(_itemsEnMundo.begin() + i); // Desaparece del mapa
                    }
                }
                // CASO B: Es una estructura fija (Horno, Caldero)
                else {
                    // No se guarda en la mochila, solo se activa su menú o función
                    itemActual->usar(jugador);
                }

                break; // Ya interactuamos con el objeto de este frame
            }
        }
    }
}