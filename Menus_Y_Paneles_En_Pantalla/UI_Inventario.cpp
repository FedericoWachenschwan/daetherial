#include "UI_Inventario.h"
#include <iostream>
#include <string>

///=============================================================///
///   #1 - CONSTRUCTOR
///=============================================================///
// #1
UI_Inventario::UI_Inventario() {
    _el_inventario_esta_abierto = false; // Empieza oculto
    _tamano_de_cada_casillero = 50.f; // Cada casillero mide 50x50 pixeles
    _margen_entre_casilleros = 10.f; // 10 pixeles de espacio entre casilleros
    _desplazamiento_x = 0.f; // Sin desplazamiento inicial
    _desplazamiento_y = 0.f; // Sin desplazamiento inicial

    _fondo_del_casillero.setSize(sf::Vector2f(_tamano_de_cada_casillero, _tamano_de_cada_casillero)); // Tamanio del rectangulo del casillero
    _fondo_del_casillero.setFillColor(sf::Color(40, 40, 40, 200)); // Fondo gris oscuro semitransparente
    _fondo_del_casillero.setOutlineColor(sf::Color::White); // Borde blanco
    _fondo_del_casillero.setOutlineThickness(2.f); // Grosor del borde

    if (_fuente_del_inventario.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR: NO SE ENCONTRO LA FUENTE PARA EL INVENTARIO." << std::endl;
    }

    _texto_de_la_cantidad.setFont(_fuente_del_inventario); // Asigna la fuente al texto de cantidad
    _texto_de_la_cantidad.setCharacterSize(14); // Tamanio de letra
    _texto_de_la_cantidad.setFillColor(sf::Color::White); // Color del texto
    _texto_de_la_cantidad.setOutlineColor(sf::Color::Black); // Contorno negro para legibilidad
    _texto_de_la_cantidad.setOutlineThickness(1.f); // Grosor del contorno
}

///=============================================================///
///   #2 - AJUSTAR POSICION Y ORIGEN
///=============================================================///
// #2
void UI_Inventario::ajustar_posicion(float desplazamiento_x, float desplazamiento_y) {
    _desplazamiento_x += desplazamiento_x; // Suma el nuevo desplazamiento
    _desplazamiento_y += desplazamiento_y; // Suma el nuevo desplazamiento
    std::cout << "HUD POS -> X: " << _desplazamiento_x << " | Y: " << _desplazamiento_y << std::endl;
}

// #3
void UI_Inventario::ajustar_origen(float origen_x, float origen_y) {
    _origen_x += origen_x; // Mueve el origen en X
    _origen_y += origen_y; // Mueve el origen en Y
    std::cout << "HUD ORIGEN -> X: " << _origen_x << " | Y: " << _origen_y << std::endl;
}

