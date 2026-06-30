#include "MaquinaDeEstados.h"
#include "GameManager.h"
#include <iostream>
#include <cmath>

void procesar_eventos_pantalla_intro(GameManager& gm, sf::Event& evento);
void actualizar_pantalla_intro(GameManager& gm, float dt);
void renderizar_pantalla_intro(GameManager& gm);

void procesar_eventos_pantalla_menu(GameManager& gm, sf::Event& evento);
void actualizar_pantalla_menu(GameManager& gm, float dt);
void renderizar_pantalla_menu(GameManager& gm);

void procesar_eventos_pantalla_lore(GameManager& gm, sf::Event& evento);
void actualizar_pantalla_lore(GameManager& gm, float dt);
void renderizar_pantalla_lore(GameManager& gm);

void procesar_eventos_pantalla_historia(GameManager& gm, sf::Event& evento);
void actualizar_pantalla_historia(GameManager& gm, float dt);
void renderizar_pantalla_historia(GameManager& gm);

void procesar_eventos_pantalla_jugando(GameManager& gm, sf::Event& evento);
void actualizar_pantalla_jugando(GameManager& gm, float dt);
void renderizar_pantalla_jugando(GameManager& gm);
void actualizar_horda_y_spawns_de_marcianitos(GameManager& gm, float dt);
void resolver_combate_de_magia_contra_enemigos(GameManager& gm);

void procesar_eventos_pantalla_creador_items(GameManager& gm, sf::Event& evento);
void actualizar_pantalla_creador_items(GameManager& gm, float dt);
void renderizar_pantalla_creador_items(GameManager& gm);

void procesar_eventos_pantalla_creditos(GameManager& gm, sf::Event& evento);
void actualizar_pantalla_creditos(GameManager& gm, float dt);
void renderizar_pantalla_creditos(GameManager& gm);

///=================================================================///
///=================================================================///
///   LAS 3 FUNCIONES PRINCIPALES - El "semaforo": reparten el
///   trabajo segun la pantalla actual
///=================================================================///
///=================================================================///
void procesar_eventos_segun_la_pantalla(GameManager& gm, sf::Event& evento) {
    if (gm.pantalla_actual == PantallaDelJuego::INTRO) procesar_eventos_pantalla_intro(gm, evento);
    else if (gm.pantalla_actual == PantallaDelJuego::MENU) procesar_eventos_pantalla_menu(gm, evento);
    else if (gm.pantalla_actual == PantallaDelJuego::LORE) procesar_eventos_pantalla_lore(gm, evento);
    else if (gm.pantalla_actual == PantallaDelJuego::HISTORIA) procesar_eventos_pantalla_historia(gm, evento);
    else if (gm.pantalla_actual == PantallaDelJuego::JUGANDO) procesar_eventos_pantalla_jugando(gm, evento);
    else if (gm.pantalla_actual == PantallaDelJuego::CREADOR_ITEMS) procesar_eventos_pantalla_creador_items(gm, evento);
    else if (gm.pantalla_actual == PantallaDelJuego::CREDITOS) procesar_eventos_pantalla_creditos(gm, evento);
}

void actualizar_segun_la_pantalla(GameManager& gm, float tiempo_transcurrido) {
    if (gm.pantalla_actual == PantallaDelJuego::INTRO) actualizar_pantalla_intro(gm, tiempo_transcurrido);
    else if (gm.pantalla_actual == PantallaDelJuego::MENU) actualizar_pantalla_menu(gm, tiempo_transcurrido);
    else if (gm.pantalla_actual == PantallaDelJuego::LORE) actualizar_pantalla_lore(gm, tiempo_transcurrido);
    else if (gm.pantalla_actual == PantallaDelJuego::HISTORIA) actualizar_pantalla_historia(gm, tiempo_transcurrido);
    else if (gm.pantalla_actual == PantallaDelJuego::JUGANDO) actualizar_pantalla_jugando(gm, tiempo_transcurrido);
    else if (gm.pantalla_actual == PantallaDelJuego::CREADOR_ITEMS) actualizar_pantalla_creador_items(gm, tiempo_transcurrido);
    else if (gm.pantalla_actual == PantallaDelJuego::CREDITOS) actualizar_pantalla_creditos(gm, tiempo_transcurrido);
}

