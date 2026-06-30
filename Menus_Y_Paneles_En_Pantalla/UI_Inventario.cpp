#include "UI_Inventario.h"
#include <iostream>
#include <string>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
UI_Inventario::UI_Inventario() {
    _el_inventario_esta_abierto = false;
    _tamano_de_cada_casillero = 50.f;
    _margen_entre_casilleros = 10.f;
    _desplazamiento_x = 0.f;
    _desplazamiento_y = 0.f;

    _fondo_del_casillero.setSize(sf::Vector2f(_tamano_de_cada_casillero, _tamano_de_cada_casillero));
    _fondo_del_casillero.setFillColor(sf::Color(40, 40, 40, 200));
    _fondo_del_casillero.setOutlineColor(sf::Color::White);
    _fondo_del_casillero.setOutlineThickness(2.f);

    if (_fuente_del_inventario.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR: NO SE ENCONTRO LA FUENTE PARA EL INVENTARIO." << std::endl;
    }

    _texto_de_la_cantidad.setFont(_fuente_del_inventario);
    _texto_de_la_cantidad.setCharacterSize(14);
    _texto_de_la_cantidad.setFillColor(sf::Color::White);
    _texto_de_la_cantidad.setOutlineColor(sf::Color::Black);
    _texto_de_la_cantidad.setOutlineThickness(1.f);
}

///=============================================================///
///   AJUSTAR POSICION Y ORIGEN
///=============================================================///
void UI_Inventario::ajustar_posicion(float desplazamiento_x, float desplazamiento_y) {
    _desplazamiento_x += desplazamiento_x;
    _desplazamiento_y += desplazamiento_y;
    std::cout << "HUD POS -> X: " << _desplazamiento_x << " | Y: " << _desplazamiento_y << std::endl;
}

void UI_Inventario::ajustar_origen(float origen_x, float origen_y) {
    _origen_x += origen_x;
    _origen_y += origen_y;
    std::cout << "HUD ORIGEN -> X: " << _origen_x << " | Y: " << _origen_y << std::endl;
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void UI_Inventario::dibujar(sf::RenderWindow& ventana_del_juego, const Inventario& mochila) {
    if (_el_inventario_esta_abierto == false) return;

    sf::View vista_original = ventana_del_juego.getView();
    ventana_del_juego.setView(ventana_del_juego.getDefaultView());

    _fondo_del_casillero.setOrigin(_origen_x, _origen_y);

    int indice_del_slot_seleccionado = mochila.getIndice_del_slot_seleccionado();
    int cantidad_de_items_guardados = mochila.getCantidad_de_items_guardados();

    float ancho_total_de_la_grilla = (CANTIDAD_DE_CASILLEROS_VISIBLES * _tamano_de_cada_casillero) + ((CANTIDAD_DE_CASILLEROS_VISIBLES - 1) * _margen_entre_casilleros);
    float posicion_x_inicial = ((ventana_del_juego.getSize().x - ancho_total_de_la_grilla) / 2.f) + _desplazamiento_x;
    float posicion_y_inicial = (ventana_del_juego.getSize().y - _tamano_de_cada_casillero - 20.f) + _desplazamiento_y;

    _area_total_del_panel = sf::FloatRect(posicion_x_inicial, posicion_y_inicial, ancho_total_de_la_grilla, _tamano_de_cada_casillero);

    for (int i = 0; i < CANTIDAD_DE_CASILLEROS_VISIBLES; i++) {

        float posicion_x_del_casillero = posicion_x_inicial + i * (_tamano_de_cada_casillero + _margen_entre_casilleros);

        _fondo_del_casillero.setPosition(posicion_x_del_casillero, posicion_y_inicial);

        if (i == indice_del_slot_seleccionado) {
            _fondo_del_casillero.setOutlineThickness(3.f);
            _fondo_del_casillero.setOutlineColor(sf::Color::Green);
        }
        else {
            _fondo_del_casillero.setOutlineThickness(2.f);
            _fondo_del_casillero.setOutlineColor(sf::Color::White);
        }
        ventana_del_juego.draw(_fondo_del_casillero);

        if (i < cantidad_de_items_guardados) {

            const Item& item_de_este_casillero = mochila.getItem_en_el_slot(i);

            sf::Sprite sprite_del_item = item_de_este_casillero.getSprite();
            sprite_del_item.setPosition(posicion_x_del_casillero + 9.f, posicion_y_inicial + 9.f);
            ventana_del_juego.draw(sprite_del_item);

            int cantidad_de_este_item = item_de_este_casillero.getCantidad();
            if (cantidad_de_este_item > 1) {
                _texto_de_la_cantidad.setString(std::to_string(cantidad_de_este_item));
                _texto_de_la_cantidad.setPosition(posicion_x_del_casillero + _tamano_de_cada_casillero - 20.f, posicion_y_inicial + _tamano_de_cada_casillero - 20.f);
                ventana_del_juego.draw(_texto_de_la_cantidad);
            }
        }
    }

    ventana_del_juego.setView(vista_original);
}

///=============================================================///
///   DETECTAR CLIC EN UN CASILLERO
///=============================================================///
void UI_Inventario::detectar_clic_en_un_casillero(sf::Vector2i posicion_del_mouse, Inventario& mochila, const sf::RenderWindow& ventana_del_juego) {

    sf::Vector2f posicion_del_mouse_en_la_ui = ventana_del_juego.mapPixelToCoords(posicion_del_mouse, ventana_del_juego.getDefaultView());

    float ancho_total_de_la_grilla = (CANTIDAD_DE_CASILLEROS_VISIBLES * _tamano_de_cada_casillero) + ((CANTIDAD_DE_CASILLEROS_VISIBLES - 1) * _margen_entre_casilleros);
    float posicion_x_inicial = ((ventana_del_juego.getSize().x - ancho_total_de_la_grilla) / 2.f) + _desplazamiento_x;
    float posicion_y_inicial = (ventana_del_juego.getSize().y - _tamano_de_cada_casillero - 20.f) + _desplazamiento_y;

    for (int i = 0; i < CANTIDAD_DE_CASILLEROS_VISIBLES; i++) {

        float posicion_x_del_casillero = posicion_x_inicial + i * (_tamano_de_cada_casillero + _margen_entre_casilleros);
        sf::FloatRect limites_del_casillero(posicion_x_del_casillero, posicion_y_inicial, _tamano_de_cada_casillero, _tamano_de_cada_casillero);

        if (limites_del_casillero.contains(posicion_del_mouse_en_la_ui) == true) {

            if (i < mochila.getCantidad_de_items_guardados()) {
                mochila.setIndice_del_slot_seleccionado(i);
                std::cout << "ITEM SELECCIONADO EN EL CASILLERO " << i << "." << std::endl;
            }
            else {
                mochila.setIndice_del_slot_seleccionado(-1);
                std::cout << "CASILLERO VACIO." << std::endl;
            }
            return;
        }
    }
}

///=============================================================///
///   CALCULAR SI EL MOUSE ESTA SOBRE EL PANEL
///=============================================================///
bool UI_Inventario::getEl_mouse_esta_sobre_el_panel(sf::Vector2i posicion_del_mouse) const {
    if (_el_inventario_esta_abierto == false) return false;
    return _area_total_del_panel.contains((float)posicion_del_mouse.x, (float)posicion_del_mouse.y);
}