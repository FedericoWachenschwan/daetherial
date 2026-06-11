#pragma once
#include <SFML/Graphics.hpp>
#include "Colisionable.h"
#include "map.h"

// La hacemos heredar de Colisionable para que pase el contrato a sus hijos
class EntidadViva : public Colisionable {
protected:
    // 🌟 PROTECTED: Los hijos (Personaje, Mascota, Golem, NPC) pueden ver y usar estas variables directamente

    // --- COMPONENTES VISUALES ---
    sf::Texture _textura;
    sf::Sprite _sprite;
    int _frameActual;
    float _tiempoFrame;
    float _velocidadAnimacion;
    int _maxFrames;
    // --- ESTADÍSTICAS ---
    int _vidaMaxima;
    int _vidaActual;
    int _danio;
    float _velocidad;
	// --- SISTEMA DE COMBATE ---
    sf::Clock _relojAtaque;
	float _cooldownAtaque;
	float _rangoAtaque;

    ///============================================================================///
    ///                    BARRA DE VIDA - Rectángulos para mostrar la salud       ///
    ///============================================================================///
    sf::RectangleShape _barraFondo; // Rectángulo rojo oscuro de fondo (el "vacío")
    sf::RectangleShape _barraVida;  // Rectángulo rojo brillante que se achica con el daño

public:
    EntidadViva();
    virtual ~EntidadViva() {}

    // Logica interna de fisicas
    void resolverColisiones(sf::Vector2f movimiento, const class Map& mapa);

    // ==========================================
    // METODOS COMUNES (Heredados tal cual)
    // ==========================================
    virtual void dibujar(sf::RenderWindow& ventana);
    virtual void setPosicionObjetivo(sf::Vector2f pos) {}
    sf::Vector2f getPosicion() const;
    void setPosicion(sf::Vector2f nuevaPos) { _sprite.setPosition(nuevaPos); }
    virtual void ajustarOrigenSprite(float dx, float dy) {
        sf::Vector2f origenActual = _sprite.getOrigin();
        _sprite.setOrigin(origenActual.x + dx, origenActual.y + dy);
    }
    bool puedeAtacar() {
        if (_relojAtaque.getElapsedTime().asSeconds() >= _cooldownAtaque) {
            _relojAtaque.restart();
            return true;
        }
        return false;
    }

    // Puente público para aplicar knockback y físicas desde afuera
    void aplicarMovimientoConColisiones(sf::Vector2f movimiento, Map& mapa) {
        resolverColisiones(movimiento, mapa);
    }

    // ==========================================
    // SISTEMA DE ESTADÍSTICAS Y VIDA
    // ==========================================
    void recibirDanio(int cantidad);
    bool estaMuerto() const { return _vidaActual <= 0; }
    bool estaVivo() const { return _vidaActual > 0; }
    int getDanio() const { return _danio; }
    sf::Vector2f getCentroFisico() const {
        sf::FloatRect bounds = getBounds();
        return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }

    // ==========================================
    // CONTRATOS VIRTUALES (Obligatorios para los hijos)
    // ==========================================
    virtual void actualizar(float dt) = 0;
    virtual sf::FloatRect getBounds() const override = 0;
};