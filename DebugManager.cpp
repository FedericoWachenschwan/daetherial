#include "DebugManager.h"
#include "UI_Inventario.h"
#include "Personaje.h"
#include "map.h"
#include <iostream>

DebugManager::DebugManager() {
    _modoDebugActivo = false;
    _objetivoActual = ObjetivoDebug::NINGUNO; // Arrancamos sin seleccionar nada
}

void DebugManager::toggleDebug() {
    _modoDebugActivo = !_modoDebugActivo;
    if (_modoDebugActivo) {
        std::cout << "🔧 MODO DEBUG ACTIVADO" << std::endl;
        std::cout << "👉 Presiona 1 para HUD | 2 para Personaje | 0 para Ninguno" << std::endl;
    }
    else {
        std::cout << "🎮 MODO DEBUG DESACTIVADO" << std::endl;
        _objetivoActual = ObjetivoDebug::NINGUNO; // Reseteamos por seguridad al salir
    }
}

void DebugManager::procesarEventos(sf::Event& evento, UI_Inventario& hud, Personaje& personaje) {
    if (!_modoDebugActivo) return;

    if (evento.type == sf::Event::KeyPressed) {

        // ==========================================
        // 1. SELECTOR DE OBJETIVO (Teclas 1, 2, 0)
        // ==========================================
        if (evento.key.code == sf::Keyboard::Num1) {
            _objetivoActual = ObjetivoDebug::HUD;
            std::cout << "🎯 [MODO EDICIÓN]: HUD del Inventario seleccionado." << std::endl;
        }
        else if (evento.key.code == sf::Keyboard::Num2) {
            _objetivoActual = ObjetivoDebug::PERSONAJE;
            std::cout << "🎯 [MODO EDICIÓN]: Origen del Personaje seleccionado." << std::endl;
        }
        else if (evento.key.code == sf::Keyboard::Num0) {
            _objetivoActual = ObjetivoDebug::NINGUNO;
            std::cout << "🎯 [MODO EDICIÓN]: Ningún objeto seleccionado." << std::endl;
        }

        // ==========================================
        // 2. EL SWITCH MAESTRO (Tu idea aplicada)
        // ==========================================
        switch (_objetivoActual) {

        case ObjetivoDebug::HUD:
            // Reusamos las flechas tranquilamente
            if (evento.key.code == sf::Keyboard::Up) hud.ajustarPosicion(0.f, -5.f);
            if (evento.key.code == sf::Keyboard::Down) hud.ajustarPosicion(0.f, 5.f);
            if (evento.key.code == sf::Keyboard::Left) hud.ajustarPosicion(-5.f, 0.f);
            if (evento.key.code == sf::Keyboard::Right) hud.ajustarPosicion(5.f, 0.f);
            // El origen del HUD lo dejamos con U y O
            if (evento.key.code == sf::Keyboard::U) hud.ajustarOrigen(-1.f, 0.f);
            if (evento.key.code == sf::Keyboard::O) hud.ajustarOrigen(1.f, 0.f);
            break;

        case ObjetivoDebug::PERSONAJE:
            // Las mismas flechas, pero como el switch nos aísla, ahora mueven el sprite
            if (evento.key.code == sf::Keyboard::Up) personaje.ajustarOrigenSprite(0.f, -1.f);
            if (evento.key.code == sf::Keyboard::Down) personaje.ajustarOrigenSprite(0.f, 1.f);
            if (evento.key.code == sf::Keyboard::Left) personaje.ajustarOrigenSprite(-1.f, 0.f);
            if (evento.key.code == sf::Keyboard::Right) personaje.ajustarOrigenSprite(1.f, 0.f);
            break;

        case ObjetivoDebug::NINGUNO:
        default:
            // Si no elegiste nada (estado 0), las flechas no hacen nada raro
            break;
        }
    }
}

void DebugManager::dibujarHitboxes(sf::RenderWindow& ventana, Personaje& personaje, Map& mapa) {
    if (!_modoDebugActivo) return;

    mapa.dibujarDebug(ventana);
    personaje.dibujarDebug(ventana);
}
