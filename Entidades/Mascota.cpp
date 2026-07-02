#include "Mascota.h"
#include <cmath>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Mascota::Mascota() {
    if (_textura_mirando_abajo.loadFromFile("assets/cat_abajo.png") == false) return; // Carga imagen mirando abajo
    if (_textura_mirando_arriba.loadFromFile("assets/cat_arriba.png") == false) return; // Carga imagen mirando arriba
    if (_textura_mirando_a_la_derecha.loadFromFile("assets/cat_derecha.png") == false) return; // Carga imagen mirando derecha
    if (_textura_mirando_a_la_izquierda.loadFromFile("assets/cat_izquierda.png") == false) return; // Carga imagen mirando izquierda

    _sprite_de_la_entidad.setTexture(_textura_mirando_abajo); // Empieza mirando hacia abajo
    _sprite_de_la_entidad.setPosition(200.f, 100.f); // Posicion inicial en el mapa

    _velocidad_de_movimiento = 120.f; // Velocidad de la mascota en px/seg
    _distancia_maxima_antes_de_seguir = 40.f; // Sigue al jugador si se aleja mas de 40px
}

///=============================================================///
///   ACTUALIZAR
///=============================================================///
void Mascota::actualizar(float tiempo_transcurrido) {
    sf::Vector2f posicion_de_la_mascota = _sprite_de_la_entidad.getPosition(); // Posicion actual del gato

    float diferencia_x = _posicion_del_dueno.x - posicion_de_la_mascota.x; // Distancia horizontal al jugador
    float diferencia_y = _posicion_del_dueno.y - posicion_de_la_mascota.y; // Distancia vertical al jugador
    float distancia_real = std::hypot(diferencia_x, diferencia_y); // Distancia total al jugador

    if (distancia_real > _distancia_maxima_antes_de_seguir) { // Si el jugador esta muy lejos

        float direccion_x = diferencia_x / distancia_real; // Normaliza el eje X
        float direccion_y = diferencia_y / distancia_real; // Normaliza el eje Y

        _sprite_de_la_entidad.move(direccion_x * _velocidad_de_movimiento * tiempo_transcurrido, direccion_y * _velocidad_de_movimiento * tiempo_transcurrido); // Mueve hacia el jugador

        if (std::abs(diferencia_x) > std::abs(diferencia_y)) { // Si la diferencia horizontal es mayor
            if (diferencia_x > 0.f) { // El jugador esta a la derecha
                _sprite_de_la_entidad.setTexture(_textura_mirando_a_la_derecha); // Cambia la imagen a derecha
            }
            else { // El jugador esta a la izquierda
                _sprite_de_la_entidad.setTexture(_textura_mirando_a_la_izquierda); // Cambia la imagen a izquierda
            }
        }
        else { // Si la diferencia vertical es mayor
            if (diferencia_y > 0.f) { // El jugador esta abajo
                _sprite_de_la_entidad.setTexture(_textura_mirando_abajo); // Cambia la imagen a abajo
            }
            else { // El jugador esta arriba
                _sprite_de_la_entidad.setTexture(_textura_mirando_arriba); // Cambia la imagen a arriba
            }
        }
    }
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void Mascota::dibujar(sf::RenderWindow& ventana_del_juego) {
    dibujar_sprite_y_barra_de_vida(ventana_del_juego); // Dibuja el gato y su barra de vida
}

///=============================================================///
///   CALCULAR CAJA DE COLISION
///=============================================================///
sf::FloatRect Mascota::calcular_caja_de_colision() const {
    sf::Vector2f posicion_actual = _sprite_de_la_entidad.getPosition(); // Posicion actual del gato
    return sf::FloatRect(posicion_actual.x, posicion_actual.y, 32.f, 32.f); // Caja de 32x32 px
}
