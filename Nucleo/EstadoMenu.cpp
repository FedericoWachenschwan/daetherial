#include "EstadoMenu.h"
#include "EstadoJugando.h"
#include "EstadoCreadorItems.h"
#include "EstadoCreditos.h"
#include "GameManager.h"
#include "Enemy.h"
#include <iostream>
#include <memory> 

// ============================================================================
// ESTADO: MENÚ
// ============================================================================
void EstadoMenu::procesarEventos(sf::Event& evento, GameManager& gm) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Up) gm._menu.moveUp();
        if (evento.key.code == sf::Keyboard::Down) gm._menu.moveDown();

        if (evento.key.code == sf::Keyboard::Enter) {
            int selected = gm._menu.getSelectedIndex();

            if (selected == 0) {
                gm.cambiarEstado(new EstadoJugando());
                gm.cambiarMusica(1);
                gm._reloj.restart();
                gm._mapa.generarClima(gm._VisualFX);
                gm._VisualFX.agregarPortal(sf::Vector2f(1632.f, 960.f), 6, true, false);    // Dibujamos dentro del menu el portal que te lleva al nivel 2
                gm._VisualFX.agregarPortal(sf::Vector2f(1376.f, 1408.f), 6, true, true);    // Dibujamos dentro del menu el portal del spawn de la horda
            }
            else if (selected == 1) {
                gm.cambiarEstado(new EstadoCreadorItems());
                gm._uiCreadorItems.actualizarSprite(gm._itemManager.getTexturaMaestra());
            }
            else if (selected == 2) {
                std::cout << "🏆 Pantalla de Logros en construccion..." << std::endl;
            }
            else if (selected == 3) {
                gm.cambiarEstado(new EstadoCreditos());
            }
            else if (selected == 4) {
                gm._ventana.close();
            }
        }
    }
}

void EstadoMenu::actualizar(float dt, GameManager& gm) {
    // El menú es estático, no necesita actualizar físicas por ahora.
}

void EstadoMenu::renderizar(GameManager& gm) {
    gm._ventana.setView(gm._ventana.getDefaultView());
    gm._menu.draw(gm._ventana);
}

