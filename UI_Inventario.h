#pragma once
#include <SFML/Graphics.hpp>
#include "Inventario.h"
#include <iostream>
#include <string>

class UI_Inventario {
private:
	sf::Texture _texturaInventario;
    sf::Sprite _spriteInventario;

    sf::RectangleShape _slotFondo;
    float _tamanioSlot;
    float _margen;
    sf::Font _fuente;
    sf::Text _textoCantidad;


    // 🌟 AJUSTES EN CALIENTE (Para centrar la grilla adentro de tu dibujo)
    float _desfaseX;
    float _desfaseY;
    float _origenX; // Ahora este origen va a ser la posición de la ventana
    float _origenY;

public:
    UI_Inventario();
    void dibujar(sf::RenderWindow& ventana, const Inventario& mochila);

    // 🌟 MÉTODOS DE CONTROL: Para que el GameManager le avise qué cambiar
    void ajustarPosicion(float x, float y);
    void ajustarOrigen(float x, float y);
	void detectarClicCasillero(sf::Vector2i posicionMouse, Inventario& mochila, const sf::RenderWindow& ventana);
};

