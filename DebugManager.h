#pragma once
#include <SFML/Graphics.hpp>
#include "Colisionable.h"

enum class ObjetivoDebug {
    NINGUNO,
    HUD,
    PERSONAJE,
    ENEMIGO,
    EXTRACTOR
};

class UI_Inventario;
class Personaje;
class Enemy;
class Colisionable;
class Map;

class DebugManager {
private:
    bool _modoDebugActivo;
    ObjetivoDebug _objetivoActual;
    
    sf::Vector2f _offsetExtractor; //Para guardar cuánto scrolleaste la imagen gigante

public:
    DebugManager();

    void toggleDebug();
    bool estaActivo() const { return _modoDebugActivo; }
	
    ObjetivoDebug getObjetivoActual() const { return _objetivoActual; }

    // 🌟 Actualizamos la firma para que reciba la ventana y al enemigo también
    void procesarEventos(sf::Event& evento, sf::RenderWindow& ventana, UI_Inventario& hud, Personaje& personaje, Enemy* enemigo);

    // Actualización en tiempo real (movimiento con flechas)
    void actualizar(UI_Inventario& hud, Personaje& personaje, Enemy* enemigo);

    // Dibuja todas las hitboxes útiles: personaje y bloques del mapa
    void dibujarHitboxes(sf::RenderWindow& ventana, Personaje& personaje, Map& mapa);

    // Dibuja la pantalla del extractor (spritesheet gigante)
    void dibujarExtractor(sf::RenderWindow& ventana, const sf::Texture& texturaMaestra);

    void dibujarCajaColision(sf::RenderWindow& ventana, const Colisionable& entidad, sf::Color color) const;
};