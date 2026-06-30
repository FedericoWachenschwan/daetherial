#include "Mascota.h"
#include <cmath>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Mascota::Mascota() {
    if (_textura_mirando_abajo.loadFromFile("assets/cat_abajo.png") == false) return;
    if (_textura_mirando_arriba.loadFromFile("assets/cat_arriba.png") == false) return;
    if (_textura_mirando_a_la_derecha.loadFromFile("assets/cat_derecha.png") == false) return;
    if (_textura_mirando_a_la_izquierda.loadFromFile("assets/cat_izquierda.png") == false) return;

    _sprite_de_la_entidad.setTexture(_textura_mirando_abajo);
    _sprite_de_la_entidad.setPosition(200.f, 100.f);

    _velocidad_de_movimiento = 120.f;
    _distancia_maxima_antes_de_seguir = 40.f;
}

///=============================================================///
///   ACTUALIZAR
///=============================================================///
void Mascota::actualizar(float tiempo_transcurrido) {
    sf::Vector2f posicion_de_la_mascota = _sprite_de_la_entidad.getPosition();

    float diferencia_x = _posicion_del_dueno.x - posicion_de_la_mascota.x;
    float diferencia_y = _posicion_del_dueno.y - posicion_de_la_mascota.y;
    float distancia_real = std::hypot(diferencia_x, diferencia_y);

    if (distancia_real > _distancia_maxima_antes_de_seguir) {

        float direccion_x = diferencia_x / distancia_real;
        float direccion_y = diferencia_y / distancia_real;

        _sprite_de_la_entidad.move(direccion_x * _velocidad_de_movimiento * tiempo_transcurrido, direccion_y * _velocidad_de_movimiento * tiempo_transcurrido);

        if (std::abs(diferencia_x) > std::abs(diferencia_y)) {
            if (diferencia_x > 0.f) {
                _sprite_de_la_entidad.setTexture(_textura_mirando_a_la_derecha);
            }
            else {
                _sprite_de_la_entidad.setTexture(_textura_mirando_a_la_izquierda);
            }
        }
        else {
            if (diferencia_y > 0.f) {
                _sprite_de_la_entidad.setTexture(_textura_mirando_abajo);
            }
            else {
                _sprite_de_la_entidad.setTexture(_textura_mirando_arriba);
            }
        }
    }
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void Mascota::dibujar(sf::RenderWindow& ventana_del_juego) {
    dibujar_sprite_y_barra_de_vida(ventana_del_juego);
}

///=============================================================///
///   CALCULAR CAJA DE COLISION
///=============================================================///
sf::FloatRect Mascota::calcular_caja_de_colision() const {
    sf::Vector2f posicion_actual = _sprite_de_la_entidad.getPosition();
    return sf::FloatRect(posicion_actual.x, posicion_actual.y, 32.f, 32.f);
}