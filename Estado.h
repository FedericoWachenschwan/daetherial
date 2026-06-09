#pragma once
#include <SFML/Graphics.hpp>
#include "Boss.h"

// Forward declaration: Le avisamos a C++ que gm existe, 
// sin tener que incluir gm.h acá (evita errores de dependencias circulares)
class GameManager;

// ==========================================
// CLASE PADRE (Interfaz) (gm = GameManager)
// ==========================================
class Estado {
public:
    virtual ~Estado() = default;

    // Los 3 métodos obligatorios para cualquier pantalla del juego
    virtual void procesarEventos(sf::Event& evento, GameManager& gm) = 0;
    virtual void actualizar(float dt, GameManager& gm) = 0;
    virtual void renderizar(GameManager& gm) = 0;
};

// ==========================================
// CLASES HIJAS (Las pantallas reales)
// ==========================================
class EstadoMenu : public Estado {
public:
    void procesarEventos(sf::Event& evento, GameManager& gm) override;
    void actualizar(float dt, GameManager& gm) override;
    void renderizar(GameManager& gm) override;
};

class EstadoJugando : public Estado {
private:
    std::vector<EntidadViva*> _enemigos; // spawn de npcs usando memoria dinamica
    float _relojSpawn = 0.f;
    float _intervaloSpawn = 5.0f; // Spawn cada 5 segundos
    void actualizarHordaYSpawns(float dt, GameManager& gm);
    void resolverCombateMagia(GameManager& gm);
    bool _bossMuerto = false;

 
  
public:
    ~EstadoJugando() override; // Declaracion del destructor para limpiar la memoria

    void procesarEventos(sf::Event& evento, GameManager& gm) override;
    void actualizar(float dt, GameManager& gm) override;
    void renderizar(GameManager& gm) override;

};

class EstadoCreadorItems : public Estado {
public:
    void procesarEventos(sf::Event& evento, GameManager& gm) override;
    void actualizar(float dt, GameManager& gm) override;
    void renderizar(GameManager& gm) override;
};

class EstadoCreditos : public Estado {
public:
    void procesarEventos(sf::Event& evento, GameManager& gm) override;
    void actualizar(float dt, GameManager& gm) override;
    void renderizar(GameManager& gm) override;
};