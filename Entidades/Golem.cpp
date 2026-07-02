#include "Golem.h"
#include "Personaje.h"
#include "map.h"
#include <cmath>
#include <iostream>

const int ANCHO_DE_CADA_FRAME_DEL_GOLEM = 152; // Ancho en pixeles de cada imagen del golem
const int ALTO_DE_CADA_FRAME_DEL_GOLEM = 147; // Alto en pixeles de cada imagen del golem

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Golem::Golem(sf::Vector2f posicion_inicial) {
    if (_textura_de_la_entidad.loadFromFile("assets/golemhielo.png") == false) { // Intenta cargar la imagen
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DEL GOLEM." << std::endl; // Avisa si falla
    }

    _sprite_de_la_entidad.setTexture(_textura_de_la_entidad); // Asocia la imagen al sprite
    _sprite_de_la_entidad.setOrigin(76.f, 115.f); // Punto de origen centrado en los pies
    _sprite_de_la_entidad.setPosition(posicion_inicial); // Ubica el golem en el mapa
    _sprite_de_la_entidad.setScale(1.5f, 1.5f); // Lo hace 1.5x mas grande que el original

    _numero_de_frame_actual = 0; // Empieza en el primer frame
    _segundos_que_dura_cada_frame = 0.12f; // Cada frame dura 0.12 segundos

    _sprite_de_la_entidad.setTextureRect(sf::IntRect(0, 0, ANCHO_DE_CADA_FRAME_DEL_GOLEM, ALTO_DE_CADA_FRAME_DEL_GOLEM)); // Recorta el primer frame

    _velocidad_de_movimiento = 55.f; // Velocidad del golem en px/seg
    _vida_maxima_de_la_entidad = 250; // Vida maxima del jefe
    _vida_actual_de_la_entidad = 250; // Arranca con vida completa
    _dano_que_hace_esta_entidad = 25; // Daño que hace al jugador
    _segundos_de_cooldown_entre_ataques = 1.5f; // 1.5 segundos entre golpes
}

///=============================================================///
///   CALCULAR CAJA DE COLISION
///=============================================================///
sf::FloatRect Golem::calcular_caja_de_colision() const {
    sf::Vector2f posicion_actual = _sprite_de_la_entidad.getPosition(); // Posicion actual del golem
    return sf::FloatRect(posicion_actual.x - 16.f, posicion_actual.y - 16.f, 32.f, 32.f); // Caja de 32x32 centrada
}

///=============================================================///
///   ACTUALIZAR
///=============================================================///
void Golem::actualizar(float tiempo_transcurrido, Map& mapa_del_juego, Personaje& jugador) {

    sf::Vector2f posicion_antes_de_actualizar = _sprite_de_la_entidad.getPosition(); // Posicion al inicio del frame
    float distancia_al_jugador = std::hypot(_posicion_objetivo_actual.x - posicion_antes_de_actualizar.x, _posicion_objetivo_actual.y - posicion_antes_de_actualizar.y); // Distancia real al jugador

    if (distancia_al_jugador <= _rango_de_ataque) { // Si el jugador esta muy cerca
        _estado_actual_del_golem = EstadoDelGolem::ATACANDO; // Cambia a estado de ataque

        if (puede_atacar_de_nuevo() == true) { // Si el cooldown de ataque termino
            jugador.recibir_dano(_dano_que_hace_esta_entidad); // Golpea al jugador
            std::cout << "EL GOLEM TE PEGO POR " << _dano_que_hace_esta_entidad << " DE DAÑO." << std::endl; // Avisa en consola
        }
        _cantidad_de_pasos_en_el_camino_actual = 0; // Borra el camino al llegar al jugador
    }
    else if (distancia_al_jugador <= 400.f) { // Si el jugador esta en el rango de persecucion
        _estado_actual_del_golem = EstadoDelGolem::PERSIGUIENDO; // Cambia a estado de persecucion
        recalcular_el_camino_si_corresponde(mapa_del_juego); // Recalcula el camino si hace falta
        mover_siguiendo_el_camino_calculado(tiempo_transcurrido, mapa_del_juego); // Sigue el camino calculado
    }
    else { // El jugador esta demasiado lejos
        _estado_actual_del_golem = EstadoDelGolem::QUIETO; // Se queda quieto
        _cantidad_de_pasos_en_el_camino_actual = 0; // Borra el camino
    }

    sf::Vector2f posicion_despues_de_actualizar = _sprite_de_la_entidad.getPosition(); // Posicion al final del frame
    sf::Vector2f movimiento_real = posicion_despues_de_actualizar - posicion_antes_de_actualizar; // Cuanto se movio realmente

    if (std::hypot(movimiento_real.x, movimiento_real.y) > 0.1f) { // Si se movio algo
        actualizar_animacion_segun_el_movimiento(tiempo_transcurrido, movimiento_real); // Anima segun el movimiento real
    }
    else { // Si no se movio
        sf::Vector2f direccion_hacia_el_jugador = _posicion_objetivo_actual - posicion_despues_de_actualizar; // Calcula donde esta el jugador
        actualizar_animacion_segun_el_movimiento(tiempo_transcurrido, direccion_hacia_el_jugador); // Mira hacia el jugador
    }
}

