#include "DebugManager.h"
#include <iostream>
#include <cmath>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
DebugManager::DebugManager() {
    _modo_debug_activo = false; // Empieza desactivado
    _objetivo_actual = ObjetivoDebug::NINGUNO; // Sin objetivo seleccionado
}

///=============================================================///
///   ACTIVAR O DESACTIVAR DEBUG
///=============================================================///
void DebugManager::activar_o_desactivar_debug() {
    _modo_debug_activo = !_modo_debug_activo; // Cambia entre activado y desactivado

    if (_modo_debug_activo == true) {
        std::cout << "MODO DEBUG ACTIVADO." << std::endl;
        std::cout << "PRESIONA 1: HUD | 2: PERSONAJE | 3: ENEMIGO | 0: NINGUNO" << std::endl;
    }
    else {
        std::cout << "MODO DEBUG DESACTIVADO." << std::endl;
        _objetivo_actual = ObjetivoDebug::NINGUNO; // Al desactivar, limpia el objetivo
    }
}

///=============================================================///
///   PROCESAR EVENTOS
///=============================================================///
void DebugManager::procesar_eventos(sf::Event& evento, sf::RenderWindow& ventana, UI_Inventario& hud, Personaje& personaje, Golem& enemigo_en_foco) {
    if (_modo_debug_activo == false) return; // Si el debug no esta activo, ignora todo

    if (evento.type == sf::Event::KeyPressed) {

        if (evento.key.code == sf::Keyboard::Num1) _objetivo_actual = ObjetivoDebug::HUD; // Tecla 1: apunta al HUD
        if (evento.key.code == sf::Keyboard::Num2) _objetivo_actual = ObjetivoDebug::PERSONAJE; // Tecla 2: apunta al personaje
        if (evento.key.code == sf::Keyboard::Num3) _objetivo_actual = ObjetivoDebug::ENEMIGO; // Tecla 3: apunta al enemigo
        if (evento.key.code == sf::Keyboard::Num0) _objetivo_actual = ObjetivoDebug::NINGUNO; // Tecla 0: desactiva el objetivo

        if (_objetivo_actual == ObjetivoDebug::HUD) {
            if (evento.key.code == sf::Keyboard::Up) hud.ajustar_posicion(0.f, -5.f); // Flecha arriba: sube el HUD
            if (evento.key.code == sf::Keyboard::Down) hud.ajustar_posicion(0.f, 5.f); // Flecha abajo: baja el HUD
            if (evento.key.code == sf::Keyboard::Left) hud.ajustar_posicion(-5.f, 0.f); // Flecha izquierda: mueve el HUD
            if (evento.key.code == sf::Keyboard::Right) hud.ajustar_posicion(5.f, 0.f); // Flecha derecha: mueve el HUD
            if (evento.key.code == sf::Keyboard::U) hud.ajustar_origen(-1.f, 0.f); // U: mueve el origen del HUD
            if (evento.key.code == sf::Keyboard::O) hud.ajustar_origen(1.f, 0.f); // O: mueve el origen del HUD
        }
        else if (_objetivo_actual == ObjetivoDebug::PERSONAJE) {
            if (evento.key.code == sf::Keyboard::Up) personaje.ajustar_origen_del_sprite(0.f, -1.f); // Sube el origen del sprite
            if (evento.key.code == sf::Keyboard::Down) personaje.ajustar_origen_del_sprite(0.f, 1.f); // Baja el origen del sprite
            if (evento.key.code == sf::Keyboard::Left) personaje.ajustar_origen_del_sprite(-1.f, 0.f); // Mueve el origen a la izquierda
            if (evento.key.code == sf::Keyboard::Right) personaje.ajustar_origen_del_sprite(1.f, 0.f); // Mueve el origen a la derecha
        }
        else if (_objetivo_actual == ObjetivoDebug::ENEMIGO) {
            if (enemigo_en_foco.getEsta_viva() == true) {
                if (evento.key.code == sf::Keyboard::Up) enemigo_en_foco.ajustar_origen_del_sprite(0.f, -1.f); // Sube el origen
                if (evento.key.code == sf::Keyboard::Down) enemigo_en_foco.ajustar_origen_del_sprite(0.f, 1.f); // Baja el origen
                if (evento.key.code == sf::Keyboard::Left) enemigo_en_foco.ajustar_origen_del_sprite(-1.f, 0.f); // Mueve a la izquierda
                if (evento.key.code == sf::Keyboard::Right) enemigo_en_foco.ajustar_origen_del_sprite(1.f, 0.f); // Mueve a la derecha
            }
        }
    }
}

