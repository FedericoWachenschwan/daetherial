#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.h"
// SFML: la libreria que usamos para graficos, ventanas, texturas y formas en este juego


///=================================================================///
///   ENTIDAD_VIVA - El "molde" base que comparten todos los seres
///   vivos del juego (el personaje, los enemigos y el jefe).
///   Guarda los datos que TODOS tienen en comun: vida, daño, sprite
///=================================================================///
class EntidadViva {
protected:

    ///=============================================================///
    ///   DIBUJO Y MOVIMIENTO
    ///=============================================================///
    sf::Texture _textura_de_la_entidad;
    sf::Sprite _sprite_de_la_entidad;
    float _velocidad_de_movimiento;

    ///=============================================================///
    ///   VIDA Y DAÑO
    ///=============================================================///
    int _vida_maxima_de_la_entidad;
    int _vida_actual_de_la_entidad;
    int _dano_que_hace_esta_entidad;

    ///=============================================================///
    ///   COOLDOWN DE ATAQUE
    ///=============================================================///
    sf::Clock _reloj_para_medir_el_cooldown_de_ataque;
    float _segundos_de_cooldown_entre_ataques;

    ///=============================================================///
    ///   ANIMACION
    ///=============================================================///
    int _numero_de_frame_actual;
    float _tiempo_acumulado_del_frame_actual;
    float _segundos_que_dura_cada_frame;
    int _cantidad_maxima_de_frames_de_la_animacion_actual;

    ///=============================================================///
    ///   BARRA DE VIDA
    ///=============================================================///
    sf::RectangleShape _rectangulo_de_fondo_de_la_barra_de_vida;
    sf::RectangleShape _rectangulo_relleno_de_la_barra_de_vida;

    ///=============================================================///
    ///   OTROS METODOS (PROTEGIDOS) - Solo lo usan las clases hijas
    ///=============================================================///
    void mover_con_colisiones(sf::Vector2f movimiento_deseado, sf::FloatRect caja_de_colision_actual, Map& mapa_del_juego);

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    EntidadViva();

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    int getVida_actual() const { return _vida_actual_de_la_entidad; }
    int getVida_maxima() const { return _vida_maxima_de_la_entidad; }
    int getDano_que_hace() const { return _dano_que_hace_esta_entidad; }
    bool getEsta_viva() const { return _vida_actual_de_la_entidad > 0; }
    bool getEsta_muerta() const { return _vida_actual_de_la_entidad <= 0; }
    sf::Vector2f getPosicion() const { return _sprite_de_la_entidad.getPosition(); }

    ///=============================================================///
    ///   SETTERS
    ///=============================================================///
    void setPosicion(sf::Vector2f posicion_nueva) { _sprite_de_la_entidad.setPosition(posicion_nueva); }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///

    // Suma vida sin pasarse del maximo: hace una cuenta con limites,
    // no es un simple guardar de valor, por eso no lleva "set" adelante
    void curar(int puntos_de_curacion);

    // POLIMORFISMO: este metodo lo puede reemplazar una clase hija.
    // EntidadViva tiene la version general (solo resta vida). Enemigo
    // tiene su propia version que ademas imprime un mensaje en consola
    virtual void recibir_dano(int cantidad_de_dano_recibido);

    // Calcula el centro del sprite cada vez que se llama (no es un
    // atributo guardado), por eso no lleva "get" adelante
    sf::Vector2f calcular_centro_fisico() const;

    bool puede_atacar_de_nuevo();

    void dibujar_sprite_y_barra_de_vida(sf::RenderWindow& ventana_del_juego);

    // Solo para el modo debug: mueve el origen del sprite a mano sin recompilar
    void ajustar_origen_del_sprite(float dx, float dy) {
        sf::Vector2f origen_actual = _sprite_de_la_entidad.getOrigin();
        _sprite_de_la_entidad.setOrigin(origen_actual.x + dx, origen_actual.y + dy);
        std::cout << "ORIGEN DEL SPRITE -> X: " << origen_actual.x + dx << " | Y: " << origen_actual.y + dy << std::endl;
    }
};