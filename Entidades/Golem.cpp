#include "Golem.h"
#include "Personaje.h"
#include "map.h"
#include <cmath>
#include <iostream>

const int ANCHO_DE_CADA_FRAME_DEL_GOLEM = 152;
const int ALTO_DE_CADA_FRAME_DEL_GOLEM = 147;

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Golem::Golem(sf::Vector2f posicion_inicial) {
    if (_textura_de_la_entidad.loadFromFile("assets/golemhielo.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DEL GOLEM." << std::endl;
    }

    _sprite_de_la_entidad.setTexture(_textura_de_la_entidad);
    _sprite_de_la_entidad.setOrigin(76.f, 115.f);
    _sprite_de_la_entidad.setPosition(posicion_inicial);
    _sprite_de_la_entidad.setScale(1.5f, 1.5f);

    _numero_de_frame_actual = 0;
    _segundos_que_dura_cada_frame = 0.12f;

    _sprite_de_la_entidad.setTextureRect(sf::IntRect(0, 0, ANCHO_DE_CADA_FRAME_DEL_GOLEM, ALTO_DE_CADA_FRAME_DEL_GOLEM));

    _velocidad_de_movimiento = 55.f;
    _vida_maxima_de_la_entidad = 250;
    _vida_actual_de_la_entidad = 250;
    _dano_que_hace_esta_entidad = 25;
    _segundos_de_cooldown_entre_ataques = 1.5f;
}

///=============================================================///
///   CALCULAR CAJA DE COLISION
///=============================================================///
sf::FloatRect Golem::calcular_caja_de_colision() const {
    sf::Vector2f posicion_actual = _sprite_de_la_entidad.getPosition();
    return sf::FloatRect(posicion_actual.x - 16.f, posicion_actual.y - 16.f, 32.f, 32.f);
}

///=============================================================///
///   ACTUALIZAR
///=============================================================///
void Golem::actualizar(float tiempo_transcurrido, Map& mapa_del_juego, Personaje& jugador) {

    sf::Vector2f posicion_antes_de_actualizar = _sprite_de_la_entidad.getPosition();
    float distancia_al_jugador = std::hypot(_posicion_objetivo_actual.x - posicion_antes_de_actualizar.x, _posicion_objetivo_actual.y - posicion_antes_de_actualizar.y);

    if (distancia_al_jugador <= _rango_de_ataque) {
        _estado_actual_del_golem = EstadoDelGolem::ATACANDO;

        if (puede_atacar_de_nuevo() == true) {
            jugador.recibir_dano(_dano_que_hace_esta_entidad);
            std::cout << "EL GOLEM TE PEGO POR " << _dano_que_hace_esta_entidad << " DE DAÑO." << std::endl;
        }
        _cantidad_de_pasos_en_el_camino_actual = 0;
    }
    else if (distancia_al_jugador <= 400.f) {
        _estado_actual_del_golem = EstadoDelGolem::PERSIGUIENDO;
        recalcular_el_camino_si_corresponde(mapa_del_juego);
        mover_siguiendo_el_camino_calculado(tiempo_transcurrido, mapa_del_juego);
    }
    else {
        _estado_actual_del_golem = EstadoDelGolem::QUIETO;
        _cantidad_de_pasos_en_el_camino_actual = 0;
    }

    sf::Vector2f posicion_despues_de_actualizar = _sprite_de_la_entidad.getPosition();
    sf::Vector2f movimiento_real = posicion_despues_de_actualizar - posicion_antes_de_actualizar;

    if (std::hypot(movimiento_real.x, movimiento_real.y) > 0.1f) {
        actualizar_animacion_segun_el_movimiento(tiempo_transcurrido, movimiento_real);
    }
    else {
        sf::Vector2f direccion_hacia_el_jugador = _posicion_objetivo_actual - posicion_despues_de_actualizar;
        actualizar_animacion_segun_el_movimiento(tiempo_transcurrido, direccion_hacia_el_jugador);
    }
}

///=============================================================///
///   RECALCULAR CAMINO
///=============================================================///
void Golem::recalcular_el_camino_si_corresponde(Map& mapa_del_juego) {
    sf::Vector2f centro_actual_del_golem = calcular_centro_fisico();

    float distancia_desde_el_ultimo_calculo = std::hypot(_posicion_objetivo_actual.x - _ultima_posicion_objetivo_calculada.x, _posicion_objetivo_actual.y - _ultima_posicion_objetivo_calculada.y);
    bool el_jugador_se_movio_mucho = distancia_desde_el_ultimo_calculo > 50.f;
    bool no_tiene_camino_todavia = _paso_actual_del_camino >= _cantidad_de_pasos_en_el_camino_actual;

    if (no_tiene_camino_todavia == true || el_jugador_se_movio_mucho == true) {
        if (_reloj_para_no_recalcular_el_camino_todo_el_tiempo.getElapsedTime().asSeconds() > 0.5f) {

            int pasos_encontrados = PathFinder::calcular_camino(mapa_del_juego, centro_actual_del_golem, _posicion_objetivo_actual, _camino_actual);

            if (pasos_encontrados > 0) {
                _cantidad_de_pasos_en_el_camino_actual = pasos_encontrados;
                _paso_actual_del_camino = 0;
                _ultima_posicion_objetivo_calculada = _posicion_objetivo_actual;
            }
            _reloj_para_no_recalcular_el_camino_todo_el_tiempo.restart();
        }
    }
}

///=============================================================///
///   MOVER SIGUIENDO EL CAMINO
///=============================================================///
void Golem::mover_siguiendo_el_camino_calculado(float tiempo_transcurrido, Map& mapa_del_juego) {
    if (_paso_actual_del_camino >= _cantidad_de_pasos_en_el_camino_actual) {
        return;
    }

    sf::Vector2f centro_actual_del_golem = calcular_centro_fisico();
    sf::Vector2f siguiente_punto_del_camino = _camino_actual[_paso_actual_del_camino];

    sf::Vector2f direccion_hacia_el_punto = siguiente_punto_del_camino - centro_actual_del_golem;
    float distancia_al_punto = std::hypot(direccion_hacia_el_punto.x, direccion_hacia_el_punto.y);

    if (distancia_al_punto <= 5.f) {
        _paso_actual_del_camino++;
        return;
    }

    direccion_hacia_el_punto /= distancia_al_punto;
    sf::Vector2f movimiento_de_este_frame = direccion_hacia_el_punto * _velocidad_de_movimiento * tiempo_transcurrido;
    mover_con_colisiones(movimiento_de_este_frame, calcular_caja_de_colision(), mapa_del_juego);
}

///=============================================================///
///   ANIMACION
///=============================================================///
void Golem::actualizar_animacion_segun_el_movimiento(float tiempo_transcurrido, sf::Vector2f direccion_hacia_donde_se_mueve) {

    static int fila_segun_la_direccion = 0;

    if (_estado_actual_del_golem != EstadoDelGolem::QUIETO && (std::abs(direccion_hacia_donde_se_mueve.x) > 0.1f || std::abs(direccion_hacia_donde_se_mueve.y) > 0.1f)) {
        if (std::abs(direccion_hacia_donde_se_mueve.x) > std::abs(direccion_hacia_donde_se_mueve.y)) {
            if (direccion_hacia_donde_se_mueve.x > 0.f) {
                fila_segun_la_direccion = 2;
            }
            else {
                fila_segun_la_direccion = 1;
            }
        }
        else {
            if (direccion_hacia_donde_se_mueve.y > 0.f) {
                fila_segun_la_direccion = 0;
            }
            else {
                fila_segun_la_direccion = 3;
            }
        }
    }

    static int paso_de_la_animacion_de_caminata = 1;

    if (_estado_actual_del_golem == EstadoDelGolem::PERSIGUIENDO) {
        _tiempo_acumulado_del_frame_actual += tiempo_transcurrido;
        if (_tiempo_acumulado_del_frame_actual >= _segundos_que_dura_cada_frame) {
            int secuencia_de_frames[4] = { 0, 1, 2, 1 };
            paso_de_la_animacion_de_caminata = (paso_de_la_animacion_de_caminata + 1) % 4;
            _numero_de_frame_actual = secuencia_de_frames[paso_de_la_animacion_de_caminata];
            _tiempo_acumulado_del_frame_actual = 0.f;
        }
    }
    else {
        _numero_de_frame_actual = 1;
        paso_de_la_animacion_de_caminata = 1;
    }

    _sprite_de_la_entidad.setTextureRect(sf::IntRect(
        _numero_de_frame_actual * ANCHO_DE_CADA_FRAME_DEL_GOLEM,
        fila_segun_la_direccion * ALTO_DE_CADA_FRAME_DEL_GOLEM,
        ANCHO_DE_CADA_FRAME_DEL_GOLEM,
        ALTO_DE_CADA_FRAME_DEL_GOLEM
    ));
}

///=============================================================///
///   DIBUJAR EL CAMINO CALCULADO
///=============================================================///
void Golem::dibujar_camino_calculado(sf::RenderWindow& ventana_del_juego) const {
    for (int i = _paso_actual_del_camino; i < _cantidad_de_pasos_en_el_camino_actual; i++) {
        sf::CircleShape puntito(3.f);
        puntito.setFillColor(sf::Color::Yellow);
        puntito.setOrigin(1.5f, 1.5f);
        puntito.setPosition(_camino_actual[i]);
        ventana_del_juego.draw(puntito);
    }
}

void Golem::dibujar(sf::RenderWindow& ventana_del_juego) {
    dibujar_sprite_y_barra_de_vida(ventana_del_juego);
}