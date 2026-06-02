#pragma once
#include "EntidadViva.h" 
#include "Inventario.h"
#include "BolaDeFuego.h"

class InputManager;

// =========================================================================
// 🌟 ENUM ALINEADO CON LAS 21 FILAS DE LA MATRIZ LPC
// =========================================================================
enum class EstadoPersonaje {
    SPELLCAST = 0,
    THRUST = 1,
    WALK = 2,
    SLASH = 3,
    SHOOT = 4,
    HURT = 5,
    IDLE,
    AIMING
};

enum class DireccionLPC {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
};

class Personaje : public EntidadViva { // 🌟 AHORA SÍ: Hereda de la clase madre
private:
    // --- COMPONENTES EXCLUSIVOS DEL JUGADOR ---
    sf::Texture textura_completa_lpc;

    EstadoPersonaje _estadoActual = EstadoPersonaje::IDLE;
    DireccionLPC _direccionActual = DireccionLPC::DOWN;

    Inventario _inventario;
    BolaDeFuego _bolaDeFuego;

    // --- 🎯 INDICADOR DE RANGO ---
    sf::CircleShape _circuloRango;
    float _radioAlcance = 200.f;
    float _radioActual = 0.f;

    // --- MÉTODOS PRIVADOS DE LÓGICA ---
    void determinarEstadoYDireccion(sf::Vector2f direccion);
    void procesarHabilidades(const InputManager& input, sf::RenderWindow& ventana, bool uiCapturaMouse);
    void controlarLimitesYTransiciones();
    void actualizarSpriteRect();
	

public:
    Personaje();

    Inventario& getInventario() { return _inventario; }
    const Inventario& getInventario() const { return _inventario; }

    void manejarInput(const InputManager& input, Map& mapa, sf::RenderWindow& ventana, bool uiCapturaMouse);

    // 🌟 Funciones que el Personaje está obligado a implementar por heredar de EntidadViva
    void actualizar(float dt) override;
    void dibujar(sf::RenderWindow& ventana) override; // 🌟 ACÁ ESTABA EL FALTANTE
    sf::FloatRect getBounds() const override {
        // 🌟 Usamos _sprite heredado de EntidadViva
        sf::Vector2f pos = _sprite.getPosition();

        float hitboxX = pos.x - 8.f;  // Centra la caja de 16 de ancho
        float hitboxY = pos.y + 16.f; // La baja a la base de los pies (32 + 16 = 48)

        return sf::FloatRect(hitboxX, hitboxY, 16.f, 16.f);
    }

    // Métodos propios extra
    void dibujarDebug(sf::RenderWindow& ventana) const; // 🌟 Le agregamos el const acá
    void ajustarOrigenSprite(float x, float y);
};