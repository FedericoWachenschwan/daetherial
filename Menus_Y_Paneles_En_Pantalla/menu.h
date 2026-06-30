#pragma once
#include <SFML/Graphics.hpp>

///=================================================================///
///   MENU - El menu principal del juego con sus opciones
///=================================================================///
class Menu {
private:

    static const int CANTIDAD_DE_OPCIONES_DEL_MENU = 5;
    sf::Text _opciones_del_menu[CANTIDAD_DE_OPCIONES_DEL_MENU]; // SFML: sf::Text dibuja texto en pantalla usando una fuente cargada
    sf::RectangleShape _botones_del_menu[CANTIDAD_DE_OPCIONES_DEL_MENU]; // SFML: un rectangulo que se puede dibujar, lo usamos de fondo de cada boton
    sf::Font _fuente_del_menu;
    int _indice_de_la_opcion_seleccionada;

    sf::Texture _textura_de_fondo;
    sf::Sprite _sprite_de_fondo;

public:

    ///=============================================================///
    ///   CONSTRUCTOR - Recibe el tamaño de la ventana para poder
    ///   centrar las opciones
    ///=============================================================///
    Menu(float ancho_de_la_ventana, float alto_de_la_ventana);

    void dibujar(sf::RenderWindow& ventana_del_juego);
    void mover_seleccion_hacia_arriba();
    void mover_seleccion_hacia_abajo();
    int getIndice_de_la_opcion_seleccionada() const { return _indice_de_la_opcion_seleccionada; }
};