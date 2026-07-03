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
    NINGUNO, // Las flechas no hacen nada
    HUD, // Las flechas mueven el inventario en pantalla
    PERSONAJE, // Las flechas ajustan el origen del sprite del jugador
    ENEMIGO // Las flechas ajustan el origen del sprite del enemigo
};

///=================================================================///
///   DEBUG_MANAGER - Herramienta de desarrollo. Con F3 activo,
///   permite mover sprites en caliente, ver hitboxes, y encontrar
///   el ID de una textura haciendo clic en el spritesheet
///=================================================================///
class DebugManager {
private:

    bool _modo_debug_activo; // Si es verdadero el modo debug esta encendido
    ObjetivoDebug _objetivo_actual; // A que elemento apuntan las teclas de debug
    sf::Vector2f _posicion_del_tile_marcado; // Donde se hizo clic en el mapa
    bool _hay_tile_marcado = false; // Si hay un tile marcado para mostrar

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    DebugManager();

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #2
    bool getEsta_activo() const { return _modo_debug_activo; } // Devuelve si el debug esta encendido
 // #3
    ObjetivoDebug getObjetivo_actual() const { return _objetivo_actual; } // Devuelve a que elemento apuntamos

 ///=============================================================///
 ///   OTROS METODOS - Reciben al Golem por referencia (Golem&)
 ///   en vez de puntero: en la practica siempre se llama con un
 ///   Golem real, nunca con "ninguno", asi que no hace falta un
 ///   puntero que pueda ser nullptr
 ///=============================================================///
 // #4
    void activar_o_desactivar_debug();
 // #5
    void procesar_eventos(sf::Event& evento, sf::RenderWindow& ventana, UI_Inventario& hud, Personaje& personaje, Golem& enemigo_en_foco);
 // #6
    void actualizar(UI_Inventario& hud, Personaje& personaje, Golem& enemigo_en_foco);
 // #7
    void dibujar_caja_de_colision(sf::RenderWindow& ventana, sf::FloatRect limites_de_la_caja, sf::Color color) const;
 // #8
    void procesar_clic_en_el_mapa(sf::Vector2i posicion_del_clic, const sf::View& vista_activa, const sf::RenderWindow& ventana);
 // #9
    void dibujar_grilla_del_mapa(sf::RenderWindow& ventana) const;
};
