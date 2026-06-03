#pragma once
#include "EntidadViva.h"
#include "PathFinder.h"
#include "map.h"
#include <vector>
#include <iostream> // Necesario para el cout del origen

class Enemy : public EntidadViva {
private:
	// --- REFERENCIAS Y DATOS DE NAVEGACIÓN --- Pathfinding y navegación
    Map* _mapaRef;
    sf::Clock _relojPathfinding;
    sf::Vector2f _posicionObjetivo;
    sf::Vector2f _ultimoDestinoConocido;
    std::vector<sf::Vector2f> _caminoActual;

    //ENEMIGO NECESITA SABER A QUIEN ATACAR
	EntidadViva* _jugadorVivoRef = nullptr; // Puntero al jugador para aplicar daño (inicializado externamente)


   

public:
    Enemy();
    Enemy(sf::Vector2f posInicial, Map* mapa);

    void setPosicionObjetivo(sf::Vector2f posJugador);
    void actualizar(float dt) override;
    sf::FloatRect getBounds() const override;

    void dibujarPathFinder(sf::RenderWindow& ventana) const;
    void dibujarHitboxEnemy(sf::RenderWindow& ventana) const;
    void activarPathfinder(float dt, sf::Vector2f posActual);
	void setObjetivoJugador(EntidadViva* jugador) { _jugadorVivoRef = jugador; }

	// FUNCIONES DE AJUSTE DE ORIGEN PARA CALIBRACIÓN DE HITBOX
    void ajustarOrigenSprite(float dx, float dy) {
        sf::Vector2f origenActual = _sprite.getOrigin();
        _sprite.setOrigin(origenActual.x + dx, origenActual.y + dy);
        std::cout << "Origen Gólem ajustado: X=" << _sprite.getOrigin().x << " Y=" << _sprite.getOrigin().y << std::endl;
    }
};