void renderizar_segun_la_pantalla(GameManager& gm) {
    if (gm.pantalla_actual == PantallaDelJuego::INTRO) renderizar_pantalla_intro(gm);
    else if (gm.pantalla_actual == PantallaDelJuego::MENU) renderizar_pantalla_menu(gm);
    else if (gm.pantalla_actual == PantallaDelJuego::LORE) renderizar_pantalla_lore(gm);
    else if (gm.pantalla_actual == PantallaDelJuego::HISTORIA) renderizar_pantalla_historia(gm);
    else if (gm.pantalla_actual == PantallaDelJuego::JUGANDO) renderizar_pantalla_jugando(gm);
    else if (gm.pantalla_actual == PantallaDelJuego::CREADOR_ITEMS) renderizar_pantalla_creador_items(gm);
    else if (gm.pantalla_actual == PantallaDelJuego::CREDITOS) renderizar_pantalla_creditos(gm);
}

///=================================================================///
///=================================================================///
///   PANTALLA 1: INTRO - Animacion de apertura del juego
///=================================================================///
///=================================================================///
void procesar_eventos_pantalla_intro(GameManager& gm, sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Enter) {
        gm.pantalla_actual = PantallaDelJuego::MENU;
    }
}

void actualizar_pantalla_intro(GameManager& gm, float dt) {

    const int CANTIDAD_DE_COLUMNAS = 6;
    const int CANTIDAD_DE_FILAS = 4;
    const int CANTIDAD_TOTAL_DE_FRAMES = CANTIDAD_DE_COLUMNAS * CANTIDAD_DE_FILAS;
    const float DURACION_DE_CADA_FRAME = 1.f / 12.f;

    gm.tiempo_acumulado_del_frame_de_intro += dt;

    if (gm.tiempo_acumulado_del_frame_de_intro >= DURACION_DE_CADA_FRAME) {
        gm.tiempo_acumulado_del_frame_de_intro = 0.f;
        gm.frame_actual_de_la_intro = gm.frame_actual_de_la_intro + 1;

        if (gm.frame_actual_de_la_intro >= CANTIDAD_TOTAL_DE_FRAMES) {
            gm.frame_actual_de_la_intro = 0;
        }
    }
}

void renderizar_pantalla_intro(GameManager& gm) {

    const int ANCHO_DE_CADA_FRAME = 480;
    const int ALTO_DE_CADA_FRAME = 270;
    const int CANTIDAD_DE_COLUMNAS = 6;

    int columna_del_frame_actual = gm.frame_actual_de_la_intro % CANTIDAD_DE_COLUMNAS;
    int fila_del_frame_actual = gm.frame_actual_de_la_intro / CANTIDAD_DE_COLUMNAS;

    gm.sprite_de_la_intro.setTextureRect(sf::IntRect(
        columna_del_frame_actual * ANCHO_DE_CADA_FRAME,
        fila_del_frame_actual * ALTO_DE_CADA_FRAME,
        ANCHO_DE_CADA_FRAME,
        ALTO_DE_CADA_FRAME
    ));

    gm.ventana.setView(gm.ventana.getDefaultView());
    gm.ventana.draw(gm.sprite_de_la_intro);

    sf::Text texto_para_continuar;
    texto_para_continuar.setFont(gm.fuente_de_textos);
    texto_para_continuar.setCharacterSize(16);
    texto_para_continuar.setFillColor(sf::Color::White);
    texto_para_continuar.setString("Presiona ENTER para continuar");
    texto_para_continuar.setPosition(560.f, 660.f);
    gm.ventana.draw(texto_para_continuar);
}

