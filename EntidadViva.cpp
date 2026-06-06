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
    _danio = 0;
    _cooldownAtaque = 0.f;
    _rangoAtaque = 0.f;

	// Configuración de la barra de vida (puede ser personalizada por cada hijo si quieren)
    float anchoBarra = 50.f;
    float altoBarra = 6.f;

    _barraFondo.setSize(sf::Vector2f(anchoBarra, altoBarra));
    _barraFondo.setFillColor(sf::Color(100, 0, 0)); // Rojo oscuro/bordó (vacío)
    _barraFondo.setOutlineThickness(1.f);
    _barraFondo.setOutlineColor(sf::Color::Black);

    _barraVida.setSize(sf::Vector2f(anchoBarra, altoBarra));
    _barraVida.setFillColor(sf::Color::Red); // Rojo brillante (lleno)
}

// Función para obtener la posición actual de la entidad (útil para IA, disparos, etc.)
sf::Vector2f EntidadViva::getPosicion() const {
    return _sprite.getPosition();
}

// Función para recibir daño: Resta la cantidad al HP actual y actualiza la barra de vida
void EntidadViva::recibirDanio(int cantidad) {
    _vidaActual -= cantidad;
    if (_vidaActual < 0) {
        _vidaActual = 0;
    }
    // Calculamos qué porcentaje de vida le queda (de 0.0 a 1.0)
    float porcentajeVida = static_cast<float>(_vidaActual) / _vidaMaxima;

    // Achicamos solo el ancho de la barra roja
    _barraVida.setSize(sf::Vector2f(50.f * porcentajeVida, _barraVida.getSize().y));
}

// Función de dibujo común para todas las entidades vivas (mago, golem, mascota)
void EntidadViva::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(_sprite);
    if (!estaMuerto()) {
        // 🌟 FIX: Usamos getGlobalBounds() del SPRITE entero, no de la hitbox física
        sf::FloatRect limitesVisuales = _sprite.getGlobalBounds();

        // Centramos la barra y la ponemos por encima de la CABEZA real de la textura
        float posX = limitesVisuales.left + (limitesVisuales.width / 2.f) - (_barraFondo.getSize().x / 2.f);
        float posY = limitesVisuales.top - 15.f;

        _barraFondo.setPosition(posX, posY);
        _barraVida.setPosition(posX, posY);

        ventana.draw(_barraFondo);
        ventana.draw(_barraVida);
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

