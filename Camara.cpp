#include "Camara.h"

Camara::Camara(float ancho, float alto) {
    _vista.reset(sf::FloatRect(0.f, 0.f, ancho, alto));
    _zoomMin = 400.f;
    _zoomMax = 1600.f;
}

void Camara::seguir(sf::Vector2f posicionObjetivo) {
    _vista.setCenter(posicionObjetivo);
}

void Camara::procesarZoom(const sf::Event& evento) {
    if (evento.type == sf::Event::MouseWheelScrolled) {
        if (evento.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            float delta = evento.mouseWheelScroll.delta;

            if (delta > 0 && _vista.getSize().x > _zoomMin) {
                _vista.zoom(0.9f); // Zoom In
            }
            else if (delta < 0 && _vista.getSize().x < _zoomMax) {
                _vista.zoom(1.1f); // Zoom Out
            }
        }
    }
}

const sf::View& Camara::getVista() const {
    return _vista;
}