#include "ObjectsManager.h"
#include "Personaje.h"
#include "InputManager.h"
#include <iostream>

ObjectsManager::~ObjectsManager() {
    for (auto* item : _itemsEnMundo) {
        delete item;
    }
    _itemsEnMundo.clear();
}

void ObjectsManager::agregarItemAlMundo(Item* nuevoItem, float x, float y, sf::FloatRect hitboxCustom) {
	if (nuevoItem != nullptr) {
        nuevoItem->colocarEnMundo(x, y, hitboxCustom); // Le decimos al item que actualice sus coordenadas espaciales
		_itemsEnMundo.push_back(nuevoItem);
	}
}

void ObjectsManager::dibujarItems(sf::RenderWindow& ventana) const {
    for (auto* item : _itemsEnMundo) {
        item->dibujar(ventana);
    }
}

void ObjectsManager::chequearInteracciones(Personaje& jugador, const InputManager& input) {
    // Comprobamos si el jugador quiere interactuar (por ejemplo, presionando una tecla)
    if (input.quiereInteractuar()) {

        // Convertimos el tamaño del vector a int para evitar problemas de signo
        int totalItems = static_cast<int>(_itemsEnMundo.size());

        for (int i = totalItems - 1; i >= 0; i--) {
            // Comprobamos si la caja del jugador intersecta con la caja del ítem
            if (jugador.getBounds().intersects(_itemsEnMundo[i]->getBounds())) {

                Item* itemActual = _itemsEnMundo[i];

                // CASO A: Es un ítem agarrable que el jugador puede llevar en su inventario
                if (itemActual->esAgarrable()) {
                    // Intentamos agregar el ítem al inventario del jugador
                    if (jugador.getInventario().agarrarItem(itemActual)) {
                        std::cout << "🎒 Guardaste en la mochila: " << itemActual->getNombre() << std::endl;
                        _itemsEnMundo.erase(_itemsEnMundo.begin() + i); // Eliminamos el ítem del mundo si se agrega al inventario
                    }
                }
                // CASO B: Es una estructura fija que puede ser utilizada por el jugador
                else {
                    itemActual->usar(jugador);
                }

                break; // Rompemos el bucle ya que solo queremos interactuar con un ítem a la vez
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