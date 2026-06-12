#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Forward declaration: Le avisamos a C++ que gm existe
class GameManager;
class EntidadViva;

// ==========================================
// CLASE PADRE (Interfaz) (gm = GameManager)
// ==========================================
class Estado {
public:
    virtual ~Estado() = default;
    
    // Los 3 métodos obligatorios para cualquier pantalla del juego
    virtual void procesarEventos(sf::Event& evento, GameManager& gm) = 0; // Eventos de SFML
    virtual void actualizar(float dt, GameManager& gm) = 0; // Logica por ciclo o frame
    virtual void renderizar(GameManager& gm) = 0; // Recibe la referencia de GameManager para acceder a sus cosas

    virtual void entrar(GameManager& gm) {}
    virtual void salir(GameManager& gm) {}
    virtual void pausa(GameManager& gm) {}
};
