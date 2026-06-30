#include "GameManager.h"
#include <iostream>
#include <cstdlib>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
GameManager::GameManager()
    : ventana(sf::VideoMode(1280, 720), "Daetherial - UTN"),
    camara(1280.f, 720.f),
    mapa(16, 1.0f),
    golem(sf::Vector2f(1696.f, 640.f)),
    tienda(sf::Vector2f(100.f, 260.f)),
    menu(1280.f, 720.f)
{
    pantalla_actual = PantallaDelJuego::INTRO;
    reloj_de_spawn_de_marcianitos = 0.f;
    intervalo_de_spawn_de_marcianitos = 5.0f;
    frame_actual_de_la_intro = 0;
    tiempo_acumulado_del_frame_de_intro = 0.f;
    cantidad_de_imagenes_de_la_historia = 0;
    imagen_actual_de_la_historia = 0;
    tiempo_acumulado_en_la_historia = 0.f;
    el_audio_de_la_historia_termino = false;
    la_musica_de_la_historia_se_cargo = false;
    la_historia_ya_empezo_a_reproducirse = false;
    es_la_primera_partida = true;

    camara.setLimites_del_mundo(sf::FloatRect(0.f, 0.f, 2000.f, 2000.f));
    ventana.setFramerateLimit(60);
    ventana.setMouseCursorVisible(false);

    if (mapa.cargar_mapa("assets/collisions_mapa_v1_background.csv", "assets/mapa_v1_background.png") == false) {
        std::cout << "ERROR CRITICO: NO SE PUDO CARGAR EL MAPA." << std::endl;
        ventana.close();
    }

    cambiar_musica(0);

    if (textura_de_la_intro.loadFromFile("assets/Daetherial-spritesheet.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR EL SPRITESHEET DE LA INTRO." << std::endl;
    }
    sprite_de_la_intro.setTexture(textura_de_la_intro);
    sprite_de_la_intro.setTextureRect(sf::IntRect(0, 0, 480, 270));

    float escala_horizontal_de_la_intro = 1280.f / 480.f;
    float escala_vertical_de_la_intro = 720.f / 270.f;
    sprite_de_la_intro.setScale(escala_horizontal_de_la_intro, escala_vertical_de_la_intro);
    sprite_de_la_intro.setPosition(0.f, 0.f);

    if (fabrica_de_items.contar_registros() == 0) {
        std::cout << "BASE DE DATOS VACIA. GENERANDO ITEMS DE PRUEBA..." << std::endl;

        RegistroDeItem registro_de_la_pocion = { 1, (int)TipoDeItem::CONSUMIBLE_DE_VIDA, "Pocion de Vida", 20, 10, (int)RarezaDelItem::COMUN, 0, true };
        RegistroDeItem registro_de_la_espada = { 2, (int)TipoDeItem::EQUIPAMIENTO, "Espada Corta", 15, 0, (int)RarezaDelItem::RARO, 1, true };
        RegistroDeItem registro_del_horno = { 3, (int)TipoDeItem::MUEBLE, "Horno de Fundicion", 2, 50, (int)RarezaDelItem::COMUN, 2, true };

        fabrica_de_items.guardar_registro(registro_de_la_pocion);
        fabrica_de_items.guardar_registro(registro_de_la_espada);
        fabrica_de_items.guardar_registro(registro_del_horno);
    }

    spawnear_drop_seguro(fabrica_de_items.crear_item_por_id(1), 300.f, 300.f);
    spawnear_drop_seguro(fabrica_de_items.crear_item_por_id(2), 350.f, 300.f);
    spawnear_drop_seguro(fabrica_de_items.crear_item_por_id(3), 400.f, 300.f);

    Item pocion_de_vida_para_la_tienda(1, "Pocion de Vida", TipoDeItem::CONSUMIBLE_DE_VIDA, 10, 1, 64, true, 20, 0, 0, 0);
    Item pocion_de_mana_para_la_tienda(2, "Pocion de Mana", TipoDeItem::CONSUMIBLE_DE_MANA, 15, 1, 64, true, 0, 0, 0, 10);

    tienda.agregar_item_en_venta(pocion_de_vida_para_la_tienda);
    tienda.agregar_item_en_venta(pocion_de_mana_para_la_tienda);
    tienda.cargar_fuente_y_cartel();

    if (fuente_de_textos.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR CARGANDO FUENTE DE TEXTOS." << std::endl;
    }

    for (int i = 0; i < CANTIDAD_MAXIMA_DE_IMAGENES_DE_HISTORIA; i++) {
        std::string numero = std::to_string(i + 1);
        if (i + 1 < 10) {
            numero = "0" + numero;
        }
        std::string nombre_del_archivo = "assets/historia_" + numero + ".png";
        if (texturas_de_la_historia[i].loadFromFile(nombre_del_archivo) == false) {
            break;
        }
        cantidad_de_imagenes_de_la_historia++;
    }

    la_musica_de_la_historia_se_cargo = musica_de_la_historia.openFromFile("assets/historia.wav");
    if (la_musica_de_la_historia_se_cargo == false) {
        std::cout << "AVISO: NO SE ENCONTRO EL AUDIO DE LA HISTORIA (assets/historia.wav)." << std::endl;
    }

    texto_de_creditos.setFont(fuente_de_textos);
    texto_de_creditos.setCharacterSize(12);
    texto_de_creditos.setFillColor(sf::Color::White);
    texto_de_creditos.setString(
        "CREDITOS\n\nDesarrollado por:\nGrupo 18 - Programacion 2\n"
        "Turno noche | Comision 102 (Virtual)\n\nIntegrantes:\n"
        "- Federico Wachenschwan\n- Juan Corbacho\n"
        "- Andres Ignacio Fernandez Escudero\n- Miguel Salazar\n\n"
        "Presiona ESC para volver"
    );
    texto_de_creditos.setPosition(120.f, 100.f);

    texto_de_lore.setFont(fuente_de_textos);
    texto_de_lore.setCharacterSize(13);
    texto_de_lore.setFillColor(sf::Color::White);
    texto_de_lore.setString(
        "Hace un siglo, tres magos errantes se conocieron en un puerto olvidado del norte, unidos por la misma sed:\n"
        "conocimiento que nadie mas se atrevia a buscar. Mucho antes de cruzarse con cualquier otro hechicero, habian\n"
        "forjado un pacto silencioso entre ellos: cualquier poder que encontraran en sus viajes seria repartido en\n"
        "partes iguales, sin excepciones, sin intrusos. Se llamaron a si mismos la Hermandad del Hielo Eterno.\n\n"
        "Su busqueda los llevo hasta las ruinas de un dungeon sepultado bajo glaciares, donde segun las leyendas\n"
        "dormia una reliquia capaz de robar la esencia magica de cualquier ser u objeto: el Corazon Vacio. Pero la\n"
        "entrada al dungeon estaba protegida por una magia demasiado antigua incluso para ellos tres, y necesitaban\n"
        "un cuarto hechicero, uno lo bastante talentoso y lo bastante ingenuo para no hacer demasiadas preguntas.\n\n"
        "Encontraron a Daetherial, el mas joven y el mas talentoso, deslumbrado por la idea de unirse a una\n"
        "expedicion legendaria. Lo que Daetherial no sabia era que su lugar en el grupo nunca fue el de un cuarto\n"
        "integrante del pacto, sino el de la llave que necesitaban para abrir la puerta del dungeon, y la fuente de\n"
        "poder que pensaban drenar y repartirse entre los tres apenas encontraran el Corazon Vacio.\n\n"
        "Durante la excavacion, un derrumbe separo a Daetherial del resto del grupo. Pasaron semanas a oscuras,\n"
        "cada uno por su lado, hasta que finalmente volvieron a encontrarse en la superficie. Pero algo habia\n"
        "cambiado: los tres habian hallado el Corazon Vacio sin el, y en la soledad de las ruinas, confirmaron el\n"
        "plan que llevaban guardado desde el principio. Daetherial, demasiado poderoso para dejarlo con vida y\n"
        "demasiado peligroso para dejarlo libre, se convirtio en el primer sacrificio de un pacto que nunca lo\n"
        "habia incluido.\n\n"
        "Lo que la Hermandad del Hielo Eterno no esperaba era que Daetherial, en el ultimo instante antes de que\n"
        "el Corazon Vacio drenara su magia, lograra canalizar toda su fuerza restante en un solo hechizo de\n"
        "teletransporte desesperado, uno que jamas habia practicado y que casi lo destruyo en el intento. Aparecio\n"
        "medio muerto en las profundidades de un dungeon de hielo, lejos de cualquier alma viviente.\n\n"
        "Alli permanecio, curandose en la oscuridad helada, jurando que cuando volviera a salir, seria mas\n"
        "poderoso que los tres traidores juntos. Para el resto del mundo, la Hermandad del Hielo Eterno ya no\n"
        "existe. Solo quedan tres hechiceros sin nombre que los una, y la promesa de un regreso que ninguno de\n"
        "ellos vera venir.\n\n\n"
        "Presiona ENTER para continuar"
    );
    texto_de_lore.setPosition(60.f, 20.f);
}

///=============================================================///
///   EJECUTAR
///=============================================================///
void procesar_eventos(GameManager& gm) {
    sf::Event evento;
    while (gm.ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            gm.ventana.close();
        }
        procesar_eventos_segun_la_pantalla(gm, evento);
    }
}

