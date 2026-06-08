#pragma once
#include "EntidadViva.h" 
#include "Inventario.h"
#include "BolaDeFuego.h"
#include "VisualFX.h"

class InputManager;

// =========================================================================
// 🌟 ENUM ALINEADO CON LAS 21 FILAS QDE LA MATRIZ LPC
// =========================================================================
enum class EstadoPersonaje {
	SPELLCAST = 0, // El estado de lanzar magia (con animación de carga)
	THRUST = 1, // El estado de ataque cuerpo a cuerpo (con animación de estocada)
	WALK = 2, // El estado de movimiento normal (con animación de caminata)
	SLASH = 3, // El estado de ataque cuerpo a cuerpo alternativo (con animación de tajo horizontal)
	SHOOT = 4, // El estado de ataque a distancia (con animación de disparo)
	HURT = 5, // El estado de recibir daño (con animación de golpe)
	IDLE, // El estado de estar quieto (con animación de respiración)
	AIMING, // El estado de apuntar la magia (con animación de preparación)
	MUERTO, // El estado de muerte (con animación de caída al suelo)
	DASH // El estado de impulso rápido (FX de walk ghost trail)
};

enum class DireccionLPC {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
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