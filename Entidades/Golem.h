#pragma once
#include "EntidadViva.h"
#include "PathFinder.h"
#include "map.h"
#include "Personaje.h"


///=================================================================///
///   ESTADO DE COMPORTAMIENTO DEL GOLEM
///=================================================================///
enum class EstadoDelGolem {
    QUIETO,
    PERSIGUIENDO,
    ATACANDO
};

///=================================================================///
///   GOLEM - HERENCIA: Golem ES una EntidadViva. Es el jefe del
///   dungeon, persigue al jugador usando el PathFinder
///=================================================================///
class Golem : public EntidadViva {
private:

    ///=============================================================///
    ///   NAVEGACION
    ///=============================================================///
    sf::Vector2f _camino_actual[PathFinder::CANTIDAD_MAXIMA_DE_PASOS_EN_UN_CAMINO];
    int _cantidad_de_pasos_en_el_camino_actual = 0;
    int _paso_actual_del_camino = 0;
    sf::Clock _reloj_para_no_recalcular_el_camino_todo_el_tiempo;
    sf::Vector2f _posicion_objetivo_actual;
    sf::Vector2f _ultima_posicion_objetivo_calculada;

    ///=============================================================///
    ///   ESTADO Y COMBATE
    ///=============================================================///
    EstadoDelGolem _estado_actual_del_golem = EstadoDelGolem::QUIETO;
    float _rango_de_ataque = 40.f;

    void recalcular_el_camino_si_corresponde(Map& mapa_del_juego);
    void mover_siguiendo_el_camino_calculado(float tiempo_transcurrido, Map& mapa_del_juego);
    void actualizar_animacion_segun_el_movimiento(float tiempo_transcurrido, sf::Vector2f direccion_hacia_donde_se_mueve);

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    Golem(sf::Vector2f posicion_inicial);

    ///=============================================================///
    ///   SETTERS
    ///=============================================================///
    void setPosicion_objetivo(sf::Vector2f posicion_del_jugador) { _posicion_objetivo_actual = posicion_del_jugador; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///

    // Calcula la caja de colision cada vez que se llama (no es un
    // atributo guardado), por eso no lleva "get" adelante
    sf::FloatRect calcular_caja_de_colision() const;

    void actualizar(float tiempo_transcurrido, Map& mapa_del_juego, Personaje& jugador);
    void dibujar_camino_calculado(sf::RenderWindow& ventana_del_juego) const;
    void dibujar(sf::RenderWindow& ventana_del_juego);
};