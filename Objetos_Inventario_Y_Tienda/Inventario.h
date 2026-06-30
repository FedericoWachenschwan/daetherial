#pragma once
#include "Item.h"

// CASO ESPECIAL (ver regla #32): Inventario necesita conocer a
// Personaje, pero Personaje.h ya incluye a Inventario (composicion:
// el Personaje TIENE un Inventario). Si pusieramos el include normal
// aca, se forma un ciclo y no compila. Por eso, solo en este caso
// puntual, usamos la declaracion adelantada en vez del include normal
class Personaje;

///=================================================================///
///   INVENTARIO - La mochila del jugador. Guarda los items en un
///   array de tamaño fijo, no en un vector
///=================================================================///
class Inventario {
private:

    static const int CAPACIDAD_MAXIMA_DE_LA_MOCHILA = 20;
    Item _items_guardados[CAPACIDAD_MAXIMA_DE_LA_MOCHILA];
    int _cantidad_de_items_guardados = 0;
    int _indice_del_slot_seleccionado = -1;

    ///=============================================================///
    ///   QUITAR UN SLOT - Corre todos los items de atras un lugar
    ///   para adelante, tapando el hueco que queda
    ///=============================================================///
    void quitar_slot_y_correr_los_siguientes(int indice_a_quitar);

public:

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    int getCantidad_de_items_guardados() const { return _cantidad_de_items_guardados; }
    const Item& getItem_en_el_slot(int indice_del_slot) const { return _items_guardados[indice_del_slot]; }
    int getIndice_del_slot_seleccionado() const { return _indice_del_slot_seleccionado; }

    ///=============================================================///
    ///   SETTERS
    ///=============================================================///
    void setIndice_del_slot_seleccionado(int indice) { _indice_del_slot_seleccionado = indice; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    bool agarrar_item(Item nuevo_item);
    void usar_item(int indice_del_slot, Personaje& jugador);
    Item extraer_item_seleccionado();

    // Suma cantidades recorriendo varios slots: es un calculo, no
    // un atributo guardado, por eso no lleva "get" adelante
    int calcular_cantidad_total_de_un_item(int id_del_item) const;
};