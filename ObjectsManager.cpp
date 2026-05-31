#include "ObjectsManager.h"
#include "Personaje.h"
#include "InputManager.h" // 🌟 SOLUCIÓN AL TIPO INCOMPLETO: Acá sí va el include completo
#include <iostream>

ObjectsManager::~ObjectsManager() {
    for (auto* item : _itemsEnMundo) {
        delete item;
    }
    _itemsEnMundo.clear();
}

void ObjectsManager::agregarItemAlMundo(Item* nuevoItem, const sf::Texture& textura, float x, float y, sf::FloatRect hitboxCustom) {
    // La interfaz pública acepta const sf::Texture& para evitar const_cast en llamadores.
    // Internamente necesitamos un sf::Texture& para colocar el sprite en el item; usamos const_cast aquí
    // porque sabemos que ItemsManager mantiene la vida de la textura en memoria durante toda la ejecución.
    sf::Texture& texRef = const_cast<sf::Texture&>(textura);
    nuevoItem->colocarEnMundo(texRef, x, y, hitboxCustom);
    _itemsEnMundo.push_back(nuevoItem);
}

void ObjectsManager::dibujarItems(sf::RenderWindow& ventana) {
    for (auto* item : _itemsEnMundo) {
        item->dibujar(ventana);
    }
}

// 🌟 ACTUALIZADO: Ahora la función implementa los 2 argumentos
void ObjectsManager::chequearInteracciones(Personaje& jugador, const InputManager& input) {

    // Cambiamos el sf::Keyboard harcodeado por la abstracción del manager
    if (input.quiereInteractuar()) {

        // 🌟 SOLUCIÓN AL SIGNED/UNSIGNED: Convertimos el size() a int con static_cast
        int totalItems = static_cast<int>(_itemsEnMundo.size());

        for (int i = totalItems - 1; i >= 0; i--) {

            if (jugador.getBounds().intersects(_itemsEnMundo[i]->getBounds())) {

                Item* itemActual = _itemsEnMundo[i];

                // CASO A: Es un ítem del piso (Poción, Madera, Oro)
                if (itemActual->esAgarrable()) {
                    if (jugador.getInventario().agarrarItem(itemActual)) {
                        std::cout << "🎒 Guardaste en la mochila: " << itemActual->getNombre() << std::endl;
                        _itemsEnMundo.erase(_itemsEnMundo.begin() + i);
                    }
                }
                // CASO B: Es una estructura fija (Horno, Caldero)
                else {
                    itemActual->usar(jugador);
                }

                break; // Ya interactuamos este frame
            }
        }
    }
}


// Funcion para recibir un ítem que el jugador soltó del inventario al piso
void ObjectsManager::recibirItemSoltado(Item* itemSoltado) {
    if (itemSoltado != nullptr) {
        _itemsEnMundo.push_back(itemSoltado); // El ítem vuelve a ser parte del piso
    }
}