///=============================================================///
///   RECALCULAR CAMINO
///=============================================================///
void Golem::recalcular_el_camino_si_corresponde(Map& mapa_del_juego) {
    sf::Vector2f centro_actual_del_golem = calcular_centro_fisico(); // Posicion del centro del golem

    float distancia_desde_el_ultimo_calculo = std::hypot(_posicion_objetivo_actual.x - _ultima_posicion_objetivo_calculada.x, _posicion_objetivo_actual.y - _ultima_posicion_objetivo_calculada.y); // Cuanto se movio el jugador
    bool el_jugador_se_movio_mucho = distancia_desde_el_ultimo_calculo > 50.f; // true si se alejo mas de 50px
    bool no_tiene_camino_todavia = _paso_actual_del_camino >= _cantidad_de_pasos_en_el_camino_actual; // true si ya termino el camino

    if (no_tiene_camino_todavia == true || el_jugador_se_movio_mucho == true) { // Si necesita recalcular
        if (_reloj_para_no_recalcular_el_camino_todo_el_tiempo.getElapsedTime().asSeconds() > 0.5f) { // Solo cada 0.5 segundos

            int pasos_encontrados = PathFinder::calcular_camino(mapa_del_juego, centro_actual_del_golem, _posicion_objetivo_actual, _camino_actual); // Calcula el nuevo camino

            if (pasos_encontrados > 0) { // Si se encontro un camino
                _cantidad_de_pasos_en_el_camino_actual = pasos_encontrados; // Guarda la cantidad de pasos
                _paso_actual_del_camino = 0; // Empieza desde el inicio
                _ultima_posicion_objetivo_calculada = _posicion_objetivo_actual; // Registra la posicion del jugador
            }
            _reloj_para_no_recalcular_el_camino_todo_el_tiempo.restart(); // Reinicia el cronometro
        }
    }
}

///=============================================================///
///   MOVER SIGUIENDO EL CAMINO
///=============================================================///
void Golem::mover_siguiendo_el_camino_calculado(float tiempo_transcurrido, Map& mapa_del_juego) {
    if (_paso_actual_del_camino >= _cantidad_de_pasos_en_el_camino_actual) { // Si ya recorrio todo el camino
        return; // No hay mas puntos donde ir
    }

    sf::Vector2f centro_actual_del_golem = calcular_centro_fisico(); // Centro del golem
    sf::Vector2f siguiente_punto_del_camino = _camino_actual[_paso_actual_del_camino]; // Proximo punto del camino

    sf::Vector2f direccion_hacia_el_punto = siguiente_punto_del_camino - centro_actual_del_golem; // Vector hacia el punto
    float distancia_al_punto = std::hypot(direccion_hacia_el_punto.x, direccion_hacia_el_punto.y); // Distancia al punto

    if (distancia_al_punto <= 5.f) { // Si llego cerca del punto
        _paso_actual_del_camino++; // Avanza al siguiente punto del camino
        return; // Espera al proximo frame para moverse al siguiente
    }

    direccion_hacia_el_punto /= distancia_al_punto; // Normaliza la direccion
    sf::Vector2f movimiento_de_este_frame = direccion_hacia_el_punto * _velocidad_de_movimiento * tiempo_transcurrido; // Calcula el desplazamiento
    mover_con_colisiones(movimiento_de_este_frame, calcular_caja_de_colision(), mapa_del_juego); // Mueve evitando paredes
}

