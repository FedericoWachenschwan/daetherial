#include "Personaje.h"
#include "InputManager.h"
#include <iostream>
#include <cmath>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Personaje::Personaje() {
    if (_textura_de_la_entidad.loadFromFile("assets/maguito_main.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA HOJA DE SPRITES DEL PERSONAJE." << std::endl;
    }
    _sprite_de_la_entidad.setTexture(_textura_de_la_entidad);
    _sprite_de_la_entidad.setPosition(100.f, 100.f);
    _sprite_de_la_entidad.setOrigin(32.f, 32.f);

    _velocidad_de_movimiento = 170.f;
    _vida_maxima_de_la_entidad = 100;
    _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad;
    _dano_que_hace_esta_entidad = 50;
    _segundos_de_cooldown_entre_ataques = 0.5f;

    _circulo_que_muestra_el_alcance.setRadius(_radio_de_alcance_del_hechizo);
    _circulo_que_muestra_el_alcance.setFillColor(sf::Color(0, 0, 0, 50));
    _circulo_que_muestra_el_alcance.setOutlineColor(sf::Color::White);
    _circulo_que_muestra_el_alcance.setOrigin(_radio_de_alcance_del_hechizo, _radio_de_alcance_del_hechizo);

    actualizar_el_recorte_del_sprite_segun_la_animacion();
}

///=============================================================///
///   RESTAURAR MANA
///=============================================================///
void Personaje::restaurar_mana(int puntos_de_mana_a_restaurar) {
    int mana_despues_de_restaurar = _mana_actual_del_jugador + puntos_de_mana_a_restaurar;

    if (mana_despues_de_restaurar > _mana_maxima_del_jugador) {
        _mana_actual_del_jugador = _mana_maxima_del_jugador;
    }
    else if (mana_despues_de_restaurar < 0) {
        _mana_actual_del_jugador = 0;
    }
    else {
        _mana_actual_del_jugador = mana_despues_de_restaurar;
    }
}

///=============================================================///
///   CALCULAR CAJA DE COLISION
///=============================================================///
sf::FloatRect Personaje::calcular_caja_de_colision() const {
    sf::Vector2f posicion_actual = _sprite_de_la_entidad.getPosition();
    float posicion_x_de_la_caja = posicion_actual.x - 8.f;
    float posicion_y_de_la_caja = posicion_actual.y + 16.f;
    return sf::FloatRect(posicion_x_de_la_caja, posicion_y_de_la_caja, 16.f, 16.f);
}

///=============================================================///
///   PROCESAR MOVIMIENTO Y ENTRADA
///=============================================================///
void Personaje::procesar_movimiento_y_entrada_del_jugador(const InputManager& entrada_del_jugador, Map& mapa_del_juego, sf::RenderWindow& ventana_del_juego, bool la_interfaz_le_esta_tapando_el_mouse, float tiempo_transcurrido) {

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::ESQUIVANDO) {
        mover_con_colisiones(_velocidad_actual_del_movimiento * tiempo_transcurrido, calcular_caja_de_colision(), mapa_del_juego);
        return;
    }

    bool no_puede_moverse_libremente =
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::LANZANDO_HECHIZO ||
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::LASTIMADO ||
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::MUERTO ||
        la_interfaz_le_esta_tapando_el_mouse;

    if (no_puede_moverse_libremente == true) {
        return;
    }

    if (_segundos_de_espera_para_volver_a_esquivar > 0.f) {
        _segundos_de_espera_para_volver_a_esquivar -= tiempo_transcurrido;
    }

    sf::Vector2f direccion_que_quiere_el_jugador = entrada_del_jugador.getDireccion_de_movimiento();

    if (entrada_del_jugador.getEl_jugador_quiere_correr() && _segundos_de_espera_para_volver_a_esquivar <= 0.f) {
        if (direccion_que_quiere_el_jugador.x != 0.f || direccion_que_quiere_el_jugador.y != 0.f) {
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::ESQUIVANDO;
            _segundos_que_quedan_de_esquive = _duracion_total_del_esquive;
            _segundos_de_espera_para_volver_a_esquivar = 1.5f;
            _velocidad_actual_del_movimiento = direccion_que_quiere_el_jugador * (_velocidad_de_movimiento * 4.0f);
            return;
        }
    }

    sf::Vector2f movimiento_deseado = direccion_que_quiere_el_jugador * _velocidad_de_movimiento;

    if (direccion_que_quiere_el_jugador.x != 0.f && direccion_que_quiere_el_jugador.y != 0.f) {
        movimiento_deseado *= 0.7071f;
    }

    if (direccion_que_quiere_el_jugador.x != 0.f || direccion_que_quiere_el_jugador.y != 0.f) {
        _velocidad_actual_del_movimiento.x += (movimiento_deseado.x - _velocidad_actual_del_movimiento.x) * _aceleracion_al_arrancar_a_moverse * tiempo_transcurrido;
        _velocidad_actual_del_movimiento.y += (movimiento_deseado.y - _velocidad_actual_del_movimiento.y) * _aceleracion_al_arrancar_a_moverse * tiempo_transcurrido;
    }
    else {
        _velocidad_actual_del_movimiento.x += (0.f - _velocidad_actual_del_movimiento.x) * _desaceleracion_al_frenar * tiempo_transcurrido;
        _velocidad_actual_del_movimiento.y += (0.f - _velocidad_actual_del_movimiento.y) * _desaceleracion_al_frenar * tiempo_transcurrido;

        if (std::hypot(_velocidad_actual_del_movimiento.x, _velocidad_actual_del_movimiento.y) < 10.f) {
            _velocidad_actual_del_movimiento = { 0.f, 0.f };
        }
    }

    decidir_animacion_y_direccion_segun_el_movimiento(_velocidad_actual_del_movimiento);
    procesar_el_lanzamiento_de_hechizos(entrada_del_jugador, ventana_del_juego, la_interfaz_le_esta_tapando_el_mouse);

    sf::Vector2f movimiento_de_este_frame = _velocidad_actual_del_movimiento * tiempo_transcurrido;
    mover_con_colisiones(movimiento_de_este_frame, calcular_caja_de_colision(), mapa_del_juego);
}

