#include "Enemigo.h"
#include "map.h"
#include <cmath>
#include <iostream>

///=============================================================///
///   #1 - CONSTRUCTOR POR DEFECTO
///=============================================================///
// #1
Enemigo::Enemigo() {
    _vida_maxima_de_la_entidad = 30; // Vida maxima del enemigo base
    _vida_actual_de_la_entidad = 0; // Sin vida: marca que no esta activo en el pool
}

///=============================================================///
///   #2 - CONSTRUCTOR CON DATOS
///=============================================================///
// #2
Enemigo::Enemigo(sf::Vector2f posicion_inicial, const std::string& ruta_de_la_imagen) {
    if (_textura_de_la_entidad.loadFromFile(ruta_de_la_imagen) == false) { // Intenta cargar la imagen
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DEL ENEMIGO: " << ruta_de_la_imagen << std::endl; // Avisa si falla
    }
    _sprite_de_la_entidad.setTexture(_textura_de_la_entidad); // Asocia la imagen al sprite
    _sprite_de_la_entidad.setOrigin(32.f, 32.f); // Centro del sprite como punto de origen
    _sprite_de_la_entidad.setPosition(posicion_inicial); // Ubica el enemigo en el mapa

    _vida_maxima_de_la_entidad = 30; // Vida maxima
    _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad; // Arranca con vida completa
    _dano_que_hace_esta_entidad = 10; // Daño que inflige al jugador
    _velocidad_de_movimiento = 70.f; // Velocidad de persecucion en px/seg
    _segundos_de_cooldown_entre_ataques = 2.f; // 2 segundos entre ataques

    actualizar_el_recorte_del_sprite_segun_la_animacion(); // Aplica el primer frame
}

///=============================================================///
///   #3 - ACTIVAR EN LA POSICION
///=============================================================///
// #3
void Enemigo::activar_en_la_posicion(sf::Vector2f posicion_inicial, const std::string& ruta_de_la_imagen) {
    if (_textura_de_la_entidad.loadFromFile(ruta_de_la_imagen) == false) { // Intenta cargar la imagen
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DEL ENEMIGO: " << ruta_de_la_imagen << std::endl; // Avisa si falla
    }
    _sprite_de_la_entidad.setTexture(_textura_de_la_entidad); // Asocia la imagen al sprite
    _sprite_de_la_entidad.setOrigin(32.f, 32.f); // Centro del sprite como punto de origen
    _sprite_de_la_entidad.setPosition(posicion_inicial); // Posiciona en el mapa

    _vida_maxima_de_la_entidad = 30; // Reinicia la vida maxima
    _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad; // Restaura la vida completa
    _dano_que_hace_esta_entidad = 10; // Daño al jugador
    _velocidad_de_movimiento = 70.f; // Velocidad de movimiento
    _segundos_de_cooldown_entre_ataques = 2.f; // Cooldown de ataque

    actualizar_el_recorte_del_sprite_segun_la_animacion(); // Aplica el primer frame
}

///=============================================================///
///   #4 - RECIBIR DAÑO - POLIMORFISMO
///=============================================================///
// #4
void Enemigo::recibir_dano(int cantidad_de_dano_recibido) {
    _vida_actual_de_la_entidad = _vida_actual_de_la_entidad - cantidad_de_dano_recibido; // Resta el daño a la vida
    std::cout << "EL ENEMIGO GRITO DE DOLOR." << std::endl; // Imprime en consola (debug)
}

///=============================================================///
///   #5 - CALCULAR CAJA DE COLISION
///=============================================================///
// #5
sf::FloatRect Enemigo::calcular_caja_de_colision() const {
    return _sprite_de_la_entidad.getGlobalBounds(); // Usa el rectangulo completo del sprite
}

