#include "ItemManager.h"
#include <fstream> // Para manejo de archivos en los métodos de ABML
#include <iostream> // Para imprimir errores de carga de texturas

ItemManager::ItemManager() {
    // Cargamos todas las texturas cuando el juego arranca
    if (!_texturaMaestraItems.loadFromFile("assets/items_spritesheet.png")) {
        std::cout << "⚠️ Error al cargar el spritesheet de items." << std::endl;
    }
}

// ============================================================================
// 📁 MÉTODOS ABML (PERSISTENCIA EN DISCO)
// ============================================================================

bool ItemManager::guardarRegistro(const ItemReg registro) const {
    std::ofstream archivo(_rutaArchivoItems, std::ios::binary | std::ios::app);
    if (!archivo.is_open()) return false;
    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(ItemReg));
    archivo.close();
    return true;
}

bool ItemManager::modificarRegistro(const ItemReg registro) const {
    int pos = buscarPorId(registro.id);
    if (pos == -1) return false;

    std::ofstream archivo(_rutaArchivoItems, std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) return false;
    archivo.seekp(pos * sizeof(ItemReg));
    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(ItemReg));
    archivo.close();
    return true;
}

ItemReg ItemManager::leerRegistro(int posicion) const {
    ItemReg reg{ -1, 0, "", 0, 0, 0, 0, false };
    std::ifstream archivo(_rutaArchivoItems, std::ios::binary);
    if (!archivo.is_open()) return reg;
    archivo.seekg(posicion * sizeof(ItemReg));
    archivo.read(reinterpret_cast<char*>(&reg), sizeof(ItemReg));
    archivo.close();
    return reg;
}

std::vector<ItemReg> ItemManager::leerTodos() const {
    std::vector<ItemReg> lista;
    std::ifstream archivo(_rutaArchivoItems, std::ios::binary);
    if (!archivo.is_open()) return lista;

    ItemReg reg{};
    while (archivo.read(reinterpret_cast<char*>(&reg), sizeof(ItemReg))) {
        if (reg.activo) lista.push_back(reg);
    }
    archivo.close();
    return lista;
}

int ItemManager::contarRegistros() const {
    std::ifstream archivo(_rutaArchivoItems, std::ios::binary | std::ios::ate);
    if (!archivo.is_open()) return 0;
    std::streampos tamano = archivo.tellg();
    archivo.close();
    return static_cast<int>(tamano / sizeof(ItemReg));
}

int ItemManager::buscarPorId(int id) const {
    std::ifstream archivo(_rutaArchivoItems, std::ios::binary);
    if (!archivo.is_open()) return -1;

    ItemReg reg{};
    int posicion = 0;
    while (archivo.read(reinterpret_cast<char*>(&reg), sizeof(ItemReg))) {
        if (reg.id == id && reg.activo) {
            archivo.close();
            return posicion;
        }
        posicion++;
    }
    archivo.close();
    return -1;
}

// ============================================================================
// 🏭 LA FÁBRICA MAESTRA: Del .dat al objeto del juego
// ============================================================================
Item* ItemManager::crearItemPorId(int id) const {
    int pos = buscarPorId(id);
    if (pos == -1) {
        std::cout << "❌ Error: El item con ID " << id << " no existe en la BD." << std::endl;
        return nullptr;
    }

    ItemReg datos = leerRegistro(pos);
    Item* nuevoItem = nullptr;
    TipoItem tipo = static_cast<TipoItem>(datos.tipoItem);

    // 1. Instanciamos el hijo correcto usando los constructores que definiste en Item.cpp
    switch (tipo) {
    case TipoItem::Consumible:
        nuevoItem = new Consumible(datos.id, datos.nombre, static_cast<float>(datos.valorEfecto), 1);
        break;

    case TipoItem::Equipamiento:
        // Usamos valorEfecto para Ataque y precio para Defensa (podés ajustarlo después)
        nuevoItem = new Equipamiento(datos.id, datos.nombre, 0, datos.valorEfecto, datos.precio);
        break;

    case TipoItem::Recurso:
        nuevoItem = new Recurso(datos.id, datos.nombre, datos.valorEfecto, 1);
        break;

    case TipoItem::Mueble:
        nuevoItem = new Mueble(datos.id, datos.nombre, datos.valorEfecto);
        break;

    default:
        return nullptr;
    }

    // 2. Configuración gráfica automática (Cortamos el icono del spritesheet)
    if (nuevoItem != nullptr) {
        const int TILE_SIZE = 32; // Ajustá esto si tus íconos son de 16x16 o 64x64
        int columnasGrilla = _texturaMaestraItems.getSize().x / TILE_SIZE;

        int col = datos.idTextura % columnasGrilla;
        int fila = datos.idTextura / columnasGrilla;

        sf::Sprite& spriteItem = nuevoItem->getSprite();
        spriteItem.setTexture(_texturaMaestraItems);
        spriteItem.setTextureRect(sf::IntRect(col * TILE_SIZE, fila * TILE_SIZE, TILE_SIZE, TILE_SIZE));
    }

    return nuevoItem;
}