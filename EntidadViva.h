#pragma once
#include <SFML/Graphics.hpp>
#include "Colisionable.h"
#include "map.h"

// La hacemos heredar de Colisionable para que pase el contrato a sus hijos
class EntidadViva : public Colisionable {
protected:
    // 🌟 PROTECTED: Los hijos (Personaje, Golem) pueden ver y usar estas variables directamente

    // --- COMPONENTES VISUALES Y FÍSICA ---
    sf::Sprite _sprite;
    float _velocidad;

    // --- ESTADÍSTICAS BÁSICAS ---
    int _vidaMaxima;
    int _vidaActual;

    // --- ANIMACIÓN BASE ---
    int _frameActual;
    float _tiempoFrame;
    float _velocidadAnimacion;
    int _maxFrames;

    // 🌟 EL FIX DE LAS COLISIONES: Ahora vive acá para que lo usen todos
    void resolverColisiones(sf::Vector2f movimiento, Map& mapa);

public:
    EntidadViva();
    virtual ~EntidadViva() {} // Destructor virtual obligatorio en herencia

    // Funciones comunes que hacen lo mismo para todos
    virtual void dibujar(sf::RenderWindow& ventana);
    sf::Vector2f getPosicion() const;
	void setPosicion(sf::Vector2f nuevaPos) { _sprite.setPosition(nuevaPos); } // Función para que los hijos (personaje y enemigos) puedan mover la entidad

    // Sistema de vida base
    void recibirDanio(int cantidad);
    bool estaMuerto() const { return _vidaActual <= 0; }

    // ==========================================
    // ⚠️ CONTRATOS VIRTUALES (Cada hijo lo hace a su manera)
    // ==========================================

    // Cada hijo se actualiza distinto (el mago lee el teclado, el Golem usa IA)
    virtual void actualizar(float dt) = 0;

    // Mantenemos el contrato de Colisionable abierto para que cada hijo 
    // ajuste su hitbox a sus propios pies (el Golem seguro es más grande que el mago)
    virtual sf::FloatRect getBounds() const override = 0;
};