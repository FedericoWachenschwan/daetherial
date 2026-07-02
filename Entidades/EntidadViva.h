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
    sf::Texture _textura_de_la_entidad; // Imagen cargada desde el disco
    sf::Sprite _sprite_de_la_entidad; // El dibujo visible en pantalla
    float _velocidad_de_movimiento; // Pixeles por segundo que se mueve

 ///=============================================================///
 ///   VIDA Y DAÑO
 ///=============================================================///
    int _vida_maxima_de_la_entidad; // Tope de vida que puede tener
    int _vida_actual_de_la_entidad; // Vida que tiene ahora mismo
    int _dano_que_hace_esta_entidad; // Cuanto daño hace al golpear

 ///=============================================================///
 ///   COOLDOWN DE ATAQUE
 ///=============================================================///
    sf::Clock _reloj_para_medir_el_cooldown_de_ataque; // Cronometro del ataque
    float _segundos_de_cooldown_entre_ataques; // Espera minima entre ataques

 ///=============================================================///
 ///   ANIMACION
 ///=============================================================///
    int _numero_de_frame_actual; // Columna del spritesheet actual
    float _tiempo_acumulado_del_frame_actual; // Segundos en el frame actual
    float _segundos_que_dura_cada_frame; // Cuanto dura cada imagen
    int _cantidad_maxima_de_frames_de_la_animacion_actual; // Frames totales de la animacion

 ///=============================================================///
 ///   BARRA DE VIDA
 ///=============================================================///
    sf::RectangleShape _rectangulo_de_fondo_de_la_barra_de_vida; // Fondo rojo oscuro
    sf::RectangleShape _rectangulo_relleno_de_la_barra_de_vida; // Relleno rojo vivo

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
    int getVida_actual() const { return _vida_actual_de_la_entidad; } // Devuelve la vida de ahora
    int getVida_maxima() const { return _vida_maxima_de_la_entidad; } // Devuelve la vida tope
    int getDano_que_hace() const { return _dano_que_hace_esta_entidad; } // Devuelve el daño que inflige
    bool getEsta_viva() const { return _vida_actual_de_la_entidad > 0; } // true si tiene vida positiva
    bool getEsta_muerta() const { return _vida_actual_de_la_entidad <= 0; } // true si llego a cero o menos
    sf::Vector2f getPosicion() const { return _sprite_de_la_entidad.getPosition(); } // Posicion en el mapa

 ///=============================================================///
 ///   SETTERS
 ///=============================================================///
    void setPosicion(sf::Vector2f posicion_nueva) { _sprite_de_la_entidad.setPosition(posicion_nueva); } // Teletransporta la entidad

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
        sf::Vector2f origen_actual = _sprite_de_la_entidad.getOrigin(); // Guarda el origen actual
        _sprite_de_la_entidad.setOrigin(origen_actual.x + dx, origen_actual.y + dy); // Suma el desplazamiento
        std::cout << "ORIGEN DEL SPRITE -> X: " << origen_actual.x + dx << " | Y: " << origen_actual.y + dy << std::endl; // Imprime el resultado
    }
};
