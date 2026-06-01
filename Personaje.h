#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"           
#include "Colisionable.h" 
#include "Inventario.h"
#include "BolaDeFuego.h"

class InputManager;

// =========================================================================
// 🌟 ENUM ALINEADO CON LAS 21 FILAS DE LA MATRIZ LPC (Nombres en Inglés)
// =========================================================================
enum class EstadoPersonaje {
    SPELLCAST = 0,  // Magia / Conjuro (Filas 0-3)
    THRUST = 1,     // Estocada / Lanza (Filas 4-7)
    WALK = 2,       // Caminar (Filas 8-11) -> Mapea movimiento y base de IDLE
    SLASH = 3,      // Espadazo / Corte (Filas 12-15)
    SHOOT = 4,      // Disparar Arco / Proyectil (Filas 16-19)
    HURT = 5,       // Herido / Muerte (Fila 20)

    // Estados lógicos de juego (No modifican directamente el multiplicador de fila)
    IDLE,           // Quieto / Espera
    AIMING          // Apuntando el rango de la habilidad
};

enum class DireccionLPC {
    UP = 0,         // Arriba
    LEFT = 1,       // Izquierda
    DOWN = 2,       // Abajo
    RIGHT = 3       // Derecha
};

class Personaje : public Colisionable {
private:
    // --- COMPONENTES VISUALES ---
    sf::Sprite sprite_del_personaje;
    sf::Texture textura_completa_lpc;

    // --- VARIABLES DE ESTADO ---
    EstadoPersonaje _estadoActual = EstadoPersonaje::IDLE;
    DireccionLPC _direccionActual = DireccionLPC::DOWN;

    // --- ANIMACIÓN ---
    int _frameActual = 0;
    float _tiempoFrame = 0.f;
    float _velocidadAnimacion = 0.09f;
    int _maxFrames = 1;

    // --- MÓDULOS ---
    float velocidad = 2.f;
    Inventario _inventario;
    BolaDeFuego _bolaDeFuego; // Tipo de clase alineado con "BolaDeFuego.h"

    // --- 🎯 INDICADOR DE RANGO ---
    sf::CircleShape _circuloRango;
    float _radioAlcance = 200.f;
    float _radioActual = 0.f;

    // MÉTODOS PRIVADOS DE REFACTORIZACIÓN (Limpieza de código)
    void determinarEstadoYDireccion(sf::Vector2f direccion);
    void procesarHabilidades(const InputManager& input, sf::RenderWindow& ventana, bool uiCapturaMouse);
    void resolverColisiones(sf::Vector2f movimiento, Map& mapa);
    void controlarLimitesYTransiciones();
    void actualizarSpriteRect();

public:
    Personaje();
    Inventario& getInventario() { return _inventario; }
    const Inventario& getInventario() const { return _inventario; }

    // Manejo de input con bypass para evitar disparar interactuando con la UI
    void manejarInput(const InputManager& input, Map& mapa, sf::RenderWindow& ventana, bool uiCapturaMouse);
    void actualizar(float dt);

    sf::Vector2f getPosicion() const;
    void dibujar(sf::RenderWindow& ventana);
    void dibujarDebug(sf::RenderWindow& ventana);
    void ajustarOrigenSprite(float x, float y);

    // =========================================================================
    // 🌟 🛡️ EL CONTRATO OBLIGATORIO: getBounds() en los pies
    sf::FloatRect getBounds() const override {
        sf::Vector2f pos = sprite_del_personaje.getPosition();
        float hitboxX = pos.x - 8.f;  // Centra la caja de 16 de ancho
        float hitboxY = pos.y + 16.f; // La baja a la base de los pies (32 + 16 = 48)
        return sf::FloatRect(hitboxX, hitboxY, 16.f, 16.f);
    }
};