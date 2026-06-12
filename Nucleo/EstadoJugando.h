#pragma once
#include "Estado.h" // Su padre.
#include <vector>
#include <memory> // por fin para los smart pointers

// Forward declarations para no sobrecargar de includes el header
class GameManager; // (gm)
class EntidadViva;

class EstadoJugando : public Estado {
private:
    std::vector<std::unique_ptr<EntidadViva>> _enemigos; // Smart pointer de entrada
    float _relojSpawn = 0.f;
    float _intervaloSpawn = 5.0f;
    bool _bossMuerto = false;

    void actualizarHordaYSpawns(float dt, GameManager& gm);
    void resolverCombateMagia(GameManager& gm);

public:
    EstadoJugando() = default;
    ~EstadoJugando() override = default; // Al usar unique_ptr, se limpia solo de forma automática

    void entrar(GameManager& gm) override;
    void procesarEventos(sf::Event& evento, GameManager& gm) override;
    void actualizar(float dt, GameManager& gm) override;
    void renderizar(GameManager& gm) override;

};