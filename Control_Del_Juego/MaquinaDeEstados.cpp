#include "MaquinaDeEstados.h" // Incluye el header de esta máquina de estados
#include "GameManager.h" // Incluye el header del manager general del juego
#include <iostream> // Para mostrar mensajes en consola
#include <cmath> // Para funciones matemáticas como hypot
#include <string> // Para usar cadenas de texto

///=================================================================///
///   DECLARACIONES ANTICIPADAS - PANTALLA 1: INTRO
///=================================================================///
// #1
void procesar_eventos_pantalla_intro(GameManager& gm, sf::Event& evento);
// #2
void actualizar_pantalla_intro(GameManager& gm, float dt);
// #3
void renderizar_pantalla_intro(GameManager& gm);

///=================================================================///
///   DECLARACIONES ANTICIPADAS - PANTALLA 2: MENU
///=================================================================///
// #4
void procesar_eventos_pantalla_menu(GameManager& gm, sf::Event& evento);
// #5
void actualizar_pantalla_menu(GameManager& gm, float dt);
// #6
void renderizar_pantalla_menu(GameManager& gm);

///=================================================================///
///   DECLARACIONES ANTICIPADAS - PANTALLA 3: HISTORIA
///=================================================================///
// #7
void procesar_eventos_pantalla_historia(GameManager& gm, sf::Event& evento);
// #8
void actualizar_pantalla_historia(GameManager& gm, float dt);
// #9
void renderizar_pantalla_historia(GameManager& gm);

///=================================================================///
///   DECLARACIONES ANTICIPADAS - PANTALLA 4: JUGANDO
///=================================================================///
// #10
void procesar_eventos_pantalla_jugando(GameManager& gm, sf::Event& evento);
// #11
void actualizar_pantalla_jugando(GameManager& gm, float dt);
// #12
void renderizar_pantalla_jugando(GameManager& gm);
// #13
void actualizar_horda_y_spawns_de_marcianitos(GameManager& gm, float dt);
// #14
void resolver_combate_de_magia_contra_enemigos(GameManager& gm);

///=================================================================///
///   DECLARACIONES ANTICIPADAS - PANTALLA 5: LOGROS
///=================================================================///
// #15
void procesar_eventos_pantalla_logros(GameManager& gm, sf::Event& evento);
// #16
void actualizar_pantalla_logros(GameManager& gm, float dt);
// #17
void renderizar_pantalla_logros(GameManager& gm);

///=================================================================///
///   DECLARACIONES ANTICIPADAS - PANTALLA 6: CREDITOS
///=================================================================///
// #18
void procesar_eventos_pantalla_creditos(GameManager& gm, sf::Event& evento);
// #19
void actualizar_pantalla_creditos(GameManager& gm, float dt);
// #20
void renderizar_pantalla_creditos(GameManager& gm);

///=================================================================///
///=================================================================///
///   LAS 3 FUNCIONES PRINCIPALES - El "semaforo": reparten el
///   trabajo segun la pantalla actual
///=================================================================///
///=================================================================///
void procesar_eventos_segun_la_pantalla(GameManager& gm, sf::Event& evento) {
    if (gm.pantalla_actual == PantallaDelJuego::INTRO) procesar_eventos_pantalla_intro(gm, evento); // Si estamos en INTRO, procesa sus eventos
    else if (gm.pantalla_actual == PantallaDelJuego::MENU) procesar_eventos_pantalla_menu(gm, evento); // Si estamos en MENU, procesa sus eventos
    else if (gm.pantalla_actual == PantallaDelJuego::HISTORIA) procesar_eventos_pantalla_historia(gm, evento);// Si estamos en HISTORIA, procesa sus eventos
    else if (gm.pantalla_actual == PantallaDelJuego::JUGANDO) procesar_eventos_pantalla_jugando(gm, evento); // Si estamos JUGANDO, procesa sus eventos
    else if (gm.pantalla_actual == PantallaDelJuego::CREDITOS) procesar_eventos_pantalla_creditos(gm, evento);// Si estamos en CREDITOS, procesa sus eventos
    else if (gm.pantalla_actual == PantallaDelJuego::LOGROS) procesar_eventos_pantalla_logros(gm, evento); // Si estamos en LOGROS, procesa sus eventos
}

void actualizar_segun_la_pantalla(GameManager& gm, float tiempo_transcurrido) {
    if (gm.pantalla_actual == PantallaDelJuego::INTRO) actualizar_pantalla_intro(gm, tiempo_transcurrido); // Actualiza la lógica de INTRO
    else if (gm.pantalla_actual == PantallaDelJuego::MENU) actualizar_pantalla_menu(gm, tiempo_transcurrido); // Actualiza la lógica del MENU
    else if (gm.pantalla_actual == PantallaDelJuego::HISTORIA) actualizar_pantalla_historia(gm, tiempo_transcurrido); // Actualiza la lógica de HISTORIA
    else if (gm.pantalla_actual == PantallaDelJuego::JUGANDO) actualizar_pantalla_jugando(gm, tiempo_transcurrido); // Actualiza la lógica del JUEGO
    else if (gm.pantalla_actual == PantallaDelJuego::CREDITOS) actualizar_pantalla_creditos(gm, tiempo_transcurrido); // Actualiza la lógica de CREDITOS
    else if (gm.pantalla_actual == PantallaDelJuego::LOGROS) actualizar_pantalla_logros(gm, tiempo_transcurrido); // Actualiza la lógica de LOGROS
}

void renderizar_segun_la_pantalla(GameManager& gm) {
    if (gm.pantalla_actual == PantallaDelJuego::INTRO) renderizar_pantalla_intro(gm); // Dibuja la pantalla de INTRO
    else if (gm.pantalla_actual == PantallaDelJuego::MENU) renderizar_pantalla_menu(gm); // Dibuja el MENU
    else if (gm.pantalla_actual == PantallaDelJuego::HISTORIA) renderizar_pantalla_historia(gm); // Dibuja la HISTORIA
    else if (gm.pantalla_actual == PantallaDelJuego::JUGANDO) renderizar_pantalla_jugando(gm); // Dibuja el JUEGO
    else if (gm.pantalla_actual == PantallaDelJuego::CREDITOS) renderizar_pantalla_creditos(gm); // Dibuja los CREDITOS
    else if (gm.pantalla_actual == PantallaDelJuego::LOGROS) renderizar_pantalla_logros(gm); // Dibuja los LOGROS
}

