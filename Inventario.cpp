#include "Inventario.h"
#include "Personaje.h"
#include <algorithm> // Para std::min
#include <iostream>

// ==========================================
// CONSTRUCTOR
// ==========================================
Inventario::Inventario(int capacidad) : _capacidadMaxima(capacidad) {}

// ==========================================
// DESTRUCTOR
// ==========================================
Inventario::~Inventario() {
    // 🧹 Limpieza al cerrar el juego
    for (int i = 0; i < _itemsGuardados.size(); i++) {
        delete _itemsGuardados[i];
    }
    _itemsGuardados.clear();
}

// ==========================================
// AGARRAR ÍTEM
// ==========================================
bool Inventario::agarrarItem(Item* nuevoItem) {
    // 1. Intentar acumular
    for (auto* itemActual : _itemsGuardados) {
        if (itemActual->getId() == nuevoItem->getId() && itemActual->getCantidad() < itemActual->getMaxStack()) {
            int espacioLibre = itemActual->getMaxStack() - itemActual->getCantidad();
            int cantidadAAgregar = std::min(espacioLibre, nuevoItem->getCantidad());

            itemActual->setCantidad(itemActual->getCantidad() + cantidadAAgregar);
            nuevoItem->setCantidad(nuevoItem->getCantidad() - cantidadAAgregar);

            if (nuevoItem->getCantidad() <= 0) {
                delete nuevoItem; // 🧹 PREVENCIÓN DE LEAK: Borramos el "envase" vacío que sobró
                return true;
            }
        }
    }

    // 2. Buscar slot nuevo
    if (static_cast<int>(_itemsGuardados.size()) < _capacidadMaxima) {
        _itemsGuardados.push_back(nuevoItem);
        return true;
    }

    std::cout << "🎒 ¡Inventario lleno! No se pudo agarrar: " << nuevoItem->getNombre() << std::endl;
    return false;
}

// ==========================================
// TIRAR ÍTEM
// ==========================================
bool Inventario::tirarItem(int idItem, int cantidad) {
    if (getCantidadTotal(idItem) < cantidad) {
        std::cout << "⚠️ No tenés suficiente cantidad para tirar." << std::endl;
        return false;
    }

    for (int i = static_cast<int>(_itemsGuardados.size()) - 1; i >= 0; i--) {
        Item* item = _itemsGuardados[i];
        if (item->getId() == idItem) {

            if (item->getCantidad() > cantidad) {
                item->setCantidad(item->getCantidad() - cantidad);
                cantidad = 0;
            }
            else {
                cantidad -= item->getCantidad();
                delete item; // 🧹 PREVENCIÓN DE LEAK: Destruimos la memoria física primero
                _itemsGuardados.erase(_itemsGuardados.begin() + i);
            }

            if (cantidad <= 0) {
                return true;
            }
        }
    }
    return true;
}

// ==========================================
// GET CANTIDAD TOTAL
// ==========================================
int Inventario::getCantidadTotal(int idItem) const {
    int total = 0;
    for (const auto& item : _itemsGuardados) {
        if (item->getId() == idItem) {
            total += item->getCantidad();
        }
    }
    return total;
}

// ==========================================
// VACIAR MOCHILA
// ==========================================
void Inventario::vaciar() {
    // 🧹 PREVENCIÓN DE LEAK: Primero borramos la memoria física
    for (int i = 0; i < _itemsGuardados.size(); i++) {
        delete _itemsGuardados[i];
    }
    _itemsGuardados.clear(); // Después vaciamos la lista
}

// ==========================================
// USAR ÍTEM
// ==========================================
void Inventario::usarItem(int indice, Personaje& jugador) {
    // 1. Validación de seguridad usando el nombre correcto (_itemsGuardados)
    if (indice < 0 || indice >= _itemsGuardados.size()) {
        std::cout << "❌ Slot vacío o inválido." << std::endl;
        return;
    }

    Item* itemElegido = _itemsGuardados[indice];

    // 2. Ejecutamos el comportamiento
    itemElegido->usar(jugador);

    // 3. Regla de desgaste para consumibles
    if (itemElegido->getTipo() == TipoItem::Consumible) {
        itemElegido->setCantidad(itemElegido->getCantidad() - 1);

        // 4. Limpieza de memoria
        if (itemElegido->getCantidad() <= 0) {
            delete itemElegido;
            _itemsGuardados.erase(_itemsGuardados.begin() + indice);
            std::cout << "💧 Consumible agotado. Memoria liberada." << std::endl;
        }
    }
}