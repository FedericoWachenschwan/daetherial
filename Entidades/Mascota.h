#pragma once
#include "EntidadViva.h"

///=================================================================///
///   MASCOTA - HERENCIA: Mascota ES una EntidadViva. Es el gatito
///   que sigue al jugador por el mapa
///=================================================================///
class Mascota : public EntidadViva {
private:

    sf::Texture _textura_mirando_abajo;
    sf::Texture _textura_mirando_arriba;
    sf::Texture _textura_mirando_a_la_derecha;
    sf::Texture _textura_mirando_a_la_izquierda;

    float _distancia_maxima_antes_de_seguir;
    sf::Vector2f _posicion_del_dueno;

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    Mascota();

    ///=============================================================///
    ///   SETTERS
    ///=============================================================///
    void setPosicion_objetivo(sf::Vector2f posicion_del_personaje) { _posicion_del_dueno = posicion_del_personaje; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///

    // Calcula la caja de colision cada vez que se llama (no es un
    // atributo guardado), por eso no lleva "get" adelante
    sf::FloatRect calcular_caja_de_colision() const;

    void actualizar(float tiempo_transcurrido);
    void dibujar(sf::RenderWindow& ventana_del_juego);
};