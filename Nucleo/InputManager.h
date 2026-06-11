#pragma once
#include <SFML/Graphics.hpp>

class InputManager {
private:
    // --- ESTADOS ANTERIORES PARA NO SPAMEAR (Flanco de subida) ---
    bool _antesInventario = false;
    bool _antesAtacar = false;
    bool _antesSaltar = false;
    bool _antesTirarItem = false;
    bool _antesInteractuar = false;
    bool _antesCorrer = false;

    // --- MOVIMIENTO ---
    sf::Vector2f _direccionMovimiento;
    bool _clickIzquierdoApretado;
    bool _quiereSaltar;
    bool _quiereCorrer;

    // --- COMBATE E INTERACCIÓN ---
    bool _quiereInteractuar;
    bool _quiereAtacar;
    bool _quiereDisparar;

    // --- INVENTARIO Y UI ---
    bool _quiereTirarItem;
    bool _quiereAbrirInventario; // 🌟 AGREGADO: Para controlar la apertura de la mochila
    int _deltaScroll;
    sf::Vector2i _posicionMousePantalla;

public:
    InputManager(); // Tu constructor manual

    void procesarEvento(const sf::Event& evento);
    void actualizarEstadoTiempoReal(const sf::RenderWindow& ventana);

    // ==========================================
    // 🌟 TODOS LOS GETTERS EXPLÍCITOS (Sincronizados)
    // ==========================================
    sf::Vector2f getDireccionMovimiento() const { return _direccionMovimiento; }
    bool quiereSaltar() const { return _quiereSaltar; }
    bool quiereCorrer() const { return _quiereCorrer; }

    bool quiereInteractuar() const { return _quiereInteractuar; }
    bool quiereAtacar() const { return _quiereAtacar; }
    bool quiereDisparar() const { return _quiereDisparar; }
    bool quiereTirarItem() const { return _quiereTirarItem; }
    bool quiereAbrirInventario() const { return _quiereAbrirInventario; } // 🌟 AGREGADO

    int getDeltaScroll() const { return _deltaScroll; }
    sf::Vector2i getPosicionMouse() const { return _posicionMousePantalla; }
};