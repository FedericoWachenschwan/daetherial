#include "ItemManager.h"
#include <iostream>

ItemManager::ItemManager() {
    // Cargamos todas las texturas cuando el juego arranca
    if (!_texturaPocionVida.loadFromFile("assets/potion_red.png")) {
        std::cout << "⚠️ Error al cargar la textura de la pocion de vida." << std::endl;
    }
    if (!_texturaHorno.loadFromFile("assets/horno.png")) {
        std::cout << "⚠️ Error al cargar la textura del horno." << std::endl;
    }
}

// Fabricamos un Consumible (ID 2, Cura 20, Cantidad 1)
Item* ItemManager::crearPocionVida() {
    return new Consumible(2, "Pocion de Vida", 20.f, 1);
}

// Fabricamos un Mueble (ID 11, TipoMueble 2)
Item* ItemManager::crearHorno() {
    return new Mueble(11, "Horno de Fundicion", 2);
}