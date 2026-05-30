#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Texture _textura;
    sf::Sprite _sprite;
    float _velocidad;
    int _vida;
    int _danio;

public:
    Enemy(sf::Vector2f posInicial);
    void actualizar(sf::Vector2f posJugador, float dt);
    void dibujar(sf::RenderWindow& ventana);

    // Getter por si necesitas su posición para algo en el mánager
    sf::Vector2f getPosicion() const { return _sprite.getPosition(); }
};