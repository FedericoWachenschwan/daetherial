#include "Camara.h"
#include <algorithm> // Obligatorio para usar std::clamp

Camara::Camara(float ancho, float alto) {
    _vista.reset(sf::FloatRect(0.f, 0.f, ancho, alto));
    _zoomMin = 400.f;
    _zoomMax = 1600.f;
    _tieneLimites = false; // Arranca sin límites por si te olvidás de setearlos
}

void Camara::setLimitesMundo(const sf::FloatRect& limites) {
    _limitesMundo = limites;
    _tieneLimites = true;
}

void Camara::seguir(sf::Vector2f posicionObjetivo, float dt) {
    // ========================================================= //
    // 1. EFECTO LERP (Suavizado elástico)
    // ========================================================= //

    sf::Vector2f posicionActual = _vista.getCenter();

    // Qué tan "elástica" es la cámara. 
    // Valores altos (ej: 10.0f) = más rígida. Valores bajos (ej: 2.0f) = más suelta.
    float velocidadSuavizado = 10.0f;

    // Fórmula Matemática: PosicionActual + (DistanciaAlObjetivo * velocidad * tiempo)
    float nuevaX = posicionActual.x + (posicionObjetivo.x - posicionActual.x) * velocidadSuavizado * dt;
    float nuevaY = posicionActual.y + (posicionObjetivo.y - posicionActual.y) * velocidadSuavizado * dt;

    // ========================================================= //
    // 2. EFECTO CLAMP (Chocar contra los bordes del mapa)
    // ========================================================= //

    if (_tieneLimites) {
        // Calculamos cuánto mide la mitad de la pantalla actual (varía si hiciste zoom)
        float mitadAncho = _vista.getSize().x / 2.f;
        float mitadAlto = _vista.getSize().y / 2.f;

        nuevaX = std::clamp(nuevaX, _limitesMundo.left + mitadAncho, _limitesMundo.left + _limitesMundo.width - mitadAncho);
        nuevaY = std::clamp(nuevaY, _limitesMundo.top + mitadAlto, _limitesMundo.top + _limitesMundo.height - mitadAlto);
    }

    // Finalmente, aplicamos la nueva posición matemática a la vista
    _vista.setCenter(nuevaX, nuevaY);
}

void Camara::procesarZoom(const sf::Event& evento) {
    if (evento.type == sf::Event::MouseWheelScrolled && evento.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            float delta = evento.mouseWheelScroll.delta;

            if (delta > 0 && _vista.getSize().x > _zoomMin) {
                _vista.zoom(0.9f); // Zoom In
        } else if (delta < 0 && _vista.getSize().x < _zoomMax) {
                _vista.zoom(1.1f); // Zoom Out
            }
        }
    }
const sf::View& Camara::getVista() const {
    return _vista;
}