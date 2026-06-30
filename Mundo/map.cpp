#include "map.h"
#include <iostream>
#include <fstream>
#include <sstream>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Map::Map(int tamano_de_cada_tile, float escala) {
    _tamano_de_cada_tile_en_pixeles = tamano_de_cada_tile;
    _escala_del_mapa = escala;
    _cantidad_de_filas_reales = 0;
    _cantidad_de_columnas_reales = 0;

    _sprite_del_fondo.setScale(_escala_del_mapa, _escala_del_mapa);
}

///=============================================================///
///   CONVERTIR TEXTO A NUMERO
///=============================================================///
int Map::convertir_texto_a_numero(const std::string& texto) const {

    int numero_resultado = 0;
    int posicion_inicial = 0;
    bool el_numero_es_negativo = false;

    if (texto.length() > 0 && texto[0] == '-') {
        el_numero_es_negativo = true;
        posicion_inicial = 1;
    }

    int cantidad_de_caracteres = (int)texto.length();
    for (int i = posicion_inicial; i < cantidad_de_caracteres; i++) {
        char caracter_actual = texto[i];
        int valor_de_este_digito = caracter_actual - '0';
        numero_resultado = numero_resultado * 10 + valor_de_este_digito;
    }

    if (el_numero_es_negativo == true) {
        numero_resultado = numero_resultado * -1;
    }

    return numero_resultado;
}

///=============================================================///
///   CARGAR MAPA
///=============================================================///
bool Map::cargar_mapa(const std::string& ruta_del_csv, const std::string& ruta_de_la_textura) {

    _cantidad_de_filas_reales = 0;
    _cantidad_de_columnas_reales = 0;

    if (_textura_del_fondo.loadFromFile(ruta_de_la_textura) == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA IMAGEN DEL MAPA EN: " << ruta_de_la_textura << std::endl;
        return false;
    }

    _textura_del_fondo.setSmooth(false);
    _sprite_del_fondo.setTexture(_textura_del_fondo);

    std::ifstream archivo_del_mapa(ruta_del_csv);
    if (archivo_del_mapa.is_open() == false) {
        std::cout << "ERROR: NO SE PUDO ABRIR EL ARCHIVO DEL MAPA EN: " << ruta_del_csv << std::endl;
        return false;
    }

    std::string linea_leida;
    int fila_actual = 0;

    while (std::getline(archivo_del_mapa, linea_leida) && fila_actual < CANTIDAD_MAXIMA_DE_FILAS) {

        if (linea_leida.length() > 0 && linea_leida[linea_leida.length() - 1] == '\r') {
            linea_leida.pop_back();
        }

        std::stringstream linea_separada_por_comas(linea_leida);
        std::string numero_como_texto;
        int columna_actual = 0;

        while (std::getline(linea_separada_por_comas, numero_como_texto, ',') && columna_actual < CANTIDAD_MAXIMA_DE_COLUMNAS) {
            if (numero_como_texto.length() > 0) {
                _grilla_de_colisiones[fila_actual][columna_actual] = convertir_texto_a_numero(numero_como_texto);
                columna_actual++;
            }
        }

        if (columna_actual > 0) {
            _cantidad_de_columnas_reales = columna_actual;
            fila_actual++;
        }
    }

    _cantidad_de_filas_reales = fila_actual;

    return true;
}

///=============================================================///
///   DIBUJAR MAPA
///=============================================================///
void Map::dibujar_mapa(sf::RenderWindow& ventana_del_juego) const {
    ventana_del_juego.draw(_sprite_del_fondo);
}

///=============================================================///
///   GENERAR CLIMA
///=============================================================///
void Map::generar_clima(VisualFX& efectos_visuales) {
    sf::Vector2f tamano_del_area_del_clima(2000.f, 2000.f);
    efectos_visuales.agregarParticulasAmbiente(tamano_del_area_del_clima, 50, sf::Color(130, 200, 36));
}

///=============================================================///
///   GETTERS
///=============================================================///
bool Map::getEs_solido(int fila, int columna) const {
    if (fila < 0 || fila >= _cantidad_de_filas_reales || columna < 0 || columna >= _cantidad_de_columnas_reales) {
        return true;
    }
    return _grilla_de_colisiones[fila][columna] != -1;
}

bool Map::getHay_colision(const sf::FloatRect& rectangulo) const {

    float tamano_real_de_cada_tile = _tamano_de_cada_tile_en_pixeles * _escala_del_mapa;

    int columna_inicial = (int)(rectangulo.left / tamano_real_de_cada_tile);
    int columna_final = (int)((rectangulo.left + rectangulo.width) / tamano_real_de_cada_tile);
    int fila_inicial = (int)(rectangulo.top / tamano_real_de_cada_tile);
    int fila_final = (int)((rectangulo.top + rectangulo.height) / tamano_real_de_cada_tile);

    for (int fila = fila_inicial; fila <= fila_final; fila++) {
        for (int columna = columna_inicial; columna <= columna_final; columna++) {
            if (getEs_solido(fila, columna) == true) {
                return true;
            }
        }
    }

    return false;
}

///=============================================================///
///   DIBUJAR DEBUG
///=============================================================///
void Map::dibujar_debug(sf::RenderWindow& ventana_del_juego) const {

    float tamano_real_de_cada_tile = _tamano_de_cada_tile_en_pixeles * _escala_del_mapa;

    sf::RectangleShape rectangulo_de_debug;
    rectangulo_de_debug.setFillColor(sf::Color(255, 0, 0, 100));
    rectangulo_de_debug.setSize(sf::Vector2f(tamano_real_de_cada_tile, tamano_real_de_cada_tile));

    for (int fila = 0; fila < _cantidad_de_filas_reales; fila++) {
        for (int columna = 0; columna < _cantidad_de_columnas_reales; columna++) {
            if (getEs_solido(fila, columna) == true) {
                rectangulo_de_debug.setPosition(columna * tamano_real_de_cada_tile, fila * tamano_real_de_cada_tile);
                ventana_del_juego.draw(rectangulo_de_debug);
            }
        }
    }
}