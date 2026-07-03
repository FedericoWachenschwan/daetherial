#include "BolaDeFuego.h"
#include "VisualFX.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

///=============================================================///
///   #1 - CONSTRUCTOR
///=============================================================///
// #1
BolaDeFuego::BolaDeFuego() {
    _la_bola_esta_activa = false; // Empieza apagada
    _velocidad_de_vuelo = 400.f; // Pixeles por segundo
    _distancia_recorrida = 0.f; // No ha recorrido nada aun
    _rango_maximo_de_vuelo = 200.f; // Rango por defecto en pixeles
    _costo_de_mana = 10; // Mana que consume lanzarla
    _el_cooldown_ya_paso = true; // Al inicio puede lanzarse
    _segundos_de_cooldown_restantes = 0.f; // Sin espera al comenzar
    _duracion_total_del_cooldown = 0.5f; // Medio segundo entre lanzamientos
    _tiempo_desde_el_ultimo_rastro = 0.f; // No hay rastro todavia

    if (_textura_de_la_bola.loadFromFile("assets/habilidades/Fire_Spell_Frame_01.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DE LA BOLA DE FUEGO." << std::endl;
    }
    else {
        _sprite_de_la_bola.setTexture(_textura_de_la_bola); // Asigna la imagen al sprite

        float centro_x = _textura_de_la_bola.getSize().x / 2.f; // Calcula el centro horizontal
        float centro_y = _textura_de_la_bola.getSize().y / 2.f; // Calcula el centro vertical
        _sprite_de_la_bola.setOrigin(centro_x, centro_y); // El sprite rota desde su centro

        float tamano_deseado_en_pixeles = 48.f; // Tamanio final que queremos
        float factor_de_escala = tamano_deseado_en_pixeles / _textura_de_la_bola.getSize().x; // Relacion entre tamanio real y deseado
        _sprite_de_la_bola.setScale(factor_de_escala, factor_de_escala); // Aplica la escala al sprite
    }
}

///=============================================================///
///   #2 - ACTIVAR - Apunta la bola hacia el objetivo y la lanza
///=============================================================///
// #2
void BolaDeFuego::activar(sf::Vector2f posicion_de_inicio, sf::Vector2f posicion_del_objetivo, float rango_en_pixeles) {
    if (_el_cooldown_ya_paso == false || _la_bola_esta_activa == true) {
        return; // No se puede lanzar si ya hay una activa o el cooldown no paso
    }

    _sprite_de_la_bola.setPosition(posicion_de_inicio); // La bola nace en la mano del jugador
    _distancia_recorrida = 0.f; // Reinicia el contador de distancia
    _rango_maximo_de_vuelo = rango_en_pixeles; // Guarda hasta donde puede ir

    float diferencia_x = posicion_del_objetivo.x - posicion_de_inicio.x; // Distancia horizontal al objetivo
    float diferencia_y = posicion_del_objetivo.y - posicion_de_inicio.y; // Distancia vertical al objetivo
    float distancia_total = std::hypot(diferencia_x, diferencia_y); // Distancia real en linea recta

    if (distancia_total > 0.001f) {
        _direccion_de_vuelo.x = diferencia_x / distancia_total; // Normaliza la componente X
        _direccion_de_vuelo.y = diferencia_y / distancia_total; // Normaliza la componente Y
    }
    else {
        _direccion_de_vuelo.x = 1.f; // Si el objetivo esta encima, vuela a la derecha
        _direccion_de_vuelo.y = 0.f;
    }

    float angulo_en_radianes = std::atan2(diferencia_y, diferencia_x); // Angulo hacia el objetivo
    float angulo_en_grados = angulo_en_radianes * (180.f / 3.14159f); // Convierte a grados para SFML
    _sprite_de_la_bola.setRotation(angulo_en_grados); // Rota el sprite hacia el objetivo

    _la_bola_esta_activa = true; // La bola empieza a volar
    _el_cooldown_ya_paso = false; // Activa el tiempo de espera
    _segundos_de_cooldown_restantes = _duracion_total_del_cooldown; // Carga el contador del cooldown
}

///=============================================================///
///   #3 - ACTUALIZAR - Mueve la bola y genera su rastro de chispas
///=============================================================///
// #3
void BolaDeFuego::actualizar(float tiempo_transcurrido, VisualFX& efectos_visuales) {

    if (_el_cooldown_ya_paso == false) {
        _segundos_de_cooldown_restantes -= tiempo_transcurrido; // Descuenta el tiempo del cooldown
        if (_segundos_de_cooldown_restantes <= 0.f) {
            _segundos_de_cooldown_restantes = 0.f; // No baja de cero
            _el_cooldown_ya_paso = true; // El cooldown termino, se puede lanzar de nuevo
        }
    }

    if (_la_bola_esta_activa == false) {
        return; // Si no esta activa no hace nada
    }

    _tiempo_desde_el_ultimo_rastro += tiempo_transcurrido; // Acumula tiempo para la siguiente chispa
    if (_tiempo_desde_el_ultimo_rastro >= 0.015f) { // Cada 15 milisegundos genera efectos
        efectos_visuales.agregarRastro(_sprite_de_la_bola, sf::Color(255, 120, 0), 600.f, true); // Copia del sprite que se desvanece

        float dispersion_y = (float)(rand() % 200) - 100.f; // Numero aleatorio entre -100 y 100
        sf::Vector2f velocidad_de_la_chispa = _direccion_de_vuelo * (_velocidad_de_vuelo * -0.4f) + sf::Vector2f(0.f, dispersion_y); // La chispa va hacia atras con dispersion
        efectos_visuales.agregarParticulaDinamica(_textura_de_la_bola, _sprite_de_la_bola.getPosition(), velocidad_de_la_chispa, sf::Color(255, 200, 0), 800.f, true); // Chispa amarilla

        _tiempo_desde_el_ultimo_rastro = 0.f; // Reinicia el temporizador del rastro
    }

    float distancia_de_este_frame = _velocidad_de_vuelo * tiempo_transcurrido; // Cuanto avanza este frame
    _sprite_de_la_bola.move(_direccion_de_vuelo * distancia_de_este_frame); // Mueve la bola en su direccion
    _distancia_recorrida += distancia_de_este_frame; // Acumula la distancia total

    if (_distancia_recorrida >= _rango_maximo_de_vuelo) {
        desactivar(); // Llego al limite, se apaga
    }
}

///=============================================================///
///   #4 - DIBUJAR
///=============================================================///
// #4
void BolaDeFuego::dibujar(sf::RenderWindow& ventana_del_juego) {
    if (_la_bola_esta_activa == true) {
        ventana_del_juego.draw(_sprite_de_la_bola, sf::BlendAdd); // BlendAdd hace que brille sobre lo que hay debajo
    }
}
