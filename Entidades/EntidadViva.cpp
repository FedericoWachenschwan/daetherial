#include "EntidadViva.h"
#include <cmath> // Para std::round
#include <algorithm> // para poder usar std::max (re estructuramos funciones)

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
    _vidaActual = std::max (0, _vidaActual -cantidad); // Calcula (_vidaActual - cantidad) y compara con 0 → guarda el mayor (0 o vida restante)
    // Calculamos qué porcentaje de vida le queda (de 0.0 a 1.0)
    float porcentajeVida = static_cast<float>(_vidaActual) / static_cast<float>(_vidaMaxima);
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
void EntidadViva::resolverColisiones(sf::Vector2f movimiento, const Map& mapa) {
    // 1. Delivery: Pedimos la lista al mapa
    const auto& paredes = mapa.getBloqueSolido();

    // 2. Definimos la herramienta de choque (Lambda)
    auto chocaConPared = [&](const sf::FloatRect& cajaFutura) {
        for (const auto& pared : paredes) {
            // Usamos getColision() tal como definimos en Colisionable
            if (cajaFutura.intersects(pared.getColision())) return true;
        }
        return false;
        };

    // 3. Wall-Sliding (Deslizamiento en X e Y)
    // Eje X
    sf::FloatRect hitboxX = getColision();
    hitboxX.left += movimiento.x;
    if (!chocaConPared(hitboxX)) {
        _sprite.move(movimiento.x, 0.f);
    }

    // Eje Y
    sf::FloatRect hitboxY = getColision();
    hitboxY.top += movimiento.y;
    if (!chocaConPared(hitboxY)) {
        _sprite.move(0.f, movimiento.y);
    }
}