///=============================================================///
///   DECIDIR ANIMACION Y DIRECCION
///=============================================================///
void Personaje::decidir_animacion_y_direccion_segun_el_movimiento(sf::Vector2f direccion_en_la_que_se_mueve) {
    if (direccion_en_la_que_se_mueve.x == 0.f && direccion_en_la_que_se_mueve.y == 0.f) {
        if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO) {
            _numero_de_frame_actual = 0;
            _tiempo_acumulado_del_frame_actual = 0.f;
        }
        else if (_estado_de_animacion_actual != EstadoDeAnimacionDelPersonaje::QUIETO) {
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO;
            _numero_de_frame_actual = 9;
            _tiempo_acumulado_del_frame_actual = 0.f;
        }
        return;
    }

    if (_estado_de_animacion_actual != EstadoDeAnimacionDelPersonaje::APUNTANDO) {
        _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::CAMINANDO;
    }

    if (std::abs(direccion_en_la_que_se_mueve.x) > std::abs(direccion_en_la_que_se_mueve.y)) {
        if (direccion_en_la_que_se_mueve.x > 0.f) {
            _direccion_hacia_donde_mira = DireccionHaciaDondeMira::DERECHA;
        }
        else {
            _direccion_hacia_donde_mira = DireccionHaciaDondeMira::IZQUIERDA;
        }
    }
    else {
        if (direccion_en_la_que_se_mueve.y > 0.f) {
            _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ABAJO;
        }
        else {
            _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ARRIBA;
        }
    }
}

///=============================================================///
///   PROCESAR LANZAMIENTO DE HECHIZOS
///=============================================================///
void Personaje::procesar_el_lanzamiento_de_hechizos(const InputManager& entrada_del_jugador, sf::RenderWindow& ventana_del_juego, bool la_interfaz_le_esta_tapando_el_mouse) {
    if (la_interfaz_le_esta_tapando_el_mouse == true) {
        return;
    }

    if (entrada_del_jugador.getEl_jugador_quiere_saltar()) {
        if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO) {
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO;
            _numero_de_frame_actual = 0;
        }
        else {
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::APUNTANDO;
            _numero_de_frame_actual = 0;
        }
    }

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO && entrada_del_jugador.getEl_jugador_quiere_atacar()) {

        int costo_de_mana_de_este_hechizo = 20;

        if (_mana_actual_del_jugador < costo_de_mana_de_este_hechizo) {
            std::cout << "NO TENES MANA SUFICIENTE PARA LANZAR EL HECHIZO. MANA ACTUAL: " << _mana_actual_del_jugador << std::endl;
            return;
        }

        _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::LANZANDO_HECHIZO;
        _numero_de_frame_actual = 0;
        _tiempo_acumulado_del_frame_actual = 0.f;

        sf::Vector2i posicion_del_mouse_en_pantalla = entrada_del_jugador.getPosicion_del_mouse();
        sf::Vector2f posicion_del_mouse_en_el_mapa = ventana_del_juego.mapPixelToCoords(posicion_del_mouse_en_pantalla);
        sf::Vector2f posicion_actual_del_personaje = getPosicion();

        if (std::abs(posicion_del_mouse_en_el_mapa.x - posicion_actual_del_personaje.x) > std::abs(posicion_del_mouse_en_el_mapa.y - posicion_actual_del_personaje.y)) {
            if (posicion_del_mouse_en_el_mapa.x > posicion_actual_del_personaje.x) {
                _direccion_hacia_donde_mira = DireccionHaciaDondeMira::DERECHA;
            }
            else {
                _direccion_hacia_donde_mira = DireccionHaciaDondeMira::IZQUIERDA;
            }
        }
        else {
            if (posicion_del_mouse_en_el_mapa.y > posicion_actual_del_personaje.y) {
                _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ABAJO;
            }
            else {
                _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ARRIBA;
            }
        }

        _mana_actual_del_jugador = _mana_actual_del_jugador - costo_de_mana_de_este_hechizo;
        _hechizo_de_bola_de_fuego.activar(posicion_actual_del_personaje, posicion_del_mouse_en_el_mapa, _radio_de_alcance_del_hechizo);
    }
}

