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

    // Opcionales para cambiar de estado
    virtual void entrar(GameManager& gm) {}
    virtual void salir(GameManager& gm) {}
    virtual void pausa(GameManager& gm) {}
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
    bool _bossMuerto = false;
    void actualizarHordaYSpawns(float dt, GameManager& gm);
    void resolverCombateMagia(GameManager& gm);
  
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