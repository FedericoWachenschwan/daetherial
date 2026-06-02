#include "DebugManager.h"
#include "UI_Inventario.h"
#include "Personaje.h"
#include "Enemy.h" // 🌟 ESTO FALTABA: Sin esto no podemos usar al Gólem
#include "Map.h"
#include <iostream>

DebugManager::DebugManager() {
    _modoDebugActivo = false;
    _objetivoActual = ObjetivoDebug::NINGUNO;
}

void DebugManager::toggleDebug() {
    _modoDebugActivo = !_modoDebugActivo;
    if (_modoDebugActivo) {
        std::cout << "🔧 MODO DEBUG ACTIVADO" << std::endl;
        std::cout << "👉 Presiona 1 para HUD | 2 para Personaje | 3 para Enemigo | 0 para Ninguno" << std::endl;
    }
    else {
        std::cout << "🎮 MODO DEBUG DESACTIVADO" << std::endl;
        _objetivoActual = ObjetivoDebug::NINGUNO;
    }
}

// =====================================================================
// SELECTOR DE OBJETIVO (Eventos únicos de teclado)
// =====================================================================
void DebugManager::procesarEventos(sf::Event& evento, UI_Inventario& hud, Personaje& personaje, Enemy& enemigo) {
    if (!_modoDebugActivo) return;

    if (evento.type == sf::Event::KeyPressed) {

        // ==========================================
        // 1. SELECTOR DE OBJETIVO (Teclas 1, 2, 3, 0)
        // ==========================================
        if (evento.key.code == sf::Keyboard::Num1) {
            _objetivoActual = ObjetivoDebug::HUD;
            std::cout << "🎯 [MODO EDICIÓN]: HUD del Inventario seleccionado." << std::endl;
        }
        else if (evento.key.code == sf::Keyboard::Num2) {
            _objetivoActual = ObjetivoDebug::PERSONAJE;
            std::cout << "🎯 [MODO EDICIÓN]: Origen del Personaje seleccionado." << std::endl;
        }
        else if (evento.key.code == sf::Keyboard::Num3) {
            _objetivoActual = ObjetivoDebug::ENEMIGO;
            std::cout << "🎯 [MODO EDICIÓN]: Origen del Enemigo seleccionado." << std::endl;
        }
        else if (evento.key.code == sf::Keyboard::Num0) {
            _objetivoActual = ObjetivoDebug::NINGUNO;
            std::cout << "🎯 [MODO EDICIÓN]: Ningún objeto seleccionado." << std::endl;
        }

        // ==========================================
        // 2. EL CONSOLA DE MANDOS (Las flechitas)
        // ==========================================
        switch (_objetivoActual) {

        case ObjetivoDebug::HUD:
            if (evento.key.code == sf::Keyboard::Up) hud.ajustarPosicion(0.f, -5.f);
            if (evento.key.code == sf::Keyboard::Down) hud.ajustarPosicion(0.f, 5.f);
            if (evento.key.code == sf::Keyboard::Left) hud.ajustarPosicion(-5.f, 0.f);
            if (evento.key.code == sf::Keyboard::Right) hud.ajustarPosicion(5.f, 0.f);
            if (evento.key.code == sf::Keyboard::U) hud.ajustarOrigen(-1.f, 0.f);
            if (evento.key.code == sf::Keyboard::O) hud.ajustarOrigen(1.f, 0.f);
            break;

        case ObjetivoDebug::PERSONAJE:
            if (evento.key.code == sf::Keyboard::Up) personaje.ajustarOrigenSprite(0.f, -1.f);
            if (evento.key.code == sf::Keyboard::Down) personaje.ajustarOrigenSprite(0.f, 1.f);
            if (evento.key.code == sf::Keyboard::Left) personaje.ajustarOrigenSprite(-1.f, 0.f);
            if (evento.key.code == sf::Keyboard::Right) personaje.ajustarOrigenSprite(1.f, 0.f);
            break;

        case ObjetivoDebug::ENEMIGO:
            if (evento.key.code == sf::Keyboard::Up) enemigo.ajustarOrigenSprite(0.f, -1.f);
            if (evento.key.code == sf::Keyboard::Down) enemigo.ajustarOrigenSprite(0.f, 1.f);
            if (evento.key.code == sf::Keyboard::Left) enemigo.ajustarOrigenSprite(-1.f, 0.f);
            if (evento.key.code == sf::Keyboard::Right) enemigo.ajustarOrigenSprite(1.f, 0.f);
            break;

        case ObjetivoDebug::NINGUNO:
        default:
            break;
        }
    }
}

// =====================================================================
// EDICIÓN EN TIEMPO REAL (Movimiento fluido con las flechas)
// =====================================================================
void DebugManager::actualizar(UI_Inventario& hud, Personaje& personaje, Enemy& enemigo) {
    if (!_modoDebugActivo || _objetivoActual == ObjetivoDebug::NINGUNO) return;

    // Detectamos las direcciones en tiempo real
    float dx = 0.f;
    float dy = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dy = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dy = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx = 1.f;

    // Si no se tocó nada, salimos para no hacer cálculos en vano
    if (dx == 0.f && dy == 0.f && !sf::Keyboard::isKeyPressed(sf::Keyboard::U) && !sf::Keyboard::isKeyPressed(sf::Keyboard::O)) return;

    // Aplicamos el movimiento al objetivo seleccionado
    switch (_objetivoActual) {
    case ObjetivoDebug::HUD:
        hud.ajustarPosicion(dx * 2.f, dy * 2.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) hud.ajustarOrigen(-1.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) hud.ajustarOrigen(1.f, 0.f);
        break;

    case ObjetivoDebug::PERSONAJE:
        personaje.ajustarOrigenSprite(dx, dy);
        break;

    case ObjetivoDebug::ENEMIGO:
        enemigo.ajustarOrigenSprite(dx, dy);
        break;

    default: break;
    }
}

// =====================================================================
// DIBUJADO POLIMÓRFICO UNIVERSAL
// =====================================================================
void DebugManager::dibujarCajaColision(sf::RenderWindow& ventana, const Colisionable& entidad, sf::Color color) const {
    if (!_modoDebugActivo) return;

    sf::FloatRect limites = entidad.getBounds();

    sf::RectangleShape caja(sf::Vector2f(limites.width, limites.height));
    caja.setPosition(limites.left, limites.top);

    // Fondo semitransparente (alpha 80) y borde sólido
    caja.setFillColor(sf::Color(color.r, color.g, color.b, 80));
    caja.setOutlineColor(color);
    caja.setOutlineThickness(1.f);

    ventana.draw(caja);
}