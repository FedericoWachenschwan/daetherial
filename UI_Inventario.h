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
    float _origenX;
    float _origenY;

    // 🌟 NUEVO: La variable que va a guardar si la mochila está visible o no
    bool _estaAbierto = false;

public:
    UI_Inventario();
    void dibujar(sf::RenderWindow& ventana, const Inventario& mochila);

    // 🌟 MÉTODOS DE CONTROL: Para que el GameManager le avise qué cambiar
    void ajustarPosicion(float x, float y);
    void ajustarOrigen(float x, float y);
    void detectarClicCasillero(sf::Vector2i posicionMouse, Inventario& mochila, const sf::RenderWindow& ventana);

    // 🌟 MÉTODOS DE ESTADO: Para abrir/cerrar la mochila
    bool isOpen() const { return _estaAbierto; }
    void setAbierto(bool abierto) { _estaAbierto = abierto; }
    void toggle() { _estaAbierto = !_estaAbierto; }

    // =========================================================================
    // 🌟 NUEVO MÉTODO: Prototipo para detectar si el mouse está sobre la mochila
    // =========================================================================
    bool mouseSobrePanel(sf::Vector2i posicionMouse) const;
};