///=============================================================///
///   #4 - DIBUJAR
///=============================================================///
// #4
void UI_Inventario::dibujar(sf::RenderWindow& ventana_del_juego, const Inventario& mochila) {
    if (_el_inventario_esta_abierto == false) return; // Si esta cerrado no dibuja nada

    sf::View vista_original = ventana_del_juego.getView(); // Guarda la vista del mundo
    ventana_del_juego.setView(ventana_del_juego.getDefaultView()); // Cambia a vista de pantalla (UI no se mueve con la camara)

    _fondo_del_casillero.setOrigin(_origen_x, _origen_y); // Aplica el offset de origen configurado

    int indice_del_slot_seleccionado = mochila.getIndice_del_slot_seleccionado(); // Cual casillero tiene el foco
    int cantidad_de_items_guardados = mochila.getCantidad_de_items_guardados(); // Cuantos items tiene la mochila

    float ancho_total_de_la_grilla = (CANTIDAD_DE_CASILLEROS_VISIBLES * _tamano_de_cada_casillero) + ((CANTIDAD_DE_CASILLEROS_VISIBLES - 1) * _margen_entre_casilleros); // Ancho total del panel
    float posicion_x_inicial = ((ventana_del_juego.getSize().x - ancho_total_de_la_grilla) / 2.f) + _desplazamiento_x; // Centra el panel horizontalmente
    float posicion_y_inicial = (ventana_del_juego.getSize().y - _tamano_de_cada_casillero - 20.f) + _desplazamiento_y; // Lo coloca cerca del borde inferior

    for (int i = 0; i < CANTIDAD_DE_CASILLEROS_VISIBLES; i++) {

        float posicion_x_del_casillero = posicion_x_inicial + i * (_tamano_de_cada_casillero + _margen_entre_casilleros); // Posicion X de este casillero

        _fondo_del_casillero.setPosition(posicion_x_del_casillero, posicion_y_inicial); // Coloca el casillero en su lugar

        if (i == indice_del_slot_seleccionado) {
            _fondo_del_casillero.setOutlineThickness(3.f); // Borde mas grueso para el seleccionado
            _fondo_del_casillero.setOutlineColor(sf::Color::Green); // Borde verde para indicar seleccion
        }
        else {
            _fondo_del_casillero.setOutlineThickness(2.f); // Borde normal para el resto
            _fondo_del_casillero.setOutlineColor(sf::Color::White); // Borde blanco normal
        }
        ventana_del_juego.draw(_fondo_del_casillero); // Dibuja el fondo del casillero

        if (i < cantidad_de_items_guardados) {

            const Item& item_de_este_casillero = mochila.getItem_en_el_slot(i); // Referencia al item en este slot

            sf::Sprite sprite_del_item = item_de_este_casillero.getSprite(); // Copia del sprite para posicionarlo
            sf::FloatRect bounds_del_sprite = sprite_del_item.getGlobalBounds();
            float escala_actual = sprite_del_item.getScale().x;
            float ajuste = std::min(_tamano_de_cada_casillero / bounds_del_sprite.width, _tamano_de_cada_casillero / bounds_del_sprite.height);
            if (ajuste < 1.f) {
                sprite_del_item.setScale(escala_actual * ajuste, escala_actual * ajuste); // Achica el sprite para que entre en el casillero
                bounds_del_sprite = sprite_del_item.getGlobalBounds();
            }
            float offset_x = (_tamano_de_cada_casillero - bounds_del_sprite.width)  / 2.f; // Centra horizontalmente
            float offset_y = (_tamano_de_cada_casillero - bounds_del_sprite.height) / 2.f; // Centra verticalmente
            sprite_del_item.setPosition(posicion_x_del_casillero + offset_x, posicion_y_inicial + offset_y);
            ventana_del_juego.draw(sprite_del_item); // Dibuja el icono del item

            int cantidad_de_este_item = item_de_este_casillero.getCantidad(); // Cuantos hay apilados
            if (cantidad_de_este_item > 1) {
                _texto_de_la_cantidad.setString(std::to_string(cantidad_de_este_item)); // Convierte el numero a texto
                _texto_de_la_cantidad.setPosition(posicion_x_del_casillero + _tamano_de_cada_casillero - 20.f, posicion_y_inicial + _tamano_de_cada_casillero - 20.f); // En la esquina inferior derecha
                ventana_del_juego.draw(_texto_de_la_cantidad); // Dibuja el numero de cantidad
            }
        }
    }

    ventana_del_juego.setView(vista_original); // Restaura la vista del mundo
}

///=============================================================///
///   #5 - DETECTAR CLIC EN UN CASILLERO
///=============================================================///
// #5
void UI_Inventario::detectar_clic_en_un_casillero(sf::Vector2i posicion_del_mouse, Inventario& mochila, const sf::RenderWindow& ventana_del_juego) {

    sf::Vector2f posicion_del_mouse_en_la_ui = ventana_del_juego.mapPixelToCoords(posicion_del_mouse, ventana_del_juego.getDefaultView()); // Convierte el clic a coordenadas de UI

    float ancho_total_de_la_grilla = (CANTIDAD_DE_CASILLEROS_VISIBLES * _tamano_de_cada_casillero) + ((CANTIDAD_DE_CASILLEROS_VISIBLES - 1) * _margen_entre_casilleros); // Ancho total del panel
    float posicion_x_inicial = ((ventana_del_juego.getSize().x - ancho_total_de_la_grilla) / 2.f) + _desplazamiento_x; // Posicion X inicial del panel
    float posicion_y_inicial = (ventana_del_juego.getSize().y - _tamano_de_cada_casillero - 20.f) + _desplazamiento_y; // Posicion Y inicial del panel

    for (int i = 0; i < CANTIDAD_DE_CASILLEROS_VISIBLES; i++) {

        float posicion_x_del_casillero = posicion_x_inicial + i * (_tamano_de_cada_casillero + _margen_entre_casilleros); // Posicion X de este casillero
        sf::FloatRect limites_del_casillero(posicion_x_del_casillero, posicion_y_inicial, _tamano_de_cada_casillero, _tamano_de_cada_casillero); // Rectangulo que ocupa el casillero

        if (limites_del_casillero.contains(posicion_del_mouse_en_la_ui) == true) {

            if (i < mochila.getCantidad_de_items_guardados()) {
                mochila.setIndice_del_slot_seleccionado(i); // Selecciona este slot
                std::cout << "ITEM SELECCIONADO EN EL CASILLERO " << i << "." << std::endl;
            }
            else {
                mochila.setIndice_del_slot_seleccionado(-1); // Deselecciona si el casillero esta vacio
                std::cout << "CASILLERO VACIO." << std::endl;
            }
            return; // Ya encontro el casillero, no sigue buscando
        }
    }
}

