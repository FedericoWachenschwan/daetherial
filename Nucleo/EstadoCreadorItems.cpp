#include "EstadoCreadorItems.h"
#include "EstadoMenu.h"
#include "EstadoJugando.h"
#include "GameManager.h"
#include "Enemy.h"
#include <iostream>
#include <memory> 

// ============================================================================
// ESTADO: CREADOR DE ÍTEMS
// ============================================================================
void EstadoCreadorItems::procesarEventos(sf::Event& evento, GameManager& gm) {
    gm._uiCreadorItems.procesarEventos(evento, gm._itemManager);

    if (evento.type == sf::Event::KeyPressed &&
        (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::Right)) {
        gm._uiCreadorItems.actualizarSprite(gm._itemManager.getTexturaMaestra());
    }

    if (gm._uiCreadorItems.quiereGuardar()) {
        ItemReg nuevoItem = gm._uiCreadorItems.generarRegistro();

        if (gm._itemManager.guardarRegistro(nuevoItem)) {
            std::cout << "✅ ÍTEM GUARDADO: " << nuevoItem.nombre << std::endl;
        }
        else {
            std::cout << "❌ Error al guardar el ítem." << std::endl;
        }
        gm._uiCreadorItems.confirmarGuardado();
    }

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        gm.cambiarEstado(new EstadoMenu());
    }
}

void EstadoCreadorItems::actualizar(float dt, GameManager& gm) {
    // La UI se actualiza por eventos principalmente
}

void EstadoCreadorItems::renderizar(GameManager& gm) {
    gm._ventana.setView(gm._ventana.getDefaultView());
    gm._uiCreadorItems.dibujar(gm._ventana);
}