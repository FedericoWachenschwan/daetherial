#pragma once
#include <SFML/Graphics.hpp>

// Forward declaration: Le avisamos a C++ que GameManager existe, 
// sin tener que incluir GameManager.h acá (evita errores de dependencias circulares)
class GameManager;

// ==========================================
// CLASE PADRE (Interfaz)
// ==========================================
class Estado {
public:
    virtual ~Estado() = default;

    // Los 3 métodos obligatorios para cualquier pantalla del juego
    virtual void procesarEventos(sf::Event& evento, GameManager& GameManager) = 0;
    virtual void actualizar(float dt, GameManager& GameManager) = 0;
    virtual void renderizar(GameManager& GameManager) = 0; // Solo pasamos GameManager porque adentro tiene la ventana
};

// ==========================================
// CLASES HIJAS (Las pantallas reales)
// ==========================================

class EstadoMenu : public Estado {
public:
    void procesarEventos(sf::Event& evento, GameManager& GameManager) override;
    void actualizar(float dt, GameManager& GameManager) override;
    void renderizar(GameManager& GameManager) override;
};

class EstadoJugando : public Estado {
public:
    void procesarEventos(sf::Event& evento, GameManager& GameManager) override;
    void actualizar(float dt, GameManager& GameManager) override;
    void renderizar(GameManager& GameManager) override;
};

class EstadoCreadorItems : public Estado {
public:
    void procesarEventos(sf::Event& evento, GameManager& GameManager) override;
    void actualizar(float dt, GameManager& GameManager) override;
    void renderizar(GameManager& GameManager) override;
};

class EstadoCreditos : public Estado {
public:
    void procesarEventos(sf::Event& evento, GameManager& GameManager) override;
    void actualizar(float dt, GameManager& GameManager) override;
    void renderizar(GameManager& GameManager) override;
};