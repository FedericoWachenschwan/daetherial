#pragma once
#include <SFML/Graphics.hpp>
// SFML: la libreria que maneja graficos, ventanas, teclado y mouse en este juego

///=================================================================///
///   INPUT_MANAGER - Lee que hizo el jugador con el teclado y el
///   mouse, y lo guarda en variables faciles de leer y preguntar
///=================================================================///
class InputManager {
private:

 ///=============================================================///
 ///   RECUERDOS DEL FRAME ANTERIOR
 ///   Los usamos para saber si una tecla se ACABA de apretar,
 ///   y no si el jugador la esta manteniendo apretada
 ///=============================================================///
    bool _tecla_de_inventario_estaba_apretada_antes = false;
    bool _tecla_de_atacar_estaba_apretada_antes = false;
    bool _tecla_de_apuntar_estaba_apretada_antes = false;
    bool _tecla_de_interactuar_estaba_apretada_antes = false;
    bool _tecla_de_dash_estaba_apretada_antes = false;
    bool _tecla_de_agarrar_oro_estaba_apretada_antes = false;

 ///=============================================================///
 ///   HACIA DONDE SE MUEVE EL JUGADOR
 ///=============================================================///
    sf::Vector2f _direccion_de_movimiento; // SFML: dos numeros (x, y) que indican hacia donde se mueve el jugador

 ///=============================================================///
 ///   QUE QUIERE HACER EL JUGADOR EN ESTE FRAME
 ///=============================================================///
    bool _el_jugador_quiere_hacer_dash;
    bool _el_jugador_quiere_apuntar;
    bool _el_jugador_quiere_interactuar;
    bool _el_jugador_quiere_atacar;
    bool _el_jugador_quiere_disparar;
    bool _el_jugador_quiere_agarrar_oro;
    bool _el_jugador_quiere_abrir_el_inventario;

 ///=============================================================///
 ///   POSICION DEL MOUSE EN LA PANTALLA
 ///=============================================================///
    sf::Vector2i _posicion_del_mouse_en_la_pantalla; // SFML: la posicion del mouse en pixeles de pantalla

public:

 ///=============================================================///
 ///   CONSTRUCTOR - Arranca todo en "no quiero hacer nada"
 ///=============================================================///
 // #1
    InputManager();

 ///=============================================================///
 ///   GETTERS - Le dicen a quien pregunte que quiere hacer el jugador
 ///=============================================================///
 // #2
    sf::Vector2f getDireccion_de_movimiento() const { return _direccion_de_movimiento; }
 // #3
    bool getEl_jugador_quiere_apuntar() const { return _el_jugador_quiere_apuntar; }
 // #4
    bool getEl_jugador_quiere_hacer_dash() const { return _el_jugador_quiere_hacer_dash; }
 // #5
    bool getEl_jugador_quiere_interactuar() const { return _el_jugador_quiere_interactuar; }
 // #6
    bool getEl_jugador_quiere_atacar() const { return _el_jugador_quiere_atacar; }
 // #7
    bool getEl_jugador_quiere_disparar() const { return _el_jugador_quiere_disparar; }
 // #8
    bool getEl_jugador_quiere_abrir_el_inventario() const { return _el_jugador_quiere_abrir_el_inventario; }
 // #9
    bool getEl_jugador_quiere_agarrar_oro() const { return _el_jugador_quiere_agarrar_oro; }
 // #10
    sf::Vector2i getPosicion_del_mouse() const { return _posicion_del_mouse_en_la_pantalla; }

 ///=============================================================///
 ///   OTROS METODOS - Leen el teclado/mouse y actualizan TODOS
 ///   los atributos de arriba a la vez, por eso no son
 ///   getters/setters simples
 ///=============================================================///
 // #11
    void actualizar_las_teclas_apretadas_en_este_momento(const sf::RenderWindow& ventana_del_juego);
};