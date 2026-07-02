#include "EntidadViva.h"
#include "map.h"

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
EntidadViva::EntidadViva() {
    _velocidad_de_movimiento = 100.f; // Velocidad inicial: 100 px/seg
    _vida_maxima_de_la_entidad = 100; // Vida maxima por defecto
    _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad; // Arranca con vida llena
    _dano_que_hace_esta_entidad = 10; // Daño inicial por defecto
    _segundos_de_cooldown_entre_ataques = 1.f; // 1 segundo entre ataques

    _numero_de_frame_actual = 0; // Empieza en la primera imagen
    _tiempo_acumulado_del_frame_actual = 0.f; // Cronometro de frame en cero
    _segundos_que_dura_cada_frame = 0.15f; // Cada frame dura 0.15 segundos
    _cantidad_maxima_de_frames_de_la_animacion_actual = 1; // Una sola imagen al principio

    _rectangulo_de_fondo_de_la_barra_de_vida.setSize(sf::Vector2f(32.f, 4.f)); // Ancho total de la barra
    _rectangulo_de_fondo_de_la_barra_de_vida.setFillColor(sf::Color(60, 0, 0)); // Color rojo oscuro de fondo

    _rectangulo_relleno_de_la_barra_de_vida.setSize(sf::Vector2f(32.f, 4.f)); // Mismo ancho, se achica al perder vida
    _rectangulo_relleno_de_la_barra_de_vida.setFillColor(sf::Color(220, 30, 30)); // Color rojo vivo para el relleno
}

///=============================================================///
///   CURAR
///=============================================================///
void EntidadViva::curar(int puntos_de_curacion) {
    int vida_despues_de_curar = _vida_actual_de_la_entidad + puntos_de_curacion; // Suma la curacion

    if (vida_despues_de_curar > _vida_maxima_de_la_entidad) { // Si se pasa del maximo
        _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad; // Limita al tope
    }
    else {
        _vida_actual_de_la_entidad = vida_despues_de_curar; // Guarda la vida curada
    }
}

///=============================================================///
///   RECIBIR DAÑO - POLIMORFISMO: version general
///=============================================================///
void EntidadViva::recibir_dano(int cantidad_de_dano_recibido) {
    _vida_actual_de_la_entidad = _vida_actual_de_la_entidad - cantidad_de_dano_recibido; // Resta el daño a la vida
}

///=============================================================///
///   PUEDE ATACAR
///=============================================================///
bool EntidadViva::puede_atacar_de_nuevo() {
    float segundos_desde_el_ultimo_ataque = _reloj_para_medir_el_cooldown_de_ataque.getElapsedTime().asSeconds(); // Tiempo transcurrido

    if (segundos_desde_el_ultimo_ataque >= _segundos_de_cooldown_entre_ataques) { // Si ya paso el cooldown
        _reloj_para_medir_el_cooldown_de_ataque.restart(); // Reinicia el cronometro
        return true; // Puede atacar
    }
    return false; // Todavia no puede atacar
}

///=============================================================///
///   CALCULAR CENTRO FISICO
///=============================================================///
sf::Vector2f EntidadViva::calcular_centro_fisico() const {
    sf::FloatRect limites_del_sprite = _sprite_de_la_entidad.getGlobalBounds(); // Rectangulo que rodea el sprite
    float centro_x = limites_del_sprite.left + limites_del_sprite.width / 2.f; // Mitad horizontal
    float centro_y = limites_del_sprite.top + limites_del_sprite.height / 2.f; // Mitad vertical
    return sf::Vector2f(centro_x, centro_y); // Devuelve el punto central
}

///=============================================================///
///   MOVER CON COLISIONES
///=============================================================///
void EntidadViva::mover_con_colisiones(sf::Vector2f movimiento_deseado, sf::FloatRect caja_de_colision_actual, Map& mapa_del_juego) {

    sf::FloatRect caja_moviendose_en_x = caja_de_colision_actual; // Copia la caja actual
    caja_moviendose_en_x.left += movimiento_deseado.x; // Simula el movimiento horizontal
    if (mapa_del_juego.getHay_colision(caja_moviendose_en_x) == false) { // Si no choca con nada
        _sprite_de_la_entidad.move(movimiento_deseado.x, 0.f); // Mueve solo en X
    }

    sf::FloatRect caja_moviendose_en_y = caja_de_colision_actual; // Copia la caja actual de nuevo
    caja_moviendose_en_y.top += movimiento_deseado.y; // Simula el movimiento vertical
    if (mapa_del_juego.getHay_colision(caja_moviendose_en_y) == false) { // Si no choca con nada
        _sprite_de_la_entidad.move(0.f, movimiento_deseado.y); // Mueve solo en Y
    }
}

///=============================================================///
///   DIBUJAR SPRITE Y BARRA DE VIDA
///=============================================================///
void EntidadViva::dibujar_sprite_y_barra_de_vida(sf::RenderWindow& ventana_del_juego) {

    ventana_del_juego.draw(_sprite_de_la_entidad); // Dibuja el personaje/enemigo

    sf::Vector2f posicion_de_la_entidad = _sprite_de_la_entidad.getPosition(); // Posicion actual del sprite
    float posicion_x_de_la_barra = posicion_de_la_entidad.x - 16.f; // Centra la barra horizontalmente
    float posicion_y_de_la_barra = posicion_de_la_entidad.y - 30.f; // Ubica la barra arriba del sprite

    _rectangulo_de_fondo_de_la_barra_de_vida.setPosition(posicion_x_de_la_barra, posicion_y_de_la_barra); // Posiciona el fondo
    ventana_del_juego.draw(_rectangulo_de_fondo_de_la_barra_de_vida); // Dibuja el fondo oscuro

    float porcentaje_de_vida_que_le_queda = (float)_vida_actual_de_la_entidad / (float)_vida_maxima_de_la_entidad; // Calcula que porcion de vida queda
    if (porcentaje_de_vida_que_le_queda < 0.f) { // Evita porcentaje negativo
        porcentaje_de_vida_que_le_queda = 0.f; // Clampea en cero
    }

    _rectangulo_relleno_de_la_barra_de_vida.setSize(sf::Vector2f(32.f * porcentaje_de_vida_que_le_queda, 4.f)); // Achica el relleno segun la vida
    _rectangulo_relleno_de_la_barra_de_vida.setPosition(posicion_x_de_la_barra, posicion_y_de_la_barra); // Posiciona el relleno igual que el fondo
    ventana_del_juego.draw(_rectangulo_relleno_de_la_barra_de_vida); // Dibuja la vida restante
}