///=============================================================///
///   ACTUALIZAR ANIMACION Y HECHIZO
///=============================================================///
void Personaje::actualizar_animacion_y_hechizo(float tiempo_transcurrido, VisualFX& efectos_visuales) {

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::ESQUIVANDO) {
        _segundos_que_quedan_de_esquive -= tiempo_transcurrido;
        _tiempo_acumulado_para_el_rastro_visual += tiempo_transcurrido;

        if (_tiempo_acumulado_para_el_rastro_visual >= 0.02f) {
            efectos_visuales.agregarRastro(_sprite_de_la_entidad, sf::Color(0, 255, 255), 500.f, true);
            _tiempo_acumulado_para_el_rastro_visual = 0.f;
        }
        if (_segundos_que_quedan_de_esquive <= 0.f) {
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO;
            _velocidad_actual_del_movimiento = { 0.f, 0.f };
        }
    }

    if (getEsta_muerta() == true) {
        _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::MUERTO;
        if (_numero_de_frame_actual >= 5) {
            _numero_de_frame_actual = 5;
            actualizar_el_recorte_del_sprite_segun_la_animacion();
            _hechizo_de_bola_de_fuego.actualizar(tiempo_transcurrido, efectos_visuales);
            return;
        }
    }

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO) {
        _circulo_que_muestra_el_alcance.setPosition(getPosicion());
    }

    float duracion_de_este_frame = _segundos_que_dura_cada_frame;
    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::QUIETO) {
        duracion_de_este_frame = 0.5f;
    }

    _tiempo_acumulado_del_frame_actual += tiempo_transcurrido;
    if (_tiempo_acumulado_del_frame_actual >= duracion_de_este_frame) {
        _tiempo_acumulado_del_frame_actual = 0.f;
        if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::MUERTO) {
            if (_numero_de_frame_actual < 5) _numero_de_frame_actual++;
        }
        else {
            _numero_de_frame_actual++;
            avanzar_de_frame_y_decidir_si_cambia_de_animacion();
        }
    }

    actualizar_el_recorte_del_sprite_segun_la_animacion();
    _hechizo_de_bola_de_fuego.actualizar(tiempo_transcurrido, efectos_visuales);
}

///=============================================================///
///   AVANZAR DE FRAME
///=============================================================///
void Personaje::avanzar_de_frame_y_decidir_si_cambia_de_animacion() {
    switch (_estado_de_animacion_actual) {
    case EstadoDeAnimacionDelPersonaje::QUIETO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 11;
        if (_numero_de_frame_actual < 9 || _numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) {
            _numero_de_frame_actual = 9;
        }
        break;
    case EstadoDeAnimacionDelPersonaje::APUNTANDO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 9;
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = 0;
        break;
    case EstadoDeAnimacionDelPersonaje::CAMINANDO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 9;
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = 0;
        break;
    case EstadoDeAnimacionDelPersonaje::ESQUIVANDO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 9;
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = 0;
        break;
    case EstadoDeAnimacionDelPersonaje::LANZANDO_HECHIZO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 7;
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) {
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO;
            _numero_de_frame_actual = 0;
        }
        break;
    case EstadoDeAnimacionDelPersonaje::LASTIMADO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 6;
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = _cantidad_maxima_de_frames_de_la_animacion_actual - 1;
        break;
    case EstadoDeAnimacionDelPersonaje::MUERTO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 6;
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = 5;
        break;
    }
}

///=============================================================///
///   ACTUALIZAR RECORTE DEL SPRITE
///=============================================================///
void Personaje::actualizar_el_recorte_del_sprite_segun_la_animacion() {
    int fila_del_spritesheet = 0;
    EstadoDeAnimacionDelPersonaje animacion_a_usar_para_calcular_la_fila = _estado_de_animacion_actual;

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::QUIETO ||
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO ||
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::ESQUIVANDO) {
        animacion_a_usar_para_calcular_la_fila = EstadoDeAnimacionDelPersonaje::CAMINANDO;
    }

    if (animacion_a_usar_para_calcular_la_fila == EstadoDeAnimacionDelPersonaje::LASTIMADO ||
        animacion_a_usar_para_calcular_la_fila == EstadoDeAnimacionDelPersonaje::MUERTO) {
        fila_del_spritesheet = 20;
    }
    else {
        fila_del_spritesheet = (int)animacion_a_usar_para_calcular_la_fila * 4 + (int)_direccion_hacia_donde_mira;
    }

    int columna_del_spritesheet = _numero_de_frame_actual;

    _sprite_de_la_entidad.setTextureRect(sf::IntRect(columna_del_spritesheet * 64, fila_del_spritesheet * 64, 64, 64));
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void Personaje::dibujar(sf::RenderWindow& ventana_del_juego) {
    _hechizo_de_bola_de_fuego.dibujar(ventana_del_juego);

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO) {
        ventana_del_juego.draw(_circulo_que_muestra_el_alcance);
    }

    dibujar_sprite_y_barra_de_vida(ventana_del_juego);
}