///=================================================================///
///=================================================================///
///   PANTALLA 1: INTRO - Animacion de apertura del juego
///=================================================================///
///=================================================================///
// #1
void procesar_eventos_pantalla_intro(GameManager& gm, sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Enter) { // Si se presionó Enter
        gm.pantalla_actual = PantallaDelJuego::MENU; // Cambia a la pantalla del menú
    }
}

// #2
void actualizar_pantalla_intro(GameManager& gm, float dt) {

    const int CANTIDAD_DE_COLUMNAS = 6; // El spritesheet tiene 6 columnas
    const int CANTIDAD_DE_FILAS = 4; // El spritesheet tiene 4 filas
    const int CANTIDAD_TOTAL_DE_FRAMES = CANTIDAD_DE_COLUMNAS * CANTIDAD_DE_FILAS;// Total de fotogramas de la animación
    const float DURACION_DE_CADA_FRAME = 1.f / 12.f; // Cada frame dura 1/12 de segundo (12 fps)

    gm.tiempo_acumulado_del_frame_de_intro += dt; // Acumula el tiempo transcurrido

    if (gm.tiempo_acumulado_del_frame_de_intro >= DURACION_DE_CADA_FRAME) { // Si pasó suficiente tiempo
        gm.tiempo_acumulado_del_frame_de_intro = 0.f; // Reinicia el contador de tiempo
        gm.frame_actual_de_la_intro = gm.frame_actual_de_la_intro + 1; // Avanza al siguiente fotograma

        if (gm.frame_actual_de_la_intro >= CANTIDAD_TOTAL_DE_FRAMES) { // Si llegó al último frame
            gm.frame_actual_de_la_intro = 0; // Vuelve al primer fotograma (loop)
        }
    }
}

// #3
void renderizar_pantalla_intro(GameManager& gm) {

    const int ANCHO_DE_CADA_FRAME = 480; // Cada fotograma mide 480 píxeles de ancho
    const int ALTO_DE_CADA_FRAME = 270; // Cada fotograma mide 270 píxeles de alto
    const int CANTIDAD_DE_COLUMNAS = 6; // El spritesheet tiene 6 columnas

    int columna_del_frame_actual = gm.frame_actual_de_la_intro % CANTIDAD_DE_COLUMNAS; // Columna del frame en el spritesheet
    int fila_del_frame_actual = gm.frame_actual_de_la_intro / CANTIDAD_DE_COLUMNAS; // Fila del frame en el spritesheet

 // Recorta la textura en el rectángulo del frame actual
    gm.sprite_de_la_intro.setTextureRect(sf::IntRect(
        columna_del_frame_actual * ANCHO_DE_CADA_FRAME, // Posición X del recorte
        fila_del_frame_actual * ALTO_DE_CADA_FRAME, // Posición Y del recorte
        ANCHO_DE_CADA_FRAME, // Ancho del recorte
        ALTO_DE_CADA_FRAME // Alto del recorte
    ));

    gm.ventana.setView(gm.ventana.getDefaultView()); // Usa la vista por defecto (sin cámara)
    gm.ventana.draw(gm.sprite_de_la_intro); // Dibuja la animación de intro

    sf::Text texto_para_continuar; // Crea un texto de instrucción
    texto_para_continuar.setFont(gm.fuente_de_textos); // Asigna la fuente del juego
    texto_para_continuar.setCharacterSize(16); // Tamaño de letra 16
    texto_para_continuar.setFillColor(sf::Color::White); // Color blanco
    texto_para_continuar.setString("Presiona ENTER para continuar"); // Texto que se muestra
    texto_para_continuar.setPosition(560.f, 660.f); // Posición en pantalla
    gm.ventana.draw(texto_para_continuar); // Dibuja el texto
}

///=================================================================///
///=================================================================///
///   PANTALLA 2: MENU
///=================================================================///
///=================================================================///
// #4
void procesar_eventos_pantalla_menu(GameManager& gm, sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed) { // Si se presionó alguna tecla
        if (evento.key.code == sf::Keyboard::Up) gm.menu.mover_seleccion_hacia_arriba(); // Flecha arriba: sube la selección
        if (evento.key.code == sf::Keyboard::Down) gm.menu.mover_seleccion_hacia_abajo(); // Flecha abajo: baja la selección

        if (evento.key.code == sf::Keyboard::Enter) { // Si se presionó Enter
            int opcion_seleccionada = gm.menu.getIndice_de_la_opcion_seleccionada(); // Obtiene la opción elegida

            if (opcion_seleccionada == 0) { // Opción 0: Jugar
                if (gm.es_la_primera_partida == true) { // Si es la primera vez que juega
                    gm.es_la_primera_partida = false; // Marca que ya no es la primera partida
                    gm.guardar_logros(); // Guarda el estado inicial de logros
                    gm.imagen_actual_de_la_historia = 0; // Empieza desde la primera imagen
                    gm.tiempo_acumulado_en_la_historia = 0.f; // Reinicia el tiempo de historia
                    gm.el_audio_de_la_historia_termino = false; // Marca que el audio no terminó
                    gm.musica_ambiente.stop(); // Detiene la música del menú
                    gm.musica_de_la_historia.setLoop(false); // La música de historia no hace loop
                    gm.musica_de_la_historia.play(); // Reproduce la música de historia
                    gm.pantalla_actual = PantallaDelJuego::HISTORIA; // Cambia a la pantalla de historia
                }
                else { // Si ya jugó antes, va directo al juego
                    gm.cambiar_musica(1); // Cambia la música al tema de juego
                    gm.reloj.restart(); // Reinicia el reloj del juego
                    gm.reloj_de_partida.restart(); // Reinicia el reloj de partida
                    gm.pantalla_actual = PantallaDelJuego::JUGANDO; // Cambia a la pantalla de juego
                }
            }
            else if (opcion_seleccionada == 1) { // Opción 1: Logros
                gm.pantalla_actual = PantallaDelJuego::LOGROS; // Cambia a la pantalla de logros
            }
            else if (opcion_seleccionada == 2) { // Opción 2: Créditos
                gm.pantalla_actual = PantallaDelJuego::CREDITOS; // Cambia a la pantalla de créditos
            }
            else if (opcion_seleccionada == 3) { // Opción 3: Salir
                gm.ventana.close(); // Cierra la ventana del juego
            }
        }
    }
}

