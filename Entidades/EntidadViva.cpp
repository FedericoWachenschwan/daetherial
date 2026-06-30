#include "EntidadViva.h"
#include "map.h"

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
EntidadViva::EntidadViva() {
    _velocidad_de_movimiento = 100.f;
    _vida_maxima_de_la_entidad = 100;
    _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad;
    _dano_que_hace_esta_entidad = 10;
    _segundos_de_cooldown_entre_ataques = 1.f;

    _numero_de_frame_actual = 0;
    _tiempo_acumulado_del_frame_actual = 0.f;
    _segundos_que_dura_cada_frame = 0.15f;
    _cantidad_maxima_de_frames_de_la_animacion_actual = 1;

    _rectangulo_de_fondo_de_la_barra_de_vida.setSize(sf::Vector2f(32.f, 4.f));
    _rectangulo_de_fondo_de_la_barra_de_vida.setFillColor(sf::Color(60, 0, 0));

    _rectangulo_relleno_de_la_barra_de_vida.setSize(sf::Vector2f(32.f, 4.f));
    _rectangulo_relleno_de_la_barra_de_vida.setFillColor(sf::Color(220, 30, 30));
}

///=============================================================///
///   CURAR
///=============================================================///
void EntidadViva::curar(int puntos_de_curacion) {
    int vida_despues_de_curar = _vida_actual_de_la_entidad + puntos_de_curacion;

    if (vida_despues_de_curar > _vida_maxima_de_la_entidad) {
        _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad;
    }
    else {
        _vida_actual_de_la_entidad = vida_despues_de_curar;
    }
}

///=============================================================///
///   RECIBIR DAÑO - POLIMORFISMO: version general
///=============================================================///
void EntidadViva::recibir_dano(int cantidad_de_dano_recibido) {
    _vida_actual_de_la_entidad = _vida_actual_de_la_entidad - cantidad_de_dano_recibido;
}

///=============================================================///
///   PUEDE ATACAR
///=============================================================///
bool EntidadViva::puede_atacar_de_nuevo() {
    float segundos_desde_el_ultimo_ataque = _reloj_para_medir_el_cooldown_de_ataque.getElapsedTime().asSeconds();

    if (segundos_desde_el_ultimo_ataque >= _segundos_de_cooldown_entre_ataques) {
        _reloj_para_medir_el_cooldown_de_ataque.restart();
        return true;
    }
    return false;
}

///=============================================================///
///   CALCULAR CENTRO FISICO
///=============================================================///
sf::Vector2f EntidadViva::calcular_centro_fisico() const {
    sf::FloatRect limites_del_sprite = _sprite_de_la_entidad.getGlobalBounds();
    float centro_x = limites_del_sprite.left + limites_del_sprite.width / 2.f;
    float centro_y = limites_del_sprite.top + limites_del_sprite.height / 2.f;
    return sf::Vector2f(centro_x, centro_y);
}

///=============================================================///
///   MOVER CON COLISIONES
///=============================================================///
void EntidadViva::mover_con_colisiones(sf::Vector2f movimiento_deseado, sf::FloatRect caja_de_colision_actual, Map& mapa_del_juego) {

    sf::FloatRect caja_moviendose_en_x = caja_de_colision_actual;
    caja_moviendose_en_x.left += movimiento_deseado.x;
    if (mapa_del_juego.getHay_colision(caja_moviendose_en_x) == false) {
        _sprite_de_la_entidad.move(movimiento_deseado.x, 0.f);
    }

    sf::FloatRect caja_moviendose_en_y = caja_de_colision_actual;
    caja_moviendose_en_y.top += movimiento_deseado.y;
    if (mapa_del_juego.getHay_colision(caja_moviendose_en_y) == false) {
        _sprite_de_la_entidad.move(0.f, movimiento_deseado.y);
    }
}

///=============================================================///
///   DIBUJAR SPRITE Y BARRA DE VIDA
///=============================================================///
void EntidadViva::dibujar_sprite_y_barra_de_vida(sf::RenderWindow& ventana_del_juego) {

    ventana_del_juego.draw(_sprite_de_la_entidad);

    sf::Vector2f posicion_de_la_entidad = _sprite_de_la_entidad.getPosition();
    float posicion_x_de_la_barra = posicion_de_la_entidad.x - 16.f;
    float posicion_y_de_la_barra = posicion_de_la_entidad.y - 30.f;

    _rectangulo_de_fondo_de_la_barra_de_vida.setPosition(posicion_x_de_la_barra, posicion_y_de_la_barra);
    ventana_del_juego.draw(_rectangulo_de_fondo_de_la_barra_de_vida);

    float porcentaje_de_vida_que_le_queda = (float)_vida_actual_de_la_entidad / (float)_vida_maxima_de_la_entidad;
    if (porcentaje_de_vida_que_le_queda < 0.f) {
        porcentaje_de_vida_que_le_queda = 0.f;
    }

    _rectangulo_relleno_de_la_barra_de_vida.setSize(sf::Vector2f(32.f * porcentaje_de_vida_que_le_queda, 4.f));
    _rectangulo_relleno_de_la_barra_de_vida.setPosition(posicion_x_de_la_barra, posicion_y_de_la_barra);
    ventana_del_juego.draw(_rectangulo_relleno_de_la_barra_de_vida);
}