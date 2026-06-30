#include "DebugManager.h"
#include <iostream>
#include <cmath>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
DebugManager::DebugManager() {
    _modo_debug_activo = false;
    _objetivo_actual = ObjetivoDebug::NINGUNO;
    _offset_del_extractor = sf::Vector2f(0.f, 0.f);
}

///=============================================================///
///   ACTIVAR O DESACTIVAR DEBUG
///=============================================================///
void DebugManager::activar_o_desactivar_debug() {
    _modo_debug_activo = !_modo_debug_activo;

    if (_modo_debug_activo == true) {
        std::cout << "MODO DEBUG ACTIVADO." << std::endl;
        std::cout << "PRESIONA 1: HUD | 2: PERSONAJE | 3: ENEMIGO | 4: EXTRACTOR | 0: NINGUNO" << std::endl;
    }
    else {
        std::cout << "MODO DEBUG DESACTIVADO." << std::endl;
        _objetivo_actual = ObjetivoDebug::NINGUNO;
    }
}

///=============================================================///
///   PROCESAR EVENTOS
///=============================================================///
void DebugManager::procesar_eventos(sf::Event& evento, sf::RenderWindow& ventana, UI_Inventario& hud, Personaje& personaje, Golem& enemigo_en_foco) {
    if (_modo_debug_activo == false) return;

    if (evento.type == sf::Event::MouseButtonPressed && _objetivo_actual == ObjetivoDebug::EXTRACTOR) {
        if (evento.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i posicion_del_mouse_en_pantalla = sf::Mouse::getPosition(ventana);
            sf::Vector2f posicion_en_el_mundo = ventana.mapPixelToCoords(posicion_del_mouse_en_pantalla);

            float posicion_x_relativa = posicion_en_el_mundo.x - _offset_del_extractor.x;
            float posicion_y_relativa = posicion_en_el_mundo.y - _offset_del_extractor.y;

            if (posicion_x_relativa >= 0.f && posicion_y_relativa >= 0.f) {
                int columna_del_tile = (int)posicion_x_relativa / 32;
                int fila_del_tile = (int)posicion_y_relativa / 32;
                int id_de_la_textura = (fila_del_tile * 64) + columna_del_tile;

                std::cout << "ID DE LA TEXTURA: " << id_de_la_textura << " (FILA: " << fila_del_tile << " | COLUMNA: " << columna_del_tile << ")" << std::endl;
            }
        }
    }

    if (evento.type == sf::Event::KeyPressed) {

        if (evento.key.code == sf::Keyboard::Num1) _objetivo_actual = ObjetivoDebug::HUD;
        if (evento.key.code == sf::Keyboard::Num2) _objetivo_actual = ObjetivoDebug::PERSONAJE;
        if (evento.key.code == sf::Keyboard::Num3) _objetivo_actual = ObjetivoDebug::ENEMIGO;
        if (evento.key.code == sf::Keyboard::Num4) _objetivo_actual = ObjetivoDebug::EXTRACTOR;
        if (evento.key.code == sf::Keyboard::Num0) _objetivo_actual = ObjetivoDebug::NINGUNO;

        if (_objetivo_actual == ObjetivoDebug::HUD) {
            if (evento.key.code == sf::Keyboard::Up) hud.ajustar_posicion(0.f, -5.f);
            if (evento.key.code == sf::Keyboard::Down) hud.ajustar_posicion(0.f, 5.f);
            if (evento.key.code == sf::Keyboard::Left) hud.ajustar_posicion(-5.f, 0.f);
            if (evento.key.code == sf::Keyboard::Right) hud.ajustar_posicion(5.f, 0.f);
            if (evento.key.code == sf::Keyboard::U) hud.ajustar_origen(-1.f, 0.f);
            if (evento.key.code == sf::Keyboard::O) hud.ajustar_origen(1.f, 0.f);
        }
        else if (_objetivo_actual == ObjetivoDebug::PERSONAJE) {
            if (evento.key.code == sf::Keyboard::Up) personaje.ajustar_origen_del_sprite(0.f, -1.f);
            if (evento.key.code == sf::Keyboard::Down) personaje.ajustar_origen_del_sprite(0.f, 1.f);
            if (evento.key.code == sf::Keyboard::Left) personaje.ajustar_origen_del_sprite(-1.f, 0.f);
            if (evento.key.code == sf::Keyboard::Right) personaje.ajustar_origen_del_sprite(1.f, 0.f);
        }
        else if (_objetivo_actual == ObjetivoDebug::ENEMIGO) {
            if (enemigo_en_foco.getEsta_viva() == true) {
                if (evento.key.code == sf::Keyboard::Up) enemigo_en_foco.ajustar_origen_del_sprite(0.f, -1.f);
                if (evento.key.code == sf::Keyboard::Down) enemigo_en_foco.ajustar_origen_del_sprite(0.f, 1.f);
                if (evento.key.code == sf::Keyboard::Left) enemigo_en_foco.ajustar_origen_del_sprite(-1.f, 0.f);
                if (evento.key.code == sf::Keyboard::Right) enemigo_en_foco.ajustar_origen_del_sprite(1.f, 0.f);
            }
        }
    }
}

