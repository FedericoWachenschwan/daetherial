#include "Inventario.h"
#include "Personaje.h"
#include <iostream>

///=============================================================///
///   #1 - QUITAR SLOT Y CORRER LOS SIGUIENTES
///=============================================================///
// #1
void Inventario::quitar_slot_y_correr_los_siguientes(int indice_a_quitar) {
    for (int i = indice_a_quitar; i < _cantidad_de_items_guardados - 1; i++) {
        _items_guardados[i] = _items_guardados[i + 1]; // Mueve cada item un lugar hacia adelante
    }
    _cantidad_de_items_guardados--; // La mochila tiene un item menos
}

///=============================================================///
///   #2 - AGARRAR ITEM
///=============================================================///
// #2
bool Inventario::agarrar_item(Item nuevo_item) {

    for (int i = 0; i < _cantidad_de_items_guardados; i++) {
        bool es_el_mismo_tipo_de_item = _items_guardados[i].getId() == nuevo_item.getId(); // Compara los IDs
        bool todavia_le_entra_mas = _items_guardados[i].getCantidad() < _items_guardados[i].getCantidad_maxima_en_el_stack(); // Hay espacio en el stack

        if (es_el_mismo_tipo_de_item == true && todavia_le_entra_mas == true) {
            int espacio_libre_en_el_stack = _items_guardados[i].getCantidad_maxima_en_el_stack() - _items_guardados[i].getCantidad(); // Cuanto espacio queda
            int cantidad_a_agregar = espacio_libre_en_el_stack; // Intenta agregar todo
            if (nuevo_item.getCantidad() < cantidad_a_agregar) {
                cantidad_a_agregar = nuevo_item.getCantidad(); // Si hay menos del espacio libre, agrega solo lo que hay
            }

            _items_guardados[i].setCantidad(_items_guardados[i].getCantidad() + cantidad_a_agregar); // Aumenta la cantidad del stack existente
            nuevo_item.setCantidad(nuevo_item.getCantidad() - cantidad_a_agregar); // Reduce la cantidad que falta guardar

            if (nuevo_item.getCantidad() <= 0) {
                return true; // Se guardo todo, termina con exito
            }
        }
    }

    if (_cantidad_de_items_guardados < CAPACIDAD_MAXIMA_DE_LA_MOCHILA) {
        _items_guardados[_cantidad_de_items_guardados] = nuevo_item; // Lo guarda en un slot nuevo
        _cantidad_de_items_guardados++; // Hay un item mas en la mochila
        return true; // Se guardo con exito
    }

    std::cout << "INVENTARIO LLENO. NO SE PUDO AGARRAR: " << nuevo_item.getNombre() << std::endl;
    return false; // No habia lugar en la mochila
}


///=============================================================///
///   #3 - CALCULAR CANTIDAD TOTAL DE UN ITEM
///=============================================================///
// #3
int Inventario::calcular_cantidad_total_de_un_item(int id_del_item) const {
    int total = 0; // Acumula la suma total
    for (int i = 0; i < _cantidad_de_items_guardados; i++) {
        if (_items_guardados[i].getId() == id_del_item) {
            total += _items_guardados[i].getCantidad(); // Suma la cantidad de este slot
        }
    }
    return total; // Devuelve el total encontrado
}


///=============================================================///
///   #4 - CONSUMIR UN ITEM DE TIPO
///=============================================================///
// #4
bool Inventario::consumir_un_item_de_tipo(TipoDeItem tipo) {
    for (int i = 0; i < _cantidad_de_items_guardados; i++) {
        if (_items_guardados[i].getTipo() == tipo) {
            int nueva_cantidad = _items_guardados[i].getCantidad() - 1; // Resta una unidad
            if (nueva_cantidad <= 0) {
                quitar_slot_y_correr_los_siguientes(i); // Elimina el slot si se agoto
            }
            else {
                _items_guardados[i].setCantidad(nueva_cantidad); // Actualiza la cantidad
            }
            return true; // Se consumio con exito
        }
    }
    return false; // No habia items de ese tipo
}

///=============================================================///
///   #5 - USAR ITEM
///=============================================================///
// #5
void Inventario::usar_item(int indice_del_slot, Personaje& jugador) {

    if (indice_del_slot < 0 || indice_del_slot >= _cantidad_de_items_guardados) {
        std::cout << "SLOT VACIO O INVALIDO." << std::endl;
        return; // El slot no existe o esta vacio
    }

    _items_guardados[indice_del_slot].usar(jugador); // Aplica el efecto del item al personaje

    TipoDeItem tipo_del_item_usado = _items_guardados[indice_del_slot].getTipo(); // Guarda el tipo para chequear despues
    bool es_pocion_de_vida = tipo_del_item_usado == TipoDeItem::CONSUMIBLE_DE_VIDA; // Verdadero si es pocion de vida
    bool es_pocion_de_mana = tipo_del_item_usado == TipoDeItem::CONSUMIBLE_DE_MANA; // Verdadero si es pocion de mana

    if (es_pocion_de_vida == true || es_pocion_de_mana == true) {
        int cantidad_que_queda = _items_guardados[indice_del_slot].getCantidad() - 1; // Consume una unidad
        _items_guardados[indice_del_slot].setCantidad(cantidad_que_queda); // Actualiza la cantidad del stack

        if (cantidad_que_queda <= 0) {
            quitar_slot_y_correr_los_siguientes(indice_del_slot); // Elimina el slot si se agoto la pocion
            std::cout << "POCION AGOTADA." << std::endl;
        }
    }

    bool es_baculo_arcano = tipo_del_item_usado == TipoDeItem::BACULO_ARCANO; // Verdadero si se equipo el baculo
    if (es_baculo_arcano == true) {
        quitar_slot_y_correr_los_siguientes(indice_del_slot); // El baculo desaparece al equiparse
    }
}


///=============================================================///
///   #6 - EXTRAER ITEM SELECCIONADO
///=============================================================///
// #6
Item Inventario::extraer_item_seleccionado() {
    if (_indice_del_slot_seleccionado >= 0 && _indice_del_slot_seleccionado < _cantidad_de_items_guardados) {
        Item copia_del_item_extraido = _items_guardados[_indice_del_slot_seleccionado]; // Guarda una copia del item
        quitar_slot_y_correr_los_siguientes(_indice_del_slot_seleccionado); // Lo elimina de la mochila
        _indice_del_slot_seleccionado = -1; // Ya no hay slot seleccionado
        return copia_del_item_extraido; // Devuelve la copia al que llamo
    }
    return Item(); // Si no habia nada seleccionado, devuelve un item vacio
}