///=================================================================///
///=================================================================///
///   PANTALLA 2: MENU
///=================================================================///
///=================================================================///
void procesar_eventos_pantalla_menu(GameManager& gm, sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Up) gm.menu.mover_seleccion_hacia_arriba();
        if (evento.key.code == sf::Keyboard::Down) gm.menu.mover_seleccion_hacia_abajo();

        if (evento.key.code == sf::Keyboard::Enter) {
            int opcion_seleccionada = gm.menu.getIndice_de_la_opcion_seleccionada();

            if (opcion_seleccionada == 0) {
                if (gm.es_la_primera_partida == true) {
                    gm.es_la_primera_partida = false;
                    gm.imagen_actual_de_la_historia = 0;
                    gm.tiempo_acumulado_en_la_historia = 0.f;
                    gm.el_audio_de_la_historia_termino = false;
                    gm.musica_ambiente.stop();
                    gm.musica_de_la_historia.setLoop(false);
                    gm.musica_de_la_historia.play();
                    gm.pantalla_actual = PantallaDelJuego::HISTORIA;
                }
                else {
                    gm.pantalla_actual = PantallaDelJuego::LORE;
                }
            }
            else if (opcion_seleccionada == 1) {
                gm.pantalla_actual = PantallaDelJuego::CREADOR_ITEMS;
                gm.ui_creador_items.actualizar_sprite_de_vista_previa(gm.fabrica_de_items.getTextura_maestra());
            }
            else if (opcion_seleccionada == 2) {
                std::cout << "PANTALLA DE LOGROS EN CONSTRUCCION..." << std::endl;
            }
            else if (opcion_seleccionada == 3) {
                gm.pantalla_actual = PantallaDelJuego::CREDITOS;
            }
            else if (opcion_seleccionada == 4) {
                gm.ventana.close();
            }
        }
    }
}

void actualizar_pantalla_menu(GameManager& gm, float dt) {
    // El menu no necesita actualizar nada por frame
}

void renderizar_pantalla_menu(GameManager& gm) {
    gm.ventana.setView(gm.ventana.getDefaultView());
    gm.menu.dibujar(gm.ventana);
}

///=================================================================///
///=================================================================///
///   PANTALLA 3: HISTORIA - Imagenes con audio al iniciar la primera
///   partida. Las imagenes se sincronizan con la duracion del audio.
///   Al terminar el audio aparece el texto para continuar.
///=================================================================///
///=================================================================///
void procesar_eventos_pantalla_historia(GameManager& gm, sf::Event& evento) {
    if (evento.type != sf::Event::KeyPressed) return;

    ///=============================================================///
    ///   EL JUGADOR APRIETA ENTER CUANDO LA HISTORIA TERMINO
    ///=============================================================///
    bool el_jugador_aprieto_enter = false;
    if (evento.key.code == sf::Keyboard::Enter) el_jugador_aprieto_enter = true;

    if (el_jugador_aprieto_enter == true && gm.el_audio_de_la_historia_termino == true) {
        gm.pantalla_actual = PantallaDelJuego::JUGANDO;
        gm.cambiar_musica(1);
        gm.reloj.restart();
        gm.mapa.generar_clima(gm.efectos_visuales);
        return;
    }

    ///=============================================================///
    ///   EL JUGADOR APRIETA ESC PARA SALTEAR LA HISTORIA
    ///=============================================================///
    bool el_jugador_aprieto_escape = false;
    if (evento.key.code == sf::Keyboard::Escape) el_jugador_aprieto_escape = true;

    if (el_jugador_aprieto_escape == true) {
        gm.musica_de_la_historia.stop();
        gm.el_audio_de_la_historia_termino = true;
        if (gm.cantidad_de_imagenes_de_la_historia > 0) {
            gm.imagen_actual_de_la_historia = gm.cantidad_de_imagenes_de_la_historia - 1;
        }
    }
}

