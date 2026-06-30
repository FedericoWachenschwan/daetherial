#include "FabricaDeItems.h"
#include <fstream>
#include <iostream>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
FabricaDeItems::FabricaDeItems() {
    if (_textura_maestra_de_los_items.loadFromFile("assets/items_spritesheet.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR EL SPRITESHEET DE ITEMS." << std::endl;
    }
}

///=============================================================///
///   GUARDAR REGISTRO
///=============================================================///
bool FabricaDeItems::guardar_registro(const RegistroDeItem registro) const {
    std::ofstream archivo(_ruta_del_archivo_de_items, std::ios::binary | std::ios::app);
    if (archivo.is_open() == false) return false;

    archivo.write((const char*)&registro, sizeof(RegistroDeItem));
    archivo.close();
    return true;
}

///=============================================================///
///   MODIFICAR REGISTRO
///=============================================================///
bool FabricaDeItems::modificar_registro(const RegistroDeItem registro) const {
    int posicion_del_registro = buscar_posicion_por_id(registro.id);
    if (posicion_del_registro == -1) return false;

    std::ofstream archivo(_ruta_del_archivo_de_items, std::ios::binary | std::ios::in | std::ios::out);
    if (archivo.is_open() == false) return false;

    archivo.seekp(posicion_del_registro * sizeof(RegistroDeItem));
    archivo.write((const char*)&registro, sizeof(RegistroDeItem));
    archivo.close();
    return true;
}

///=============================================================///
///   LEER REGISTRO
///=============================================================///
RegistroDeItem FabricaDeItems::leer_registro(int posicion) const {
    RegistroDeItem registro_leido{ -1, 0, "", 0, 0, 0, 0, false };

    std::ifstream archivo(_ruta_del_archivo_de_items, std::ios::binary);
    if (archivo.is_open() == false) return registro_leido;

    archivo.seekg(posicion * sizeof(RegistroDeItem));
    archivo.read((char*)&registro_leido, sizeof(RegistroDeItem));
    archivo.close();
    return registro_leido;
}

///=============================================================///
///   CONTAR REGISTROS
///=============================================================///
int FabricaDeItems::contar_registros() const {
    std::ifstream archivo(_ruta_del_archivo_de_items, std::ios::binary | std::ios::ate);
    if (archivo.is_open() == false) return 0;

    std::streampos tamano_del_archivo = archivo.tellg();
    archivo.close();
    return (int)(tamano_del_archivo / sizeof(RegistroDeItem));
}

///=============================================================///
///   BUSCAR POSICION POR ID
///=============================================================///
int FabricaDeItems::buscar_posicion_por_id(int id) const {
    std::ifstream archivo(_ruta_del_archivo_de_items, std::ios::binary);
    if (archivo.is_open() == false) return -1;

    RegistroDeItem registro_leido{};
    int posicion_actual = 0;

    while (archivo.read((char*)&registro_leido, sizeof(RegistroDeItem))) {
        if (registro_leido.id == id && registro_leido.esta_activo == true) {
            archivo.close();
            return posicion_actual;
        }
        posicion_actual++;
    }
    archivo.close();
    return -1;
}

///=============================================================///
///   LEER TODOS LOS REGISTROS ACTIVOS
///=============================================================///
int FabricaDeItems::leer_todos_los_registros_activos(RegistroDeItem registros_encontrados[CANTIDAD_MAXIMA_DE_REGISTROS_A_LEER]) const {
    std::ifstream archivo(_ruta_del_archivo_de_items, std::ios::binary);
    if (archivo.is_open() == false) return 0;

    RegistroDeItem registro_leido{};
    int cantidad_encontrada = 0;

    while (archivo.read((char*)&registro_leido, sizeof(RegistroDeItem)) && cantidad_encontrada < CANTIDAD_MAXIMA_DE_REGISTROS_A_LEER) {
        if (registro_leido.esta_activo == true) {
            registros_encontrados[cantidad_encontrada] = registro_leido;
            cantidad_encontrada++;
        }
    }
    archivo.close();
    return cantidad_encontrada;
}

///=============================================================///
///   FABRICAR ITEM POR ID
///=============================================================///
Item FabricaDeItems::crear_item_por_id(int id) const {

    int posicion_en_el_archivo = buscar_posicion_por_id(id);

    if (posicion_en_el_archivo == -1) {
        std::cout << "ERROR: EL ITEM CON ID " << id << " NO EXISTE EN LA BASE DE DATOS." << std::endl;
        return Item();
    }

    RegistroDeItem datos_del_item = leer_registro(posicion_en_el_archivo);
    TipoDeItem tipo_del_item = (TipoDeItem)datos_del_item.tipo_de_item;

    Item item_fabricado(
        datos_del_item.id,
        datos_del_item.nombre,
        tipo_del_item,
        datos_del_item.precio,
        1,
        64,
        true,
        datos_del_item.valor_del_efecto,
        0,
        0
    );

    int tamano_de_cada_icono = 32;
    int columnas_en_la_textura_maestra = _textura_maestra_de_los_items.getSize().x / tamano_de_cada_icono;
    int columna_del_icono = datos_del_item.id_de_la_textura % columnas_en_la_textura_maestra;
    int fila_del_icono = datos_del_item.id_de_la_textura / columnas_en_la_textura_maestra;

    item_fabricado.getSprite().setTexture(_textura_maestra_de_los_items);
    item_fabricado.getSprite().setTextureRect(sf::IntRect(
        columna_del_icono * tamano_de_cada_icono,
        fila_del_icono * tamano_de_cada_icono,
        tamano_de_cada_icono,
        tamano_de_cada_icono
    ));

    return item_fabricado;
}