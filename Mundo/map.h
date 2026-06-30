#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "VisualFX.h"

///=================================================================///
///   MAP - El mapa del nivel: la imagen de fondo que se ve, y una
///   grilla invisible que dice donde hay paredes
///=================================================================///
class Map {
private:

    ///=============================================================///
    ///   GRILLA DE COLISIONES
    ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_FILAS = 200;
    static const int CANTIDAD_MAXIMA_DE_COLUMNAS = 200;
    int _grilla_de_colisiones[CANTIDAD_MAXIMA_DE_FILAS][CANTIDAD_MAXIMA_DE_COLUMNAS];

    int _cantidad_de_filas_reales;
    int _cantidad_de_columnas_reales;
    int _tamano_de_cada_tile_en_pixeles;
    float _escala_del_mapa;

    sf::Texture _textura_del_fondo;
    sf::Sprite _sprite_del_fondo;

    int convertir_texto_a_numero(const std::string& texto) const;

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    Map(int tamano_de_cada_tile = 32, float escala = 1.0f);

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    bool getEs_solido(int fila, int columna) const;
    bool getHay_colision(const sf::FloatRect& rectangulo) const;

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    bool cargar_mapa(const std::string& ruta_del_csv, const std::string& ruta_de_la_textura);
    void dibujar_mapa(sf::RenderWindow& ventana_del_juego) const;
    void dibujar_debug(sf::RenderWindow& ventana_del_juego) const;
    void generar_clima(VisualFX& efectos_visuales);
};