void actualizar(GameManager& gm) {
    gm.cursor.actualizar(gm.ventana);
    float tiempo_transcurrido = gm.reloj.restart().asSeconds();
    actualizar_segun_la_pantalla(gm, tiempo_transcurrido);
}

void renderizar(GameManager& gm) {
    gm.ventana.clear(sf::Color(30, 30, 30));
    renderizar_segun_la_pantalla(gm);
    gm.cursor.dibujar(gm.ventana);
    gm.ventana.display();
}

void ejecutar(GameManager& gm) {
    while (gm.ventana.isOpen() == true) {
        procesar_eventos(gm);
        actualizar(gm);
        renderizar(gm);
    }
}

///=============================================================///
///   RESOLVER COLISION ENTRE PERSONAJE Y GOLEM
///=============================================================///
void GameManager::resolver_colision_entre_personaje_y_golem() {
    sf::FloatRect caja_del_personaje = personaje.calcular_caja_de_colision();
    sf::FloatRect caja_del_golem = golem.calcular_caja_de_colision();
    sf::FloatRect zona_de_interseccion;

    if (caja_del_personaje.intersects(caja_del_golem, zona_de_interseccion) == true) {
        sf::Vector2f correccion(0.f, 0.f);

        if (zona_de_interseccion.width < zona_de_interseccion.height) {
            if (caja_del_personaje.left < caja_del_golem.left) { correccion.x = -zona_de_interseccion.width; }
            else { correccion.x = zona_de_interseccion.width; }
        }
        else {
            if (caja_del_personaje.top < caja_del_golem.top) { correccion.y = -zona_de_interseccion.height; }
            else { correccion.y = zona_de_interseccion.height; }
        }

        sf::Vector2f posicion_actual_del_personaje = personaje.getPosicion();
        personaje.setPosicion(sf::Vector2f(posicion_actual_del_personaje.x + correccion.x, posicion_actual_del_personaje.y + correccion.y));
    }
}

