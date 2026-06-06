#pragma once
#include "Boss.h"
#include "Personaje.h" // Para heredar los estados del Personaje

class Enemy : public EntidadViva {
private:
    // --- VARIABLES DE ANIMACIÓN ---
    EstadoPersonaje _estadoActual = EstadoPersonaje::IDLE;
    DireccionLPC _direccionActual = DireccionLPC::DOWN;
    Map* _mapaRef;
    // --- MÉTODOS PRIVADOS DE ANIMACIÓN ---
    void determinarEstadoYDireccion(sf::Vector2f direccionMovimiento);
    void controlarLimitesYTransiciones();
    void actualizarSpriteRect();
    sf::Vector2f _posicionObjetivo;

public:
    // 🌟 Recibe la ruta del PNG para que puedas crear distintos monstruos con la misma clase
    Enemy(sf::Vector2f posInicial, Map* mapa, const std::string& rutaTextura);

    // Sobrescribimos el actualizar para combinar la IA con la animación
    void actualizar(float dt) override;
    void setPosicionObjetivo(sf::Vector2f pos) override { _posicionObjetivo = pos; }

    void dibujar(sf::RenderWindow& ventana) override;
    sf::FloatRect getBounds() const override;
};