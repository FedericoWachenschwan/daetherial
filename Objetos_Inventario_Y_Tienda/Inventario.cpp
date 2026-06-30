#include "Inventario.h"
#include "Personaje.h"
#include <iostream>

///=============================================================///
///   QUITAR SLOT Y CORRER LOS SIGUIENTES
///=============================================================///
void Inventario::quitar_slot_y_correr_los_siguientes(int indice_a_quitar) {
    for (int i = indice_a_quitar; i < _cantidad_de_items_guardados - 1; i++) {
        _items_guardados[i] = _items_guardados[i + 1];
    }
    _cantidad_de_items_guardados--;
}

///=============================================================///
///   AGARRAR ITEM
///=============================================================///
bool Inventario::agarrar_item(Item nuevo_item) {

    for (int i = 0; i < _cantidad_de_items_guardados; i++) {
        bool es_el_mismo_tipo_de_item = _items_guardados[i].getId() == nuevo_item.getId();
        bool todavia_le_entra_mas = _items_guardados[i].getCantidad() < _items_guardados[i].getCantidad_maxima_en_el_stack();

        if (es_el_mismo_tipo_de_item == true && todavia_le_entra_mas == true) {
            int espacio_libre_en_el_stack = _items_guardados[i].getCantidad_maxima_en_el_stack() - _items_guardados[i].getCantidad();
            int cantidad_a_agregar = espacio_libre_en_el_stack;
            if (nuevo_item.getCantidad() < cantidad_a_agregar) {
                cantidad_a_agregar = nuevo_item.getCantidad();
            }

            _items_guardados[i].setCantidad(_items_guardados[i].getCantidad() + cantidad_a_agregar);
            nuevo_item.setCantidad(nuevo_item.getCantidad() - cantidad_a_agregar);

            if (nuevo_item.getCantidad() <= 0) {
                return true;
            }
        }
    }

    if (_cantidad_de_items_guardados < CAPACIDAD_MAXIMA_DE_LA_MOCHILA) {
        _items_guardados[_cantidad_de_items_guardados] = nuevo_item;
        _cantidad_de_items_guardados++;
        return true;
    }

    std::cout << "INVENTARIO LLENO. NO SE PUDO AGARRAR: " << nuevo_item.getNombre() << std::endl;
    return false;
}


///=============================================================///
///   CALCULAR CANTIDAD TOTAL DE UN ITEM
///=============================================================///
int Inventario::calcular_cantidad_total_de_un_item(int id_del_item) const {
    int total = 0;
    for (int i = 0; i < _cantidad_de_items_guardados; i++) {
        if (_items_guardados[i].getId() == id_del_item) {
            total += _items_guardados[i].getCantidad();
        }
    }
    return total;
}


///=============================================================///
///   USAR ITEM
///=============================================================///
void Inventario::usar_item(int indice_del_slot, Personaje& jugador) {

    if (indice_del_slot < 0 || indice_del_slot >= _cantidad_de_items_guardados) {
        std::cout << "SLOT VACIO O INVALIDO." << std::endl;
        return;
    }

    _items_guardados[indice_del_slot].usar(jugador);

    TipoDeItem tipo_del_item_usado = _items_guardados[indice_del_slot].getTipo();
    bool es_pocion_de_vida = tipo_del_item_usado == TipoDeItem::CONSUMIBLE_DE_VIDA;
    bool es_pocion_de_mana = tipo_del_item_usado == TipoDeItem::CONSUMIBLE_DE_MANA;

    if (es_pocion_de_vida == true || es_pocion_de_mana == true) {
        int cantidad_que_queda = _items_guardados[indice_del_slot].getCantidad() - 1;
        _items_guardados[indice_del_slot].setCantidad(cantidad_que_queda);

        if (cantidad_que_queda <= 0) {
            quitar_slot_y_correr_los_siguientes(indice_del_slot);
            std::cout << "POCION AGOTADA." << std::endl;
        }
    }
}


///=============================================================///
///   EXTRAER ITEM SELECCIONADO
///=============================================================///
Item Inventario::extraer_item_seleccionado() {
    if (_indice_del_slot_seleccionado >= 0 && _indice_del_slot_seleccionado < _cantidad_de_items_guardados) {
        Item copia_del_item_extraido = _items_guardados[_indice_del_slot_seleccionado];
        quitar_slot_y_correr_los_siguientes(_indice_del_slot_seleccionado);
        _indice_del_slot_seleccionado = -1;
        return copia_del_item_extraido;
    }
    return Item();
}