void actualizar_pantalla_historia(GameManager& gm, float dt) {

    if (gm.el_audio_de_la_historia_termino == true) {
        return;
    }

    if (gm.cantidad_de_imagenes_de_la_historia == 0) {
        return;
    }

    if (gm.la_musica_de_la_historia_se_cargo == true) {

        float posicion_actual = gm.musica_de_la_historia.getPlayingOffset().asSeconds();

        // Solo marcamos el audio como terminado si ya estuvo reproduciendose
        // (posicion > 1 segundo). Evita un falso Stopped en el primer frame
        // antes de que SFML termine de arrancar el stream de audio.
        if (gm.musica_de_la_historia.getStatus() == sf::Music::Stopped) {
            if (posicion_actual > 1.0f) {
                gm.el_audio_de_la_historia_termino = true;
                gm.imagen_actual_de_la_historia = gm.cantidad_de_imagenes_de_la_historia - 1;
            }
            return;
        }

        // TIEMPOS DE CAMBIO DE IMAGEN - Cada numero es el segundo del audio
        // en que aparece esa imagen. Editá estos valores a tu gusto.
        // El audio dura 177 segundos en total.
        const float TIEMPOS_DE_CAMBIO[20] = {
             0.0f,  // imagen  1: historia_01.png  (portada)
             1.6f,  // imagen  2: historia_02.png  (el pacto)
            15.5f,  // imagen  3: historia_03.png  (el juramento)
            31.7f,  // imagen  4: historia_04.png  (la reliquia)
            43.0f,  // imagen  5: historia_05.png  (la leyenda)
            50.0f,  // imagen  6: historia_06.png  (la travesia)
            61.0f,  // imagen  7: historia_07.png  (el reclutamiento)  → 1:01
            70.0f,  // imagen  8: historia_08.png  (la llegada)         → 1:10
            80.0f,  // imagen  9: historia_09.png  (la intencion)       → 1:20
            83.0f,  // imagen 10: historia_10.png  (la excavacion)      → 1:23
            89.0f,  // imagen 11: historia_11.png  (el descenso)        → 1:29
            93.0f,  // imagen 12: historia_12.png  (el derrumbe)        → 1:33
            95.0f,  // imagen 13: historia_13.png  (en la oscuridad)    → 1:35
           101.3f,  // imagen 14: historia_14.png  (el hallazgo)        → 1:41.3
           114.1f,  // imagen 15: historia_15.png  (la traicion)        → 1:54.1
           136.0f,  // imagen 16: historia_16.png  (el ultimo hechizo)  → 2:15
           147.6f,  // imagen 17: historia_17.png  (el exilio)          → 2:28
           155.0f,  // imagen 18: historia_18.png  (la recuperacion)    → 2:35
           165.0f,  // imagen 19: historia_19.png  (tres sin nombre)    → 2:50
           173.0f,  // imagen 20: historia_20.png  (la promesa)         → 2:53
        };

        int nueva_imagen = 0;
        for (int i = 0; i < gm.cantidad_de_imagenes_de_la_historia; i++) {
            if (posicion_actual >= TIEMPOS_DE_CAMBIO[i]) {
                nueva_imagen = i;
            }
        }
        gm.imagen_actual_de_la_historia = nueva_imagen;
    }
    else {
        // Sin audio: cada imagen dura 4 segundos y luego avanza
        const float SEGUNDOS_POR_IMAGEN = 4.f;
        gm.tiempo_acumulado_en_la_historia += dt;

        if (gm.tiempo_acumulado_en_la_historia >= SEGUNDOS_POR_IMAGEN) {
            gm.tiempo_acumulado_en_la_historia = 0.f;
            gm.imagen_actual_de_la_historia++;

            if (gm.imagen_actual_de_la_historia >= gm.cantidad_de_imagenes_de_la_historia) {
                gm.imagen_actual_de_la_historia = gm.cantidad_de_imagenes_de_la_historia - 1;
                gm.el_audio_de_la_historia_termino = true;
            }
        }
    }
}

void renderizar_pantalla_historia(GameManager& gm) {

    gm.ventana.setView(gm.ventana.getDefaultView());
    gm.ventana.clear(sf::Color::Black);

    if (gm.cantidad_de_imagenes_de_la_historia > 0) {
        int indice = gm.imagen_actual_de_la_historia;
        // sf::Sprite: objeto de SFML que dibuja una textura en pantalla
        gm.sprite_de_la_historia.setTexture(gm.texturas_de_la_historia[indice]);

        float escala_x = 1280.f / (float)gm.texturas_de_la_historia[indice].getSize().x;
        float escala_y = 720.f / (float)gm.texturas_de_la_historia[indice].getSize().y;
        gm.sprite_de_la_historia.setScale(escala_x, escala_y);
        gm.sprite_de_la_historia.setPosition(0.f, 0.f);
        gm.ventana.draw(gm.sprite_de_la_historia);
    }

    ///=============================================================///
    ///   CARTEL "ESC PARA OMITIR" MIENTRAS LA HISTORIA TRANSCURRE
    ///=============================================================///
    if (gm.el_audio_de_la_historia_termino == false) {
        sf::Text texto_omitir;
        texto_omitir.setFont(gm.fuente_de_textos);
        texto_omitir.setCharacterSize(16);
        texto_omitir.setFillColor(sf::Color(200, 200, 200));
        texto_omitir.setString("Presione ESC para omitir");
        texto_omitir.setPosition(20.f, 20.f);
        gm.ventana.draw(texto_omitir);
    }

    ///=============================================================///
    ///   CARTEL "ENTER PARA CONTINUAR" CUANDO LA HISTORIA TERMINO
    ///=============================================================///
    if (gm.el_audio_de_la_historia_termino == true) {
        sf::Text texto_continuar;
        texto_continuar.setFont(gm.fuente_de_textos);
        texto_continuar.setCharacterSize(18);
        texto_continuar.setFillColor(sf::Color::White);
        texto_continuar.setString("Presione Enter para continuar...");
        texto_continuar.setPosition(430.f, 680.f);
        gm.ventana.draw(texto_continuar);
    }
}

