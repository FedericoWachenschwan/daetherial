#include "Estado.h"
#include "GameManager.h"
#include <iostream>
#include "Enemy.h"

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

// ============================================================================
// ESTADO: JUGANDO
// ============================================================================
EstadoJugando::~EstadoJugando() {
    for (int i = 0; i < (int)_enemigos.size(); i++) {
        delete _enemigos[i];
    }
    _enemigos.clear();
}

void EstadoJugando::procesarEventos(sf::Event& evento, GameManager& gm) {
    gm._input.procesarEvento(evento);
    gm._camara.procesarZoom(evento);

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F3) {
        gm._debug.toggleDebug();
    }
    gm._debug.procesarEventos(evento, gm._ventana, gm._hudInventario, gm._personaje, gm._golem);
    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
        gm._debug.procesarClicMapa(sf::Mouse::getPosition(gm._ventana), gm._camara.getVista(), gm._ventana);
    }
}

void EstadoJugando::actualizar(float dt, GameManager& gm) {
    // 1. SISTEMAS CORE
    gm._input.actualizarEstadoTiempoReal(gm._ventana);
    gm._camara.seguir(gm._personaje.getPosicion(), dt);
    gm._ventana.setView(gm._camara.getVista());

    // 2. SISTEMAS DE INTERFAZ E INVENTARIO
    if (gm._input.quiereAbrirInventario()) gm._hudInventario.toggle();

    if (gm._input.quiereAtacar()) {
        gm._hudInventario.detectarClicCasillero(gm._input.getPosicionMouse(), gm._personaje.getInventario(), gm._ventana);
    }

    if (gm._input.quiereTirarItem()) {
        Item* itemATirar = gm._personaje.getInventario().extraerItemPorIndice();
        if (itemATirar != nullptr) {
            itemATirar->setPosicion(gm._personaje.getPosicion());
            gm._objectsManager.recibirItemSoltado(itemATirar);
        }
    }

    // 3. ACTUALIZACIÓN DE ENTIDADES (Personaje y Boss)
    gm._personaje.manejarInput(gm._input, gm._mapa, gm._ventana, gm._hudInventario.isOpen(), dt);
    gm._personaje.actualizar(dt, gm._VisualFX);

    if (gm._golem != nullptr) {
        gm._golem->setPosicionObjetivo(gm._personaje.getCentroFisico());
        gm._golem->actualizar(dt);
        gm.colisionEntreEntidades(gm._personaje, *gm._golem);
    }

    // 4. LOGICA MODULAR DE ENEMIGOS EN COMBATE
    actualizarHordaYSpawns(dt, gm);
    resolverCombateMagia(gm);

    // 5. INTERACCIONES FÍSICAS MUNDO-PERSONAJE
    gm._VisualFX.actualizar(dt);
    gm._mascota.setPosicionObjetivo(gm._personaje.getCentroFisico());
    gm._mascota.actualizar(dt);
    gm._niebla.actualizar(dt);
    gm._objectsManager.chequearInteracciones(gm._personaje, gm._input);
    gm._debug.actualizar(gm._hudInventario, gm._personaje, gm._golem);

    ///============================================================///
    ///    LÓGICA DEL PORTAL DE SALIDA (NIVEL COMPLETADO)
    ///============================================================///
    sf::Vector2f coordenadaSalida(1632.f, 960.f);

    // 1. Creamos la Hitbox del portal (ejemplo: un rectángulo de 64x64 píxeles)
    sf::FloatRect hitboxPortal(coordenadaSalida.x, coordenadaSalida.y, 64.f, 64.f);

    // 2. ¿El Golem acaba de morir? ¡ENCENDEMOS LA ANIMACIÓN!
    if (gm._golem == nullptr && _bossMuerto == false) {
        gm._VisualFX.activarPortales();
        _bossMuerto = true; //Candado para que no vuelva a entrar al bucle
    }

    // 3. LLAMAMOS A COLISIONABLE: ¿El personaje pisa la hitbox del portal?
    if (gm._personaje.getBounds().intersects(hitboxPortal)) {
        if (gm._golem == nullptr) { // ¡Ganamos!
            std::cout << "¡Nivel completado! Cruzando el portal..." << std::endl;
            // gm.cambiarEstado(new EstadoMenu()); 
        }
        else {
            // ¡El patovica te rebota!
            // Como pisó el portal desde algún lado, lo tiramos para atrás en el eje Y o X
            // Un rebote rápido y simple sin tanta matemática:
            gm._personaje.setPosicion(gm._personaje.getPosicion() + sf::Vector2f(0.f, 30.f));
        }
    }


    ///============================================================///
    ///     TIENDA - Actualizamos y chequeamos si el jugador compra
    ///============================================================///
    if (gm._tienda != nullptr) {
        gm._tienda->actualizar_tienda(gm._personaje.getPosicion()); // Chequeamos si el jugador está cerca
        bool jugador_quiere_comprar = sf::Keyboard::isKeyPressed(sf::Keyboard::E); // Chequeamos si presiona E
        bool jugador_esta_cerca = gm._tienda->getJugadorEstaCercaDeLaTienda(); // Chequeamos si está cerca
        if (jugador_esta_cerca == true && jugador_quiere_comprar == true) {
            gm._tienda->intentar_comprar_item(gm._personaje); // Intentamos comprar
        }
    }
}

