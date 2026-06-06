#pragma once
#include <SFML/Graphics.hpp>
#include "Colisionable.h"
#include "map.h"

// La hacemos heredar de Colisionable para que pase el contrato a sus hijos
class EntidadViva : public Colisionable {
protected:
    // 🌟 PROTECTED: Los hijos (Personaje, Golem) pueden ver y usar estas variables directamente

    // --- COMPONENTES VISUALES Y FÍSICA ---
    sf::Texture _textura;
    sf::Sprite _sprite;
    float _velocidad;

    // --- ESTADÍSTICAS BÁSICAS ---
    int _vidaMaxima;
    int _vidaActual;
    int _danio;
  
	// --- SISTEMA DE COMBATE BASE ---
    sf::Clock _relojAtaque;
	float _cooldownAtaque;
	float _rangoAtaque;

    // --- ANIMACIÓN BASE ---
    int _frameActual;
    float _tiempoFrame;
    float _velocidadAnimacion;
    int _maxFrames;

    // Ahora vive acá para que lo usen todos
    void resolverColisiones(sf::Vector2f movimiento, Map& mapa);

    ///============================================================================///
    ///                    BARRA DE VIDA - Rectángulos para mostrar la salud       ///
    ///============================================================================///
    sf::RectangleShape _barraFondo; // Rectángulo rojo oscuro de fondo (el "vacío")
    sf::RectangleShape _barraVida;  // Rectángulo rojo brillante que se achica con el daño

public:
    EntidadViva();
    virtual ~EntidadViva() {} // Destructor virtual obligatorio en herencia

    // Funciones comunes que hacen lo mismo para todos
    virtual void dibujar(sf::RenderWindow& ventana);
    virtual void setPosicionObjetivo(sf::Vector2f pos) {}
    sf::Vector2f getPosicion() const;
	void setPosicion(sf::Vector2f nuevaPos) { _sprite.setPosition(nuevaPos); } // Función para que los hijos (personaje y enemigos) puedan mover la entidad
    // Ajuste de origen para el modo Debug (Lo heredan todos los personajes y monstruos)
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
   

    // Sistema de vida base
    void recibirDanio(int cantidad);
    bool estaMuerto() const { return _vidaActual <= 0; }
    bool estaVivo() const { return _vidaActual >= 0; }
    int getDanio() const { return _danio; }
   
    sf::Vector2f getCentroFisico() const {
        sf::FloatRect bounds = getBounds();
        return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }

    // ==========================================
    // ⚠️ CONTRATOS VIRTUALES (Cada hijo lo hace a su manera)
    // ==========================================

    // Cada hijo se actualiza distinto (el mago lee el teclado, el Golem usa IA)
    virtual void actualizar(float dt) = 0;

    // Mantenemos el contrato de Colisionable abierto para que cada hijo 
    virtual sf::FloatRect getBounds() const override = 0;

    void getresolverColisiones(sf::Vector2f movimiento, Map& mapa) { resolverColisiones(movimiento, mapa); }
};