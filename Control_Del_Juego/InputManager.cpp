#include "InputManager.h"
#include <cmath> // Para la raiz cuadrada que usamos al normalizar el movimiento

///=============================================================///
///   #1 - CONSTRUCTOR - Todo arranca en falso, el jugador no quiere
///   hacer nada todavia
///=============================================================///
// #1
InputManager::InputManager() {
    _direccion_de_movimiento = sf::Vector2f(0.f, 0.f);
    _el_jugador_quiere_correr = false;
    _el_jugador_quiere_saltar = false;
    _el_jugador_quiere_interactuar = false;
    _el_jugador_quiere_atacar = false;
    _el_jugador_quiere_disparar = false;
    _el_jugador_quiere_agarrar_oro = false;
    _el_jugador_quiere_tirar_item = false;
    _el_jugador_quiere_abrir_el_inventario = false;
    _posicion_del_mouse_en_la_pantalla = sf::Vector2i(0, 0);
}

///=============================================================///
///   #2 - ACTUALIZAR TECLAS - Se llama una vez por frame y revisa
///   todo el teclado y el mouse de una sola vez
///=============================================================///
// #2
void InputManager::actualizar_las_teclas_apretadas_en_este_momento(const sf::RenderWindow& ventana_del_juego) {

    _posicion_del_mouse_en_la_pantalla = sf::Mouse::getPosition(ventana_del_juego);

    _direccion_de_movimiento = sf::Vector2f(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    _direccion_de_movimiento.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  _direccion_de_movimiento.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  _direccion_de_movimiento.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) _direccion_de_movimiento.x += 1.f;

    float largo_del_vector_de_movimiento = std::sqrt(_direccion_de_movimiento.x * _direccion_de_movimiento.x + _direccion_de_movimiento.y * _direccion_de_movimiento.y);
    if (largo_del_vector_de_movimiento != 0.f) {
        _direccion_de_movimiento /= largo_del_vector_de_movimiento;
    }

    _el_jugador_quiere_disparar = sf::Mouse::isButtonPressed(sf::Mouse::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);

    bool la_tecla_de_inventario_esta_apretada_ahora = sf::Keyboard::isKeyPressed(sf::Keyboard::I);
    bool la_tecla_de_atacar_esta_apretada_ahora = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool la_tecla_de_saltar_esta_apretada_ahora = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool la_tecla_de_tirar_item_esta_apretada_ahora = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
    bool la_tecla_de_interactuar_esta_apretada_ahora = sf::Keyboard::isKeyPressed(sf::Keyboard::F); // F agarra items del suelo (gemas, pociones, etc.)
    bool la_tecla_de_correr_esta_apretada_ahora = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    bool la_tecla_de_agarrar_oro_esta_apretada_ahora = sf::Keyboard::isKeyPressed(sf::Keyboard::F); // F tambien agarra el oro (misma tecla que items)

    _el_jugador_quiere_abrir_el_inventario = (la_tecla_de_inventario_esta_apretada_ahora && !_tecla_de_inventario_estaba_apretada_antes);
    _el_jugador_quiere_atacar = (la_tecla_de_atacar_esta_apretada_ahora && !_tecla_de_atacar_estaba_apretada_antes);
    _el_jugador_quiere_saltar = (la_tecla_de_saltar_esta_apretada_ahora && !_tecla_de_saltar_estaba_apretada_antes);
    _el_jugador_quiere_tirar_item = (la_tecla_de_tirar_item_esta_apretada_ahora && !_tecla_de_tirar_item_estaba_apretada_antes);
    _el_jugador_quiere_interactuar = (la_tecla_de_interactuar_esta_apretada_ahora && !_tecla_de_interactuar_estaba_apretada_antes);
    _el_jugador_quiere_correr = (la_tecla_de_correr_esta_apretada_ahora && !_tecla_de_correr_estaba_apretada_antes);
    _el_jugador_quiere_agarrar_oro = (la_tecla_de_agarrar_oro_esta_apretada_ahora && !_tecla_de_agarrar_oro_estaba_apretada_antes);

    _tecla_de_inventario_estaba_apretada_antes = la_tecla_de_inventario_esta_apretada_ahora;
    _tecla_de_atacar_estaba_apretada_antes = la_tecla_de_atacar_esta_apretada_ahora;
    _tecla_de_saltar_estaba_apretada_antes = la_tecla_de_saltar_esta_apretada_ahora;
    _tecla_de_tirar_item_estaba_apretada_antes = la_tecla_de_tirar_item_esta_apretada_ahora;
    _tecla_de_interactuar_estaba_apretada_antes = la_tecla_de_interactuar_esta_apretada_ahora;
    _tecla_de_correr_estaba_apretada_antes = la_tecla_de_correr_esta_apretada_ahora;
    _tecla_de_agarrar_oro_estaba_apretada_antes = la_tecla_de_agarrar_oro_esta_apretada_ahora;
}