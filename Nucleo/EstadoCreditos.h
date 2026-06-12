#pragma once
#include "Estado.h" // Su padre.
#include <vector>
#include <memory> // por fin para los smart pointers

class EstadoCreditos : public Estado {
public:
    void procesarEventos(sf::Event& evento, GameManager& gm) override;
    void actualizar(float dt, GameManager& gm) override;
    void renderizar(GameManager& gm) override;
};