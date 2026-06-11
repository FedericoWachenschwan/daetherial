#pragma once
#include "EntidadViva.h"

class Mascota : public EntidadViva { // 🌟 MAGIA OOP: Hereda del motor central
private:
    // Mantenemos tus 4 texturas individuales
    sf::Texture textura_abajo;
    sf::Texture textura_arriba;
    sf::Texture textura_derecha;
    sf::Texture textura_izquierda;

    float distancia_maxima;

    // Su cerebro necesita saber dónde está el dueño
    sf::Vector2f _posicionDuenio;

public:
    Mascota();

    // Reemplazamos el "seguir()" por el setter lógico de la IA
    void setPosicionObjetivo(sf::Vector2f posicion_del_personaje) { _posicionDuenio = posicion_del_personaje; }

    // =========================================================================
    // 🌟 LOS CONTRATOS OBLIGATORIOS DE LA CLASE MADRE
    // =========================================================================
    void actualizar(float dt) override;
    void dibujar(sf::RenderWindow& ventana) override;
    sf::FloatRect getBounds() const override;
};