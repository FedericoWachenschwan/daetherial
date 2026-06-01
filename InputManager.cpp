#include "InputManager.h"
#include <cmath> // 🌟 Necesario para std::sqrt()

InputManager::InputManager() {
    _direccionMovimiento = sf::Vector2f(0.f, 0.f);
    _clickIzquierdoApretado = false;
    _quiereSaltar = false;
    _quiereCorrer = false;
    _quiereInteractuar = false;
    _quiereAtacar = false;
    _quiereDisparar = false;
    _quiereTirarItem = false;
    _quiereAbrirInventario = false;
    _deltaScroll = 0;
    _posicionMousePantalla = sf::Vector2i(0, 0);
}

void InputManager::procesarEvento(const sf::Event& evento) {
    // Leemos la ruedita del mouse (SFML no tiene lectura en tiempo real para el delta del scroll)
    if (evento.type == sf::Event::MouseWheelScrolled) {
        if (evento.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            _deltaScroll = static_cast<int>(evento.mouseWheelScroll.delta);
        }
    }
}

void InputManager::actualizarEstadoTiempoReal(const sf::RenderWindow& ventana) {
    // Actualizamos la posición del mouse en base a tu variable real
    _posicionMousePantalla = sf::Mouse::getPosition(ventana);

    // =========================================================================
    // 🏃 1. ENTRADAS CONTINUAS: Movimiento y Modificadores (Fluido)
    // =========================================================================
    _direccionMovimiento = sf::Vector2f(0.f, 0.f);

    // Soporte nativo para WASD o Flechas de dirección
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    _direccionMovimiento.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  _direccionMovimiento.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  _direccionMovimiento.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) _direccionMovimiento.x += 1.f;

    // Normalización del vector de movimiento (Para que no corra más rápido de costado)
    float longitud = std::sqrt(_direccionMovimiento.x * _direccionMovimiento.x + _direccionMovimiento.y * _direccionMovimiento.y);
    if (longitud != 0.f) {
        _direccionMovimiento /= longitud;
    }

    // Correr es continuo: si mantenés Shift, corre todo el tiempo
    _quiereCorrer = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

    // Disparo secundario continuo (Clic Derecho o Control Izquierdo)
    _quiereDisparar = sf::Mouse::isButtonPressed(sf::Mouse::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);

    // =========================================================================
    // ⚡ 2. ENTRADAS DISCRETAS: Filtrado por Flanco de Subida (Edge Detection)
    // =========================================================================

    // Captura lo que pasa en ESTE instante exacto
    bool ahoraInventario = sf::Keyboard::isKeyPressed(sf::Keyboard::I);
    bool ahoraAtacar = sf::Mouse::isButtonPressed(sf::Mouse::Left); // Clic izquierdo para interactuar/atacar
    bool ahoraSaltar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool ahoraTirarItem = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
    bool ahoraInteractuar = sf::Keyboard::isKeyPressed(sf::Keyboard::E);

    // El filtro mágico: Solo da TRUE en el frame exacto que se presiona
    _quiereAbrirInventario = (ahoraInventario && !_antesInventario);
    _quiereAtacar = (ahoraAtacar && !_antesAtacar);
    _quiereSaltar = (ahoraSaltar && !_antesSaltar);
    _quiereTirarItem = (ahoraTirarItem && !_antesTirarItem);
    _quiereInteractuar = (ahoraInteractuar && !_antesInteractuar);

    // Guardamos el estado actual para el análisis del próximo frame
    _antesInventario = ahoraInventario;
    _antesAtacar = ahoraAtacar;
    _antesSaltar = ahoraSaltar;
    _antesTirarItem = ahoraTirarItem;
    _antesInteractuar = ahoraInteractuar;

    // Copia de respaldo por si usan la vieja variable de eventos
    _clickIzquierdoApretado = ahoraAtacar;
}