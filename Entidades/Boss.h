#pragma once
#include "EntidadViva.h"
#include "PathFinder.h"
#include "map.h"
#include <vector>
#include <iostream> // Necesario para el cout del origen


enum class BossState {
	IDLE,
	PERSIGUIENDO,
	ATACANDO
};


class Boss : public EntidadViva {
protected:
    // --- REFERENCIAS Y DATOS DE NAVEGACIÓN --- Pathfinding y navegación
    Map* _mapaRef;
    sf::Clock _relojPathfinding;
    sf::Vector2f _posicionObjetivo;
    sf::Vector2f _ultimoDestinoConocido;
    std::vector<sf::Vector2f> _caminoActual;

    // --- REFERENCIA DE COMBATE ---
    EntidadViva* _jugadorVivoRef = nullptr; // Puntero al jugador para aplicar daño (inicializado externamente)

    BossState _estadoActual;
    void actualizarAnimacion(float dt, sf::Vector2f direccion, BossState estado);

public:
    Boss();
    Boss(sf::Vector2f posInicial, Map* mapa);

    void setPosicionObjetivo(sf::Vector2f posJugador);
    void actualizar(float dt) override;
    sf::FloatRect getBounds() const override;

    void dibujarPathFinder(sf::RenderWindow& ventana) const;
    void dibujarHitboxEnemy(sf::RenderWindow& ventana) const;
    void activarPathfinder(float dt, sf::Vector2f posActual);
    void setObjetivoJugador(EntidadViva* jugador) { _jugadorVivoRef = jugador; }
    void ajustarOrigenSprite(float dx, float dy);

};