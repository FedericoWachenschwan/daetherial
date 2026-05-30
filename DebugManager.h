#pragma once
#include <SFML/Graphics.hpp>

// 🌟 Forward Declarations: Le decimos al compilador que confíe en que estas clases existen
class Personaje;
class UI_Inventario;
class Map;

// 🌟 La lista de cosas que podemos editar
enum class ObjetivoDebug {
    NINGUNO,
    HUD,
    PERSONAJE
    // Golem, Mascota, Niebla... (para el futuro)
};

class DebugManager {
private:
    bool _modoDebugActivo;
    ObjetivoDebug _objetivoActual; // Guardamos qué estamos tocando ahora mismo

public:
    DebugManager();

    void toggleDebug();
    bool estaActivo() const { return _modoDebugActivo; }

    void procesarEventos(sf::Event& evento, UI_Inventario& hud, Personaje& personaje);

    // 🌟 CORREGIDO: Ahora dice Map& en lugar de Mapa&
    void dibujarHitboxes(sf::RenderWindow& ventana, Personaje& personaje, Map& mapa);
};