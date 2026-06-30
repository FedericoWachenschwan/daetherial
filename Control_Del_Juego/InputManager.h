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
    bool _tecla_de_saltar_estaba_apretada_antes = false;
    bool _tecla_de_tirar_item_estaba_apretada_antes = false;
    bool _tecla_de_interactuar_estaba_apretada_antes = false;
    bool _tecla_de_correr_estaba_apretada_antes = false;
    bool _tecla_de_agarrar_oro_estaba_apretada_antes = false;

    ///=============================================================///
    ///   HACIA DONDE SE MUEVE EL JUGADOR
    ///=============================================================///
    sf::Vector2f _direccion_de_movimiento; // SFML: dos numeros (x, y) que indican hacia donde se mueve el jugador

    ///=============================================================///
    ///   QUE QUIERE HACER EL JUGADOR EN ESTE FRAME
    ///=============================================================///
    bool _el_jugador_quiere_correr;
    bool _el_jugador_quiere_saltar;
    bool _el_jugador_quiere_interactuar;
    bool _el_jugador_quiere_atacar;
    bool _el_jugador_quiere_disparar;
    bool _el_jugador_quiere_agarrar_oro;
    bool _el_jugador_quiere_tirar_item;
    bool _el_jugador_quiere_abrir_el_inventario;

    ///=============================================================///
    ///   POSICION DEL MOUSE EN LA PANTALLA
    ///=============================================================///
    sf::Vector2i _posicion_del_mouse_en_la_pantalla; // SFML: la posicion del mouse en pixeles de pantalla

public:

    ///=============================================================///
    ///   CONSTRUCTOR - Arranca todo en "no quiero hacer nada"
    ///=============================================================///
    InputManager();

    ///=============================================================///
    ///   GETTERS - Le dicen a quien pregunte que quiere hacer el jugador
    ///=============================================================///
    sf::Vector2f getDireccion_de_movimiento() const { return _direccion_de_movimiento; }
    bool getEl_jugador_quiere_saltar() const { return _el_jugador_quiere_saltar; }
    bool getEl_jugador_quiere_correr() const { return _el_jugador_quiere_correr; }
    bool getEl_jugador_quiere_interactuar() const { return _el_jugador_quiere_interactuar; }
    bool getEl_jugador_quiere_atacar() const { return _el_jugador_quiere_atacar; }
    bool getEl_jugador_quiere_disparar() const { return _el_jugador_quiere_disparar; }
    bool getEl_jugador_quiere_tirar_item() const { return _el_jugador_quiere_tirar_item; }
    bool getEl_jugador_quiere_abrir_el_inventario() const { return _el_jugador_quiere_abrir_el_inventario; }
    bool getEl_jugador_quiere_agarrar_oro() const { return _el_jugador_quiere_agarrar_oro; }
    sf::Vector2i getPosicion_del_mouse() const { return _posicion_del_mouse_en_la_pantalla; }

    ///=============================================================///
    ///   OTROS METODOS - Leen el teclado/mouse y actualizan TODOS
    ///   los atributos de arriba a la vez, por eso no son
    ///   getters/setters simples
    ///=============================================================///
    void procesar_un_evento_del_teclado_o_mouse(const sf::Event& evento_ocurrido); // SFML: sf::Event es un "aviso" de que algo paso (una tecla, un clic, etc)
    void actualizar_las_teclas_apretadas_en_este_momento(const sf::RenderWindow& ventana_del_juego);
};