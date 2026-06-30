#include "menu.h"
#include <iostream>
#include <string>

///=============================================================///
///   CONSTRUCTOR - Arma las 5 opciones del menu con su boton y
///   su texto, ya centrados en pantalla
///=============================================================///
Menu::Menu(float ancho_de_la_ventana, float alto_de_la_ventana) {

    if (_fuente_del_menu.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA FUENTE DEL MENU." << std::endl;
    }

    if (_textura_de_fondo.loadFromFile("assets/fondo_menu.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR EL FONDO DEL MENU." << std::endl;
    }

    _sprite_de_fondo.setTexture(_textura_de_fondo);
    _sprite_de_fondo.setScale(ancho_de_la_ventana / _textura_de_fondo.getSize().x, alto_de_la_ventana / _textura_de_fondo.getSize().y);

    std::string nombres_de_las_opciones[CANTIDAD_DE_OPCIONES_DEL_MENU] = {
        "Inicio", "Crear items", "Logros", "Creditos", "Salir"
    };

    float centro_x_de_la_ventana = ancho_de_la_ventana / 2.f;
    float posicion_y_de_la_primera_opcion = alto_de_la_ventana * 0.35f;
    float separacion_entre_opciones = 70.f;

    for (int i = 0; i < CANTIDAD_DE_OPCIONES_DEL_MENU; i++) {

        ///=========================================================///
        ///   BOTON DE FONDO DE ESTA OPCION
        ///=========================================================///
        _botones_del_menu[i].setSize(sf::Vector2f(300.f, 60.f));
        _botones_del_menu[i].setPosition(centro_x_de_la_ventana - 150.f, posicion_y_de_la_primera_opcion + i * separacion_entre_opciones);
        _botones_del_menu[i].setFillColor(sf::Color(200, 180, 140));
        _botones_del_menu[i].setOutlineThickness(2.f);
        _botones_del_menu[i].setOutlineColor(sf::Color(60, 40, 20));

        ///=========================================================///
        ///   TEXTO DE ESTA OPCION
        ///=========================================================///
        _opciones_del_menu[i].setFont(_fuente_del_menu);
        _opciones_del_menu[i].setString(nombres_de_las_opciones[i]);
        _opciones_del_menu[i].setCharacterSize(40);

        if (i == 0) {
            _opciones_del_menu[i].setFillColor(sf::Color(180, 50, 30)); // La primera arranca marcada
        }
        else {
            _opciones_del_menu[i].setFillColor(sf::Color(60, 40, 20));
        }

        ///=========================================================///
        ///   CENTRAMOS EL TEXTO ADENTRO DE SU BOTON
        ///=========================================================///
        sf::FloatRect limites_del_texto = _opciones_del_menu[i].getLocalBounds(); // SFML: el tamaño real que ocupa el texto dibujado
        _opciones_del_menu[i].setOrigin(limites_del_texto.left + limites_del_texto.width / 2.f, limites_del_texto.top + limites_del_texto.height / 2.f);
        _opciones_del_menu[i].setPosition(centro_x_de_la_ventana, posicion_y_de_la_primera_opcion + i * separacion_entre_opciones + 30.f);
    }

    _indice_de_la_opcion_seleccionada = 0;
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void Menu::dibujar(sf::RenderWindow& ventana_del_juego) {
    ventana_del_juego.draw(_sprite_de_fondo);

    for (int i = 0; i < CANTIDAD_DE_OPCIONES_DEL_MENU; i++) {
        ventana_del_juego.draw(_botones_del_menu[i]);
    }
    for (int i = 0; i < CANTIDAD_DE_OPCIONES_DEL_MENU; i++) {
        ventana_del_juego.draw(_opciones_del_menu[i]);
    }
}

///=============================================================///
///   MOVER SELECCION HACIA ARRIBA
///=============================================================///
void Menu::mover_seleccion_hacia_arriba() {
    if (_indice_de_la_opcion_seleccionada > 0) {
        _opciones_del_menu[_indice_de_la_opcion_seleccionada].setFillColor(sf::Color(60, 40, 20));
        _indice_de_la_opcion_seleccionada--;
        _opciones_del_menu[_indice_de_la_opcion_seleccionada].setFillColor(sf::Color(180, 150, 90));
    }
}

///=============================================================///
///   MOVER SELECCION HACIA ABAJO
///=============================================================///
void Menu::mover_seleccion_hacia_abajo() {
    if (_indice_de_la_opcion_seleccionada < CANTIDAD_DE_OPCIONES_DEL_MENU - 1) {
        _opciones_del_menu[_indice_de_la_opcion_seleccionada].setFillColor(sf::Color(60, 40, 20));
        _indice_de_la_opcion_seleccionada++;
        _opciones_del_menu[_indice_de_la_opcion_seleccionada].setFillColor(sf::Color(180, 150, 90));
    }
}