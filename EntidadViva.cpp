#include "EntidadViva.h"
#include <cmath> // Para std::round

// Constructor: Inicializa las variables protegidas que van a usar sus hijos
EntidadViva::EntidadViva() {
    _velocidad = 2.0f;
    _vidaMaxima = 100;
    _vidaActual = _vidaMaxima;

    _frameActual = 0;
    _tiempoFrame = 0.f;
    _velocidadAnimacion = 0.09f;
    _maxFrames = 1;
}

sf::Vector2f EntidadViva::getPosicion() const {
    return _sprite.getPosition();
}

void EntidadViva::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(_sprite);
}

void EntidadViva::recibirDanio(int cantidad) {
    _vidaActual -= cantidad;
    if (_vidaActual < 0) {
        _vidaActual = 0;
    }
}

// 🌟 El sistema unificado de colisiones. ¡Sirve para el mago, el golem y la mascota!
void EntidadViva::resolverColisiones(sf::Vector2f movimiento, Map& mapa) {
    // 🧱 EJE X
    if (movimiento.x != 0.f) {
        _sprite.move(movimiento.x, 0.f);

        for (const auto& bloque : mapa.getBloquesSolidos()) {
            if (this->chequearColision(bloque)) {
                _sprite.move(-movimiento.x, 0.f);
                sf::Vector2f posActual = _sprite.getPosition();
                _sprite.setPosition(std::round(posActual.x), posActual.y);
                break;
            }
        }
    }

    // 🧱 EJE Y
    if (movimiento.y != 0.f) {
        _sprite.move(0.f, movimiento.y);

        for (const auto& bloque : mapa.getBloquesSolidos()) {
            if (this->chequearColision(bloque)) {
                _sprite.move(0.f, -movimiento.y);
                sf::Vector2f posActual = _sprite.getPosition();
                _sprite.setPosition(posActual.x, std::round(posActual.y));
                break;
            }
        }
    }
}