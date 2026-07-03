#include "Personaje.h"
#include "InputManager.h"
#include <iostream>
#include <cmath>

///=============================================================///
///   #1 - CONSTRUCTOR
///=============================================================///
// #1
Personaje::Personaje() {
    if (_textura_de_la_entidad.loadFromFile("assets/maguito_main.png") == false) { // Intenta cargar la imagen
        std::cout << "ERROR: NO SE PUDO CARGAR LA HOJA DE SPRITES DEL PERSONAJE." << std::endl; // Avisa si falla
    }
    _sprite_de_la_entidad.setTexture(_textura_de_la_entidad); // Asocia la imagen al sprite
    _sprite_de_la_entidad.setPosition(100.f, 100.f); // Posicion inicial en el mapa
    _sprite_de_la_entidad.setOrigin(32.f, 32.f); // Punto de referencia del sprite

    _velocidad_de_movimiento = 170.f; // Velocidad del personaje en px/seg
    _vida_maxima_de_la_entidad = 100; // Vida maxima del jugador
    _vida_actual_de_la_entidad = _vida_maxima_de_la_entidad; // Arranca con vida completa
    _dano_que_hace_esta_entidad = 15; // Daño del ataque principal
    _segundos_de_cooldown_entre_ataques = 0.5f; // Medio segundo entre ataques

    _circulo_que_muestra_el_alcance.setRadius(_radio_de_alcance_del_hechizo); // Radio del circulo visual
    _circulo_que_muestra_el_alcance.setFillColor(sf::Color(0, 0, 0, 50)); // Relleno negro semitransparente
    _circulo_que_muestra_el_alcance.setOutlineColor(sf::Color::White); // Borde blanco
    _circulo_que_muestra_el_alcance.setOrigin(_radio_de_alcance_del_hechizo, _radio_de_alcance_del_hechizo); // Centra el circulo

    actualizar_el_recorte_del_sprite_segun_la_animacion(); // Aplica el primer frame de la animacion
}

///=============================================================///
///   #2 - REINICIAR - Devuelve al personaje a su estado inicial.
///   Se llama cuando el jugador muere y quiere volver a jugar.
///=============================================================///
// #2
void Personaje::reiniciar() {
    _vida_actual_de_la_entidad  = _vida_maxima_de_la_entidad; // Vida al maximo
    _mana_actual_del_jugador    = _mana_maxima_del_jugador;   // Mana al maximo
    _cantidad_de_oro_del_jugador = 100;                        // Oro inicial
    _velocidad_actual_del_movimiento = sf::Vector2f(0.f, 0.f); // Sin inercia
    _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO; // Animacion neutral
    _sprite_de_la_entidad.setPosition(100.f, 100.f); // Posicion de inicio del mapa
    _mochila_del_personaje.limpiar(); // Vacia el inventario
}

///=============================================================///
///   #3 - RESTAURAR MANA
///=============================================================///
// #3
void Personaje::restaurar_mana(int puntos_de_mana_a_restaurar) {
    int mana_despues_de_restaurar = _mana_actual_del_jugador + puntos_de_mana_a_restaurar; // Suma el mana

    if (mana_despues_de_restaurar > _mana_maxima_del_jugador) { // Si supera el maximo
        _mana_actual_del_jugador = _mana_maxima_del_jugador; // Limita al tope
    }
    else if (mana_despues_de_restaurar < 0) { // Si baja de cero
        _mana_actual_del_jugador = 0; // No permite mana negativo
    }
    else {
        _mana_actual_del_jugador = mana_despues_de_restaurar; // Guarda el mana correcto
    }
}

///=============================================================///
///   #4 - CALCULAR CAJA DE COLISION
///=============================================================///
// #4
sf::FloatRect Personaje::calcular_caja_de_colision() const {
    sf::Vector2f posicion_actual = _sprite_de_la_entidad.getPosition(); // Posicion del centro del sprite
    float posicion_x_de_la_caja = posicion_actual.x - 8.f; // Desplaza 8px a la izquierda
    float posicion_y_de_la_caja = posicion_actual.y + 16.f; // Ubica la caja a la altura de los pies
    return sf::FloatRect(posicion_x_de_la_caja, posicion_y_de_la_caja, 16.f, 16.f); // Caja de 16x16 px
}

