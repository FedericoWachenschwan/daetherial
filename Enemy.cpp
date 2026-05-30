#include "Enemy.h"
#include <iostream>
#include <cmath>

Enemy::Enemy(sf::Vector2f posInicial) {
    if (!_textura.loadFromFile("assets/icegolem.png")) {
        std::cout << "❌ Error al cargar la textura del Enemy (Gólem de Hielo)." << std::endl;
    }
    _sprite.setTexture(_textura);

    // Setear el origen en el centro de la base (los pies) para que pise bien el mapa
    _sprite.setOrigin(_textura.getSize().x / 2.f, (float)_textura.getSize().y);
    _sprite.setPosition(posInicial);

    // Ajustes de tipo "Tanque" para el Gólem
    _velocidad = 45.f; // Más lento que el personaje para poder torearlo
    _vida = 250;
    _danio = 25;
}

void Enemy::actualizar(sf::Vector2f posJugador, float dt) {
    sf::Vector2f posEnemy = _sprite.getPosition();

    // Vector de dirección hacia el jugador (Destino - Origen)
    sf::Vector2f direccion = posJugador - posEnemy;

    // Distancia usando Pitágoras
    float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    // Si está a más de 10 píxeles, lo persigue (evita el "glitch" de parpadeo encima del PJ)
    if (distancia > 100.f) {
        direccion /= distancia; // Normalizamos el vector
        _sprite.move(direccion * _velocidad * dt);
    }
}

void Enemy::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(_sprite);
}