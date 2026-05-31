#pragma once
#include <SFML/Graphics.hpp>

class Camara {
private:
    sf::View _vista;
    float _zoomMin;
    float _zoomMax;

    // 🌟 Nuevas variables para los límites del mundo
    sf::FloatRect _limitesMundo;
    bool _tieneLimites;

public:
    Camara(float ancho, float alto);

    void seguir(sf::Vector2f posicionObjetivo, float dt);
    
    void procesarZoom(const sf::Event& evento);

    // 🌟 Nueva función para que el GameManager le pase el tamaño del mapa
    void setLimitesMundo(const sf::FloatRect& limites);

    // Retornamos una referencia constante para poder aplicarla a la ventana sin copiar datos
    const sf::View& getVista() const;
};