#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Item.h"

///=================================================================///
///   FABRICA_DE_ITEMS - PATRON FABRICA
///=================================================================///
class FabricaDeItems {
private:

    sf::Texture _textura_maestra_de_los_items;
    std::string _ruta_del_archivo_de_items = "assets/items.dat";

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    FabricaDeItems();

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    sf::Texture& getTextura_maestra() { return _textura_maestra_de_los_items; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    bool guardar_registro(const RegistroDeItem registro) const;
    bool modificar_registro(const RegistroDeItem registro) const;
    RegistroDeItem leer_registro(int posicion) const;
    int contar_registros() const;
    int buscar_posicion_por_id(int id) const;

    static const int CANTIDAD_MAXIMA_DE_REGISTROS_A_LEER = 200;
    int leer_todos_los_registros_activos(RegistroDeItem registros_encontrados[CANTIDAD_MAXIMA_DE_REGISTROS_A_LEER]) const;

    Item crear_item_por_id(int id) const;
};