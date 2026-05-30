#pragma once
#include <SFML/Graphics.hpp>
#include "Inventario.h"
#include <iostream>
#include <string>

class UI_Inventario {
private:
    sf::RectangleShape _slotFondo;
    float _tamanioSlot;
    float _margen;
    sf::Font _fuente;
    sf::Text _textoCantidad;


    // 🌟 VARIABLES DE DEBUG: Para mover en caliente
    float _desfaseX;
    float _desfaseY;
    float _origenX;
    float _origenY;

public:
    UI_Inventario();
    void dibujar(sf::RenderWindow& ventana, const Inventario& mochila);

    // 🌟 MÉTODOS DE CONTROL: Para que el GameManager le avise qué cambiar
    void ajustarPosicion(float x, float y);
    void ajustarOrigen(float x, float y);
};

