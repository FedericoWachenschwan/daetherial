#include "BolaDeFuego.h"
#include "VisualFX.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
BolaDeFuego::BolaDeFuego() {
    _la_bola_esta_activa = false;
    _velocidad_de_vuelo = 400.f;
    _distancia_recorrida = 0.f;
    _rango_maximo_de_vuelo = 200.f;
    _dano_de_la_bola = 50;
    _el_cooldown_ya_paso = true;
    _segundos_de_cooldown_restantes = 0.f;
    _duracion_total_del_cooldown = 0.5f;
    _tiempo_desde_el_ultimo_rastro = 0.f;

    if (_textura_de_la_bola.loadFromFile("assets/habilidades/Fire_Spell_Frame_01.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DE LA BOLA DE FUEGO." << std::endl;
    }
    else {
        _sprite_de_la_bola.setTexture(_textura_de_la_bola);

        float centro_x = _textura_de_la_bola.getSize().x / 2.f;
        float centro_y = _textura_de_la_bola.getSize().y / 2.f;
        _sprite_de_la_bola.setOrigin(centro_x, centro_y);

        float tamano_deseado_en_pixeles = 48.f;
        float factor_de_escala = tamano_deseado_en_pixeles / _textura_de_la_bola.getSize().x;
        _sprite_de_la_bola.setScale(factor_de_escala, factor_de_escala);
    }
}

///=============================================================///
///   ACTIVAR - Apunta la bola hacia el objetivo y la lanza
///=============================================================///
void BolaDeFuego::activar(sf::Vector2f posicion_de_inicio, sf::Vector2f posicion_del_objetivo, float rango_en_pixeles) {
    if (_el_cooldown_ya_paso == false || _la_bola_esta_activa == true) {
        return;
    }

    _sprite_de_la_bola.setPosition(posicion_de_inicio);
    _distancia_recorrida = 0.f;
    _rango_maximo_de_vuelo = rango_en_pixeles;

    float diferencia_x = posicion_del_objetivo.x - posicion_de_inicio.x;
    float diferencia_y = posicion_del_objetivo.y - posicion_de_inicio.y;
    float distancia_total = std::hypot(diferencia_x, diferencia_y);

    if (distancia_total > 0.001f) {
        _direccion_de_vuelo.x = diferencia_x / distancia_total;
        _direccion_de_vuelo.y = diferencia_y / distancia_total;
    }
    else {
        _direccion_de_vuelo.x = 1.f;
        _direccion_de_vuelo.y = 0.f;
    }

    float angulo_en_radianes = std::atan2(diferencia_y, diferencia_x);
    float angulo_en_grados = angulo_en_radianes * (180.f / 3.14159f);
    _sprite_de_la_bola.setRotation(angulo_en_grados);

    _la_bola_esta_activa = true;
    _el_cooldown_ya_paso = false;
    _segundos_de_cooldown_restantes = _duracion_total_del_cooldown;
}

///=============================================================///
///   ACTUALIZAR - Mueve la bola y genera su rastro de chispas
///=============================================================///
void BolaDeFuego::actualizar(float tiempo_transcurrido, VisualFX& efectos_visuales) {

    if (_el_cooldown_ya_paso == false) {
        _segundos_de_cooldown_restantes -= tiempo_transcurrido;
        if (_segundos_de_cooldown_restantes <= 0.f) {
            _segundos_de_cooldown_restantes = 0.f;
            _el_cooldown_ya_paso = true;
        }
    }

    if (_la_bola_esta_activa == false) {
        return;
    }

    _tiempo_desde_el_ultimo_rastro += tiempo_transcurrido;
    if (_tiempo_desde_el_ultimo_rastro >= 0.015f) {
        efectos_visuales.agregarRastro(_sprite_de_la_bola, sf::Color(255, 120, 0), 600.f, true);

        float dispersion_y = (float)(rand() % 200) - 100.f;
        sf::Vector2f velocidad_de_la_chispa = _direccion_de_vuelo * (_velocidad_de_vuelo * -0.4f) + sf::Vector2f(0.f, dispersion_y);
        efectos_visuales.agregarParticulaDinamica(_textura_de_la_bola, _sprite_de_la_bola.getPosition(), velocidad_de_la_chispa, sf::Color(255, 200, 0), 800.f, true);

        _tiempo_desde_el_ultimo_rastro = 0.f;
    }

    float distancia_de_este_frame = _velocidad_de_vuelo * tiempo_transcurrido;
    _sprite_de_la_bola.move(_direccion_de_vuelo * distancia_de_este_frame);
    _distancia_recorrida += distancia_de_este_frame;

    if (_distancia_recorrida >= _rango_maximo_de_vuelo) {
        desactivar();
    }
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void BolaDeFuego::dibujar(sf::RenderWindow& ventana_del_juego) {
    if (_la_bola_esta_activa == true) {
        ventana_del_juego.draw(_sprite_de_la_bola, sf::BlendAdd);
    }
}