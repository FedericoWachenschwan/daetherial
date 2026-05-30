#pragma once
#include <SFML/Graphics.hpp>

class Niebla {
private:
    sf::Texture _textura;
    sf::Sprite _sprite;
    float _velocidadX;
    float _velocidadY;
    float _offsetX;
    float _offsetY;
    float _tiempoAcumulado = 0.f; // Para controlar la onda de la niebla

public:
    Niebla();
    void actualizar(float deltaTime);
    void dibujar(sf::RenderWindow& ventana, const sf::View& camara);
};