// #5
void actualizar_pantalla_menu(GameManager& gm, float dt) {
 // El menu no necesita actualizar nada por frame
}

// #6
void renderizar_pantalla_menu(GameManager& gm) {
    gm.ventana.setView(gm.ventana.getDefaultView()); // Usa la vista por defecto
    gm.menu.dibujar(gm.ventana); // Dibuja el menú en pantalla
}

///=================================================================///
///=================================================================///
///   PANTALLA 3: HISTORIA - Imagenes con audio al iniciar la primera
///   partida. Las imagenes se sincronizan con la duracion del audio.
///   Al terminar el audio aparece el texto para continuar.
///=================================================================///
///=================================================================///
// #7
void procesar_eventos_pantalla_historia(GameManager& gm, sf::Event& evento) {
    if (evento.type != sf::Event::KeyPressed) return; // Ignora eventos que no sean teclas

 ///=============================================================///
 ///   EL JUGADOR APRIETA ENTER CUANDO LA HISTORIA TERMINO
 ///=============================================================///
    bool el_jugador_aprieto_enter = false; // Variable para detectar Enter
    if (evento.key.code == sf::Keyboard::Enter) el_jugador_aprieto_enter = true; // Marca que se presionó Enter

    if (el_jugador_aprieto_enter == true && gm.el_audio_de_la_historia_termino == true) { // Si Enter y la historia terminó
        gm.pantalla_actual = PantallaDelJuego::JUGANDO; // Cambia a la pantalla de juego
        gm.cambiar_musica(1); // Cambia la música al tema de juego
        gm.reloj.restart(); // Reinicia el reloj del juego
        gm.reloj_de_partida.restart(); // Reinicia el reloj de partida
        gm.mapa.generar_clima(gm.efectos_visuales); // Genera el clima del mapa
        return; // Sale de la función
    }

 ///=============================================================///
 ///   EL JUGADOR APRIETA ESC PARA SALTEAR LA HISTORIA
 ///=============================================================///
    bool el_jugador_aprieto_escape = false; // Variable para detectar Escape
    if (evento.key.code == sf::Keyboard::Escape) el_jugador_aprieto_escape = true; // Marca que se presionó Escape

    if (el_jugador_aprieto_escape == true) { // Si se presionó Escape
        gm.musica_de_la_historia.stop(); // Detiene la música de historia
        gm.el_audio_de_la_historia_termino = true; // Marca el audio como terminado
        if (gm.cantidad_de_imagenes_de_la_historia > 0) { // Si hay imágenes cargadas
            gm.imagen_actual_de_la_historia = gm.cantidad_de_imagenes_de_la_historia - 1; // Muestra la última imagen
        }
    }
}

// #8
void actualizar_pantalla_historia(GameManager& gm, float dt) {

    if (gm.el_audio_de_la_historia_termino == true) { // Si el audio ya terminó
        return; // No hay nada que actualizar
    }

    if (gm.cantidad_de_imagenes_de_la_historia == 0) { // Si no hay imágenes cargadas
        return; // No hay nada que mostrar
    }

    if (gm.la_musica_de_la_historia_se_cargo == true) { // Si el audio de historia está disponible

        if (gm.musica_de_la_historia.getStatus() == sf::Music::Playing) { // Si el audio está sonando
            gm.la_historia_ya_empezo_a_reproducirse = true; // Marca que la historia arrancó
        }

        if (gm.musica_de_la_historia.getStatus() == sf::Music::Stopped && gm.la_historia_ya_empezo_a_reproducirse == true) { // Si el audio paró tras haber empezado
            gm.el_audio_de_la_historia_termino = true; // Marca que el audio terminó
            gm.imagen_actual_de_la_historia = gm.cantidad_de_imagenes_de_la_historia - 1; // Queda en la última imagen
            return; // Sale de la función
        }

        float posicion_actual = gm.musica_de_la_historia.getPlayingOffset().asSeconds(); // Obtiene el segundo actual del audio

 // TIEMPOS DE CAMBIO DE IMAGEN - Cada numero es el segundo del audio
 // en que aparece esa imagen. Editá estos valores a tu gusto.
 // El audio dura 177 segundos en total.
        const float TIEMPOS_DE_CAMBIO[20] = {
             0.0f, // imagen  1: historia_01.png  (portada)
             1.6f, // imagen  2: historia_02.png  (el pacto)
            15.5f, // imagen  3: historia_03.png  (el juramento)
            31.7f, // imagen  4: historia_04.png  (la reliquia)
            43.0f, // imagen  5: historia_05.png  (la leyenda)
            50.0f, // imagen  6: historia_06.png  (la travesia)
            61.0f, // imagen  7: historia_07.png  (el reclutamiento)  → 1:01
            70.0f, // imagen  8: historia_08.png  (la llegada)         → 1:10
            80.0f, // imagen  9: historia_09.png  (la intencion)       → 1:20
            83.0f, // imagen 10: historia_10.png  (la excavacion)      → 1:23
            89.0f, // imagen 11: historia_11.png  (el descenso)        → 1:29
            93.0f, // imagen 12: historia_12.png  (el derrumbe)        → 1:33
            95.0f, // imagen 13: historia_13.png  (en la oscuridad)    → 1:35
           101.3f, // imagen 14: historia_14.png  (el hallazgo)        → 1:41.3
           114.1f, // imagen 15: historia_15.png  (la traicion)        → 1:54.1
           136.0f, // imagen 16: historia_16.png  (el ultimo hechizo)  → 2:15
           147.6f, // imagen 17: historia_17.png  (el exilio)          → 2:28
           155.0f, // imagen 18: historia_18.png  (la recuperacion)    → 2:35
           165.0f, // imagen 19: historia_19.png  (tres sin nombre)    → 2:50
           173.0f, // imagen 20: historia_20.png  (la promesa)         → 2:53
        };

        int nueva_imagen = 0; // Empieza asumiendo la primera imagen
        for (int i = 0; i < gm.cantidad_de_imagenes_de_la_historia; i++) {// Recorre todos los tiempos de cambio
            if (posicion_actual >= TIEMPOS_DE_CAMBIO[i]) { // Si el audio llegó a ese momento
                nueva_imagen = i; // Esta es la imagen que corresponde
            }
        }
        gm.imagen_actual_de_la_historia = nueva_imagen; // Actualiza la imagen que se muestra
    }
    else {
 // Sin audio: cada imagen dura 4 segundos y luego avanza
        const float SEGUNDOS_POR_IMAGEN = 4.f; // Cada imagen dura 4 segundos sin audio
        gm.tiempo_acumulado_en_la_historia += dt; // Acumula el tiempo en la imagen actual

        if (gm.tiempo_acumulado_en_la_historia >= SEGUNDOS_POR_IMAGEN) { // Si pasaron 4 segundos
            gm.tiempo_acumulado_en_la_historia = 0.f; // Reinicia el contador
            gm.imagen_actual_de_la_historia++; // Avanza a la siguiente imagen

            if (gm.imagen_actual_de_la_historia >= gm.cantidad_de_imagenes_de_la_historia) { // Si llegó al final
                gm.imagen_actual_de_la_historia = gm.cantidad_de_imagenes_de_la_historia - 1; // Queda en la última
                gm.el_audio_de_la_historia_termino = true; // Marca historia como terminada
            }
        }
    }
}