///=============================================================///
///   #5 - PROCESAR MOVIMIENTO Y ENTRADA
///=============================================================///
// #5
void Personaje::procesar_movimiento_y_entrada_del_jugador(const InputManager& entrada_del_jugador, Map& mapa_del_juego, sf::RenderWindow& ventana_del_juego, bool la_interfaz_le_esta_tapando_el_mouse, float tiempo_transcurrido) {

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::ESQUIVANDO) { // Si esta en dash
        mover_con_colisiones(_velocidad_actual_del_movimiento * tiempo_transcurrido, calcular_caja_de_colision(), mapa_del_juego); // Mueve rapido en la direccion del dash
        return; // No procesa mas entradas mientras esquiva
    }

    bool no_puede_moverse_libremente =
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::LANZANDO_HECHIZO || // Ocupado lanzando
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::LASTIMADO || // Recibio dano
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::MUERTO || // Esta muerto
        la_interfaz_le_esta_tapando_el_mouse; // El menu esta abierto

    if (no_puede_moverse_libremente == true) { // Si alguna condicion bloquea el movimiento
        return; // Corta la funcion sin mover nada
    }

    if (_segundos_de_espera_para_volver_a_esquivar > 0.f) { // Si el cooldown del dash esta activo
        _segundos_de_espera_para_volver_a_esquivar -= tiempo_transcurrido; // Descuenta el tiempo transcurrido
    }

    sf::Vector2f direccion_que_quiere_el_jugador = entrada_del_jugador.getDireccion_de_movimiento(); // Lee las teclas WASD

    if (entrada_del_jugador.getEl_jugador_quiere_correr() && _segundos_de_espera_para_volver_a_esquivar <= 0.f) { // Si presiono Shift y el cooldown termino
        if (direccion_que_quiere_el_jugador.x != 0.f || direccion_que_quiere_el_jugador.y != 0.f) { // Y esta moviendo el personaje
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::ESQUIVANDO; // Activa la animacion de dash
            _segundos_que_quedan_de_esquive = _duracion_total_del_esquive; // Carga la duracion del dash
            _segundos_de_espera_para_volver_a_esquivar = 1.5f; // Cooldown: 1.5 segundos
            _velocidad_actual_del_movimiento = direccion_que_quiere_el_jugador * (_velocidad_de_movimiento * 4.0f); // Impulso del dash
            return; // El resto del frame solo corre el dash
        }
    }

    sf::Vector2f movimiento_deseado = direccion_que_quiere_el_jugador * _velocidad_de_movimiento; // Calcula la velocidad objetivo

    if (direccion_que_quiere_el_jugador.x != 0.f && direccion_que_quiere_el_jugador.y != 0.f) { // Si mueve en diagonal
        movimiento_deseado *= 0.7071f; // Normaliza para que no sea mas rapido en diagonal
    }

    if (direccion_que_quiere_el_jugador.x != 0.f || direccion_que_quiere_el_jugador.y != 0.f) { // Si hay tecla presionada
        _velocidad_actual_del_movimiento.x += (movimiento_deseado.x - _velocidad_actual_del_movimiento.x) * _aceleracion_al_arrancar_a_moverse * tiempo_transcurrido; // Acelera en X
        _velocidad_actual_del_movimiento.y += (movimiento_deseado.y - _velocidad_actual_del_movimiento.y) * _aceleracion_al_arrancar_a_moverse * tiempo_transcurrido; // Acelera en Y
    }
    else { // Si no hay tecla presionada, frena gradualmente
        _velocidad_actual_del_movimiento.x += (0.f - _velocidad_actual_del_movimiento.x) * _desaceleracion_al_frenar * tiempo_transcurrido; // Frena en X
        _velocidad_actual_del_movimiento.y += (0.f - _velocidad_actual_del_movimiento.y) * _desaceleracion_al_frenar * tiempo_transcurrido; // Frena en Y

        if (std::hypot(_velocidad_actual_del_movimiento.x, _velocidad_actual_del_movimiento.y) < 10.f) { // Si la velocidad es casi cero
            _velocidad_actual_del_movimiento = { 0.f, 0.f }; // Para completamente
        }
    }

    decidir_animacion_y_direccion_segun_el_movimiento(_velocidad_actual_del_movimiento); // Elige que animacion mostrar
    procesar_el_lanzamiento_de_hechizos(entrada_del_jugador, ventana_del_juego, la_interfaz_le_esta_tapando_el_mouse); // Chequea si quiere lanzar hechizo

    sf::Vector2f movimiento_de_este_frame = _velocidad_actual_del_movimiento * tiempo_transcurrido; // Desplazamiento real este frame
    mover_con_colisiones(movimiento_de_este_frame, calcular_caja_de_colision(), mapa_del_juego); // Mueve evitando paredes
}

