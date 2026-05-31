#include "InputManager.h"
#include <cmath> // 🌟 Necesario para std::sqrt()

InputManager::InputManager() {
    // --- Inicializamos todo en cero/falso por seguridad ---
    _direccionMovimiento = sf::Vector2f(0.f, 0.f);
    _quiereSaltar = false;
    _quiereCorrer = false;
    _quiereInteractuar = false;
    _quiereAtacar = false;
    _quiereDisparar = false;
    _quiereTirarItem = false;
    _clickIzquierdoApretado = false;
    _deltaScroll = 0;
    _posicionMousePantalla = sf::Vector2i(0, 0);
}

void InputManager::procesarEvento(const sf::Event& evento) {
    // Reseteamos el scroll cada frame para que no se quede trabado girando infinito
    _deltaScroll = 0;

    // Leemos la ruedita del mouse
    if (evento.type == sf::Event::MouseWheelScrolled) {
        if (evento.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            _deltaScroll = static_cast<int>(evento.mouseWheelScroll.delta);
        }
    }

    // 🌟 TRUCO: Acciones de "Una sola vez" (Press and release)
    // Para interactuar o tirar ítems, no queremos que pase 60 veces por segundo si dejamos apretado
    if (evento.type == sf::Event::KeyPressed) {
		if (evento.key.code == sf::Keyboard::E) _quiereInteractuar = true; // E de "Interactuar"
		if (evento.key.code == sf::Keyboard::Q) _quiereTirarItem = true; // Q de "Quitar" o "Tirar"
		if (evento.key.code == sf::Keyboard::Space) _quiereSaltar = true; // Space de "Saltar"
    }
    else if (evento.type == sf::Event::KeyReleased) {
        if (evento.key.code == sf::Keyboard::E) _quiereInteractuar = false;
        if (evento.key.code == sf::Keyboard::Q) _quiereTirarItem = false;
        if (evento.key.code == sf::Keyboard::Space) _quiereSaltar = false;
    }
}

void InputManager::actualizarEstadoTiempoReal(const sf::RenderWindow& ventana) {
    // 1. --- CÁLCULO DEL VECTOR DE MOVIMIENTO (WASD o Flechas) ---
    float x = 0.f;
    float y = 0.f;

    // SFML usa Y positivo hacia ABAJO de la pantalla
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    y -= 1.f; // W de "Arriba" o Flecha Arriba
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  y += 1.f; // S de "Abajo" o Flecha Abajo
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  x -= 1.f; // A de "Izquierda" o Flecha Izquierda
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x += 1.f; // D de "Derecha" o Flecha Derecha

    // 2. --- NORMALIZACIÓN (Desactivada temporalmente para probar) ---
    _direccionMovimiento.x = x;
    _direccionMovimiento.y = y;

    // 3. --- ESTADOS CONTINUOS (Se pueden mantener apretados) ---
    _quiereCorrer = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

    // Ataque cuerpo a cuerpo (Clic Izquierdo)
    _quiereAtacar = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    // Disparo (Clic Derecho o Control)
    _quiereDisparar = sf::Mouse::isButtonPressed(sf::Mouse::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);

    // 4. --- PUNTERO ---
    _posicionMousePantalla = sf::Mouse::getPosition(ventana);
}