///=============================================================///
///   SPAWNEAR DROP SEGURO
///=============================================================///
void GameManager::spawnear_drop_seguro(Item item_a_soltar, float posicion_x, float posicion_y) {
    if (item_a_soltar.getEsta_vacio() == true) return;

    sf::FloatRect hitbox_de_prueba = item_a_soltar.getCaja_de_colision();
    hitbox_de_prueba.left = posicion_x;
    hitbox_de_prueba.top = posicion_y;

    int intentos_realizados = 0;
    int cantidad_maxima_de_intentos = 100;

    while (mapa.getHay_colision(hitbox_de_prueba) == true && intentos_realizados < cantidad_maxima_de_intentos) {
        posicion_x += (rand() % 21 - 10);
        posicion_y += (rand() % 21 - 10);
        hitbox_de_prueba.left = posicion_x;
        hitbox_de_prueba.top = posicion_y;
        intentos_realizados++;
    }

    if (intentos_realizados >= cantidad_maxima_de_intentos) {
        std::cout << "DROP BLOQUEADO EN PARED: " << item_a_soltar.getNombre() << std::endl;
    }

    administrador_de_objetos.agregar_item_al_mundo(item_a_soltar, posicion_x, posicion_y);
}

///=============================================================///
///   CAMBIAR MUSICA
///=============================================================///
void GameManager::cambiar_musica(int id_de_la_musica) {
    musica_ambiente.stop();

    if (id_de_la_musica == 0) { musica_ambiente.openFromFile("assets/menu_song.ogg"); }
    else if (id_de_la_musica == 1) { musica_ambiente.openFromFile("assets/ambient.wav"); }

    musica_ambiente.setLoop(true);
    musica_ambiente.play();
}