///=============================================================///
///   #6 - ACTUALIZAR
///=============================================================///
// #6
void Enemigo::actualizar(float tiempo_transcurrido, Map& mapa_del_juego) {
    sf::Vector2f posicion_antes_de_moverse = _sprite_de_la_entidad.getPosition(); // Guarda la posicion antes de mover

    sf::Vector2f direccion_hacia_el_objetivo = _posicion_a_donde_quiere_llegar - posicion_antes_de_moverse; // Vector hacia el jugador
    float distancia_al_objetivo = std::hypot(direccion_hacia_el_objetivo.x, direccion_hacia_el_objetivo.y); // Distancia real al jugador

    if (distancia_al_objetivo > 5.f) { // Si esta a mas de 5px del objetivo
        direccion_hacia_el_objetivo /= distancia_al_objetivo; // Normaliza para obtener solo la direccion
        sf::Vector2f movimiento_de_este_frame = direccion_hacia_el_objetivo * _velocidad_de_movimiento * tiempo_transcurrido; // Calcula el desplazamiento
        mover_con_colisiones(movimiento_de_este_frame, calcular_caja_de_colision(), mapa_del_juego); // Mueve evitando paredes
    }

    sf::Vector2f posicion_despues_de_moverse = _sprite_de_la_entidad.getPosition(); // Posicion tras el movimiento
    sf::Vector2f movimiento_real_de_este_frame = posicion_despues_de_moverse - posicion_antes_de_moverse; // Movimiento real ocurrido

    decidir_animacion_y_direccion_segun_el_movimiento(movimiento_real_de_este_frame); // Elige la animacion correcta

    _tiempo_acumulado_del_frame_actual += tiempo_transcurrido; // Acumula tiempo del frame actual
    if (_tiempo_acumulado_del_frame_actual >= _segundos_que_dura_cada_frame) { // Si el frame ya duro suficiente
        _tiempo_acumulado_del_frame_actual = 0.f; // Reinicia el cronometro
        _numero_de_frame_actual++; // Avanza al siguiente frame
        avanzar_de_frame_si_corresponde(); // Decide si reinicia la animacion
    }

    actualizar_el_recorte_del_sprite_segun_la_animacion(); // Aplica el frame actual al sprite
}

///=============================================================///
///   #7 - DECIDIR ANIMACION Y DIRECCION
///=============================================================///
// #7
void Enemigo::decidir_animacion_y_direccion_segun_el_movimiento(sf::Vector2f direccion_en_la_que_se_mueve) {
    if (std::abs(direccion_en_la_que_se_mueve.x) < 0.1f && std::abs(direccion_en_la_que_se_mueve.y) < 0.1f) { // Si casi no se movio
        _estado_de_animacion_actual = EstadoDeAnimacionDelEnemigo::QUIETO; // Pone animacion de quieto
        return; // Termina sin cambiar la direccion
    }

    _estado_de_animacion_actual = EstadoDeAnimacionDelEnemigo::CAMINANDO; // Activa la animacion de caminar

    if (std::abs(direccion_en_la_que_se_mueve.x) > std::abs(direccion_en_la_que_se_mueve.y)) { // Si se mueve mas en horizontal
        if (direccion_en_la_que_se_mueve.x > 0.f) { // Se mueve a la derecha
            _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::DERECHA;
        }
        else { // Se mueve a la izquierda
            _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::IZQUIERDA;
        }
    }
    else { // Se mueve mas en vertical
        if (direccion_en_la_que_se_mueve.y > 0.f) { // Se mueve hacia abajo
            _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::ABAJO;
        }
        else { // Se mueve hacia arriba
            _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::ARRIBA;
        }
    }
}

///=============================================================///
///   #8 - AVANZAR DE FRAME
///=============================================================///
// #8
void Enemigo::avanzar_de_frame_si_corresponde() {
    if (_estado_de_animacion_actual == EstadoDeAnimacionDelEnemigo::QUIETO) { // Si esta quieto
        _cantidad_maxima_de_frames_de_la_animacion_actual = 1; // Solo 1 frame cuando esta parado
        _numero_de_frame_actual = 0; // Siempre en el frame inicial
    }
    else { // Si esta caminando
        _cantidad_maxima_de_frames_de_la_animacion_actual = 9; // 9 frames de animacion de caminar
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) {
            _numero_de_frame_actual = 0; // Reinicia la animacion en loop
        }
    }
}

///=============================================================///
///   #9 - ACTUALIZAR RECORTE DEL SPRITE
///=============================================================///
// #9
void Enemigo::actualizar_el_recorte_del_sprite_segun_la_animacion() {
    int fila_del_spritesheet = 8 + (int)_direccion_hacia_donde_mira; // Las filas del enemigo empiezan en la 8

    int columna_del_spritesheet = 0; // Por defecto primera columna (quieto)
    if (_estado_de_animacion_actual != EstadoDeAnimacionDelEnemigo::QUIETO) { // Si esta en movimiento
        columna_del_spritesheet = _numero_de_frame_actual; // Usa el frame animado
    }

    _sprite_de_la_entidad.setTextureRect(sf::IntRect(columna_del_spritesheet * 64, fila_del_spritesheet * 64, 64, 64)); // Recorta el frame correcto
}

///=============================================================///
///   #10 - DIBUJAR
///=============================================================///
// #10
void Enemigo::dibujar(sf::RenderWindow& ventana_del_juego) {
    dibujar_sprite_y_barra_de_vida(ventana_del_juego); // Dibuja el sprite y la barra de vida
}
