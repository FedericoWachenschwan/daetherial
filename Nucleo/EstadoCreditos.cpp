#include "EstadoCreditos.h"
#include "EstadoMenu.h"
#include "EstadoJugando.h"
#include "EstadoCreadorItems.h"
#include "GameManager.h"
#include "Enemy.h"
#include <iostream>
#include <memory> 


// ============================================================================
// ESTADO: CRÉDITOS
// ============================================================================
void EstadoCreditos::procesarEventos(sf::Event& evento, GameManager& gm) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        gm.cambiarEstado(new EstadoMenu());
    }
}

void EstadoCreditos::actualizar(float dt, GameManager& gm) {
    // Créditos estáticos
}

void EstadoCreditos::renderizar(GameManager& gm) {
    gm._ventana.setView(gm._ventana.getDefaultView());
    gm._ventana.draw(gm._textoCreditos);
}