///=================================================================///
///=================================================================///
///   PANTALLA 4: LORE - La historia antes de empezar a jugar
///=================================================================///
///=================================================================///
void procesar_eventos_pantalla_lore(GameManager& gm, sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Enter) {
        gm.pantalla_actual = PantallaDelJuego::JUGANDO;
        gm.cambiar_musica(1);
        gm.reloj.restart();
        gm.mapa.generar_clima(gm.efectos_visuales);
    }
}

void actualizar_pantalla_lore(GameManager& gm, float dt) {
    // La pantalla de lore no necesita actualizar nada por frame
}

void renderizar_pantalla_lore(GameManager& gm) {
    gm.ventana.setView(gm.ventana.getDefaultView());
    gm.ventana.clear(sf::Color(10, 14, 20));
    gm.ventana.draw(gm.texto_de_lore);
}

///=================================================================///
///=================================================================///
///   PANTALLA 5: JUGANDO - El gameplay en si
///=================================================================///
///=================================================================///
void procesar_eventos_pantalla_jugando(GameManager& gm, sf::Event& evento) {

    if (gm.hud_inventario.getEsta_abierto() == true && evento.type == sf::Event::KeyPressed) {

        int indice_actual = gm.personaje.getMochila().getIndice_del_slot_seleccionado();
        int cantidad_de_slots = gm.personaje.getMochila().getCantidad_de_items_guardados();

        if (evento.key.code == sf::Keyboard::Right || evento.key.code == sf::Keyboard::D) {
            if (indice_actual < cantidad_de_slots - 1) {
                gm.personaje.getMochila().setIndice_del_slot_seleccionado(indice_actual + 1);
            }
        }
        if (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::A) {
            if (indice_actual > 0) {
                gm.personaje.getMochila().setIndice_del_slot_seleccionado(indice_actual - 1);
            }
        }
        if (evento.key.code == sf::Keyboard::E) {
            gm.personaje.getMochila().usar_item(indice_actual, gm.personaje);
        }
        if (evento.key.code == sf::Keyboard::Escape) {
            gm.hud_inventario.alternar_abierto_y_cerrado();
        }
        return;
    }

    if (evento.type == sf::Event::KeyPressed) {

        if (gm.tienda.getLa_tienda_esta_abierta() == true) {
            if (evento.key.code == sf::Keyboard::Right || evento.key.code == sf::Keyboard::D) gm.tienda.seleccionar_item_siguiente();
            if (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::A) gm.tienda.seleccionar_item_anterior();
            if (evento.key.code == sf::Keyboard::Up || evento.key.code == sf::Keyboard::W) gm.tienda.aumentar_cantidad_a_comprar();
            if (evento.key.code == sf::Keyboard::Down || evento.key.code == sf::Keyboard::S) gm.tienda.disminuir_cantidad_a_comprar();
            if (evento.key.code == sf::Keyboard::E) gm.tienda.intentar_comprar_item_seleccionado(gm.personaje);
            if (evento.key.code == sf::Keyboard::Escape) gm.tienda.cerrar_tienda();
            return;
        }

        if (gm.tienda.getEl_jugador_esta_cerca_de_la_tienda() == true && evento.key.code == sf::Keyboard::E) {
            gm.tienda.abrir_tienda();
            return;
        }
    }

    gm.input.procesar_un_evento_del_teclado_o_mouse(evento);
    gm.camara.procesar_zoom(evento);

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F3) {
        gm.debug.activar_o_desactivar_debug();
    }

    gm.debug.procesar_eventos(evento, gm.ventana, gm.hud_inventario, gm.personaje, gm.golem);

    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
        gm.debug.procesar_clic_en_el_mapa(sf::Mouse::getPosition(gm.ventana), gm.camara.getVista(), gm.ventana);
    }
}