///=============================================================///
///   ACTUALIZAR - Movimiento continuo con flechas mantenidas
///=============================================================///
void DebugManager::actualizar(UI_Inventario& hud, Personaje& personaje, Golem& enemigo_en_foco) {
    if (_modo_debug_activo == false || _objetivo_actual == ObjetivoDebug::NINGUNO) return; // Si no hay objetivo activo, no hace nada

    if (_objetivo_actual == ObjetivoDebug::ENEMIGO) {
        if (enemigo_en_foco.getEsta_viva() == false) {
            _objetivo_actual = ObjetivoDebug::NINGUNO; // El enemigo murio, limpia el objetivo
            std::cout << "EL ENEMIGO SELECCIONADO MURIO. OBJETIVO: NINGUNO." << std::endl;
            return;
        }
    }

    float dx = 0.f; // Movimiento horizontal acumulado
    float dy = 0.f; // Movimiento vertical acumulado
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dy = -1.f; // Flecha arriba: mover hacia arriba
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dy = 1.f; // Flecha abajo: mover hacia abajo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx = -1.f; // Flecha izquierda: mover a la izquierda
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx = 1.f; // Flecha derecha: mover a la derecha

    bool hay_movimiento = (dx != 0.f || dy != 0.f); // Verdadero si alguna flecha esta presionada
    bool hay_tecla_u = sf::Keyboard::isKeyPressed(sf::Keyboard::U); // U presionada
    bool hay_tecla_o = sf::Keyboard::isKeyPressed(sf::Keyboard::O); // O presionada

    if (hay_movimiento == false && hay_tecla_u == false && hay_tecla_o == false) return; // Sin input, no hace nada

    if (_objetivo_actual == ObjetivoDebug::HUD) {
        hud.ajustar_posicion(dx * 2.f, dy * 2.f); // Mueve el HUD con las flechas
        if (hay_tecla_u == true) hud.ajustar_origen(-1.f, 0.f); // U ajusta el origen
        if (hay_tecla_o == true) hud.ajustar_origen(1.f, 0.f); // O ajusta el origen
    }
    else if (_objetivo_actual == ObjetivoDebug::PERSONAJE) {
        personaje.ajustar_origen_del_sprite(dx, dy); // Mueve el origen del personaje
    }
    else if (_objetivo_actual == ObjetivoDebug::ENEMIGO) {
        if (enemigo_en_foco.getEsta_viva() == true) {
            enemigo_en_foco.ajustar_origen_del_sprite(dx, dy); // Mueve el origen del enemigo
        }
    }
}

///=============================================================///
///   DIBUJAR CAJA DE COLISION
///=============================================================///
void DebugManager::dibujar_caja_de_colision(sf::RenderWindow& ventana, sf::FloatRect limites_de_la_caja, sf::Color color) const {
    if (_modo_debug_activo == false) return; // Solo dibuja si el debug esta activo

    sf::RectangleShape caja_visual;
    caja_visual.setPosition(limites_de_la_caja.left, limites_de_la_caja.top); // Posicion de la esquina superior izquierda
    caja_visual.setSize(sf::Vector2f(limites_de_la_caja.width, limites_de_la_caja.height)); // Tamanio de la caja
    caja_visual.setFillColor(sf::Color(color.r, color.g, color.b, 80)); // Interior semitransparente del color dado
    caja_visual.setOutlineColor(color); // Borde del color dado
    caja_visual.setOutlineThickness(1.f); // Borde de 1 pixel de grosor

    ventana.draw(caja_visual); // Dibuja la caja en pantalla
}

///=============================================================///
///   PROCESAR CLIC EN EL MAPA
///=============================================================///
void DebugManager::procesar_clic_en_el_mapa(sf::Vector2i posicion_del_clic, const sf::View& vista_activa, const sf::RenderWindow& ventana) {
    if (_modo_debug_activo == false) return; // Solo funciona con debug activo

    sf::Vector2f posicion_en_el_mundo = ventana.mapPixelToCoords(posicion_del_clic, vista_activa); // Convierte el clic a coordenadas del mapa

    float tile_x = std::floor(posicion_en_el_mundo.x / 32.f) * 32.f; // Redondea al tile mas cercano en X
    float tile_y = std::floor(posicion_en_el_mundo.y / 32.f) * 32.f; // Redondea al tile mas cercano en Y

    _posicion_del_tile_marcado = sf::Vector2f(tile_x, tile_y); // Guarda la posicion del tile clicado
    _hay_tile_marcado = true; // Hay un tile para mostrar en pantalla

    std::cout << "TILE MARCADO EN X: " << tile_x << " | Y: " << tile_y << std::endl;
}

///=============================================================///
///   DIBUJAR GRILLA DEL MAPA
///=============================================================///
void DebugManager::dibujar_grilla_del_mapa(sf::RenderWindow& ventana) const {
    if (_modo_debug_activo == false || _hay_tile_marcado == false) return; // Solo si hay algo marcado

    sf::RectangleShape marca_del_tile(sf::Vector2f(32.f, 32.f)); // Rectangulo del tamanio de un tile
    marca_del_tile.setPosition(_posicion_del_tile_marcado); // Lo pone encima del tile marcado
    marca_del_tile.setFillColor(sf::Color(255, 255, 0, 60)); // Interior amarillo semitransparente
    marca_del_tile.setOutlineColor(sf::Color::Yellow); // Borde amarillo
    marca_del_tile.setOutlineThickness(1.5f); // Grosor del borde

    ventana.draw(marca_del_tile); // Dibuja la marca sobre el mapa
}
