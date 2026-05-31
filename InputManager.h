#pragma once
#include <SFML/Graphics.hpp>

class InputManager {
private:
    // --- MOVIMIENTO ---
    sf::Vector2f _direccionMovimiento;
    bool _clickIzquierdoApretado; // 🌟 Se mantiene por si procesas eventos
    bool _quiereSaltar;
    bool _quiereCorrer;

    // --- COMBATE E INTERACCIÓN ---
    bool _quiereInteractuar;
    bool _quiereAtacar;
    bool _quiereDisparar;

    // --- INVENTARIO Y UI ---
    bool _quiereTirarItem;
    int _deltaScroll;
    sf::Vector2i _posicionMousePantalla;

public:
    InputManager();

    void procesarEvento(const sf::Event& evento);
    void actualizarEstadoTiempoReal(const sf::RenderWindow& ventana);

    // ==========================================
    // 🌟 TODOS LOS GETTERS EXPLÍCITOS (Limpios y sin duplicar)
    // ==========================================
    sf::Vector2f getDireccionMovimiento() const { return _direccionMovimiento; }
    bool quiereSaltar() const { return _quiereSaltar; }
    bool quiereCorrer() const { return _quiereCorrer; }

    bool quiereInteractuar() const { return _quiereInteractuar; }
    bool quiereAtacar() const { return _quiereAtacar; } // 🌟 Devuelve tu flag de combate
    bool quiereDisparar() const { return _quiereDisparar; }
    bool quiereTirarItem() const { return _quiereTirarItem; }

    int getDeltaScroll() const { return _deltaScroll; }
    sf::Vector2i getPosicionMouse() const { return _posicionMousePantalla; } // 🌟 Apunta a tu variable real
};