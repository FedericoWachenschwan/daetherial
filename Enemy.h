#pragma once
#include "EntidadViva.h"
#include "PathFinder.h"
#include "map.h"
#include <vector>
#include <iostream> // Necesario para el cout del origen

class Enemy : public EntidadViva {
private:
    sf::Texture _textura;
    int _danio;
    Map* _mapaRef;

    sf::Clock _relojPathfinding;
    sf::Vector2f _posicionObjetivo;
    sf::Vector2f _ultimoDestinoConocido;
    std::vector<sf::Vector2f> _caminoActual;

    void activarPathfinder(float dt, sf::Vector2f posActual);

public:
    Enemy();
    Enemy(sf::Vector2f posInicial, Map* mapa);

    void setPosicionObjetivo(sf::Vector2f posJugador);
    void actualizar(float dt) override;
    void dibujar(sf::RenderWindow& ventana) override;
    sf::FloatRect getBounds() const override;
    int getDanio() const { return _danio; }

    void dibujarPathFinder(sf::RenderWindow& ventana) const;
    void dibujarHitboxEnemy(sf::RenderWindow& ventana) const;

    // 🌟 ACÁ ESTÁ LA FUNCIÓN PARA QUE EL DEBUG MANAGER PUEDA MOVERLO
    void ajustarOrigenSprite(float dx, float dy) {
        sf::Vector2f origenActual = _sprite.getOrigin();
        _sprite.setOrigin(origenActual.x + dx, origenActual.y + dy);
        std::cout << "Origen Gólem ajustado: X=" << _sprite.getOrigin().x << " Y=" << _sprite.getOrigin().y << std::endl;
    }
};