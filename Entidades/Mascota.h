#pragma once
#include "EntidadViva.h"

///=================================================================///
///   MASCOTA - HERENCIA: Mascota ES una EntidadViva. Es el gatito
///   que sigue al jugador por el mapa
///=================================================================///
class Mascota : public EntidadViva {
private:

    sf::Texture _textura_mirando_abajo; // Imagen del gato mirando hacia abajo
    sf::Texture _textura_mirando_arriba; // Imagen del gato mirando hacia arriba
    sf::Texture _textura_mirando_a_la_derecha; // Imagen del gato mirando a la derecha
    sf::Texture _textura_mirando_a_la_izquierda; // Imagen del gato mirando a la izquierda

    float _distancia_maxima_antes_de_seguir; // A que distancia empieza a seguir al jugador
    sf::Vector2f _posicion_del_dueno; // Posicion actual del jugador

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
    Mascota();

 ///=============================================================///
 ///   SETTERS
 ///=============================================================///
    void setPosicion_objetivo(sf::Vector2f posicion_del_personaje) { _posicion_del_dueno = posicion_del_personaje; } // Actualiza donde esta el jugador

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///

 // Calcula la caja de colision cada vez que se llama (no es un
 // atributo guardado), por eso no lleva "get" adelante
    sf::FloatRect calcular_caja_de_colision() const;

    void actualizar(float tiempo_transcurrido);
    void dibujar(sf::RenderWindow& ventana_del_juego);
};