// #9
void renderizar_pantalla_historia(GameManager& gm) {

    gm.ventana.setView(gm.ventana.getDefaultView()); // Usa la vista por defecto (sin cámara)
    gm.ventana.clear(sf::Color::Black); // Limpia la pantalla con negro

    if (gm.cantidad_de_imagenes_de_la_historia > 0) { // Si hay imágenes cargadas
        int indice = gm.imagen_actual_de_la_historia; // Índice de la imagen actual
 // sf::Sprite: objeto de SFML que dibuja una textura en pantalla
        gm.sprite_de_la_historia.setTexture(gm.texturas_de_la_historia[indice]); // Asigna la textura al sprite

        float escala_x = 1280.f / (float)gm.texturas_de_la_historia[indice].getSize().x; // Escala horizontal para llenar pantalla
        float escala_y = 720.f / (float)gm.texturas_de_la_historia[indice].getSize().y; // Escala vertical para llenar pantalla
        gm.sprite_de_la_historia.setScale(escala_x, escala_y); // Aplica la escala calculada
        gm.sprite_de_la_historia.setPosition(0.f, 0.f); // Posiciona en la esquina superior izquierda
        gm.ventana.draw(gm.sprite_de_la_historia); // Dibuja la imagen de historia
    }

 ///=============================================================///
 ///   CARTEL "ESC PARA OMITIR" MIENTRAS LA HISTORIA TRANSCURRE
 ///=============================================================///
    if (gm.el_audio_de_la_historia_termino == false) { // Si la historia aún no terminó
        sf::Text texto_omitir; // Crea el texto de omitir
        texto_omitir.setFont(gm.fuente_de_textos); // Asigna la fuente del juego
        texto_omitir.setCharacterSize(16); // Tamaño de letra 16
        texto_omitir.setFillColor(sf::Color(200, 200, 200)); // Color gris claro
        texto_omitir.setString("Presione ESC para omitir"); // Texto que se muestra
        texto_omitir.setPosition(20.f, 20.f); // Posición en la esquina superior izquierda
        gm.ventana.draw(texto_omitir); // Dibuja el texto
    }

 ///=============================================================///
 ///   CARTEL "ENTER PARA CONTINUAR" CUANDO LA HISTORIA TERMINO
 ///=============================================================///
    if (gm.el_audio_de_la_historia_termino == true) { // Si la historia terminó
        sf::Text texto_continuar; // Crea el texto de continuar
        texto_continuar.setFont(gm.fuente_de_textos); // Asigna la fuente del juego
        texto_continuar.setCharacterSize(18); // Tamaño de letra 18
        texto_continuar.setFillColor(sf::Color::White); // Color blanco
        texto_continuar.setString("Presione Enter para continuar..."); // Texto que se muestra
        texto_continuar.setPosition(430.f, 680.f); // Posición en la parte inferior
        gm.ventana.draw(texto_continuar); // Dibuja el texto
    }
}

///=================================================================///
///=================================================================///
///   PANTALLA 4: JUGANDO - El gameplay en si
///=================================================================///
///=================================================================///
// #10
void procesar_eventos_pantalla_jugando(GameManager& gm, sf::Event& evento) {

    if (gm.hud_inventario.getEsta_abierto() == true && evento.type == sf::Event::KeyPressed) { // Si el inventario está abierto y se presionó una tecla

        int indice_actual = gm.personaje.getMochila().getIndice_del_slot_seleccionado(); // Obtiene el slot actualmente seleccionado
        int cantidad_de_slots = gm.personaje.getMochila().getCantidad_de_items_guardados(); // Obtiene cuántos items hay en la mochila

        if (evento.key.code == sf::Keyboard::Right || evento.key.code == sf::Keyboard::D) { // Flecha derecha o D
            if (indice_actual < cantidad_de_slots - 1) { // Si no es el último slot
                gm.personaje.getMochila().setIndice_del_slot_seleccionado(indice_actual + 1);// Selecciona el siguiente slot
            }
        }
        if (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::A) { // Flecha izquierda o A
            if (indice_actual > 0) { // Si no es el primer slot
                gm.personaje.getMochila().setIndice_del_slot_seleccionado(indice_actual - 1);// Selecciona el slot anterior
            }
        }
        if (evento.key.code == sf::Keyboard::E) { // Tecla E: usar item
            gm.personaje.getMochila().usar_item(indice_actual, gm.personaje); // Usa el item seleccionado
        }
        if (evento.key.code == sf::Keyboard::Escape) { // Tecla Escape: cierra inventario
            gm.hud_inventario.alternar_abierto_y_cerrado(); // Alterna el estado del inventario
        }
        return; // Sale sin procesar más eventos
    }

    if (evento.type == sf::Event::KeyPressed) { // Si se presionó una tecla

        if (gm.tienda.getLa_tienda_esta_abierta() == true) { // Si la tienda está abierta
            if (evento.key.code == sf::Keyboard::Right || evento.key.code == sf::Keyboard::D) gm.tienda.seleccionar_item_siguiente(); // Selecciona el item siguiente
            if (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::A) gm.tienda.seleccionar_item_anterior(); // Selecciona el item anterior
            if (evento.key.code == sf::Keyboard::Up || evento.key.code == sf::Keyboard::W) gm.tienda.aumentar_cantidad_a_comprar(); // Aumenta la cantidad a comprar
            if (evento.key.code == sf::Keyboard::Down || evento.key.code == sf::Keyboard::S) gm.tienda.disminuir_cantidad_a_comprar();// Disminuye la cantidad a comprar
            if (evento.key.code == sf::Keyboard::E) gm.tienda.intentar_comprar_item_seleccionado(gm.personaje); // Intenta comprar el item seleccionado
            if (evento.key.code == sf::Keyboard::Escape) gm.tienda.cerrar_tienda(); // Cierra la tienda
            return; // Sale sin procesar más eventos
        }

        if (gm.tienda.getEl_jugador_esta_cerca_de_la_tienda() == true && evento.key.code == sf::Keyboard::E) { // Si el jugador está cerca y presiona E
            gm.tienda.abrir_tienda(); // Abre la tienda
            return; // Sale sin procesar más eventos
        }
    }

    gm.input.procesar_un_evento_del_teclado_o_mouse(evento); // Procesa teclado y mouse para el juego
    gm.camara.procesar_zoom(evento); // Procesa el zoom de la cámara

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F3) { // Si se presionó F3
        gm.debug.activar_o_desactivar_debug(); // Activa o desactiva el modo debug
    }

    gm.debug.procesar_eventos(evento, gm.ventana, gm.hud_inventario, gm.personaje, gm.golem); // Procesa eventos del panel debug

    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) { // Si se hizo clic izquierdo
        gm.debug.procesar_clic_en_el_mapa(sf::Mouse::getPosition(gm.ventana), gm.camara.getVista(), gm.ventana); // Registra el clic en el mapa (debug)
    }
}