///=============================================================///
///   ANIMACION
///=============================================================///
void Golem::actualizar_animacion_segun_el_movimiento(float tiempo_transcurrido, sf::Vector2f direccion_hacia_donde_se_mueve) {

    static int fila_segun_la_direccion = 0; // Guarda la fila entre llamadas (static = persiste)

    if (_estado_actual_del_golem != EstadoDelGolem::QUIETO && (std::abs(direccion_hacia_donde_se_mueve.x) > 0.1f || std::abs(direccion_hacia_donde_se_mueve.y) > 0.1f)) { // Si se mueve o persigue
        if (std::abs(direccion_hacia_donde_se_mueve.x) > std::abs(direccion_hacia_donde_se_mueve.y)) { // Mas movimiento horizontal
            if (direccion_hacia_donde_se_mueve.x > 0.f) { // Va a la derecha
                fila_segun_la_direccion = 2; // Fila de animacion hacia la derecha
            }
            else { // Va a la izquierda
                fila_segun_la_direccion = 1; // Fila de animacion hacia la izquierda
            }
        }
        else { // Mas movimiento vertical
            if (direccion_hacia_donde_se_mueve.y > 0.f) { // Va hacia abajo
                fila_segun_la_direccion = 0; // Fila de animacion hacia abajo
            }
            else { // Va hacia arriba
                fila_segun_la_direccion = 3; // Fila de animacion hacia arriba
            }
        }
    }

    static int paso_de_la_animacion_de_caminata = 1; // Frame actual de la caminata (static = persiste)

    if (_estado_actual_del_golem == EstadoDelGolem::PERSIGUIENDO) { // Si esta persiguiendo
        _tiempo_acumulado_del_frame_actual += tiempo_transcurrido; // Acumula el tiempo
        if (_tiempo_acumulado_del_frame_actual >= _segundos_que_dura_cada_frame) { // Si el frame termino
            int secuencia_de_frames[4] = { 0, 1, 2, 1 }; // Secuencia de frames de caminata
            paso_de_la_animacion_de_caminata = (paso_de_la_animacion_de_caminata + 1) % 4; // Avanza en la secuencia
            _numero_de_frame_actual = secuencia_de_frames[paso_de_la_animacion_de_caminata]; // Aplica el frame
            _tiempo_acumulado_del_frame_actual = 0.f; // Reinicia el cronometro
        }
    }
    else { // Si esta quieto o atacando
        _numero_de_frame_actual = 1; // Frame neutral del golem
        paso_de_la_animacion_de_caminata = 1; // Reinicia la secuencia de caminar
    }

    _sprite_de_la_entidad.setTextureRect(sf::IntRect(
        _numero_de_frame_actual * ANCHO_DE_CADA_FRAME_DEL_GOLEM, // Columna = frame actual
        fila_segun_la_direccion * ALTO_DE_CADA_FRAME_DEL_GOLEM, // Fila = direccion
        ANCHO_DE_CADA_FRAME_DEL_GOLEM, // Ancho del recorte
        ALTO_DE_CADA_FRAME_DEL_GOLEM // Alto del recorte
    )); // Aplica el recorte correcto al sprite
}

///=============================================================///
///   DIBUJAR EL CAMINO CALCULADO
///=============================================================///
void Golem::dibujar_camino_calculado(sf::RenderWindow& ventana_del_juego) const {
    for (int i = _paso_actual_del_camino; i < _cantidad_de_pasos_en_el_camino_actual; i++) { // Para cada punto restante del camino
        sf::CircleShape puntito(3.f); // Circulo pequeño de 3px de radio
        puntito.setFillColor(sf::Color::Yellow); // Color amarillo para visualizar
        puntito.setOrigin(1.5f, 1.5f); // Centra el puntito
        puntito.setPosition(_camino_actual[i]); // Lo ubica en el punto del camino
        ventana_del_juego.draw(puntito); // Lo dibuja en pantalla
    }
}

void Golem::dibujar(sf::RenderWindow& ventana_del_juego) {
    dibujar_sprite_y_barra_de_vida(ventana_del_juego); // Dibuja el golem y su barra de vida
}
