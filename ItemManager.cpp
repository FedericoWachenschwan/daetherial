#include "ItemManager.h"
#include <fstream>
#include <iostream>

ItemManager::ItemManager() {
    if (!_texturaMaestraItems.loadFromFile("assets/items_spritesheet.png")) {
        std::cout << "ERROR: NO SE PUDO CARGAR EL SPRITESHEET DE ITEMS." << std::endl;
    }
}

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

///=============================================================///
///   CREAR ITEM POR ID - Lee el archivo y crea el item correspondiente
///=============================================================///
Item* ItemManager::crearItemPorId(int id) const {

    int posicion_en_el_archivo = buscarPorId(id); // Buscamos el item en el archivo

    if (posicion_en_el_archivo == -1) {
        std::cout << "ERROR: EL ITEM CON ID " << id << " NO EXISTE EN LA BASE DE DATOS." << std::endl;
        return nullptr; // Si no existe devolvemos nulo
    }

    ItemReg datos_del_item = leerRegistro(posicion_en_el_archivo); // Leemos los datos del archivo
    TipoItem tipo_del_item = static_cast<TipoItem>(datos_del_item.tipoItem); // Convertimos el int al enum

    ///=========================================================///
    ///   CREAMOS EL ITEM - Con todos sus datos leídos del archivo
    ///=========================================================///
    Item* nuevo_item = new Item(
        datos_del_item.id,           // ID del item
        datos_del_item.nombre,       // Nombre del item
        tipo_del_item,               // Tipo del item
        datos_del_item.precio,       // Precio en la tienda
        1,                           // Cantidad inicial
        64,                          // Cantidad máxima en el stack
        true,                        // El jugador puede agarrarlo
        datos_del_item.valorEfecto,  // Puntos de curación (si es poción)
        0,                           // Bonus de ataque (se puede expandir después)
        0                            // Bonus de defensa (se puede expandir después)
    );

    ///=========================================================///
    ///   CONFIGURAMOS EL SPRITE - Cortamos el icono del spritesheet
    ///=========================================================///
    int tamanio_del_tile = 32; // Cada icono mide 32x32 píxeles en el spritesheet
    int columnas_del_spritesheet = _texturaMaestraItems.getSize().x / tamanio_del_tile; // Cuántas columnas hay
    int columna_del_sprite = datos_del_item.idTextura % columnas_del_spritesheet; // En qué columna está
    int fila_del_sprite = datos_del_item.idTextura / columnas_del_spritesheet; // En qué fila está

    sf::Sprite& sprite_del_item = nuevo_item->getSprite(); // Obtenemos el sprite del item
    sprite_del_item.setTexture(_texturaMaestraItems); // Le asignamos el spritesheet
    sprite_del_item.setTextureRect(sf::IntRect( // Cortamos el icono del spritesheet
        columna_del_sprite * tamanio_del_tile,
        fila_del_sprite * tamanio_del_tile,
        tamanio_del_tile,
        tamanio_del_tile
    ));

    return nuevo_item; // Devolvemos el item creado
}