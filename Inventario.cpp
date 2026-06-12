#include "Inventario.h"
#include "Personaje.h"
#include <algorithm>
#include <iostream>

Inventario::Inventario(int capacidad) : _capacidadMaxima(capacidad), _indiceSeleccionado(-1) {}

Inventario::~Inventario() {
    for (int i = 0; i < static_cast<int>(_itemsGuardados.size()); i++) {
        delete _itemsGuardados[i];
    }
    _itemsGuardados.clear();
}

bool Inventario::agarrarItem(Item* nuevoItem) {
    // 1. Intentar acumular en un slot existente del mismo item
    for (auto* itemActual : _itemsGuardados) {
        if (itemActual->getId() == nuevoItem->getId() && itemActual->getCantidad() < itemActual->getMaxStack()) {
            int espacioLibre = itemActual->getMaxStack() - itemActual->getCantidad();
            int cantidadAAgregar = std::min(espacioLibre, nuevoItem->getCantidad());

            itemActual->setCantidad(itemActual->getCantidad() + cantidadAAgregar);
            nuevoItem->setCantidad(nuevoItem->getCantidad() - cantidadAAgregar);

            if (nuevoItem->getCantidad() <= 0) {
                delete nuevoItem;
                return true;
            }
        }
    }

    // 2. Si no se pudo acumular, buscar un slot vacío
    if (static_cast<int>(_itemsGuardados.size()) < _capacidadMaxima) {
        _itemsGuardados.push_back(nuevoItem);
        return true;
    }

    std::cout << "INVENTARIO LLENO. NO SE PUDO AGARRAR: " << nuevoItem->getNombre() << std::endl;
    return false;
}

bool Inventario::tirarItem(int idItem, int cantidad) {
    if (getCantidadTotal(idItem) < cantidad) {
        std::cout << "NO TENES SUFICIENTE CANTIDAD PARA TIRAR." << std::endl;
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
                delete item;
                _itemsGuardados.erase(_itemsGuardados.begin() + i);
            }
            if (cantidad <= 0) return true;
        }
    }
    return true;
}

int Inventario::getCantidadTotal(int idItem) const {
    int total = 0;
    for (const auto& item : _itemsGuardados) {
        if (item->getId() == idItem) total += item->getCantidad();
    }
    return total;
}

void Inventario::vaciar() {
    for (int i = 0; i < static_cast<int>(_itemsGuardados.size()); i++) {
        delete _itemsGuardados[i];
    }
    _itemsGuardados.clear();
}

///=============================================================///
///   USAR ITEM - Usa el item del slot indicado
///=============================================================///
void Inventario::usarItem(int indice, Personaje& jugador) {

    if (indice < 0 || indice >= static_cast<int>(_itemsGuardados.size())) {
        std::cout << "SLOT VACIO O INVALIDO." << std::endl;
        return;
    }

    Item* itemElegido = _itemsGuardados[indice]; // El item que eligió el jugador

    itemElegido->usar(jugador); // Ejecutamos el efecto del item sobre el jugador

    ///=========================================================///
    ///   DESGASTE - Los consumibles y pociones se gastan al usarse
    ///=========================================================///
    bool es_pocion_de_vida = itemElegido->getTipo() == TipoItem::Consumible;
    bool es_pocion_de_mana = itemElegido->getTipo() == TipoItem::PocionMana;

    if (es_pocion_de_vida == true || es_pocion_de_mana == true) {
        itemElegido->setCantidad(itemElegido->getCantidad() - 1); // Gastamos una unidad

        if (itemElegido->getCantidad() <= 0) {
            delete itemElegido;                                         // Liberamos la memoria
            _itemsGuardados.erase(_itemsGuardados.begin() + indice);   // Lo sacamos del inventario
            std::cout << "POCION AGOTADA." << std::endl;
        }
    }
}

TipoItem Inventario::getItemTipo(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(_itemsGuardados.size()) || _itemsGuardados[indice] == nullptr) {
        return TipoItem::Desconocido;
    }
    return _itemsGuardados[indice]->getTipo();
}

Item* Inventario::extraerItemPorIndice() {
    if (_indiceSeleccionado >= 0 && _indiceSeleccionado < static_cast<int>(_itemsGuardados.size())) {
        Item* itemATirar = _itemsGuardados[_indiceSeleccionado];
        if (itemATirar != nullptr) {
            _itemsGuardados.erase(_itemsGuardados.begin() + _indiceSeleccionado);
            _indiceSeleccionado = -1;
            return itemATirar;
        }
    }
    return nullptr;
}