///=============================================================///
///   #6 - DECIDIR ANIMACION Y DIRECCION
///=============================================================///
// #6
void Personaje::decidir_animacion_y_direccion_segun_el_movimiento(sf::Vector2f direccion_en_la_que_se_mueve) {
    if (direccion_en_la_que_se_mueve.x == 0.f && direccion_en_la_que_se_mueve.y == 0.f) { // Si no se mueve
        if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO) { // Si estaba apuntando
            _numero_de_frame_actual = 0; // Reinicia el frame
            _tiempo_acumulado_del_frame_actual = 0.f; // Reinicia el cronometro
        }
        else if (_estado_de_animacion_actual != EstadoDeAnimacionDelPersonaje::QUIETO) { // Si no estaba quieto
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO; // Cambia a quieto
            _numero_de_frame_actual = 9; // Frame de idle
            _tiempo_acumulado_del_frame_actual = 0.f; // Reinicia el cronometro
        }
        return; // No hay nada mas que hacer
    }

    if (_estado_de_animacion_actual != EstadoDeAnimacionDelPersonaje::APUNTANDO) { // Si no esta apuntando
        _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::CAMINANDO; // Cambia a caminar
    }

    if (std::abs(direccion_en_la_que_se_mueve.x) > std::abs(direccion_en_la_que_se_mueve.y)) { // Mas movimiento horizontal
        if (direccion_en_la_que_se_mueve.x > 0.f) { // Se mueve a la derecha
            _direccion_hacia_donde_mira = DireccionHaciaDondeMira::DERECHA;
        }
        else { // Se mueve a la izquierda
            _direccion_hacia_donde_mira = DireccionHaciaDondeMira::IZQUIERDA;
        }
    }
    else { // Mas movimiento vertical
        if (direccion_en_la_que_se_mueve.y > 0.f) { // Se mueve hacia abajo
            _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ABAJO;
        }
        else { // Se mueve hacia arriba
            _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ARRIBA;
        }
    }
}

///=============================================================///
///   #7 - PROCESAR LANZAMIENTO DE HECHIZOS
///=============================================================///
// #7
void Personaje::procesar_el_lanzamiento_de_hechizos(const InputManager& entrada_del_jugador, sf::RenderWindow& ventana_del_juego, bool la_interfaz_le_esta_tapando_el_mouse) {
    if (la_interfaz_le_esta_tapando_el_mouse == true) { // Si el menu cubre la pantalla
        return; // No procesa el hechizo
    }

    if (entrada_del_jugador.getEl_jugador_quiere_saltar()) { // Si presiono la tecla de modo apuntado
        if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO) { // Si ya estaba apuntando
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO; // Cancela el modo apuntado
            _numero_de_frame_actual = 0; // Reinicia el frame
        }
        else { // Si no estaba apuntando
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::APUNTANDO; // Activa el modo apuntado
            _numero_de_frame_actual = 0; // Reinicia el frame
        }
    }

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO && entrada_del_jugador.getEl_jugador_quiere_atacar()) { // Si apunta y hace clic

        int costo_de_mana_de_este_hechizo = _bolas_de_fuego[0].getCosto_de_mana(); // El costo lo define la propia BolaDeFuego

        if (_mana_actual_del_jugador < costo_de_mana_de_este_hechizo) { // Si no hay mana suficiente
            std::cout << "NO TENES MANA SUFICIENTE PARA LANZAR EL HECHIZO. MANA ACTUAL: " << _mana_actual_del_jugador << std::endl; // Avisa en consola
            return; // No lanza el hechizo
        }

        _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::LANZANDO_HECHIZO; // Activa la animacion de lanzamiento
        _numero_de_frame_actual = 0; // Empieza desde el primer frame
        _tiempo_acumulado_del_frame_actual = 0.f; // Reinicia el cronometro de frame

        sf::Vector2i posicion_del_mouse_en_pantalla = entrada_del_jugador.getPosicion_del_mouse(); // Posicion del mouse en pixeles de pantalla
        sf::Vector2f posicion_del_mouse_en_el_mapa = ventana_del_juego.mapPixelToCoords(posicion_del_mouse_en_pantalla); // Convierte a coordenadas del mapa
        sf::Vector2f posicion_actual_del_personaje = getPosicion(); // Posicion actual del jugador

        if (std::abs(posicion_del_mouse_en_el_mapa.x - posicion_actual_del_personaje.x) > std::abs(posicion_del_mouse_en_el_mapa.y - posicion_actual_del_personaje.y)) { // Si el mouse esta mas lejos en X
            if (posicion_del_mouse_en_el_mapa.x > posicion_actual_del_personaje.x) { // Mouse a la derecha
                _direccion_hacia_donde_mira = DireccionHaciaDondeMira::DERECHA;
            }
            else { // Mouse a la izquierda
                _direccion_hacia_donde_mira = DireccionHaciaDondeMira::IZQUIERDA;
            }
        }
        else { // El mouse esta mas lejos en Y
            if (posicion_del_mouse_en_el_mapa.y > posicion_actual_del_personaje.y) { // Mouse abajo
                _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ABAJO;
            }
            else { // Mouse arriba
                _direccion_hacia_donde_mira = DireccionHaciaDondeMira::ARRIBA;
            }
        }

        for (int b = 0; b < CANTIDAD_MAXIMA_DE_BOLAS; b++) { // Busca la primera bola libre del pool
            if (_bolas_de_fuego[b].getEsta_activa() == false) { // Si esta bola no esta en vuelo
                _mana_actual_del_jugador = _mana_actual_del_jugador - costo_de_mana_de_este_hechizo; // Descuenta el mana
                _bolas_de_fuego[b].activar(posicion_actual_del_personaje, posicion_del_mouse_en_el_mapa, _radio_de_alcance_del_hechizo); // Dispara esta bola
                break; // Solo activa una por clic
            }
        }
    }
}

