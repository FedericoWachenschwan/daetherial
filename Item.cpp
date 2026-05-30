#include "Item.h"
#include <iostream>

// ========================================================
// 1. IMPLEMENTACIÓN DE LA CLASE BASE (Item)
// ========================================================

Item::Item(int id, std::string nombre, TipoItem tipo, int cantidad, int maxStack, bool esAgarrable)
    : _id(id), _nombre(nombre), _tipo(tipo), _cantidad(cantidad), _maxStack(maxStack), _esAgarrable(esAgarrable), _estaEnElMundo(false) {
}

void Item::colocarEnMundo(sf::Texture& textura, float x, float y, sf::FloatRect hitboxCustom) {
    _sprite.setTexture(textura);
    _sprite.setPosition(x, y);
    _estaEnElMundo = true;

    // 🌟 SIEMPRE movemos la hitbox a la coordenada donde spawnea
    _hitbox.left = x;
    _hitbox.top = y;

    // (Opcional) Si le pasaste una hitboxCustom, pisamos el tamaño
    if (hitboxCustom.width != 0) {
        _hitbox.width = hitboxCustom.width;
        _hitbox.height = hitboxCustom.height;
    }
}

void Item::dibujar(sf::RenderWindow& ventana) {
    if (_estaEnElMundo) {
        ventana.draw(_sprite);
    }
}

// ========================================================
// 2. IMPLEMENTACIÓN DE LAS CLASES HIJAS
// ========================================================

// --- CONSUMIBLE ---
Consumible::Consumible(int id, std::string nombre, float cura, int cantidad)
    : Item(id, nombre, TipoItem::Consumible, cantidad, 64, true), _curacion(cura) {

    // 🌟 La magia va ADENTRO del constructor
    _hitbox.width = 16.f;
    _hitbox.height = 16.f;
}

void Consumible::usar(Personaje& jugador) {
    std::cout << "🍎 Consumiste " << _nombre << ". Te curaste " << _curacion << " de vida." << std::endl;
}

// --- MUEBLE ---
Mueble::Mueble(int id, std::string nombre, int tipoMueble)
    : Item(id, nombre, TipoItem::Mueble, 1, 1, false), _tipoMueble(tipoMueble) {

    // Los muebles suelen ser más grandes
    _hitbox.width = 32.f;
    _hitbox.height = 32.f;
}

void Mueble::usar(Personaje& jugador) {
    if (_tipoMueble == 1) std::cout << "🏺 Abriendo Caldero..." << std::endl;
    else if (_tipoMueble == 2) std::cout << "🔥 Abriendo Horno..." << std::endl;
}

// --- RECURSO ---
Recurso::Recurso(int id, std::string nombre, int tipoRecurso, int cantidad)
    : Item(id, nombre, TipoItem::Recurso, cantidad, 999, true), _tipoRecurso(tipoRecurso) {

    // Tamaño estándar de un drop en el piso
    _hitbox.width = 16.f;
    _hitbox.height = 16.f;
}

void Recurso::usar(Personaje& jugador) {
    std::cout << "🪵 Es un recurso material, no se puede 'usar' directo. Sirve para craftear." << std::endl;
}

// --- EQUIPAMIENTO ---
Equipamiento::Equipamiento(int id, std::string nombre, int tipoEquipamiento, int bonusAtaque, int bonusDefensa)
    : Item(id, nombre, TipoItem::Equipamiento, 1, 1, true), _tipoEquipamiento(tipoEquipamiento), _bonusAtaque(bonusAtaque), _bonusDefensa(bonusDefensa) {

    // La espada/escudo en el piso
    _hitbox.width = 24.f;
    _hitbox.height = 24.f;
}

void Equipamiento::usar(Personaje& jugador) {
    std::cout << "⚔️ Equipaste " << _nombre << ". (+Ataque: " << _bonusAtaque << " | +Defensa: " << _bonusDefensa << ")" << std::endl;
}