///=============================================================///
///   ACTUALIZAR - Movimiento continuo con flechas mantenidas
///=============================================================///
void DebugManager::actualizar(UI_Inventario& hud, Personaje& personaje, Golem& enemigo_en_foco) {
    if (_modo_debug_activo == false || _objetivo_actual == ObjetivoDebug::NINGUNO) return;

    if (_objetivo_actual == ObjetivoDebug::ENEMIGO) {
        if (enemigo_en_foco.getEsta_viva() == false) {
            _objetivo_actual = ObjetivoDebug::NINGUNO;
            std::cout << "EL ENEMIGO SELECCIONADO MURIO. OBJETIVO: NINGUNO." << std::endl;
            return;
        }
    }

    float dx = 0.f;
    float dy = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dy = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dy = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx = 1.f;

    bool hay_movimiento = (dx != 0.f || dy != 0.f);
    bool hay_tecla_u = sf::Keyboard::isKeyPressed(sf::Keyboard::U);
    bool hay_tecla_o = sf::Keyboard::isKeyPressed(sf::Keyboard::O);

    if (hay_movimiento == false && hay_tecla_u == false && hay_tecla_o == false) return;

    if (_objetivo_actual == ObjetivoDebug::HUD) {
        hud.ajustar_posicion(dx * 2.f, dy * 2.f);
        if (hay_tecla_u == true) hud.ajustar_origen(-1.f, 0.f);
        if (hay_tecla_o == true) hud.ajustar_origen(1.f, 0.f);
    }
    else if (_objetivo_actual == ObjetivoDebug::PERSONAJE) {
        personaje.ajustar_origen_del_sprite(dx, dy);
    }
    else if (_objetivo_actual == ObjetivoDebug::ENEMIGO) {
        if (enemigo_en_foco.getEsta_viva() == true) {
            enemigo_en_foco.ajustar_origen_del_sprite(dx, dy);
        }
    }
    else if (_objetivo_actual == ObjetivoDebug::EXTRACTOR) {
        _offset_del_extractor.x += dx * -25.f;
        _offset_del_extractor.y += dy * -25.f;
    }
}

///=============================================================///
///   DIBUJAR CAJA DE COLISION
///=============================================================///
void DebugManager::dibujar_caja_de_colision(sf::RenderWindow& ventana, sf::FloatRect limites_de_la_caja, sf::Color color) const {
    if (_modo_debug_activo == false) return;

    sf::RectangleShape caja_visual;
    caja_visual.setPosition(limites_de_la_caja.left, limites_de_la_caja.top);
    caja_visual.setSize(sf::Vector2f(limites_de_la_caja.width, limites_de_la_caja.height));
    caja_visual.setFillColor(sf::Color(color.r, color.g, color.b, 80));
    caja_visual.setOutlineColor(color);
    caja_visual.setOutlineThickness(1.f);

    ventana.draw(caja_visual);
}

///=============================================================///
///   DIBUJAR EXTRACTOR
///=============================================================///
void DebugManager::dibujar_extractor(sf::RenderWindow& ventana, const sf::Texture& textura_maestra) {
    if (_modo_debug_activo == false || _objetivo_actual != ObjetivoDebug::EXTRACTOR) return;

    ventana.clear(sf::Color(15, 15, 15));
    ventana.setView(ventana.getDefaultView());

    sf::Sprite sprite_del_spritesheet(textura_maestra);
    sprite_del_spritesheet.setPosition(_offset_del_extractor);
    ventana.draw(sprite_del_spritesheet);
}

///=============================================================///
///   PROCESAR CLIC EN EL MAPA
///=============================================================///
void DebugManager::procesar_clic_en_el_mapa(sf::Vector2i posicion_del_clic, const sf::View& vista_activa, const sf::RenderWindow& ventana) {
    if (_modo_debug_activo == false) return;

    sf::Vector2f posicion_en_el_mundo = ventana.mapPixelToCoords(posicion_del_clic, vista_activa);

    float tile_x = std::floor(posicion_en_el_mundo.x / 32.f) * 32.f;
    float tile_y = std::floor(posicion_en_el_mundo.y / 32.f) * 32.f;

    _posicion_del_tile_marcado = sf::Vector2f(tile_x, tile_y);
    _hay_tile_marcado = true;

    std::cout << "TILE MARCADO EN X: " << tile_x << " | Y: " << tile_y << std::endl;
}

///=============================================================///
///   DIBUJAR GRILLA DEL MAPA
///=============================================================///
void DebugManager::dibujar_grilla_del_mapa(sf::RenderWindow& ventana) const {
    if (_modo_debug_activo == false || _hay_tile_marcado == false) return;

    sf::RectangleShape marca_del_tile(sf::Vector2f(32.f, 32.f));
    marca_del_tile.setPosition(_posicion_del_tile_marcado);
    marca_del_tile.setFillColor(sf::Color(255, 255, 0, 60));
    marca_del_tile.setOutlineColor(sf::Color::Yellow);
    marca_del_tile.setOutlineThickness(1.5f);

    ventana.draw(marca_del_tile);
}