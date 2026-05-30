#pragma once
#include <vector>
#include "Item.h"

class Personaje; // Forward declaration para evitar dependencias circulares

class Inventario {
private:
	std::vector<Item*> _itemsGuardados; // Nuestra mochila: un vector dinámico de punteros a ítems (puede contener nullptrs si hay slots vacíos)
    int _capacidadMaxima;     // Cuántos espacios independientes tiene la mochila (Ej: 20)

public:
    // Constructor: por defecto le damos 20 espacios, pero puede cambiar
    Inventario(int capacidad = 20);
	~Inventario(); // Importante para liberar memoria de los ítems que quedan en el inventario al destruirlo

    // 🎒 Métodos principales para interactuar con el juego
    bool agarrarItem(Item* nuevoItem);
    bool tirarItem(int idItem, int cantidad);

    // Le pasamos el índice del vector y el jugador que se va a tomar la poción
    void usarItem(int indice, Personaje& jugador);

    // 🔍 Métodos auxiliares (Claves para el farmeo y la interfaz)
    int getCantidadTotal(int idItem) const;
    void vaciar();

    // Getter constante para que el GameManager o la UI puedan chusmear qué hay dentro sin modificarlo
    const std::vector<Item*>& getSlots() const { return _itemsGuardados; }
};