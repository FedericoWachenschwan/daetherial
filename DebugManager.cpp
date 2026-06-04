#include "DebugManager.h"
#include "UI_Inventario.h"
#include "Personaje.h"
#include "Enemy.h"
#include "map.h"
#include <iostream>

DebugManager::DebugManager() {
    _modoDebugActivo = false;
    _objetivoActual = ObjetivoDebug::NINGUNO;
    _offsetExtractor = sf::Vector2f(0.f, 0.f);
}

void DebugManager::toggleDebug() {
    _modoDebugActivo = !_modoDebugActivo;
    if (_modoDebugActivo) {
        std::cout << "🔧 MODO DEBUG ACTIVADO" << std::endl;
        std::cout << "👉 Presiona 1 para HUD | 2 para Personaje | 3 para Enemigo | 4 para Extractor | 0 para Ninguno" << std::endl;
    }
    else {
        std::cout << "🎮 MODO DEBUG DESACTIVADO" << std::endl;
        _objetivoActual = ObjetivoDebug::NINGUNO;
    }
}

// =====================================================================
// SELECTOR DE OBJETIVO (Eventos únicos de teclado)
// =====================================================================
void DebugManager::procesarEventos(sf::Event& evento, sf::RenderWindow& ventana, UI_Inventario& hud, Personaje& personaje, Enemy* enemigo) {
    if (!_modoDebugActivo) return;

    // --- 1. CLIC IZQUIERDO: EL MOTOR MATEMÁTICO DEL EXTRACTOR ---
    if (evento.type == sf::Event::MouseButtonPressed && _objetivoActual == ObjetivoDebug::EXTRACTOR) {
        if (evento.mouseButton.button == sf::Mouse::Left) {

            // Leemos dónde hizo clic en la pantalla
            sf::Vector2i pixelPos = sf::Mouse::getPosition(ventana);
            sf::Vector2f worldPos = ventana.mapPixelToCoords(pixelPos);

            // Le restamos el offset de la imagen para saber qué parte real del PNG tocó
            float imgX = worldPos.x - _offsetExtractor.x;
            float imgY = worldPos.y - _offsetExtractor.y;

            // Si hizo clic dentro de los límites de la imagen
            if (imgX >= 0 && imgY >= 0) {
                int col = static_cast<int>(imgX) / 32;
                int fila = static_cast<int>(imgY) / 32;

                // 🌟 TU FÓRMULA MÁGICA: 64 columnas fijas
                int idTextura = (fila * 64) + col;

                std::cout << "\n============================================\n";
                std::cout << "🎯 TILE ENCONTRADO:\n";
                std::cout << "👉 ID a usar en el .dat: " << idTextura << "\n";
                std::cout << "   (Coordenadas: Fila " << fila << " | Col " << col << ")\n";
                std::cout << "============================================\n";
            }
        }
    }

    // --- 2. TECLAS DE SELECCIÓN Y AJUSTE: Cambia el objetivo o ajusta su posición/origen ---
    if (evento.type == sf::Event::KeyPressed) {

        // ==========================================
        // 1. SELECTOR DE OBJETIVO (Teclas 1, 2, 3, 4, 0)
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
        else if (evento.key.code == sf::Keyboard::Num4) {
            _objetivoActual = ObjetivoDebug::EXTRACTOR;
            std::cout << "🎯 [MODO EXTRACTOR]: Usa las flechas para moverte y click izquierdo para extraer ID." << std::endl;
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
            if (enemigo != nullptr && !enemigo->estaMuerto()) {
                if (evento.key.code == sf::Keyboard::Up) enemigo->ajustarOrigenSprite(0.f, -1.f);
                if (evento.key.code == sf::Keyboard::Down) enemigo->ajustarOrigenSprite(0.f, 1.f);
                if (evento.key.code == sf::Keyboard::Left) enemigo->ajustarOrigenSprite(-1.f, 0.f);
                if (evento.key.code == sf::Keyboard::Right) enemigo->ajustarOrigenSprite(1.f, 0.f);
            }
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
void DebugManager::actualizar(UI_Inventario& hud, Personaje& personaje, Enemy* enemigo) {
    if (!_modoDebugActivo || _objetivoActual == ObjetivoDebug::NINGUNO) return;

    // 🛡️ ESCUDO: Si el objetivo seleccionado es el enemigo pero ya murió, volvemos a NINGUNO para evitar problemas
    if (_objetivoActual == ObjetivoDebug::ENEMIGO && (enemigo == nullptr || enemigo->estaMuerto())) {
        _objetivoActual = ObjetivoDebug::NINGUNO;
        std::cout << "👻 El enemigo seleccionado murió. Volviendo a Objetivo: NINGUNO." << std::endl;
        return;
    }

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
        enemigo->ajustarOrigenSprite(dx, dy);
        break;

        // 🌟 AGREGÁ ESTE CASE ACÁ:
    case ObjetivoDebug::EXTRACTOR:
        // Movemos el mapa de texturas 25 píxeles por frame con las flechas
        _offsetExtractor.x += dx * -25.f;
        _offsetExtractor.y += dy * -25.f;
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

// =====================================================================
// EL DIBUJADOR DEL EXTRACTOR
// =====================================================================
void DebugManager::dibujarExtractor(sf::RenderWindow& ventana, const sf::Texture& texturaMaestra) {
    if (!_modoDebugActivo || _objetivoActual != ObjetivoDebug::EXTRACTOR) return;

    // Ponemos la pantalla de color oscuro para tapar el juego normal
    ventana.clear(sf::Color(15, 15, 15));
    ventana.setView(ventana.getDefaultView()); // Clavamos la vista

    // Dibujamos la textura completa en su posición offseteada
    sf::Sprite spriteSpritesheet(texturaMaestra);
    spriteSpritesheet.setPosition(_offsetExtractor);
    ventana.draw(spriteSpritesheet);
}