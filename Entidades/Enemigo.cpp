#include "Enemigo.h"
#include "map.h"
#include <cmath>
#include <iostream>

///=============================================================///
///   CONSTRUCTOR POR DEFECTO
///=============================================================///
Enemigo::Enemigo() {
    _vida_maxima_de_la_entidad = 30;
    _vida_actual_de_la_entidad = 0;
}

///=============================================================///
///   CONSTRUCTOR CON DATOS
///=============================================================///
Enemigo::Enemigo(sf::Vector2f posicion_inicial, const std::string& ruta_de_la_imagen) {
    if (_textura_de_la_entidad.loadFromFile(ruta_de_la_imagen) == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DEL ENEMIGO: " << ruta_de_la_imagen << std::endl;
    }
    _sprite_de_la_entidad.setTexture(_textura_de_la_entidad);
    _sprite_de_la_entidad.setOrigin(32.f, 32.f);
    _sprite_de_la_entidad.setPosition(posicion_inicial);

    _vida_maxima_de_la_entidad = 30;
    _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad;
    _dano_que_hace_esta_entidad = 10;
    _velocidad_de_movimiento = 70.f;
    _segundos_de_cooldown_entre_ataques = 2.f;

    actualizar_el_recorte_del_sprite_segun_la_animacion();
}

///=============================================================///
///   ACTIVAR EN LA POSICION
///=============================================================///
void Enemigo::activar_en_la_posicion(sf::Vector2f posicion_inicial, const std::string& ruta_de_la_imagen) {
    if (_textura_de_la_entidad.loadFromFile(ruta_de_la_imagen) == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DEL ENEMIGO: " << ruta_de_la_imagen << std::endl;
    }
    _sprite_de_la_entidad.setTexture(_textura_de_la_entidad);
    _sprite_de_la_entidad.setOrigin(32.f, 32.f);
    _sprite_de_la_entidad.setPosition(posicion_inicial);

    _vida_maxima_de_la_entidad = 30;
    _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad;
    _dano_que_hace_esta_entidad = 10;
    _velocidad_de_movimiento = 70.f;
    _segundos_de_cooldown_entre_ataques = 2.f;

    actualizar_el_recorte_del_sprite_segun_la_animacion();
}

///=============================================================///
///   RECIBIR DAÑO - POLIMORFISMO
///=============================================================///
void Enemigo::recibir_dano(int cantidad_de_dano_recibido) {
    _vida_actual_de_la_entidad = _vida_actual_de_la_entidad - cantidad_de_dano_recibido;
    std::cout << "EL ENEMIGO GRITO DE DOLOR." << std::endl;
}

///=============================================================///
///   CALCULAR CAJA DE COLISION
///=============================================================///
sf::FloatRect Enemigo::calcular_caja_de_colision() const {
    return _sprite_de_la_entidad.getGlobalBounds();
}

///=============================================================///
///   ACTUALIZAR
///=============================================================///
void Enemigo::actualizar(float tiempo_transcurrido, Map& mapa_del_juego) {
    sf::Vector2f posicion_antes_de_moverse = _sprite_de_la_entidad.getPosition();

    sf::Vector2f direccion_hacia_el_objetivo = _posicion_a_donde_quiere_llegar - posicion_antes_de_moverse;
    float distancia_al_objetivo = std::hypot(direccion_hacia_el_objetivo.x, direccion_hacia_el_objetivo.y);

    if (distancia_al_objetivo > 5.f) {
        direccion_hacia_el_objetivo /= distancia_al_objetivo;
        sf::Vector2f movimiento_de_este_frame = direccion_hacia_el_objetivo * _velocidad_de_movimiento * tiempo_transcurrido;
        mover_con_colisiones(movimiento_de_este_frame, calcular_caja_de_colision(), mapa_del_juego);
    }

    sf::Vector2f posicion_despues_de_moverse = _sprite_de_la_entidad.getPosition();
    sf::Vector2f movimiento_real_de_este_frame = posicion_despues_de_moverse - posicion_antes_de_moverse;

    decidir_animacion_y_direccion_segun_el_movimiento(movimiento_real_de_este_frame);

    _tiempo_acumulado_del_frame_actual += tiempo_transcurrido;
    if (_tiempo_acumulado_del_frame_actual >= _segundos_que_dura_cada_frame) {
        _tiempo_acumulado_del_frame_actual = 0.f;
        _numero_de_frame_actual++;
        avanzar_de_frame_si_corresponde();
    }

    actualizar_el_recorte_del_sprite_segun_la_animacion();
}

///=============================================================///
///   DECIDIR ANIMACION Y DIRECCION
///=============================================================///
void Enemigo::decidir_animacion_y_direccion_segun_el_movimiento(sf::Vector2f direccion_en_la_que_se_mueve) {
    if (std::abs(direccion_en_la_que_se_mueve.x) < 0.1f && std::abs(direccion_en_la_que_se_mueve.y) < 0.1f) {
        _estado_de_animacion_actual = EstadoDeAnimacionDelEnemigo::QUIETO;
        return;
    }

    _estado_de_animacion_actual = EstadoDeAnimacionDelEnemigo::CAMINANDO;

    if (std::abs(direccion_en_la_que_se_mueve.x) > std::abs(direccion_en_la_que_se_mueve.y)) {
        if (direccion_en_la_que_se_mueve.x > 0.f) {
            _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::DERECHA;
        }
        else {
            _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::IZQUIERDA;
        }
    }
    else {
        if (direccion_en_la_que_se_mueve.y > 0.f) {
            _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::ABAJO;
        }
        else {
            _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::ARRIBA;
        }
    }
}

///=============================================================///
///   AVANZAR DE FRAME
///=============================================================///
void Enemigo::avanzar_de_frame_si_corresponde() {
    if (_estado_de_animacion_actual == EstadoDeAnimacionDelEnemigo::QUIETO) {
        _cantidad_maxima_de_frames_de_la_animacion_actual = 1;
        _numero_de_frame_actual = 0;
    }
    else {
        _cantidad_maxima_de_frames_de_la_animacion_actual = 9;
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) {
            _numero_de_frame_actual = 0;
        }
    }
}

///=============================================================///
///   ACTUALIZAR RECORTE DEL SPRITE
///=============================================================///
void Enemigo::actualizar_el_recorte_del_sprite_segun_la_animacion() {
    int fila_del_spritesheet = 2 * 4 + (int)_direccion_hacia_donde_mira;

    int columna_del_spritesheet = 0;
    if (_estado_de_animacion_actual != EstadoDeAnimacionDelEnemigo::QUIETO) {
        columna_del_spritesheet = _numero_de_frame_actual;
    }

    _sprite_de_la_entidad.setTextureRect(sf::IntRect(columna_del_spritesheet * 64, fila_del_spritesheet * 64, 64, 64));
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void Enemigo::dibujar(sf::RenderWindow& ventana_del_juego) {
    dibujar_sprite_y_barra_de_vida(ventana_del_juego);
}