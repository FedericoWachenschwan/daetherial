#include "Niebla.h"
#include "cmath"

Niebla::Niebla() {
    if (_textura.loadFromFile("assets/fog.png")) {
        // 🌟 EL TRUCO: Le decimos a SFML que la imagen se repita infinitamente como un azulejo
        _tiempoAcumulado = 0.f;
        _textura.setRepeated(true);
        _sprite.setTexture(_textura);

        // Le damos una opacidad baja (por ejemplo, 80 de 255) para que sea semitransparente
        _sprite.setColor(sf::Color(255, 255, 255, 80));
    }

    // Velocidad de movimiento del viento de la niebla
    _velocidadX = 15.f;
    _velocidadY = 5.f;
    _offsetX = 0.f;
    _offsetY = 0.f;
}

void Niebla::actualizar(float deltaTime) {
    // Acumulamos el movimiento en base al tiempo transcurrido
    _offsetX += _velocidadX * deltaTime;
    _offsetY += _velocidadY * deltaTime;

	// ACUMULAMOS Acumulamos el tiempo para crear una onda de movimiento vertical
	_tiempoAcumulado += deltaTime;

    // Calculamos la opacidad dinámicamente usando el seno
    // Multiplicamos _tiempoAcumulado por 0.5f para que la transición sea lenta y sutil
    float onda = std::sin(_tiempoAcumulado * 0.5f);

    // Mapeamos el resultado (que da entre -1 y 1) para que oscile entre 40 y 90 de opacidad
    int opacidadDinamica = 30 + (int)(onda * 25);

    // Le aplicamos la nueva opacidad al sprite en tiempo real
    _sprite.setColor(sf::Color(255, 255, 255, opacidadDinamica));

}

void Niebla::dibujar(sf::RenderWindow& ventana, const sf::View& camara) {
    // 1. Le preguntamos a la cámara qué tamaño real tiene en este milisegundo (con o sin zoom)
    sf::Vector2f tamCamara = camara.getSize();
    sf::Vector2f centroCamara = camara.getCenter();

    // 2. Ajustamos el tamaño del recorte dinámicamente para que cubra TODO lo que ve la cámara
    _sprite.setTextureRect(sf::IntRect((int)_offsetX, (int)_offsetY, (int)tamCamara.x, (int)tamCamara.y));

    // 3. Posicionamos el sprite exactamente en la esquina superior izquierda de la vista actual
    _sprite.setPosition(centroCamara.x - (tamCamara.x / 2.f), centroCamara.y - (tamCamara.y / 2.f));

    ventana.draw(_sprite);
}