void actualizar_pantalla_jugando(GameManager& gm, float dt) {

    gm.tienda.actualizar_tienda(gm.personaje.getPosicion());

    bool la_tienda_esta_abierta = gm.tienda.getLa_tienda_esta_abierta();
    bool el_inventario_esta_abierto = gm.hud_inventario.getEsta_abierto();

    if (la_tienda_esta_abierta == true || el_inventario_esta_abierto == true) {
        return;
    }

    gm.input.actualizar_las_teclas_apretadas_en_este_momento(gm.ventana);
    gm.camara.seguir_al_objetivo(gm.personaje.getPosicion(), dt);
    gm.ventana.setView(gm.camara.getVista());

    if (gm.input.getEl_jugador_quiere_abrir_el_inventario() == true) {
        gm.hud_inventario.alternar_abierto_y_cerrado();
        if (gm.hud_inventario.getEsta_abierto() == true) {
            gm.personaje.getMochila().setIndice_del_slot_seleccionado(0);
        }
    }

    if (gm.input.getEl_jugador_quiere_atacar() == true) {
        gm.hud_inventario.detectar_clic_en_un_casillero(gm.input.getPosicion_del_mouse(), gm.personaje.getMochila(), gm.ventana);
    }

    if (gm.input.getEl_jugador_quiere_tirar_item() == true) {
        Item item_a_tirar = gm.personaje.getMochila().extraer_item_seleccionado();
        if (item_a_tirar.getEsta_vacio() == false) {
            item_a_tirar.reposicionar_en_el_mundo(gm.personaje.getPosicion());
            gm.administrador_de_objetos.recibir_item_soltado(item_a_tirar);
        }
    }

    gm.personaje.procesar_movimiento_y_entrada_del_jugador(gm.input, gm.mapa, gm.ventana, gm.hud_inventario.getEsta_abierto(), dt);
    gm.personaje.actualizar_animacion_y_hechizo(dt, gm.efectos_visuales);

    if (gm.golem.getEsta_viva() == true) {
        gm.golem.setPosicion_objetivo(gm.personaje.calcular_centro_fisico());
        gm.golem.actualizar(dt, gm.mapa, gm.personaje);
        gm.resolver_colision_entre_personaje_y_golem();
    }

    actualizar_horda_y_spawns_de_marcianitos(gm, dt);
    resolver_combate_de_magia_contra_enemigos(gm);

    gm.efectos_visuales.actualizar(dt);
    gm.mascota.setPosicion_objetivo(gm.personaje.calcular_centro_fisico());
    gm.mascota.actualizar(dt);
    gm.niebla.actualizar(dt);
    gm.administrador_de_objetos.chequear_interacciones(gm.personaje, gm.input);
    gm.administrador_de_objetos.chequear_recoger_oro(gm.personaje, gm.input);

    gm.debug.actualizar(gm.hud_inventario, gm.personaje, gm.golem);
}

