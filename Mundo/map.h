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
    static const int CANTIDAD_MAXIMA_DE_FILAS = 200; // Limite de filas del mapa
    static const int CANTIDAD_MAXIMA_DE_COLUMNAS = 200; // Limite de columnas del mapa
    int _grilla_de_colisiones[CANTIDAD_MAXIMA_DE_FILAS][CANTIDAD_MAXIMA_DE_COLUMNAS]; // Tabla de paredes: -1 es libre, otro numero es solido

    int _cantidad_de_filas_reales; // Filas que realmente tiene el mapa cargado
    int _cantidad_de_columnas_reales; // Columnas que realmente tiene el mapa cargado
    int _tamano_de_cada_tile_en_pixeles; // Cuantos pixeles mide cada celda de la grilla
    float _escala_del_mapa; // Factor de escala del mapa (1.0 = tamanio original)

    sf::Texture _textura_del_fondo; // Imagen del mapa cargada desde disco
    sf::Sprite _sprite_del_fondo; // Lo que se dibuja como fondo del nivel

 // #8
    int convertir_texto_a_numero(const std::string& texto) const; // Convierte "42" al numero 42

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    Map(int tamano_de_cada_tile = 32, float escala = 1.0f);

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #2
    bool getEs_solido(int fila, int columna) const; // Devuelve si esa celda es una pared
 // #3
    bool getHay_colision(const sf::FloatRect& rectangulo) const; // Devuelve si un rectangulo choca con el mapa

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
 // #4
    bool cargar_mapa(const std::string& ruta_del_csv, const std::string& ruta_de_la_textura);
 // #5
    void dibujar_mapa(sf::RenderWindow& ventana_del_juego) const;
 // #6
    void dibujar_debug(sf::RenderWindow& ventana_del_juego) const;
 // #7
    void generar_clima(VisualFX& efectos_visuales);
};