// #11
void actualizar_pantalla_jugando(GameManager& gm, float dt) {

    gm.tienda.actualizar_tienda(gm.personaje.getPosicion()); // Actualiza la tienda con la posición del jugador

    bool la_tienda_esta_abierta = gm.tienda.getLa_tienda_esta_abierta(); // Chequea si la tienda está abierta
    bool el_inventario_esta_abierto = gm.hud_inventario.getEsta_abierto(); // Chequea si el inventario está abierto

    if (la_tienda_esta_abierta == true || el_inventario_esta_abierto == true) { // Si alguno está abierto
        return; // El juego queda pausado
    }

    gm.input.actualizar_las_teclas_apretadas_en_este_momento(gm.ventana); // Lee qué teclas están apretadas ahora
    gm.camara.seguir_al_objetivo(gm.personaje.getPosicion(), dt); // La cámara sigue al personaje
    gm.ventana.setView(gm.camara.getVista()); // Aplica la vista de la cámara a la ventana

    if (gm.input.getEl_jugador_quiere_abrir_el_inventario() == true) { // Si el jugador quiere abrir el inventario
        gm.hud_inventario.alternar_abierto_y_cerrado(); // Alterna el estado del inventario
        if (gm.hud_inventario.getEsta_abierto() == true) { // Si quedó abierto
            gm.personaje.getMochila().setIndice_del_slot_seleccionado(0); // Selecciona el primer slot
        }
    }

    if (gm.input.getEl_jugador_quiere_atacar() == true) { // Si el jugador hizo clic para atacar
        gm.hud_inventario.detectar_clic_en_un_casillero(gm.input.getPosicion_del_mouse(), gm.personaje.getMochila(), gm.ventana); // Detecta si hizo clic en el inventario
    }

    if (gm.input.getEl_jugador_quiere_tirar_item() == true) { // Si el jugador quiere tirar un item
        Item item_a_tirar = gm.personaje.getMochila().extraer_item_seleccionado();// Extrae el item seleccionado de la mochila
        if (item_a_tirar.getEsta_vacio() == false) { // Si el slot no estaba vacío
            item_a_tirar.reposicionar_en_el_mundo(gm.personaje.getPosicion()); // Ubica el item en el mundo
            gm.administrador_de_objetos.recibir_item_soltado(item_a_tirar); // Lo registra como objeto suelto en el mapa
        }
    }

    gm.personaje.procesar_movimiento_y_entrada_del_jugador(gm.input, gm.mapa, gm.ventana, gm.hud_inventario.getEsta_abierto(), dt); // Mueve al personaje según el input
    gm.personaje.actualizar_animacion_y_hechizo(dt, gm.efectos_visuales); // Actualiza la animación y el hechizo activo

    if (gm.golem.getEsta_viva() == true) { // Si el golem está vivo
        gm.golem.setPosicion_objetivo(gm.personaje.calcular_centro_fisico()); // El golem apunta al centro del personaje
        gm.golem.actualizar(dt, gm.mapa, gm.personaje); // Actualiza el movimiento y lógica del golem
        gm.resolver_colision_entre_personaje_y_golem(); // Resuelve si se están tocando
    }

    actualizar_horda_y_spawns_de_marcianitos(gm, dt); // Genera y actualiza los enemigos pequeños
    resolver_combate_de_magia_contra_enemigos(gm); // Chequea si el hechizo golpeó a algún enemigo

    gm.efectos_visuales.actualizar(dt); // Actualiza las partículas y efectos visuales
    gm.mascota.setPosicion_objetivo(gm.personaje.calcular_centro_fisico()); // La mascota sigue al personaje
    gm.mascota.actualizar(dt); // Actualiza el movimiento de la mascota
    gm.niebla.actualizar(dt); // Actualiza el efecto de niebla
    gm.administrador_de_objetos.chequear_interacciones(gm.personaje, gm.input);// Chequea si el jugador interactúa con objetos
    gm.administrador_de_objetos.chequear_recoger_oro(gm.personaje, gm.input); // Chequea si el jugador recoge oro

    gm.debug.actualizar(gm.hud_inventario, gm.personaje, gm.golem); // Actualiza el panel de debug

    if (gm.portal_de_victoria_activo == true) { // Si el portal de victoria está activo
        gm.timer_respawn_del_portal_de_victoria += dt; // Acumula tiempo para reanimar el portal
        if (gm.timer_respawn_del_portal_de_victoria >= 0.9f) { // Cada 0.9 segundos
            gm.timer_respawn_del_portal_de_victoria = 0.f; // Reinicia el timer
            gm.efectos_visuales.agregarPortal(gm.posicion_del_portal_de_victoria, 1.0f, 6, true); // Genera efecto visual del portal
        }

        sf::FloatRect zona_del_portal( // Define el área de colisión del portal
            gm.posicion_del_portal_de_victoria.x - 40.f, // Borde izquierdo
            gm.posicion_del_portal_de_victoria.y - 40.f, // Borde superior
            80.f, 80.f // Ancho y alto del área
        );
        if (zona_del_portal.intersects(gm.personaje.calcular_caja_de_colision()) == true) { // Si el jugador entró al portal
            gm.partida_ganada = true; // Marca la partida como ganada
            gm.tiempo_final_de_la_partida = gm.reloj_de_partida.getElapsedTime().asSeconds();// Guarda el tiempo final

            gm.logros[0].desbloqueado = true; // Logro: completar la partida
            if (gm.enemigos_eliminados >= 10) gm.logros[1].desbloqueado = true; // Logro: eliminar 10 enemigos
            if (gm.enemigos_eliminados >= 20) gm.logros[2].desbloqueado = true; // Logro: eliminar 20 enemigos
            if (gm.tiempo_final_de_la_partida <= 180.f) gm.logros[3].desbloqueado = true; // Logro: ganar en menos de 3 minutos
            if (gm.personaje.getOro() >= 50) gm.logros[4].desbloqueado = true; // Logro: tener 50 de oro

            gm.portal_de_victoria_activo = false; // Desactiva el portal
            gm.guardar_logros(); // Guarda los logros en disco
            gm.pantalla_actual = PantallaDelJuego::LOGROS; // Cambia a la pantalla de logros
        }
    }
}

