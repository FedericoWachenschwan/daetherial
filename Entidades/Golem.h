#pragma once
#include "EntidadViva.h"
#include "PathFinder.h"
#include "map.h"
#include "Personaje.h"


///=================================================================///
///   ESTADO DE COMPORTAMIENTO DEL GOLEM
///=================================================================///
enum class EstadoDelGolem {
    QUIETO, // El golem no persigue a nadie
    PERSIGUIENDO, // El golem sigue al jugador
    ATACANDO // El golem esta golpeando al jugador
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
    sf::Vector2f _camino_actual[PathFinder::CANTIDAD_MAXIMA_DE_PASOS_EN_UN_CAMINO]; // Lista de puntos del camino calculado
    int _cantidad_de_pasos_en_el_camino_actual = 0; // Cuantos puntos tiene el camino actual
    int _paso_actual_del_camino = 0; // En que punto del camino esta ahora
    sf::Clock _reloj_para_no_recalcular_el_camino_todo_el_tiempo; // Evita calcular el camino cada frame
    sf::Vector2f _posicion_objetivo_actual; // Donde esta el jugador ahora
    sf::Vector2f _ultima_posicion_objetivo_calculada; // Donde estaba el jugador cuando se calculo el camino

 ///=============================================================///
 ///   ESTADO Y COMBATE
 ///=============================================================///
    EstadoDelGolem _estado_actual_del_golem = EstadoDelGolem::QUIETO; // Comportamiento actual del golem
    float _rango_de_ataque = 40.f; // Distancia para golpear al jugador

    void recalcular_el_camino_si_corresponde(Map& mapa_del_juego);
    void mover_siguiendo_el_camino_calculado(float tiempo_transcurrido, Map& mapa_del_juego);
    void actualizar_animacion_segun_el_movimiento(float tiempo_transcurrido, sf::Vector2f direccion_hacia_donde_se_mueve);

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    Golem(sf::Vector2f posicion_inicial);

 ///=============================================================///
 ///   SETTERS
 ///=============================================================///
 // #2
    void setPosicion_objetivo(sf::Vector2f posicion_del_jugador) { _posicion_objetivo_actual = posicion_del_jugador; } // Actualiza donde perseguir

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///

 // Calcula la caja de colision cada vez que se llama (no es un
 // atributo guardado), por eso no lleva "get" adelante
 // #3
    sf::FloatRect calcular_caja_de_colision() const;
 // #4
    void actualizar(float tiempo_transcurrido, Map& mapa_del_juego, Personaje& jugador);
 // #5
    void dibujar_camino_calculado(sf::RenderWindow& ventana_del_juego) const;
 // #6
    void dibujar(sf::RenderWindow& ventana_del_juego);
};
