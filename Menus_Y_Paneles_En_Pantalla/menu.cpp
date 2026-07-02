#include "menu.h"
#include <iostream>
#include <string>

///=============================================================///
///   CONSTRUCTOR - Arma las 4 opciones del menu con su boton y
///   su texto, ya centrados en pantalla
///=============================================================///
Menu::Menu(float ancho_de_la_ventana, float alto_de_la_ventana) {

    if (_fuente_del_menu.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA FUENTE DEL MENU." << std::endl;
    }

    if (_textura_de_fondo.loadFromFile("assets/fondo_menu.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR EL FONDO DEL MENU." << std::endl;
    }

    _sprite_de_fondo.setTexture(_textura_de_fondo); // asocia la imagen al sprite
    _sprite_de_fondo.setScale(ancho_de_la_ventana / _textura_de_fondo.getSize().x, alto_de_la_ventana / _textura_de_fondo.getSize().y); // escala la imagen para que cubra toda la ventana

    std::string nombres_de_las_opciones[CANTIDAD_DE_OPCIONES_DEL_MENU] = {
        "Comenzar Partida", "Logros", "Creditos", "Salir" // texto que aparece en cada boton del menu
    };

    float centro_x_de_la_ventana = ancho_de_la_ventana / 2.f; // punto horizontal del centro de la pantalla
    float posicion_y_de_la_primera_opcion = alto_de_la_ventana * 0.28f; // altura a la que arranca el primer boton (28% desde arriba)
    float separacion_entre_opciones = 85.f; // distancia vertical entre cada boton en pixeles
    const float ANCHO_DEL_BOTON = 260.f; // ancho de cada boton

    for (int i = 0; i < CANTIDAD_DE_OPCIONES_DEL_MENU; i++) {

 ///=========================================================///
 ///   BOTON DE FONDO DE ESTA OPCION
 ///=========================================================///
        _botones_del_menu[i].setSize(sf::Vector2f(ANCHO_DEL_BOTON, 60.f)); // ancho y alto del rectangulo de fondo del boton
        _botones_del_menu[i].setPosition(centro_x_de_la_ventana - ANCHO_DEL_BOTON / 2.f, posicion_y_de_la_primera_opcion + i * separacion_entre_opciones); // centra el boton horizontalmente
        _botones_del_menu[i].setFillColor(sf::Color(40, 70, 90)); // color de relleno del boton (azul oscuro)
        _botones_del_menu[i].setOutlineThickness(2.f); // grosor del borde del boton
        _botones_del_menu[i].setOutlineColor(sf::Color(100, 160, 190)); // color del borde (azul claro)

 ///=========================================================///
 ///   TEXTO DE ESTA OPCION
 ///=========================================================///
        _opciones_del_menu[i].setFont(_fuente_del_menu); // fuente que usa el texto
        _opciones_del_menu[i].setString(nombres_de_las_opciones[i]); // texto visible del boton
        _opciones_del_menu[i].setCharacterSize(30); // tamanio de la letra en pixeles

        if (i == 0) {
            _opciones_del_menu[i].setFillColor(sf::Color(220, 240, 255)); // la primera opcion arranca resaltada en blanco azulado
        }
        else {
            _opciones_del_menu[i].setFillColor(sf::Color(160, 200, 220)); // las demas arrancan en azul claro
        }

 ///=========================================================///
 ///   CENTRAMOS EL TEXTO ADENTRO DE SU BOTON
 ///=========================================================///
        sf::FloatRect limites_del_texto = _opciones_del_menu[i].getLocalBounds(); // rectangulo que envuelve el texto para saber su tamanio real
        _opciones_del_menu[i].setOrigin(limites_del_texto.left + limites_del_texto.width / 2.f, limites_del_texto.top + limites_del_texto.height / 2.f); // mueve el punto de anclaje al centro del texto
        _opciones_del_menu[i].setPosition(centro_x_de_la_ventana, posicion_y_de_la_primera_opcion + i * separacion_entre_opciones + 30.f); // posiciona el texto centrado dentro del boton
    }

    _indice_de_la_opcion_seleccionada = 0; // arranca con la primera opcion seleccionada
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void Menu::dibujar(sf::RenderWindow& ventana_del_juego) {
    ventana_del_juego.draw(_sprite_de_fondo); // dibuja la imagen de fondo del menu

    for (int i = 0; i < CANTIDAD_DE_OPCIONES_DEL_MENU; i++) {
        ventana_del_juego.draw(_botones_del_menu[i]); // dibuja el rectangulo de fondo de cada boton
    }
    for (int i = 0; i < CANTIDAD_DE_OPCIONES_DEL_MENU; i++) {
        ventana_del_juego.draw(_opciones_del_menu[i]); // dibuja el texto encima de cada boton
    }
}

///=============================================================///
///   MOVER SELECCION HACIA ARRIBA
///=============================================================///
void Menu::mover_seleccion_hacia_arriba() {
    if (_indice_de_la_opcion_seleccionada > 0) { // solo mueve si no estamos ya en la primera opcion
        _opciones_del_menu[_indice_de_la_opcion_seleccionada].setFillColor(sf::Color(160, 200, 220)); // desresalta la opcion actual
        _indice_de_la_opcion_seleccionada--; // sube una posicion
        _opciones_del_menu[_indice_de_la_opcion_seleccionada].setFillColor(sf::Color(220, 240, 255)); // resalta la nueva opcion seleccionada
    }
}

///=============================================================///
///   MOVER SELECCION HACIA ABAJO
///=============================================================///
void Menu::mover_seleccion_hacia_abajo() {
    if (_indice_de_la_opcion_seleccionada < CANTIDAD_DE_OPCIONES_DEL_MENU - 1) { // solo mueve si no estamos ya en la ultima opcion
        _opciones_del_menu[_indice_de_la_opcion_seleccionada].setFillColor(sf::Color(160, 200, 220)); // desresalta la opcion actual
        _indice_de_la_opcion_seleccionada++; // baja una posicion
        _opciones_del_menu[_indice_de_la_opcion_seleccionada].setFillColor(sf::Color(220, 240, 255)); // resalta la nueva opcion seleccionada
    }
}