///=============================================================///
///   #8 - ACTUALIZAR ANIMACION Y HECHIZO
///=============================================================///
// #8
void Personaje::actualizar_animacion_y_hechizo(float tiempo_transcurrido, VisualFX& efectos_visuales) {

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::ESQUIVANDO) { // Si esta en dash
        _segundos_que_quedan_de_esquive -= tiempo_transcurrido; // Resta el tiempo del dash
        _tiempo_acumulado_para_el_rastro_visual += tiempo_transcurrido; // Acumula para el efecto de estela

        if (_tiempo_acumulado_para_el_rastro_visual >= 0.02f) { // Cada 0.02 segundos
            efectos_visuales.agregarRastro(_sprite_de_la_entidad, sf::Color(0, 255, 255), 500.f, true); // Crea efecto de estela cyan
            _tiempo_acumulado_para_el_rastro_visual = 0.f; // Reinicia el cronometro del rastro
        }
        if (_segundos_que_quedan_de_esquive <= 0.f) { // Si el dash termino
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO; // Vuelve a quieto
            _velocidad_actual_del_movimiento = { 0.f, 0.f }; // Detiene el impulso
        }
    }

    if (getEsta_muerto() == true) { // Si el personaje no tiene vida
        _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::MUERTO; // Fuerza animacion de muerte
        if (_numero_de_frame_actual >= 5) { // Si llego al ultimo frame de muerte
            _numero_de_frame_actual = 5; // Congela en el ultimo frame
            actualizar_el_recorte_del_sprite_segun_la_animacion(); // Actualiza el sprite
            for (int b = 0; b < CANTIDAD_MAXIMA_DE_BOLAS; b++) {
                _bolas_de_fuego[b].actualizar(tiempo_transcurrido, efectos_visuales); // Sigue actualizando bolas activas
            }
            return; // No procesa mas
        }
    }

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO) { // Si esta apuntando
        _circulo_que_muestra_el_alcance.setPosition(getPosicion()); // Mueve el circulo al personaje
    }

    float duracion_de_este_frame = _segundos_que_dura_cada_frame; // Duracion normal del frame
    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::QUIETO) { // Si esta quieto
        duracion_de_este_frame = 0.5f; // Los frames del idle duran mas
    }

    _tiempo_acumulado_del_frame_actual += tiempo_transcurrido; // Acumula el tiempo del frame actual
    if (_tiempo_acumulado_del_frame_actual >= duracion_de_este_frame) { // Si el frame ya duro suficiente
        _tiempo_acumulado_del_frame_actual = 0.f; // Reinicia el cronometro
        if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::MUERTO) { // Si esta muerto
            if (_numero_de_frame_actual < 5) _numero_de_frame_actual++; // Avanza al siguiente frame de muerte
        }
        else {
            _numero_de_frame_actual++; // Avanza al siguiente frame
            avanzar_de_frame_y_decidir_si_cambia_de_animacion(); // Decide si la animacion termino
        }
    }

    actualizar_el_recorte_del_sprite_segun_la_animacion(); // Aplica el frame actual al sprite
    for (int b = 0; b < CANTIDAD_MAXIMA_DE_BOLAS; b++) {
        _bolas_de_fuego[b].actualizar(tiempo_transcurrido, efectos_visuales); // Mueve cada bola activa
    }
}

