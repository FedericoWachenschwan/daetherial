#pragma once
#include "EntidadViva.h"
#include <string>
#include "map.h"

///=================================================================///
///   ANIMACION DEL ENEMIGO - Mucho mas simple que la del personaje:
///   solo tiene "quieto" y "caminando"
///=================================================================///
enum class EstadoDeAnimacionDelEnemigo {
    QUIETO,
    CAMINANDO
};

///=================================================================///
///   DIRECCION HACIA DONDE MIRA EL ENEMIGO EN EL SPRITESHEET
///=================================================================///
enum class DireccionHaciaDondeMiraElEnemigo {
    ARRIBA = 0,
    IZQUIERDA = 1,
    ABAJO = 2,
    DERECHA = 3
};

///=================================================================///
///   ENEMIGO - HERENCIA: Enemigo ES una EntidadViva, hereda su
///   vida, su daño y su forma de moverse con colisiones
///=================================================================///
class Enemigo : public EntidadViva {
private:

    ///=============================================================///
    ///   ANIMACION Y DESTINO
    ///=============================================================///
    EstadoDeAnimacionDelEnemigo _estado_de_animacion_actual = EstadoDeAnimacionDelEnemigo::QUIETO;
    DireccionHaciaDondeMiraElEnemigo _direccion_hacia_donde_mira = DireccionHaciaDondeMiraElEnemigo::ABAJO;
    sf::Vector2f _posicion_a_donde_quiere_llegar;

    ///=============================================================///
    ///   METODOS INTERNOS
    ///=============================================================///
    void decidir_animacion_y_direccion_segun_el_movimiento(sf::Vector2f direccion_en_la_que_se_mueve);
    void avanzar_de_frame_si_corresponde();
    void actualizar_el_recorte_del_sprite_segun_la_animacion();

public:

    ///=============================================================///
    ///   CONSTRUCTORES
    ///=============================================================///

    // Constructor por defecto: arranca "sin vida", listo para ocupar
    // un lugar libre del pool mas adelante
    Enemigo();

    // Recibe la ruta de la imagen para poder crear distintos tipos
    // de monstruos con la misma clase
    Enemigo(sf::Vector2f posicion_inicial, const std::string& ruta_de_la_imagen);

    ///=============================================================///
    ///   SETTERS
    ///=============================================================///

    // Lo actualiza el GameManager cada frame con la posicion del jugador
    void setPosicion_objetivo(sf::Vector2f posicion_del_jugador) { _posicion_a_donde_quiere_llegar = posicion_del_jugador; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///

    // "Enciende" este casillero del pool: carga la textura, lo
    // posiciona y le devuelve la vida
    void activar_en_la_posicion(sf::Vector2f posicion_inicial, const std::string& ruta_de_la_imagen);

    // POLIMORFISMO: reemplazamos la version general de EntidadViva
    // para que ademas imprima un mensaje en consola
    void recibir_dano(int cantidad_de_dano_recibido) override;

    // Calcula la caja de colision cada vez que se llama (no es un
    // atributo guardado), por eso no lleva "get" adelante
    sf::FloatRect calcular_caja_de_colision() const;

    void actualizar(float tiempo_transcurrido, Map& mapa_del_juego);
    void dibujar(sf::RenderWindow& ventana_del_juego);
};