#pragma once
#include <SFML/Graphics.hpp>

class Camara {
private:
    sf::View _vista;
    float _zoomMin;
    float _zoomMax;

public:
    Camara(float ancho, float alto);

    void seguir(sf::Vector2f posicionObjetivo);
    void procesarZoom(const sf::Event& evento);

    // Retornamos una referencia constante para poder aplicarla a la ventana sin copiar datos
    const sf::View& getVista() const;
};