void renderizar_pantalla_jugando(GameManager& gm) {

    gm.ventana.setView(gm.camara.getVista());
    gm.mapa.dibujar_mapa(gm.ventana);
    gm.efectos_visuales.dibujar(gm.ventana);
    gm.administrador_de_objetos.dibujar_items(gm.ventana);
    gm.administrador_de_objetos.dibujar_oro(gm.ventana);

    gm.tienda.dibujar_sprite_en_el_mapa(gm.ventana, gm.debug.getEsta_activo());

    for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) {
        if (gm.marcianitos[i].getEsta_viva() == true) {
            gm.marcianitos[i].dibujar(gm.ventana);
        }
    }

    gm.personaje.dibujar(gm.ventana);
    gm.mascota.dibujar(gm.ventana);

    if (gm.golem.getEsta_viva() == true) {
        gm.golem.dibujar(gm.ventana);
    }

    gm.niebla.dibujar(gm.ventana, gm.camara.getVista());

    if (gm.debug.getEsta_activo() == true) {
        gm.mapa.dibujar_debug(gm.ventana);
        gm.debug.dibujar_caja_de_colision(gm.ventana, gm.personaje.calcular_caja_de_colision(), sf::Color::Green);
        gm.debug.dibujar_grilla_del_mapa(gm.ventana);

        for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) {
            if (gm.marcianitos[i].getEsta_viva() == true) {
                gm.debug.dibujar_caja_de_colision(gm.ventana, gm.marcianitos[i].calcular_caja_de_colision(), sf::Color::Red);
            }
        }

        if (gm.golem.getEsta_viva() == true) {
            gm.golem.dibujar_camino_calculado(gm.ventana);
            gm.debug.dibujar_caja_de_colision(gm.ventana, gm.golem.calcular_caja_de_colision(), sf::Color::Magenta);
        }
    }

    gm.ventana.setView(gm.ventana.getDefaultView());
    gm.hud_inventario.dibujar(gm.ventana, gm.personaje.getMochila());

    sf::Text texto_del_oro;
    texto_del_oro.setFont(gm.fuente_de_textos);
    texto_del_oro.setCharacterSize(16);
    texto_del_oro.setFillColor(sf::Color::Yellow);
    texto_del_oro.setString("Oro:  " + std::to_string(gm.personaje.getOro()));
    texto_del_oro.setPosition(1100.f, 10.f);
    gm.ventana.draw(texto_del_oro);

    sf::Text texto_de_la_vida;
    texto_de_la_vida.setFont(gm.fuente_de_textos);
    texto_de_la_vida.setCharacterSize(16);
    texto_de_la_vida.setFillColor(sf::Color::Red);
    texto_de_la_vida.setString("Vida: " + std::to_string(gm.personaje.getVida_actual()) + "/" + std::to_string(gm.personaje.getVida_maxima()));
    texto_de_la_vida.setPosition(1100.f, 30.f);
    gm.ventana.draw(texto_de_la_vida);

    sf::Text texto_del_mana;
    texto_del_mana.setFont(gm.fuente_de_textos);
    texto_del_mana.setCharacterSize(16);
    texto_del_mana.setFillColor(sf::Color::Cyan);
    texto_del_mana.setString("Mana: " + std::to_string(gm.personaje.getMana_actual()) + "/" + std::to_string(gm.personaje.getMana_maxima()));
    texto_del_mana.setPosition(1100.f, 50.f);
    gm.ventana.draw(texto_del_mana);

    gm.tienda.dibujar_interfaz_de_compra(gm.ventana, gm.fuente_de_textos);

    if (gm.debug.getEsta_activo() == true && gm.debug.getObjetivo_actual() == ObjetivoDebug::EXTRACTOR) {
        gm.debug.dibujar_extractor(gm.ventana, gm.fabrica_de_items.getTextura_maestra());
    }
}

///=================================================================///
///   HORDA Y SPAWNS - OBJECT POOL EN ACCION
///=================================================================///
void actualizar_horda_y_spawns_de_marcianitos(GameManager& gm, float dt) {

    sf::Vector2f centro_del_jugador = gm.personaje.calcular_centro_fisico();

    if (gm.golem.getEsta_viva() == true) {
        gm.reloj_de_spawn_de_marcianitos += dt;

        if (gm.reloj_de_spawn_de_marcianitos >= gm.intervalo_de_spawn_de_marcianitos) {

            sf::Vector2f posicion_de_spawn(1344.f, 1408.f);
            gm.efectos_visuales.agregarPortal(posicion_de_spawn);

            for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) {
                if (gm.marcianitos[i].getEsta_viva() == false) {
                    gm.marcianitos[i].activar_en_la_posicion(posicion_de_spawn, "assets/marciano.png");
                    break;
                }
            }

            gm.reloj_de_spawn_de_marcianitos = 0.f;
        }
    }

    for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) {
        if (gm.marcianitos[i].getEsta_viva() == false) continue;

        gm.marcianitos[i].setPosicion_objetivo(centro_del_jugador);
        gm.marcianitos[i].actualizar(dt, gm.mapa);

        if (gm.marcianitos[i].calcular_caja_de_colision().intersects(gm.personaje.calcular_caja_de_colision()) == true) {
            if (gm.marcianitos[i].puede_atacar_de_nuevo() == true) {

                gm.personaje.recibir_dano(gm.marcianitos[i].getDano_que_hace());

                sf::Vector2f direccion_del_empujon = gm.personaje.getPosicion() - gm.marcianitos[i].getPosicion();
                float largo_del_empujon = std::hypot(direccion_del_empujon.x, direccion_del_empujon.y);
                if (largo_del_empujon != 0.f) {
                    direccion_del_empujon /= largo_del_empujon;
                }
                gm.personaje.empujar_por_colision(direccion_del_empujon * 25.f, gm.mapa);

                std::cout << "GOLPE Y EMPUJE!" << std::endl;
            }
        }
    }
}