// #12
void renderizar_pantalla_jugando(GameManager& gm) {

    gm.ventana.setView(gm.camara.getVista()); // Aplica la vista de la cámara
    gm.mapa.dibujar_mapa(gm.ventana); // Dibuja el mapa del mundo
    gm.efectos_visuales.dibujar(gm.ventana); // Dibuja partículas y efectos
    gm.administrador_de_objetos.dibujar_items(gm.ventana);// Dibuja los items sueltos en el suelo
    gm.administrador_de_objetos.dibujar_oro(gm.ventana); // Dibuja el oro en el suelo

    gm.tienda.dibujar_sprite_en_el_mapa(gm.ventana, gm.debug.getEsta_activo()); // Dibuja el edificio de la tienda

    gm.personaje.dibujar(gm.ventana); // Dibuja al personaje jugable
    gm.mascota.dibujar(gm.ventana); // Dibuja la mascota

    if (gm.golem.getEsta_viva() == true) { // Si el golem está vivo
        gm.golem.dibujar(gm.ventana); // Dibuja al golem
    }

    gm.niebla.dibujar(gm.ventana, gm.camara.getVista()); // Dibuja el efecto de niebla sobre el mapa

    for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) { // Recorre todos los enemigos pequeños
        if (gm.marcianitos[i].getEsta_viva() == true) { // Si este enemigo está vivo
            gm.marcianitos[i].dibujar(gm.ventana); // Lo dibuja en pantalla
        }
    }

    if (gm.debug.getEsta_activo() == true) { // Si el modo debug está activado
        gm.mapa.dibujar_debug(gm.ventana); // Dibuja info debug del mapa
        gm.debug.dibujar_caja_de_colision(gm.ventana, gm.personaje.calcular_caja_de_colision(), sf::Color::Green); // Dibuja la caja de colisión del personaje
        gm.debug.dibujar_grilla_del_mapa(gm.ventana); // Dibuja la grilla de tiles del mapa

        for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) { // Recorre todos los enemigos
            if (gm.marcianitos[i].getEsta_viva() == true) { // Si está vivo
                gm.debug.dibujar_caja_de_colision(gm.ventana, gm.marcianitos[i].calcular_caja_de_colision(), sf::Color::Red); // Dibuja su caja de colisión en rojo
            }
        }

        if (gm.golem.getEsta_viva() == true) { // Si el golem está vivo
            gm.golem.dibujar_camino_calculado(gm.ventana); // Dibuja el camino que calcula el golem
            gm.debug.dibujar_caja_de_colision(gm.ventana, gm.golem.calcular_caja_de_colision(), sf::Color::Magenta); // Dibuja su caja de colisión en magenta
        }
    }

    gm.ventana.setView(gm.ventana.getDefaultView()); // Vuelve a la vista por defecto para el HUD
    gm.hud_inventario.dibujar(gm.ventana, gm.personaje.getMochila()); // Dibuja el panel del inventario

    sf::Text texto_del_oro; // Crea el texto del contador de oro
    texto_del_oro.setFont(gm.fuente_de_textos); // Asigna la fuente del juego
    texto_del_oro.setCharacterSize(16); // Tamaño de letra 16
    texto_del_oro.setFillColor(sf::Color::Yellow); // Color amarillo
    texto_del_oro.setString("Oro:  " + std::to_string(gm.personaje.getOro())); // Texto con el valor de oro actual
    texto_del_oro.setPosition(1100.f, 10.f); // Posición en la esquina superior derecha
    gm.ventana.draw(texto_del_oro); // Dibuja el texto de oro

    sf::Text texto_de_la_vida; // Crea el texto de vida
    texto_de_la_vida.setFont(gm.fuente_de_textos); // Asigna la fuente del juego
    texto_de_la_vida.setCharacterSize(16); // Tamaño de letra 16
    texto_de_la_vida.setFillColor(sf::Color::Red); // Color rojo
    texto_de_la_vida.setString("Vida: " + std::to_string(gm.personaje.getVida_actual()) + "/" + std::to_string(gm.personaje.getVida_maxima())); // Texto con vida actual y máxima
    texto_de_la_vida.setPosition(1100.f, 30.f); // Posición debajo del oro
    gm.ventana.draw(texto_de_la_vida); // Dibuja el texto de vida

    sf::Text texto_del_mana; // Crea el texto de maná
    texto_del_mana.setFont(gm.fuente_de_textos); // Asigna la fuente del juego
    texto_del_mana.setCharacterSize(16); // Tamaño de letra 16
    texto_del_mana.setFillColor(sf::Color::Cyan); // Color cian
    texto_del_mana.setString("Mana: " + std::to_string(gm.personaje.getMana_actual()) + "/" + std::to_string(gm.personaje.getMana_maxima())); // Texto con maná actual y máximo
    texto_del_mana.setPosition(1100.f, 50.f); // Posición debajo de la vida
    gm.ventana.draw(texto_del_mana); // Dibuja el texto de maná

    gm.tienda.dibujar_interfaz_de_compra(gm.ventana, gm.fuente_de_textos); // Dibuja el panel de compra si está abierto

}

