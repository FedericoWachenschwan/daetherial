#pragma once
#include "EntidadViva.h" 
#include "Inventario.h"
#include "BolaDeFuego.h"
#include "VisualFX.h"

class InputManager;

enum class EstadoPersonaje {
    SPELLCAST = 0,
    THRUST = 1,
    WALK = 2,
    SLASH = 3,
    SHOOT = 4,
    HURT = 5,
    IDLE,
    AIMING,
    MUERTO,
    DASH
};

enum class DireccionLPC {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
};

class Personaje : public EntidadViva {
private:
    EstadoPersonaje _estadoActual = EstadoPersonaje::IDLE;
    DireccionLPC    _direccionActual = DireccionLPC::DOWN;
    Inventario      _inventario;
    BolaDeFuego     _bolaDeFuego;

    ///=========================================================///
    ///   ORO Y MANÁ DEL JUGADOR
    ///=========================================================///
    int _oro = 100; // Oro inicial del jugador
    int _mana_actual = 100; // Maná actual del jugador
    int _mana_maxima = 100; // Maná máximo del jugador

    sf::Vector2f _velocidadActual = { 0.f, 0.f };
    float _aceleracion = 0.f;
    float _desaceleracion = 0.f;

    sf::CircleShape _circuloRango;
    float _radioAlcance = 200.f;
    float _radioActual = 0.f;

    float _tiempoDash = 0.f;
    const float _DuracionDash = 0.15f;
    float _relojSpawnRastro = 0.f;
    float _cooldownDash = 0.f;

    void determinarEstadoYDireccion(sf::Vector2f direccion);
    void procesarHabilidades(const InputManager& input, sf::RenderWindow& ventana, bool uiCapturaMouse);
    void controlarLimitesYTransiciones();
    void actualizarSpriteRect();

public:
    Personaje();

    ///=========================================================///
    ///   GETTERS Y SETTERS DE ORO
    ///=========================================================///
    int  getOro() const;
    void setOro(int nuevo_oro_del_jugador);

    ///=========================================================///
    ///   GETTERS Y SETTERS DE VIDA
    ///=========================================================///
    int  getVida() const;
    int  getVidaMaxima() const;
    void setVida(int nueva_vida_del_personaje);

    ///=========================================================///
    ///   GETTERS Y SETTERS DE MANÁ
    ///=========================================================///
    int  getMana() const;
    int  getManaMaXima() const;
    void setMana(int nuevo_mana_del_personaje);

    Inventario& getInventario() { return _inventario; }
    const Inventario& getInventario() const { return _inventario; }
    BolaDeFuego& getBolaDeFuego() { return _bolaDeFuego; }

    void manejarInput(const InputManager& input, Map& mapa, sf::RenderWindow& ventana, bool uiCapturaMouse, float dt);
    void actualizar(float dt) override {}
    void actualizar(float dt, VisualFX& vfx);
    void dibujar(sf::RenderWindow& ventana) override;

    sf::FloatRect getBounds() const override {
        sf::Vector2f pos = _sprite.getPosition();
        float hitboxX = pos.x - 8.f;
        float hitboxY = pos.y + 16.f;
        return sf::FloatRect(hitboxX, hitboxY, 16.f, 16.f);
    }
};