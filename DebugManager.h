#pragma once
#include <SFML/Graphics.hpp>
#include "Colisionable.h"

class UI_Inventario;
class Personaje;
class Enemy; // 🌟 Le avisamos que existe la clase Enemy
class Map; // Forward declaration para dibujar hitboxes del mapa

// 🌟 Agregamos al ENEMIGO en la lista
enum class ObjetivoDebug {
    NINGUNO,
    HUD,
    PERSONAJE,
    ENEMIGO
};

class DebugManager {
private:
    bool _modoDebugActivo;
    ObjetivoDebug _objetivoActual;

public:
    DebugManager();

    void toggleDebug();
    bool estaActivo() const { return _modoDebugActivo; }

    // 🌟 Actualizamos la firma para que reciba al enemigo también
    void procesarEventos(sf::Event& evento, UI_Inventario& hud, Personaje& personaje, Enemy* enemigo);

    // Actualización en tiempo real (movimiento con flechas)
    void actualizar(UI_Inventario& hud, Personaje& personaje, Enemy* enemigo);

    // Dibuja todas las hitboxes útiles: personaje y bloques del mapa
    void dibujarHitboxes(sf::RenderWindow& ventana, Personaje& personaje, Map& mapa);

    void dibujarCajaColision(sf::RenderWindow& ventana, const Colisionable& entidad, sf::Color color) const;
};