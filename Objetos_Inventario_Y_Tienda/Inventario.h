#pragma once
#include "Item.h"

// Inventario necesita conocer a
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

    static const int CAPACIDAD_MAXIMA_DE_LA_MOCHILA = 20; // Maximo de items que puede guardar
    Item _items_guardados[CAPACIDAD_MAXIMA_DE_LA_MOCHILA]; // Array de todos los items guardados
    int _cantidad_de_items_guardados = 0; // Cuantos items hay en la mochila ahora
    int _indice_del_slot_seleccionado = -1; // Cual slot tiene el foco (-1 = ninguno)

 ///=============================================================///
 ///   QUITAR UN SLOT - Corre todos los items de atras un lugar
 ///   para adelante, tapando el hueco que queda
 ///=============================================================///
 // #1
    void quitar_slot_y_correr_los_siguientes(int indice_a_quitar); // Elimina un slot y cierra el hueco

public:

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #2
    int getCantidad_de_items_guardados() const { return _cantidad_de_items_guardados; } // Cuantos items hay en la mochila
 // #3
    const Item& getItem_en_el_slot(int indice_del_slot) const { return _items_guardados[indice_del_slot]; } // Devuelve el item de ese slot
 // #4
    int getIndice_del_slot_seleccionado() const { return _indice_del_slot_seleccionado; } // Cual slot esta seleccionado

 ///=============================================================///
 ///   SETTERS
 ///=============================================================///
 // #5
    void setIndice_del_slot_seleccionado(int indice) { _indice_del_slot_seleccionado = indice; } // Cambia el slot activo

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
 // #6
    bool agarrar_item(Item nuevo_item); // Intenta agregar un item a la mochila
 // #7
    void usar_item(int indice_del_slot, Personaje& jugador); // Usa el item del slot indicado
 // #8
    Item extraer_item_seleccionado(); // Saca el item seleccionado de la mochila

 // Suma cantidades recorriendo varios slots: es un calculo, no
 // un atributo guardado, por eso no lleva "get" adelante
 // #9
    int calcular_cantidad_total_de_un_item(int id_del_item) const; // Cuenta cuantos items de ese ID hay en total
 // #10
    bool consumir_un_item_de_tipo(TipoDeItem tipo); // Quita 1 unidad del primer item de ese tipo. Devuelve false si no habia
 // #11
    void limpiar() { _cantidad_de_items_guardados = 0; _indice_del_slot_seleccionado = -1; } // Vacia la mochila completamente
};
