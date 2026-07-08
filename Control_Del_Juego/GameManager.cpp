#include "GameManager.h"
#include <iostream> // para mostrar mensajes de error en consola
#include <cstdlib> // para rand() al buscar posiciones de drop

///=============================================================///
///   #1 - CONSTRUCTOR
///=============================================================///
// #1
GameManager::GameManager()
    : ventana(sf::VideoMode(1280, 720), "Daetherial - UTN"), // crea la ventana de 1280x720
    camara(1280.f, 720.f), // inicializa la camara con el tamano de la ventana
    mapa(16, 1.0f), // mapa con tiles de 16px y escala 1.0
    golem(sf::Vector2f(1696.f, 640.f)), // coloca al Golem en esa posicion del mundo
    tienda(sf::Vector2f(100.f, 260.f)), // coloca la tienda en esa posicion del mundo
    menu(1280.f, 720.f) // inicializa el menu con el tamano de la ventana
{
 // -- ESTADO GENERAL --
    pantalla_actual = PantallaDelJuego::INTRO; // arranca mostrando la pantalla de intro
    partida_ganada = false; // la partida no esta ganada todavia
    enemigos_eliminados = 0; // el jugador no elimino ningun enemigo aun
    la_gema_fue_entregada = false; // la gema todavia no aparecio
    tiempo_final_de_la_partida = 0.f; // todavia no hay tiempo de partida registrado
    portal_de_victoria_activo = false; // el portal de salida no esta activo aun
    posicion_del_portal_de_victoria = sf::Vector2f(0.f, 0.f); // posicion inicial en el origen del mapa
    timer_respawn_del_portal_de_victoria = 0.f; // temporizador del portal en cero

 // -- MARCIANITOS --
    reloj_de_spawn_de_marcianitos = 0.f; // reinicia el temporizador de aparicion de enemigos
    intervalo_de_spawn_de_marcianitos = 5.0f; // un enemigo nuevo cada 5 segundos

 // -- LOGROS --
    logros = std::make_unique<Logro[]>(CANTIDAD_MAXIMA_DE_LOGROS); // reserva memoria para los logros
    strncpy_s(logros[0].nombre, 64, "Primera victoria - Derrotaste al Golem", 63); logros[0].desbloqueado = false; // logro 0: derrotar al Golem
    strncpy_s(logros[1].nombre, 64, "Cazador - Eliminaste 10 enemigos",       63); logros[1].desbloqueado = false; // logro 1: matar 10 enemigos
    strncpy_s(logros[2].nombre, 64, "Masacre - Eliminaste 20 enemigos",       63); logros[2].desbloqueado = false; // logro 2: matar 20 enemigos
    strncpy_s(logros[3].nombre, 64, "Velocista - Ganaste en menos de 3 min",  63); logros[3].desbloqueado = false; // logro 3: ganar rapido
    strncpy_s(logros[4].nombre, 64, "Rico - Tenias 50 o mas de oro al ganar", 63); logros[4].desbloqueado = false; // logro 4: ganar con mucho oro

 // -- VENTANA Y MUNDO --
    camara.setLimites_del_mundo(sf::FloatRect(0.f, 0.f, 2000.f, 2000.f)); // la camara no sale del mapa 2000x2000
    ventana.setFramerateLimit(60); // limita el juego a 60 cuadros por segundo
    ventana.setMouseCursorVisible(false); // oculta el cursor del sistema operativo

    if (mapa.cargar_mapa("assets/collisions_mapa_v1_background.csv", "assets/mapa_v1_background.png") == false) {
        std::cout << "ERROR CRITICO: NO SE PUDO CARGAR EL MAPA." << std::endl; // avisa que el mapa no cargo
        ventana.close(); // cierra el juego si el mapa falla, ya que es indispensable
    }

 // -- MUSICA --
    cambiar_musica(0); // reproduce la musica del menu al iniciar

 // -- INTRO --
    frame_actual_de_la_intro = 0; // comienza en el primer frame de la animacion de intro
    tiempo_acumulado_del_frame_de_intro = 0.f; // no hay tiempo acumulado en el frame de intro

    if (textura_de_la_intro.loadFromFile("assets/Daetherial-spritesheet.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR EL SPRITESHEET DE LA INTRO." << std::endl; // avisa del error
    }
    sprite_de_la_intro.setTexture(textura_de_la_intro); // asigna la textura al sprite de intro
    sprite_de_la_intro.setTextureRect(sf::IntRect(0, 0, 480, 270)); // muestra el primer frame (480x270 px)

    float escala_horizontal_de_la_intro = 1280.f / 480.f; // calcula cuanto escalar horizontalmente para llenar la ventana
    float escala_vertical_de_la_intro = 720.f / 270.f; // calcula cuanto escalar verticalmente para llenar la ventana
    sprite_de_la_intro.setScale(escala_horizontal_de_la_intro, escala_vertical_de_la_intro); // aplica la escala al sprite
    sprite_de_la_intro.setPosition(0.f, 0.f); // coloca el sprite en la esquina superior izquierda

 // -- HISTORIA --
    cantidad_de_imagenes_de_la_historia = 0; // todavia no se cargo ninguna imagen de historia
    imagen_actual_de_la_historia = 0; // empieza desde la primera imagen de la historia
    tiempo_acumulado_en_la_historia = 0.f; // tiempo transcurrido en la imagen actual es cero
    el_audio_de_la_historia_termino = false; // el audio de la historia no termino aun
    la_musica_de_la_historia_se_cargo = false; // todavia no se intento cargar el audio de historia
    la_historia_ya_empezo_a_reproducirse = false; // la historia no comenzo a reproducirse aun
    es_la_primera_partida = true; // asume que es la primera vez que se juega

    for (int i = 0; i < CANTIDAD_MAXIMA_DE_IMAGENES_DE_HISTORIA; i++) { // recorre los posibles archivos de historia
        std::string numero = std::to_string(i + 1); // convierte el numero de imagen a texto
        if (i + 1 < 10) {
            numero = "0" + numero; // agrega un cero adelante para tener formato "01", "02", etc.
        }
        std::string nombre_del_archivo = "assets/historia_" + numero + ".png"; // arma el nombre del archivo
        if (texturas_de_la_historia[i].loadFromFile(nombre_del_archivo) == false) {
            break; // si no se pudo cargar la imagen, no hay mas imagenes: sale del ciclo
        }
        cantidad_de_imagenes_de_la_historia++; // suma una imagen cargada exitosamente
    }

    la_musica_de_la_historia_se_cargo = musica_de_la_historia.openFromFile("assets/historia.wav"); // intenta cargar el audio de historia
    if (la_musica_de_la_historia_se_cargo == false) {
        std::cout << "AVISO: NO SE ENCONTRO EL AUDIO DE LA HISTORIA (assets/historia.wav)." << std::endl; // avisa que falta el audio
    }

 // -- ITEMS Y TIENDA --
    Item pocion_de_vida_para_la_tienda(1, "Pocion de Vida", TipoDeItem::CONSUMIBLE_DE_VIDA, 15, 1, 64, true, 10, 0); // crea el item de pocion de vida para vender (15 oro, cura 10 vida)
    Item pocion_de_mana_para_la_tienda(2, "Pocion de Mana", TipoDeItem::CONSUMIBLE_DE_MANA, 10, 1, 64, true, 0, 30); // crea el item de pocion de mana para vender (10 oro, da 20 mana)
    Item baculo_para_la_tienda(4, "Baculo Arcano", TipoDeItem::BACULO_ARCANO, 150, 1, 1, false, 0, 0); // crea el baculo magico para vender (requiere gema arcana)

    tienda.agregar_item_en_venta(pocion_de_vida_para_la_tienda); // agrega la pocion de vida al catalogo de la tienda
    tienda.agregar_item_en_venta(pocion_de_mana_para_la_tienda); // agrega la pocion de mana al catalogo de la tienda
    tienda.agregar_item_en_venta(baculo_para_la_tienda); // agrega el baculo al catalogo de la tienda
    tienda.cargar_fuente_y_cartel(); // carga la fuente y el cartel visual de la tienda

 // -- CREDITOS --
    if (fuente_de_textos.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR CARGANDO FUENTE DE TEXTOS." << std::endl; // avisa si la fuente no se pudo cargar
    }

    texto_de_creditos.setFont(fuente_de_textos); // asigna la fuente al texto de creditos
    texto_de_creditos.setCharacterSize(12); // tamano de letra 12 puntos
    texto_de_creditos.setFillColor(sf::Color::White); // texto de color blanco
    texto_de_creditos.setString(
        "CREDITOS\n\nDesarrollado por:\nGrupo 18 - Programacion 2\n"
        "Turno noche | Comision 102 (Virtual)\n\nIntegrantes:\n"
        "- Federico Wachenschwan\n- Juan Corbacho\n"
        "- Andres Ignacio Fernandez Escudero\n- Miguel Salazar\n\n"
        "Presiona ESC para volver"
    ); // texto completo con los integrantes del grupo
    texto_de_creditos.setPosition(120.f, 100.f); // posiciona el texto en la pantalla

    cargar_logros(); // lee el archivo de logros y restaura el progreso anterior
}

///=============================================================///
///   #2 - PROCESAR EVENTOS
///=============================================================///
// #2
void procesar_eventos(GameManager& gm) {
    sf::Event evento; // variable que guarda el evento del sistema operativo
    while (gm.ventana.pollEvent(evento)) { // lee todos los eventos pendientes uno por uno
        if (evento.type == sf::Event::Closed) {
            gm.ventana.close(); // cierra la ventana si el usuario presiono la X
        }
        procesar_eventos_segun_la_pantalla(gm, evento); // delega el evento a la pantalla activa
    }
}

///=============================================================///
///   #3 - ACTUALIZAR
///=============================================================///
// #3
void actualizar(GameManager& gm) {
    gm.cursor.actualizar(gm.ventana); // actualiza la posicion del cursor personalizado
    float tiempo_transcurrido = gm.reloj.restart().asSeconds(); // mide cuanto tiempo paso desde el frame anterior
    actualizar_segun_la_pantalla(gm, tiempo_transcurrido); // actualiza la logica de la pantalla activa
}

///=============================================================///
///   #4 - RENDERIZAR
///=============================================================///
// #4
void renderizar(GameManager& gm) {
    gm.ventana.clear(sf::Color(30, 30, 30)); // limpia la pantalla con color gris oscuro
    renderizar_segun_la_pantalla(gm); // dibuja todo lo que corresponde a la pantalla activa
    gm.cursor.dibujar(gm.ventana); // dibuja el cursor personalizado encima de todo
    gm.ventana.display(); // muestra en pantalla lo que se acabo de dibujar
}

///=============================================================///
///   #5 - EJECUTAR
///=============================================================///
// #5
void ejecutar(GameManager& gm) {
    while (gm.ventana.isOpen() == true) { // repite el bucle mientras la ventana este abierta
        procesar_eventos(gm); // lee y responde a los eventos del usuario
        actualizar(gm); // actualiza la logica del juego
        renderizar(gm); // dibuja el estado actual del juego
    }
}

///=============================================================///
///   #6 - RESOLVER COLISION ENTRE PERSONAJE Y GOLEM
///=============================================================///
// #6
void GameManager::resolver_colision_entre_personaje_y_golem() {
    sf::FloatRect caja_del_personaje = personaje.calcular_caja_de_colision(); // obtiene el rectangulo de colision del personaje
    sf::FloatRect caja_del_golem = golem.calcular_caja_de_colision(); // obtiene el rectangulo de colision del Golem
    sf::FloatRect zona_de_interseccion; // area donde ambos rectangulos se superponen

    if (caja_del_personaje.intersects(caja_del_golem, zona_de_interseccion) == true) { // si estan chocando
        sf::Vector2f correccion(0.f, 0.f); // vector de cuanto hay que mover al personaje para separarlo

        if (zona_de_interseccion.width < zona_de_interseccion.height) { // el choque es mas horizontal que vertical
            if (caja_del_personaje.left < caja_del_golem.left) { correccion.x = -zona_de_interseccion.width; } // personaje viene por la izquierda
            else { correccion.x = zona_de_interseccion.width; } // personaje viene por la derecha
        }
        else { // el choque es mas vertical que horizontal
            if (caja_del_personaje.top < caja_del_golem.top) { correccion.y = -zona_de_interseccion.height; } // personaje viene desde arriba
            else { correccion.y = zona_de_interseccion.height; } // personaje viene desde abajo
        }

        sf::Vector2f posicion_actual_del_personaje = personaje.getPosicion(); // guarda la posicion actual del personaje
        personaje.setPosicion(sf::Vector2f(posicion_actual_del_personaje.x + correccion.x, posicion_actual_del_personaje.y + correccion.y)); // mueve al personaje fuera del Golem
    }
}

///=============================================================///
///   #7 - SPAWNEAR DROP SEGURO
///=============================================================///
// #7
void GameManager::spawnear_drop_seguro(Item item_a_soltar, float posicion_x, float posicion_y) {
    if (item_a_soltar.getEsta_vacio() == true) return; // si el item esta vacio no hay nada que soltar

    sf::FloatRect hitbox_de_prueba = item_a_soltar.getCaja_de_colision(); // toma la hitbox del item
    hitbox_de_prueba.left = posicion_x; // coloca la hitbox en la posicion X propuesta
    hitbox_de_prueba.top = posicion_y; // coloca la hitbox en la posicion Y propuesta

    int intentos_realizados = 0; // contador de intentos de reubicacion
    int cantidad_maxima_de_intentos = 100; // maximo de intentos antes de cancelar el drop

    while (mapa.getHay_colision(hitbox_de_prueba) == true && intentos_realizados < cantidad_maxima_de_intentos) { // mientras este dentro de una pared
        posicion_x += (rand() % 21 - 10); // desplaza aleatoriamente en X entre -10 y +10
        posicion_y += (rand() % 21 - 10); // desplaza aleatoriamente en Y entre -10 y +10
        hitbox_de_prueba.left = posicion_x; // actualiza la hitbox con la nueva X
        hitbox_de_prueba.top = posicion_y; // actualiza la hitbox con la nueva Y
        intentos_realizados++; // cuenta un intento mas
    }

    if (intentos_realizados >= cantidad_maxima_de_intentos) {
        std::cout << "DROP BLOQUEADO EN PARED: " << item_a_soltar.getNombre() << std::endl; // avisa que no se pudo colocar el item
    }

    administrador_de_objetos.agregar_item_al_mundo(item_a_soltar, posicion_x, posicion_y); // coloca el item en el mundo en la posicion valida
}

///=============================================================///
///   #8 - CAMBIAR MUSICA
///=============================================================///
// #8
void GameManager::cambiar_musica(int id_de_la_musica) {
    musica_ambiente.stop(); // detiene la musica que estaba sonando

    if (id_de_la_musica == 0) { musica_ambiente.openFromFile("assets/menu_song.ogg"); } // carga la musica del menu
    else if (id_de_la_musica == 1) { musica_ambiente.openFromFile("assets/ambient.wav"); } // carga la musica de juego

    musica_ambiente.setLoop(true); // hace que la musica se repita en bucle
    musica_ambiente.play(); // comienza a reproducir la musica
}

///=============================================================///
///   #9 - REINICIAR PARTIDA - Borra todo el estado de la partida
///   actual para que el jugador empiece de cero. Se llama cuando
///   muere y presiona ENTER en la pantalla de muerte.
///=============================================================///
// #9 (el resto se renumera)
void GameManager::reiniciar_partida() {

 // -- PERSONAJE --
 // Restaura vida, mana, oro y posicion al estado inicial
    personaje.reiniciar();

 // -- ENEMIGOS CHICOS --
 // Marca todos como inactivos en el pool (vida en cero = libre)
    for (int i = 0; i < CANTIDAD_MAXIMA_DE_MARCIANITOS; i++) {
        marcianitos[i].desactivar();
    }
    reloj_de_spawn_de_marcianitos = 0.f; // Reinicia el temporizador de spawn

 // -- GOLEM --
 // Lo revive y lo vuelve a su posicion original del mapa
    golem.curar(99999); // curar() no pasa el maximo, asi que lo llena completo
    golem.setPosicion(sf::Vector2f(1696.f, 640.f));

 // -- ITEMS Y ORO EN EL SUELO --
 // Borra todo lo que estaba tirado en el mundo
    administrador_de_objetos.limpiar();

 // -- ESTADO DE PARTIDA --
    enemigos_eliminados = 0;
    la_gema_fue_entregada = false;
    partida_ganada = false;
    portal_de_victoria_activo = false;
    timer_respawn_del_portal_de_victoria = 0.f;

 // -- ARCHIVO DE LOGROS --
 // Pone todos los logros en false y sobreescribe el archivo
 // Esto borra el progreso guardado para que la nueva partida empiece limpia
    for (int i = 0; i < CANTIDAD_MAXIMA_DE_LOGROS; i++) {
        logros[i].desbloqueado = false;
    }
    guardar_logros();

 // -- MUSICA Y PANTALLA --
    cambiar_musica(0); // Vuelve a la musica del menu
    pantalla_actual = PantallaDelJuego::MENU;
}

///=============================================================///
///   #11 - GUARDAR LOGROS - Escribe el estado de cada logro en un
///   archivo binario para que persista entre sesiones
///=============================================================///
// #11
void GameManager::guardar_logros() {
    std::ofstream archivo("assets/logros.dat", std::ios::binary); // abre el archivo en modo escritura binaria
    if (archivo.is_open() == false) return; // si no se pudo abrir, cancela la operacion

    for (int i = 0; i < CANTIDAD_MAXIMA_DE_LOGROS; i++) {
        archivo.write((const char*)&logros[i], sizeof(Logro)); // escribe cada logro como bytes en el archivo
    }
    archivo.write((const char*)&partida_ganada,      sizeof(bool)); // guarda si la partida fue ganada
    archivo.write((const char*)&es_la_primera_partida, sizeof(bool)); // guarda si es la primera partida
    archivo.close(); // cierra el archivo correctamente
}

///=============================================================///
///   #12 - CARGAR LOGROS - Lee el archivo binario y restaura el estado
///   de los logros. Si el archivo no existe, no hace nada (los
///   logros quedan en false, como los inicializo el constructor)
///=============================================================///
// #12
void GameManager::cargar_logros() {
    std::ifstream archivo("assets/logros.dat", std::ios::binary); // abre el archivo en modo lectura binaria
    if (archivo.is_open() == false) return; // si no existe el archivo, no hay nada que leer

    Logro logro_leido{}; // variable temporal para leer un logro del archivo
    for (int i = 0; i < CANTIDAD_MAXIMA_DE_LOGROS; i++) {
        if (archivo.read((char*)&logro_leido, sizeof(Logro))) { // lee el logro del archivo
            logros[i].desbloqueado = logro_leido.desbloqueado; // restaura solo si estaba desbloqueado
        }
    }
    archivo.read((char*)&partida_ganada,        sizeof(bool)); // restaura si habia ganado antes
    archivo.read((char*)&es_la_primera_partida, sizeof(bool)); // restaura si era primera partida
    archivo.close(); // cierra el archivo correctamente
}