///=================================================================///
///   PANTALLA 4 (aux): HORDA Y SPAWNS - Object pool en accion
///=================================================================///
// #13
void actualizar_horda_y_spawns_de_marcianitos(GameManager& gm, float dt) {

    sf::Vector2f centro_del_jugador = gm.personaje.calcular_centro_fisico(); // Obtiene la posición central del jugador

    if (gm.golem.getEsta_viva() == true) { // Solo spawnea enemigos si el golem está vivo
        gm.reloj_de_spawn_de_marcianitos += dt; // Acumula el tiempo desde el último spawn

        if (gm.reloj_de_spawn_de_marcianitos >= gm.intervalo_de_spawn_de_marcianitos) { // Si llegó el momento de spawnear

            sf::Vector2f posicion_de_spawn(1344.f, 1408.f); // Posición fija donde aparecen los enemigos
            gm.efectos_visuales.agregarPortal(posicion_de_spawn); // Muestra efecto de portal en el spawn

            for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) { // Busca un slot libre en el pool
                if (gm.marcianitos[i].getEsta_viva() == false) { // Si este enemigo está inactivo
                    gm.marcianitos[i].activar_en_la_posicion(posicion_de_spawn, "assets/DuendeHielo.png"); // Lo activa en el punto de spawn
                    break; // Sale del loop tras activar uno
                }
            }

            gm.reloj_de_spawn_de_marcianitos = 0.f; // Reinicia el temporizador de spawn
        }
    }

    for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) { // Recorre todos los enemigos del pool
        if (gm.marcianitos[i].getEsta_viva() == false) continue; // Ignora los que están inactivos

        gm.marcianitos[i].setPosicion_objetivo(centro_del_jugador); // El enemigo apunta al jugador
        gm.marcianitos[i].actualizar(dt, gm.mapa); // Actualiza el movimiento del enemigo

        if (gm.marcianitos[i].calcular_caja_de_colision().intersects(gm.personaje.calcular_caja_de_colision()) == true) { // Si el enemigo toca al jugador
            if (gm.marcianitos[i].puede_atacar_de_nuevo() == true) { // Si el enemigo puede atacar

                gm.personaje.recibir_dano(gm.marcianitos[i].getDano_que_hace()); // El jugador recibe daño

                sf::Vector2f direccion_del_empujon = gm.personaje.getPosicion() - gm.marcianitos[i].getPosicion(); // Calcula la dirección del empuje
                float largo_del_empujon = std::hypot(direccion_del_empujon.x, direccion_del_empujon.y); // Calcula la magnitud del vector
                if (largo_del_empujon != 0.f) { // Evita dividir por cero
                    direccion_del_empujon /= largo_del_empujon; // Normaliza el vector de empuje
                }
                gm.personaje.empujar_por_colision(direccion_del_empujon * 25.f, gm.mapa); // Empuja al jugador

                std::cout << "GOLPE Y EMPUJE!" << std::endl; // Mensaje de debug en consola
            }
        }
    }
}

///=================================================================///
///   PANTALLA 4 (aux): COMBATE DE MAGIA contra enemigos
///=================================================================///
// #14
void resolver_combate_de_magia_contra_enemigos(GameManager& gm) {

    if (gm.personaje.getHechizo_de_bola_de_fuego().getEsta_activa() == false) return; // Si no hay hechizo activo, no hace nada

    bool hubo_impacto = false; // Bandera: indica si el hechizo golpeó algo

    for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) { // Recorre todos los enemigos pequeños
        if (gm.marcianitos[i].getEsta_viva() == false) continue; // Ignora los inactivos

        if (gm.personaje.getHechizo_de_bola_de_fuego().calcular_caja_de_colision().intersects(gm.marcianitos[i].calcular_caja_de_colision()) == true) { // Si el hechizo toca al enemigo

            gm.marcianitos[i].recibir_dano(gm.personaje.getDano_que_hace()); // El enemigo recibe el daño del personaje
            hubo_impacto = true; // Marca que hubo impacto

            if (gm.marcianitos[i].getEsta_muerta() == true) { // Si el enemigo murió
                gm.administrador_de_objetos.soltar_oro_en_el_piso(gm.marcianitos[i].getPosicion(), 15); // Suelta 15 de oro donde murió
                gm.enemigos_eliminados++; // Incrementa el contador de kills
            }
            break; // El hechizo solo golpea a un enemigo
        }
    }

    if (hubo_impacto == false && gm.golem.getEsta_viva() == true) { // Si no golpeó a nadie y el golem existe
        if (gm.personaje.getHechizo_de_bola_de_fuego().calcular_caja_de_colision().intersects(gm.golem.calcular_caja_de_colision()) == true) { // Si el hechizo toca al golem

            gm.golem.recibir_dano(gm.personaje.getDano_que_hace()); // El golem recibe el daño del personaje
            hubo_impacto = true; // Marca que hubo impacto

            if (gm.golem.getEsta_muerta() == true) { // Si el golem murió
                gm.portal_de_victoria_activo = true; // Activa el portal de victoria
                gm.posicion_del_portal_de_victoria = gm.golem.getPosicion(); // El portal aparece donde estaba el golem
                gm.timer_respawn_del_portal_de_victoria = 0.f; // Reinicia el timer del portal
                gm.efectos_visuales.agregarPortal(gm.posicion_del_portal_de_victoria, 1.0f, 6, true); // Muestra el efecto visual del portal
            }
        }
    }

    if (hubo_impacto == true) { // Si el hechizo golpeó algo
        gm.personaje.getHechizo_de_bola_de_fuego().desactivar(); // Desactiva el hechizo tras el impacto
    }
}

///=================================================================///
///=================================================================///
///   PANTALLA 5: LOGROS
///=================================================================///
///=================================================================///
// #15
void procesar_eventos_pantalla_logros(GameManager& gm, sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) { // Si se presionó Escape
        gm.pantalla_actual = PantallaDelJuego::MENU; // Vuelve al menú principal
    }
}

// #16
void actualizar_pantalla_logros(GameManager& gm, float dt) {
 // No necesita actualizar nada por frame
}

