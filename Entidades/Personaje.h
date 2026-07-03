#pragma once
#include "EntidadViva.h"
#include "Inventario.h" // COMPOSICION: el Personaje va a TENER un Inventario adentro
#include "BolaDeFuego.h" // COMPOSICION: el Personaje va a TENER su propio hechizo
#include "VisualFX.h"
#include "map.h"
#include "InputManager.h"

///=================================================================///
///   ANIMACION DEL PERSONAJE - En que pose esta dibujado ahora
///=================================================================///
enum class EstadoDeAnimacionDelPersonaje {
    LANZANDO_HECHIZO = 0, // Esta lanzando un hechizo
    CAMINANDO = 2, // Esta caminando
    LASTIMADO = 5, // Recibio un golpe
    QUIETO, // Sin moverse
    APUNTANDO, // Apuntando para lanzar hechizo
    MUERTO, // Sin vida
    ESQUIVANDO // Realizando el dash
};

///=================================================================///
///   DIRECCION HACIA DONDE MIRA EL PERSONAJE EN EL SPRITESHEET
///=================================================================///
enum class DireccionHaciaDondeMira {
    ARRIBA = 0, // Mira hacia arriba
    IZQUIERDA = 1, // Mira hacia la izquierda
    ABAJO = 2, // Mira hacia abajo
    DERECHA = 3 // Mira hacia la derecha
};

///=================================================================///
///   PERSONAJE - HERENCIA: Personaje ES una EntidadViva, hereda
///   su vida, su daño y su forma de moverse con colisiones
///=================================================================///
class Personaje : public EntidadViva {
private:

 ///=============================================================///
 ///   ANIMACION ACTUAL
 ///=============================================================///
    EstadoDeAnimacionDelPersonaje _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO; // Animacion que se muestra ahora
    DireccionHaciaDondeMira _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ABAJO; // Hacia donde apunta el sprite

 ///=============================================================///
 ///   COMPOSICION - El Personaje TIENE una mochila y TIENE su
 ///   propio hechizo, no los hereda, los contiene como atributos
 ///=============================================================///
    Inventario _mochila_del_personaje; // El inventario de items del jugador
    BolaDeFuego _bolas_de_fuego[5]; // Pool de proyectiles: hasta 5 bolas activas al mismo tiempo

 ///=============================================================///
 ///   ORO Y MANA
 ///=============================================================///
    int _cantidad_de_oro_del_jugador = 100; // Dinero para comprar en la tienda
    int _mana_actual_del_jugador = 100; // Mana disponible para hechizos
    int _mana_maxima_del_jugador = 100; // Tope de mana que puede tener

 ///=============================================================///
 ///   MOVIMIENTO SUAVE (ACELERACION Y FRENADO GRADUAL)
 ///=============================================================///
    sf::Vector2f _velocidad_actual_del_movimiento = { 0.f, 0.f }; // Velocidad real en este frame
    float _aceleracion_al_arrancar_a_moverse = 10.f; // Que tan rapido gana velocidad
    float _desaceleracion_al_frenar = 8.f; // Que tan rapido pierde velocidad

 ///=============================================================///
 ///   CIRCULO QUE MUESTRA EL ALCANCE DEL HECHIZO AL APUNTAR
 ///=============================================================///
    sf::CircleShape _circulo_que_muestra_el_alcance; // Circulo visual de rango
    float _radio_de_alcance_del_hechizo = 200.f; // Distancia maxima del hechizo

 ///=============================================================///
 ///   ESQUIVE (DASH)
 ///=============================================================///
    float _segundos_que_quedan_de_esquive = 0.f; // Tiempo restante del dash actual
    const float _duracion_total_del_esquive = 0.15f; // Cuanto dura el dash en total
    float _segundos_de_espera_para_volver_a_esquivar = 0.f; // Cooldown entre dashes
    float _tiempo_acumulado_para_el_rastro_visual = 0.f; // Cronometro del efecto de estela

 ///=============================================================///
 ///   METODOS INTERNOS - Solo los usa el Personaje por dentro
 ///=============================================================///
    void decidir_animacion_y_direccion_segun_el_movimiento(sf::Vector2f direccion_en_la_que_se_mueve);
    void procesar_el_lanzamiento_de_hechizos(const InputManager& entrada_del_jugador, sf::RenderWindow& ventana_del_juego, bool la_interfaz_le_esta_tapando_el_mouse);
    void avanzar_de_frame_y_decidir_si_cambia_de_animacion();
    void actualizar_el_recorte_del_sprite_segun_la_animacion();

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    Personaje();

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #2
    int getOro() const { return _cantidad_de_oro_del_jugador; } // Devuelve el oro actual
 // #3
    int getMana_actual() const { return _mana_actual_del_jugador; } // Devuelve el mana actual
 // #4
    int getMana_maxima() const { return _mana_maxima_del_jugador; } // Devuelve el mana tope
 // #5
    Inventario& getMochila() { return _mochila_del_personaje; } // Devuelve la mochila del jugador
 // #6
    BolaDeFuego& getBola(int indice) { return _bolas_de_fuego[indice]; } // Devuelve la bola en ese slot del pool
    static const int CANTIDAD_MAXIMA_DE_BOLAS = 5; // Cuantas bolas pueden volar al mismo tiempo

 ///=============================================================///
 ///   SETTERS
 ///=============================================================///
 // #7
    void setOro(int nueva_cantidad_de_oro) { _cantidad_de_oro_del_jugador = nueva_cantidad_de_oro; } // Cambia el oro del jugador

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///

 // Suma mana con limites (no baja de 0 ni pasa el maximo): hace una
 // cuenta, no es un simple guardar de valor, por eso no lleva "set"
 // #8
    void restaurar_mana(int puntos_de_mana_a_restaurar);

 // Calcula una caja chica a la altura de los pies cada vez que se
 // llama (no es un atributo guardado), por eso no lleva "get"
 // #9
    sf::FloatRect calcular_caja_de_colision() const;
 // #10
    void empujar_por_colision(sf::Vector2f movimiento, Map& mapa_del_juego) { mover_con_colisiones(movimiento, calcular_caja_de_colision(), mapa_del_juego); } // Mueve al personaje empujado por algo
 // #11
    void procesar_movimiento_y_entrada_del_jugador(const InputManager& entrada_del_jugador, Map& mapa_del_juego, sf::RenderWindow& ventana_del_juego, bool la_interfaz_le_esta_tapando_el_mouse, float tiempo_transcurrido);
 // #12
    void actualizar_animacion_y_hechizo(float tiempo_transcurrido, VisualFX& efectos_visuales);
 // #13
    void dibujar(sf::RenderWindow& ventana_del_juego);
 // #14
    void reiniciar(); // Restaura vida, mana, oro, posicion e inventario al estado inicial
};
