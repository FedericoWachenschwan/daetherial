#pragma once
#include "EntidadViva.h" 
#include "Inventario.h"
#include "BolaDeFuego.h"
#include "VisualFX.h"

class InputManager;

// =========================================================================
// 🌟 ENUM ALINEADO CON LAS 21 FILAS DE LA MATRIZ LPC
// =========================================================================

enum class DireccionLPC {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
};

enum class EstadoPersonaje {
    // --- ESTADOS LIGADOS AL SPRITESHEET (Filas Base) ---
    SPELLCAST = 0,  // Filas 0 a 3
    THRUST = 4,     // Filas 4 a 7
    WALK = 8,       // Filas 8 a 11
    SLASH = 12,     // Filas 12 a 15
    SHOOT = 16,     // Filas 16 a 19
    HURT = 20,      // Fila 20 (LPC suele usar 1 sola fila para Hurt)

    // --- ESTADOS LÓGICOS (No tienen fila propia en el sprite) ---
    // Les ponemos números altos para que no se pisen con las filas reales
    IDLE = 50,      // Usa el frame 0 de la fila WALK
    AIMING = 51,    // Usa un frame estático de SPELLCAST
    MUERTO = 52,    // Usa el último frame de HURT
    DASH = 53       // Usa la fila WALK pero la lógica de C++ acelera la velocidad
};

class Personaje : public EntidadViva { // Hereda de la clase madre
private:
    // --- COMPONENTES EXCLUSIVOS DEL JUGADOR ---
    EstadoPersonaje _estadoActual = EstadoPersonaje::IDLE;
    DireccionLPC _direccionActual = DireccionLPC::DOWN;
    Inventario _inventario;
    BolaDeFuego _bolaDeFuego;
    
	///  --------------ORO DEL JUGADOR----------------
    int _oro = 100; // Oro inicial que tiene el jugador para comprar en la tienda (empieza con 100)


	// --- FISICAS DEL PERSONAJE ---
	sf::Vector2f _velocidadActual = { 0.f, 0.f };
	float _aceleracion = 0.f; // Que tan rapido alcanza la velocidad máxima
	float _desaceleracion = 0.f; // Que tan rapido frena al soltar el movimiento (debe ser mayor que la aceleración para que no se sienta pegajoso)

    // --- 🎯 INDICADOR DE RANGO ---
    sf::CircleShape _circuloRango;
    float _radioAlcance = 200.f;
    float _radioActual = 0.f;

	// --- CONTROL DE DASH ---
	float _tiempoDash = 0.f;
	const float _DuracionDash = 0.15f; // Duración total del dash en segundos
	float _relojSpawnRastro = 0.f; // Reloj para controlar el spawn de los rastros
	float _cooldownDash = 0.f; // Tiempo de recarga del dash

    // --- MÉTODOS PRIVADOS DE LÓGICA ---
    void determinarEstadoYDireccion(sf::Vector2f direccion);
    void procesarHabilidades(const InputManager& input, sf::RenderWindow& ventana, bool uiCapturaMouse);
    void controlarLimitesYTransiciones();
    void actualizarSpriteRect();

public:
    Personaje();

	///===================================================
    ///      MÉTODOS DE ORO - Getter y setter del oro
	///===================================================
    int getOro(); // Devuelve cuánto oro tiene el jugador
	void setOro(int nuevo_oro_del_jugador); // Establece la cantidad de oro del jugador

    Inventario& getInventario() { return _inventario; }
    const Inventario& getInventario() const { return _inventario; }
    BolaDeFuego& getBolaDeFuego() { return _bolaDeFuego; } // Getter para acceder a la bola de fuego desde el GameManager o la UI

	void manejarInput(const InputManager& input, Map& mapa, sf::RenderWindow& ventana, bool uiCapturaMouse, float dt); // Método para procesar el input del jugador (movimiento, habilidades, etc.)	
    
    // 🌟 Funciones que el Personaje está obligado a implementar por heredar de EntidadViva
    void actualizar(float dt) override {}
    void actualizar(float dt, VisualFX& vfx);
    void dibujar(sf::RenderWindow& ventana) override;

    sf::FloatRect getBounds() const override {
        // _sprite heredado de EntidadViva
        sf::Vector2f pos = _sprite.getPosition();
        float hitboxX = pos.x - 8.f;  // Centra la caja de 16 de ancho
        float hitboxY = pos.y + 16.f; // La baja a la base de los pies (32 + 16 = 48)
		return sf::FloatRect(hitboxX, hitboxY, 16.f, 16.f); // Caja de colisión de 16x16 centrada en la base del sprite
    }

};