///=================================================================///
///   COMBATE DE MAGIA
///=================================================================///
void resolver_combate_de_magia_contra_enemigos(GameManager& gm) {

    if (gm.personaje.getHechizo_de_bola_de_fuego().getEsta_activa() == false) return;

    bool hubo_impacto = false;

    for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) {
        if (gm.marcianitos[i].getEsta_viva() == false) continue;

        if (gm.personaje.getHechizo_de_bola_de_fuego().calcular_caja_de_colision().intersects(gm.marcianitos[i].calcular_caja_de_colision()) == true) {

            gm.marcianitos[i].recibir_dano(gm.personaje.getDano_que_hace());
            hubo_impacto = true;

            if (gm.marcianitos[i].getEsta_muerta() == true) {
                gm.administrador_de_objetos.soltar_oro_en_el_piso(gm.marcianitos[i].getPosicion(), 15);
            }
            break;
        }
    }

    if (hubo_impacto == false && gm.golem.getEsta_viva() == true) {
        if (gm.personaje.getHechizo_de_bola_de_fuego().calcular_caja_de_colision().intersects(gm.golem.calcular_caja_de_colision()) == true) {

            gm.golem.recibir_dano(gm.personaje.getDano_que_hace());
            hubo_impacto = true;

            if (gm.golem.getEsta_muerta() == true) {
                std::cout << "EL GOLEM HA SIDO DERROTADO!" << std::endl;
            }
        }
    }

    if (hubo_impacto == true) {
        gm.personaje.getHechizo_de_bola_de_fuego().desactivar();
    }
}

///=================================================================///
///=================================================================///
///   PANTALLA 6: CREADOR DE ITEMS
///=================================================================///
///=================================================================///
void procesar_eventos_pantalla_creador_items(GameManager& gm, sf::Event& evento) {

    gm.ui_creador_items.procesar_eventos(evento, gm.fabrica_de_items);

    if (evento.type == sf::Event::KeyPressed && (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::Right)) {
        gm.ui_creador_items.actualizar_sprite_de_vista_previa(gm.fabrica_de_items.getTextura_maestra());
    }

    if (gm.ui_creador_items.getSe_solicito_guardar() == true) {
        RegistroDeItem registro_nuevo = gm.ui_creador_items.generar_registro_con_los_datos_ingresados();

        if (gm.fabrica_de_items.guardar_registro(registro_nuevo) == true) {
            std::cout << "ITEM GUARDADO: " << registro_nuevo.nombre << std::endl;
        }
        else {
            std::cout << "ERROR AL GUARDAR EL ITEM." << std::endl;
        }
        gm.ui_creador_items.confirmar_guardado();
    }

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        gm.pantalla_actual = PantallaDelJuego::MENU;
    }
}

void actualizar_pantalla_creador_items(GameManager& gm, float dt) {
    // No necesita actualizar nada por frame
}

void renderizar_pantalla_creador_items(GameManager& gm) {
    gm.ventana.setView(gm.ventana.getDefaultView());
    gm.ui_creador_items.dibujar(gm.ventana);
}

///=================================================================///
///=================================================================///
///   PANTALLA 7: CREDITOS
///=================================================================///
///=================================================================///
void procesar_eventos_pantalla_creditos(GameManager& gm, sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        gm.pantalla_actual = PantallaDelJuego::MENU;
    }
}

void actualizar_pantalla_creditos(GameManager& gm, float dt) {
    // No necesita actualizar nada por frame
}

void renderizar_pantalla_creditos(GameManager& gm) {
    gm.ventana.setView(gm.ventana.getDefaultView());
    gm.ventana.draw(gm.texto_de_creditos);
}