void EstadoJugando::renderizar(GameManager& gm) {
    // --- CAPA 1: MUNDO Y ENTIDADES ---
    gm._ventana.setView(gm._camara.getVista());
    gm._mapa.dibujarMapa(gm._ventana);
    gm._VisualFX.dibujar(gm._ventana);
    gm._objectsManager.dibujarItems(gm._ventana);

    ///============================================================///
    ///     TIENDA - Dibujamos la tienda en el mundo con la cámara activa
    ///============================================================///
    if (gm._tienda != nullptr) {
        gm._tienda->dibujar_tienda(gm._ventana, gm._debug.estaActivo()); // Dibujamos con la vista del mundo
    }

    for (int i = 0; i < (int)_enemigos.size(); i++) {
        _enemigos[i]->dibujar(gm._ventana);
    }
    gm._personaje.dibujar(gm._ventana);
    gm._mascota.dibujar(gm._ventana);

    if (gm._golem != nullptr) {
        gm._golem->dibujar(gm._ventana);
    }
    gm._niebla.dibujar(gm._ventana, gm._camara.getVista());

    // --- CAPA 2: MODO DEBUG ---
    if (gm._debug.estaActivo()) {
        gm._mapa.dibujarDebug(gm._ventana);
        gm._debug.dibujarCajaColision(gm._ventana, gm._personaje, sf::Color::Green);
        gm._debug.dibujarGrillaMapa(gm._ventana);

        for (int i = 0; i < (int)_enemigos.size(); i++) {
            gm._debug.dibujarCajaColision(gm._ventana, *_enemigos[i], sf::Color::Red);
        }

        if (gm._golem != nullptr) {
            gm._golem->dibujarPathFinder(gm._ventana);
            gm._debug.dibujarCajaColision(gm._ventana, *gm._golem, sf::Color::Magenta);
        }
    }

    // --- CAPA 3: INTERFAZ Y HUD ---
    gm._ventana.setView(gm._ventana.getDefaultView());
    gm._hudInventario.dibujar(gm._ventana, gm._personaje.getInventario());

    // --- CAPA 4: EXTRACTOR DEBUG ---
    if (gm._debug.estaActivo() && gm._debug.getObjetivoActual() == ObjetivoDebug::EXTRACTOR) {
        gm._debug.dibujarExtractor(gm._ventana, gm._itemManager.getTexturaMaestra());
    }
}

// ----------------------------------------------------------------------------
// SUB-FUNCIONES DE LÓGICA
// ----------------------------------------------------------------------------
void EstadoJugando::actualizarHordaYSpawns(float dt, GameManager& gm) {
    sf::Vector2f centroJugador = gm._personaje.getCentroFisico();

    if (gm._personaje.estaVivo() && gm._golem != nullptr && gm._golem->estaVivo()) {
        _relojSpawn += dt;
        if (_relojSpawn >= _intervaloSpawn) {
            sf::Vector2f posVFX(1376.f, 1408.f);
            EntidadViva* duendeHielo = new Enemy(posVFX, &gm._mapa, "assets/duendeHielo.png");
            _enemigos.push_back(duendeHielo);
            _relojSpawn = 0.f;
        }
    }

    for (int i = 0; i < (int)_enemigos.size(); i++) {
        _enemigos[i]->setPosicionObjetivo(centroJugador);
        _enemigos[i]->actualizar(dt);

        if (_enemigos[i]->getBounds().intersects(gm._personaje.getBounds())) {
            if (_enemigos[i]->puedeAtacar()) {
                gm._personaje.recibirDanio(_enemigos[i]->getDanio());

                sf::Vector2f dirEmpuje = gm._personaje.getPosicion() - _enemigos[i]->getPosicion();
                float len = std::hypot(dirEmpuje.x, dirEmpuje.y);
                if (len != 0) dirEmpuje /= len;

                sf::Vector2f fuerzaEmpuje = dirEmpuje * 25.f;
                gm._personaje.aplicarMovimientoConColisiones(fuerzaEmpuje, gm._mapa);

                std::cout << "💥 ¡GOLPE Y EMPUJE!" << std::endl;
            }
        }
    }
}

void EstadoJugando::resolverCombateMagia(GameManager& gm) {
    BolaDeFuego& magia = gm._personaje.getBolaDeFuego();
    if (!magia.estaActiva()) return;

    bool impacto = false;

    for (int i = 0; i < (int)_enemigos.size(); i++) {
        if (magia.getBounds().intersects(_enemigos[i]->getBounds())) {
            _enemigos[i]->recibirDanio(gm._personaje.getDanio());
            impacto = true;
            std::cout << "🔥 ¡IMPACTO! Marciano herido." << std::endl;

            if (_enemigos[i]->estaMuerto()) {
                std::cout << "💀 ¡Marciano fulminado!" << std::endl;
                delete _enemigos[i];
                _enemigos.erase(_enemigos.begin() + i);
                i--;
            }
            break;
        }
    }

    if (!impacto && gm._golem != nullptr) {
        if (magia.getBounds().intersects(gm._golem->getBounds())) {
            gm._golem->recibirDanio(gm._personaje.getDanio());
            impacto = true;
            std::cout << "🔥 ¡IMPACTO! El Gólem recibió daño." << std::endl;

            if (gm._golem->estaMuerto()) {
                std::cout << "💀 ¡EL GÓLEM HA SIDO DERROTADO!" << std::endl;
                delete gm._golem;
                gm._golem = nullptr;
            }
        }
    }

    if (impacto) magia.desactivar();
}

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