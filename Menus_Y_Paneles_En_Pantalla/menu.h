#pragma once
#include <SFML/Graphics.hpp>

///=================================================================///
///   MENU - El menu principal del juego con sus opciones
///=================================================================///
class Menu {
private:

    static const int CANTIDAD_DE_OPCIONES_DEL_MENU = 4; // Cuantas opciones tiene el menu
    sf::Text _opciones_del_menu[CANTIDAD_DE_OPCIONES_DEL_MENU]; // SFML: sf::Text dibuja texto en pantalla usando una fuente cargada
    sf::RectangleShape _botones_del_menu[CANTIDAD_DE_OPCIONES_DEL_MENU]; // SFML: un rectangulo que se puede dibujar, lo usamos de fondo de cada boton
    sf::Font _fuente_del_menu; // La tipografia que usan los textos del menu
    int _indice_de_la_opcion_seleccionada; // Cual de las opciones esta resaltada ahora

    sf::Texture _textura_de_fondo; // Imagen de fondo del menu cargada desde disco
    sf::Sprite _sprite_de_fondo; // Lo que se dibuja como fondo de pantalla

public:

 ///=============================================================///
 ///   CONSTRUCTOR - Recibe el tamaño de la ventana para poder
 ///   centrar las opciones
 ///=============================================================///
 // #1
    Menu(float ancho_de_la_ventana, float alto_de_la_ventana);
 // #2
    void dibujar(sf::RenderWindow& ventana_del_juego); // Dibuja el menu completo en pantalla
 // #3
    void mover_seleccion_hacia_arriba(); // Sube el cursor del menu
 // #4
    void mover_seleccion_hacia_abajo(); // Baja el cursor del menu
 // #5
    int getIndice_de_la_opcion_seleccionada() const { return _indice_de_la_opcion_seleccionada; } // Devuelve cual opcion esta elegida
};
