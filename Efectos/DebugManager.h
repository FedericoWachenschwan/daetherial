#pragma once
#include <SFML/Graphics.hpp>
#include "UI_Inventario.h"
#include "Personaje.h"
#include "Golem.h"

///=================================================================///
///   OBJETIVO_DEBUG - A que parte del juego estamos apuntando
///   con las flechas cuando el debug esta activo
///=================================================================///
enum class ObjetivoDebug {
    NINGUNO,
    HUD,
    PERSONAJE,
    ENEMIGO,
    EXTRACTOR
};

///=================================================================///
///   DEBUG_MANAGER - Herramienta de desarrollo. Con F3 activo,
///   permite mover sprites en caliente, ver hitboxes, y encontrar
///   el ID de una textura haciendo clic en el spritesheet
///=================================================================///
class DebugManager {
private:

    bool _modo_debug_activo;
    ObjetivoDebug _objetivo_actual;
    sf::Vector2f _offset_del_extractor;
    sf::Vector2f _posicion_del_tile_marcado;
    bool _hay_tile_marcado = false;

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    DebugManager();

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    bool getEsta_activo() const { return _modo_debug_activo; }
    ObjetivoDebug getObjetivo_actual() const { return _objetivo_actual; }

    ///=============================================================///
    ///   OTROS METODOS - Reciben al Golem por referencia (Golem&)
    ///   en vez de puntero: en la practica siempre se llama con un
    ///   Golem real, nunca con "ninguno", asi que no hace falta un
    ///   puntero que pueda ser nullptr
    ///=============================================================///
    void activar_o_desactivar_debug();
    void procesar_eventos(sf::Event& evento, sf::RenderWindow& ventana, UI_Inventario& hud, Personaje& personaje, Golem& enemigo_en_foco);
    void actualizar(UI_Inventario& hud, Personaje& personaje, Golem& enemigo_en_foco);

    void dibujar_caja_de_colision(sf::RenderWindow& ventana, sf::FloatRect limites_de_la_caja, sf::Color color) const;
    void dibujar_extractor(sf::RenderWindow& ventana, const sf::Texture& textura_maestra);
    void procesar_clic_en_el_mapa(sf::Vector2i posicion_del_clic, const sf::View& vista_activa, const sf::RenderWindow& ventana);
    void dibujar_grilla_del_mapa(sf::RenderWindow& ventana) const;
};