///=============================================================///
///   #9 - AVANZAR DE FRAME
///=============================================================///
// #9
void Personaje::avanzar_de_frame_y_decidir_si_cambia_de_animacion() {
    switch (_estado_de_animacion_actual) {
    case EstadoDeAnimacionDelPersonaje::QUIETO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 11; // Idle tiene 11 frames
        if (_numero_de_frame_actual < 9 || _numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) {
            _numero_de_frame_actual = 9; // El idle empieza en el frame 9
        }
        break;
    case EstadoDeAnimacionDelPersonaje::APUNTANDO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 9; // 9 frames al apuntar
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = 0; // Vuelve al inicio
        break;
    case EstadoDeAnimacionDelPersonaje::CAMINANDO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 9; // 9 frames al caminar
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = 0; // Repite en loop
        break;
    case EstadoDeAnimacionDelPersonaje::ESQUIVANDO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 9; // 9 frames al esquivar
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = 0; // Repite en loop
        break;
    case EstadoDeAnimacionDelPersonaje::LANZANDO_HECHIZO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 7; // 7 frames al lanzar
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) { // Si termino el lanzamiento
            _estado_de_animacion_actual = EstadoDeAnimacionDelPersonaje::QUIETO; // Vuelve a quieto
            _numero_de_frame_actual = 0; // Reinicia el frame
        }
        break;
    case EstadoDeAnimacionDelPersonaje::LASTIMADO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 6; // 6 frames al recibir dano
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = _cantidad_maxima_de_frames_de_la_animacion_actual - 1; // Congela en el ultimo
        break;
    case EstadoDeAnimacionDelPersonaje::MUERTO:
        _cantidad_maxima_de_frames_de_la_animacion_actual = 6; // 6 frames de muerte
        if (_numero_de_frame_actual >= _cantidad_maxima_de_frames_de_la_animacion_actual) _numero_de_frame_actual = 5; // Congela en el frame 5
        break;
    }
}

///=============================================================///
///   #10 - ACTUALIZAR RECORTE DEL SPRITE
///=============================================================///
// #10
void Personaje::actualizar_el_recorte_del_sprite_segun_la_animacion() {
    int fila_del_spritesheet = 0; // Numero de fila en la imagen grande
    EstadoDeAnimacionDelPersonaje animacion_a_usar_para_calcular_la_fila = _estado_de_animacion_actual; // Estado que define la fila

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::QUIETO ||
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO ||
        _estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::ESQUIVANDO) {
        animacion_a_usar_para_calcular_la_fila = EstadoDeAnimacionDelPersonaje::CAMINANDO; // Usa la misma fila que caminar
    }

    if (animacion_a_usar_para_calcular_la_fila == EstadoDeAnimacionDelPersonaje::LASTIMADO ||
        animacion_a_usar_para_calcular_la_fila == EstadoDeAnimacionDelPersonaje::MUERTO) {
        fila_del_spritesheet = 20; // Las animaciones de dano y muerte estan en la fila 20
    }
    else {
        fila_del_spritesheet = (int)animacion_a_usar_para_calcular_la_fila * 4 + (int)_direccion_hacia_donde_mira; // Fila = animacion * 4 + direccion
    }

    int columna_del_spritesheet = _numero_de_frame_actual; // La columna es el frame actual

    _sprite_de_la_entidad.setTextureRect(sf::IntRect(columna_del_spritesheet * 64, fila_del_spritesheet * 64, 64, 64)); // Recorta el frame correcto de la imagen
}

///=============================================================///
///   #11 - DIBUJAR
///=============================================================///
// #11
void Personaje::dibujar(sf::RenderWindow& ventana_del_juego) {
    for (int b = 0; b < CANTIDAD_MAXIMA_DE_BOLAS; b++) {
        _bolas_de_fuego[b].dibujar(ventana_del_juego); // Dibuja cada bola activa del pool
    }

    if (_estado_de_animacion_actual == EstadoDeAnimacionDelPersonaje::APUNTANDO) { // Si esta apuntando
        ventana_del_juego.draw(_circulo_que_muestra_el_alcance); // Muestra el circulo de rango
    }

    dibujar_sprite_y_barra_de_vida(ventana_del_juego); // Dibuja el personaje y su barra de vida
}
