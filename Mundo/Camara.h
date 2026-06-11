#pragma once
#include <SFML/Graphics.hpp>

class Camara {
private:
    sf::View _vista;
    float _zoomMin;
    float _zoomMax;
    sf::FloatRect _limitesMundo;
    bool _tieneLimites;

public:
    Camara(float ancho, float alto);

    void seguir(sf::Vector2f posicionObjetivo, float dt);
    
    void procesarZoom(const sf::Event& evento);

    void setLimitesMundo(const sf::FloatRect& limites);

    const sf::View& getVista() const;
};