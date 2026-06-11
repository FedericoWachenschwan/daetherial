#include "DebugManager.h"
#include "UI_Inventario.h"
#include "Personaje.h"
#include "EntidadViva.h" // 🌟 FIX: Incluimos al Padre
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

// 
void DebugManager::procesarEventos(sf::Event& evento, sf::RenderWindow& ventana, UI_Inventario& hud, Personaje& personaje, EntidadViva* enemigoFocus) {
    if (!_modoDebugActivo) return;

    // --- 1. CLIC IZQUIERDO: EL MOTOR MATEMÁTICO DEL EXTRACTOR ---
    if (evento.type == sf::Event::MouseButtonPressed && _objetivoActual == ObjetivoDebug::EXTRACTOR) {
        if (evento.mouseButton.button == sf::Mouse::Left) {

            // 🌟 LA MAGIA DEL ZOOM Y LA CÁMARA (SFML mapPixelToCoords)
            sf::Vector2i pixelPos = sf::Mouse::getPosition(ventana);
            // Esto traduce el píxel de la pantalla a la coordenada real del mundo 2D, considerando el zoom y la posición de la cámara
            sf::Vector2f worldPos = ventana.mapPixelToCoords(pixelPos);

            // Calculamos relativo al offset de donde moviste la imagen del Extractor
            float imgX = worldPos.x - _offsetExtractor.x;
            float imgY = worldPos.y - _offsetExtractor.y;

            if (imgX >= 0 && imgY >= 0) {
                int col = static_cast<int>(imgX) / 32;
                int fila = static_cast<int>(imgY) / 32;

                int idTextura = (fila * 64) + col;

                std::cout << "\n============================================\n";
                std::cout << "🎯 TILE ENCONTRADO:\n";
                std::cout << "👉 ID a usar en el .dat: " << idTextura << "\n";
                std::cout << "   (Coordenadas: Fila " << fila << " | Col " << col << ")\n";
                std::cout << "============================================\n";
            }
        }
    }

    if (evento.type == sf::Event::KeyPressed) {

        // El selector original (teclas 1, 2, 3, 4, 0)...
        if (evento.key.code == sf::Keyboard::Num1) _objetivoActual = ObjetivoDebug::HUD;
        if (evento.key.code == sf::Keyboard::Num2) _objetivoActual = ObjetivoDebug::PERSONAJE;
        if (evento.key.code == sf::Keyboard::Num3) _objetivoActual = ObjetivoDebug::ENEMIGO;
        if (evento.key.code == sf::Keyboard::Num4) _objetivoActual = ObjetivoDebug::EXTRACTOR;
        if (evento.key.code == sf::Keyboard::Num0) _objetivoActual = ObjetivoDebug::NINGUNO;

        // Movimientos puntuales según el objetivo
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
            // Ahora no nos importa si es un Boss o un Marciano, mientras esté vivo, le podemos ajustar el origen
            if (enemigoFocus != nullptr && enemigoFocus->estaVivo()) {
                if (evento.key.code == sf::Keyboard::Up) enemigoFocus->ajustarOrigenSprite(0.f, -1.f);
                if (evento.key.code == sf::Keyboard::Down) enemigoFocus->ajustarOrigenSprite(0.f, 1.f);
                if (evento.key.code == sf::Keyboard::Left) enemigoFocus->ajustarOrigenSprite(-1.f, 0.f);
                if (evento.key.code == sf::Keyboard::Right) enemigoFocus->ajustarOrigenSprite(1.f, 0.f);
            }
            break;
        default: break;
        }
    }
}
void DebugManager::actualizar(UI_Inventario& hud, Personaje& personaje, EntidadViva* enemigoFocus) {
    if (!_modoDebugActivo || _objetivoActual == ObjetivoDebug::NINGUNO) return;

    if (_objetivoActual == ObjetivoDebug::ENEMIGO && (enemigoFocus == nullptr || !enemigoFocus->estaVivo())) {
        _objetivoActual = ObjetivoDebug::NINGUNO;
        std::cout << "👻 El enemigo seleccionado murió. Volviendo a Objetivo: NINGUNO." << std::endl;
        return;
    }

    float dx = 0.f; float dy = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dy = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dy = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx = 1.f;

    if (dx == 0.f && dy == 0.f && !sf::Keyboard::isKeyPressed(sf::Keyboard::U) && !sf::Keyboard::isKeyPressed(sf::Keyboard::O)) return;

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
        if (enemigoFocus != nullptr && enemigoFocus->estaVivo()) {
            enemigoFocus->ajustarOrigenSprite(dx, dy);
        }
        break;
    case ObjetivoDebug::EXTRACTOR:
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
    caja.setFillColor(sf::Color(color.r, color.g, color.b, 80));
    caja.setOutlineColor(color);
    caja.setOutlineThickness(1.f);

    ventana.draw(caja);
}
void DebugManager::dibujarExtractor(sf::RenderWindow& ventana, const sf::Texture& texturaMaestra) {
    if (!_modoDebugActivo || _objetivoActual != ObjetivoDebug::EXTRACTOR) return;

    ventana.clear(sf::Color(15, 15, 15));

    // IMPORTANTE: Reseteamos la vista a la de por defecto para que el spritesheet no se vea afectado por la cámara del jugador
    ventana.setView(ventana.getDefaultView());

    sf::Sprite spriteSpritesheet(texturaMaestra);
    spriteSpritesheet.setPosition(_offsetExtractor);
    ventana.draw(spriteSpritesheet);
}

// ============================================================================
// HERRAMIENTA DE GRILLA (Tile Snapping para Debug del Mapa)
// ============================================================================
void DebugManager::procesarClicMapa(sf::Vector2i pixelPos, const sf::View& vistaActiva, const sf::RenderWindow& ventana) {
    if (!_modoDebugActivo) return;

    // Traducimos el clic usando la cámara actual del juego
    sf::Vector2f worldPos = ventana.mapPixelToCoords(pixelPos, vistaActiva);

    // Matemática de Snapping: redondea a múltiplo de 32
    float tileX = std::floor(worldPos.x / 32.f) * 32.f;
    float tileY = std::floor(worldPos.y / 32.f) * 32.f;

    _posTileMarcado = sf::Vector2f(tileX, tileY);
    _dibujarMarcaTile = true;

    std::cout << "🟨 [GRILLA] Clic en Tile -> X: " << _posTileMarcado.x
        << " | Y: " << _posTileMarcado.y << std::endl;
}
void DebugManager::dibujarGrillaMapa(sf::RenderWindow& ventana) const {
    if (!_modoDebugActivo || !_dibujarMarcaTile) return;

    sf::RectangleShape marcaTile(sf::Vector2f(32.f, 32.f));
    marcaTile.setPosition(_posTileMarcado);

    // Color Amarillo Transparente
    marcaTile.setFillColor(sf::Color(255, 255, 0, 60));
    marcaTile.setOutlineColor(sf::Color::Yellow);
    marcaTile.setOutlineThickness(1.5f);

    ventana.draw(marcaTile);
}