// #17
void renderizar_pantalla_logros(GameManager& gm) {
    gm.ventana.setView(gm.ventana.getDefaultView()); // Usa la vista por defecto

    const float CENTRO_X = 640.f; // Centro horizontal de la pantalla (1280/2)

    sf::Text titulo; // Crea el texto del título
    titulo.setFont(gm.fuente_de_textos); // Asigna la fuente del juego
    titulo.setCharacterSize(52); // Tamaño de letra grande
    titulo.setFillColor(sf::Color::White); // Color blanco
    titulo.setString("LOGROS"); // Texto del título
    titulo.setOrigin(titulo.getLocalBounds().left + titulo.getLocalBounds().width / 2.f, 0.f); // Centra el texto horizontalmente
    titulo.setPosition(CENTRO_X, 40.f); // Posición en la parte superior
    gm.ventana.draw(titulo); // Dibuja el título

    if (gm.partida_ganada == false) { // Si el jugador aún no ganó
        sf::Text mensaje; // Crea el texto de aviso
        mensaje.setFont(gm.fuente_de_textos); // Asigna la fuente
        mensaje.setCharacterSize(26); // Tamaño de letra mediano
        mensaje.setFillColor(sf::Color(200, 200, 200)); // Color gris claro
        mensaje.setString("Todavia no jugaste ninguna partida.\nDerrota al Golem para desbloquear logros."); // Mensaje informativo
        mensaje.setOrigin(mensaje.getLocalBounds().left + mensaje.getLocalBounds().width / 2.f, 0.f); // Centra el texto
        mensaje.setPosition(CENTRO_X, 280.f); // Posición en el centro de pantalla
        gm.ventana.draw(mensaje); // Dibuja el mensaje
    }
    else { // Si el jugador ya ganó al menos una partida
        int minutos = static_cast<int>(gm.tiempo_final_de_la_partida) / 60; // Calcula los minutos del tiempo final
        int segundos = static_cast<int>(gm.tiempo_final_de_la_partida) % 60; // Calcula los segundos restantes

        sf::Text estadisticas; // Crea el texto de estadísticas
        estadisticas.setFont(gm.fuente_de_textos); // Asigna la fuente
        estadisticas.setCharacterSize(22); // Tamaño de letra mediano
        estadisticas.setFillColor(sf::Color(180, 220, 180)); // Color verde suave
        estadisticas.setString(
            "Enemigos eliminados: " + std::to_string(gm.enemigos_eliminados) + "\n" + // Muestra kills
            "Oro al terminar:     " + std::to_string(gm.personaje.getOro()) + "\n" + // Muestra oro final
            "Tiempo de partida:   " + std::to_string(minutos) + "m " + std::to_string(segundos) + "s" // Muestra tiempo final
        );
        estadisticas.setOrigin(estadisticas.getLocalBounds().left + estadisticas.getLocalBounds().width / 2.f, 0.f); // Centra el bloque de texto
        estadisticas.setPosition(CENTRO_X, 130.f); // Posición en la parte superior
        gm.ventana.draw(estadisticas); // Dibuja las estadísticas

        sf::Text titulo_logros; // Crea el subtítulo de logros
        titulo_logros.setFont(gm.fuente_de_textos); // Asigna la fuente
        titulo_logros.setCharacterSize(26); // Tamaño de letra mediano
        titulo_logros.setFillColor(sf::Color::Yellow); // Color amarillo
        titulo_logros.setString("Logros desbloqueados:"); // Texto del subtítulo
        titulo_logros.setOrigin(titulo_logros.getLocalBounds().left + titulo_logros.getLocalBounds().width / 2.f, 0.f); // Centra el texto
        titulo_logros.setPosition(CENTRO_X, 270.f); // Posición debajo de las estadísticas
        gm.ventana.draw(titulo_logros); // Dibuja el subtítulo

        for (int i = 0; i < GameManager::CANTIDAD_MAXIMA_DE_LOGROS; i++) { // Recorre todos los logros
            sf::Text texto_logro; // Crea el texto para este logro
            texto_logro.setFont(gm.fuente_de_textos); // Asigna la fuente
            texto_logro.setCharacterSize(22); // Tamaño de letra mediano

            if (gm.logros[i].desbloqueado == true) { // Si el logro está desbloqueado
                texto_logro.setFillColor(sf::Color(100, 220, 100)); // Color verde brillante
                texto_logro.setString("[X] " + std::string(gm.logros[i].nombre)); // Muestra con tilde de completado
            }
            else { // Si el logro no está desbloqueado
                texto_logro.setFillColor(sf::Color(120, 120, 120)); // Color gris oscuro
                texto_logro.setString("[ ] " + std::string(gm.logros[i].nombre)); // Muestra con casilla vacía
            }

            texto_logro.setOrigin(texto_logro.getLocalBounds().left + texto_logro.getLocalBounds().width / 2.f, 0.f); // Centra el texto del logro
            texto_logro.setPosition(CENTRO_X, 320.f + i * 50.f); // Posición vertical espaciada entre logros
            gm.ventana.draw(texto_logro); // Dibuja el logro en pantalla
        }
    }

    sf::Text volver; // Crea el texto para volver al menú
    volver.setFont(gm.fuente_de_textos); // Asigna la fuente
    volver.setCharacterSize(18); // Tamaño de letra pequeño
    volver.setFillColor(sf::Color(150, 150, 150)); // Color gris
    volver.setString("ESC para volver al menu"); // Texto de instrucción
    volver.setOrigin(volver.getLocalBounds().left + volver.getLocalBounds().width / 2.f, 0.f); // Centra el texto
    volver.setPosition(CENTRO_X, 660.f); // Posición en la parte inferior
    gm.ventana.draw(volver); // Dibuja el texto de volver
}

///=================================================================///
///=================================================================///
///   PANTALLA 6: CREDITOS
///=================================================================///
///=================================================================///
// #18
void procesar_eventos_pantalla_creditos(GameManager& gm, sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) { // Si se presionó Escape
        gm.pantalla_actual = PantallaDelJuego::MENU; // Vuelve al menú principal
    }
}

// #19
void actualizar_pantalla_creditos(GameManager& gm, float dt) {
 // No necesita actualizar nada por frame
}

// #20
void renderizar_pantalla_creditos(GameManager& gm) {
    gm.ventana.setView(gm.ventana.getDefaultView()); // Usa la vista por defecto
    gm.ventana.draw(gm.texto_de_creditos); // Dibuja el texto de créditos
}
