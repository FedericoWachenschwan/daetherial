#include "UI_CreadorItems.h"
#include <iostream>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
UI_CreadorItems::UI_CreadorItems() {
    _estado_actual = EstadoDelCreadorDeItems::SELECCIONANDO_SPRITE;
    _id_de_la_textura_actual = 0;
    _campo_activo_del_formulario = 0;
    _se_solicito_guardar = false;
    _cantidad_de_items_en_la_lista = 0;

    if (_fuente.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR: NO SE ENCONTRO LA FUENTE." << std::endl;
    }

    _nombres_de_los_campos[0] = "1. ID Base de Datos:";
    _nombres_de_los_campos[1] = "2. Nombre del Item:";
    _nombres_de_los_campos[2] = "3. Tipo (1:Vida, 2:Equipamiento, 3:Recurso, 4:Mueble, 5:Mana):";
    _nombres_de_los_campos[3] = "4. Efecto (Danio/Cura):";
    _nombres_de_los_campos[4] = "5. Precio:";
    _nombres_de_los_campos[5] = "6. Rareza (0 a 3):";

    for (int i = 0; i < CANTIDAD_DE_CAMPOS_DEL_FORMULARIO; i++) {
        _textos_que_escribio_el_usuario[i] = "";
    }
}

///=============================================================///
///   CONVERTIR TEXTO A NUMERO
///=============================================================///
int UI_CreadorItems::convertir_texto_a_numero_entero(const std::string& texto) const {

    int numero_resultado = 0;
    int cantidad_de_caracteres = (int)texto.length(); // Convertimos a int para que la comparacion de abajo sea entre dos numeros del mismo tipo

    for (int i = 0; i < cantidad_de_caracteres; i++) {
        char caracter_actual = texto[i];
        int valor_de_este_digito = caracter_actual - '0';
        numero_resultado = numero_resultado * 10 + valor_de_este_digito;
    }

    return numero_resultado;
}

///=============================================================///
///   ACTUALIZAR SPRITE DE VISTA PREVIA
///=============================================================///
void UI_CreadorItems::actualizar_sprite_de_vista_previa(const sf::Texture& textura_maestra) {
    const int TAMANO_DEL_TILE = 32;
    const int CANTIDAD_DE_COLUMNAS = 64;

    int columna = _id_de_la_textura_actual % CANTIDAD_DE_COLUMNAS;
    int fila = _id_de_la_textura_actual / CANTIDAD_DE_COLUMNAS;

    _vista_previa_del_sprite.setTexture(textura_maestra);
    _vista_previa_del_sprite.setTextureRect(sf::IntRect(columna * TAMANO_DEL_TILE, fila * TAMANO_DEL_TILE, TAMANO_DEL_TILE, TAMANO_DEL_TILE));

    _vista_previa_del_sprite.setScale(5.f, 5.f);
    _vista_previa_del_sprite.setPosition(640.f - (TAMANO_DEL_TILE * 2.5f), 150.f);
}

///=============================================================///
///   PROCESAR EVENTOS
///=============================================================///
void UI_CreadorItems::procesar_eventos(sf::Event& evento, const FabricaDeItems& fabrica_de_items) {

    if (_estado_actual == EstadoDelCreadorDeItems::SELECCIONANDO_SPRITE && evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Right) {
            _id_de_la_textura_actual++;
            if (_id_de_la_textura_actual >= CANTIDAD_MAXIMA_DE_TEXTURAS) _id_de_la_textura_actual = 0;
        }
        else if (evento.key.code == sf::Keyboard::Left) {
            _id_de_la_textura_actual--;
            if (_id_de_la_textura_actual < 0) _id_de_la_textura_actual = CANTIDAD_MAXIMA_DE_TEXTURAS - 1;
        }
        else if (evento.key.code == sf::Keyboard::Enter) {
            _estado_actual = EstadoDelCreadorDeItems::LLENANDO_FORMULARIO;
            _campo_activo_del_formulario = 0;
        }
        else if (evento.key.code == sf::Keyboard::L) {
            _estado_actual = EstadoDelCreadorDeItems::MOSTRANDO_LISTA;
            _cantidad_de_items_en_la_lista = fabrica_de_items.leer_todos_los_registros_activos(_lista_de_items_para_mostrar);
        }
    }
    else if (_estado_actual == EstadoDelCreadorDeItems::MOSTRANDO_LISTA && evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Escape) {
            _estado_actual = EstadoDelCreadorDeItems::SELECCIONANDO_SPRITE;
        }
    }
    else if (_estado_actual == EstadoDelCreadorDeItems::LLENANDO_FORMULARIO) {
        if (evento.type == sf::Event::KeyPressed) {
            if (evento.key.code == sf::Keyboard::Down) {
                _campo_activo_del_formulario = (_campo_activo_del_formulario + 1) % CANTIDAD_DE_CAMPOS_DEL_FORMULARIO;
            }
            else if (evento.key.code == sf::Keyboard::Up) {
                _campo_activo_del_formulario = (_campo_activo_del_formulario - 1 + CANTIDAD_DE_CAMPOS_DEL_FORMULARIO) % CANTIDAD_DE_CAMPOS_DEL_FORMULARIO;
            }
            else if (evento.key.code == sf::Keyboard::Escape) {
                _estado_actual = EstadoDelCreadorDeItems::SELECCIONANDO_SPRITE;
            }
            else if (evento.key.code == sf::Keyboard::Enter) {
                _se_solicito_guardar = true;
            }
        }

        if (evento.type == sf::Event::TextEntered) {
            sf::Uint32 codigo_unicode_del_caracter = evento.text.unicode;

            if (codigo_unicode_del_caracter == 8) {
                if (_textos_que_escribio_el_usuario[_campo_activo_del_formulario] == "") {
                    // No hay nada para borrar, no hacemos nada
                }
                else {
                    _textos_que_escribio_el_usuario[_campo_activo_del_formulario].pop_back();
                }
            }
            else if (codigo_unicode_del_caracter >= 32 && codigo_unicode_del_caracter <= 126) {
                char caracter_escrito = (char)codigo_unicode_del_caracter;

                bool este_campo_solo_acepta_numeros = (_campo_activo_del_formulario != 1);
                bool el_caracter_no_es_un_numero = (caracter_escrito < '0' || caracter_escrito > '9');

                if (este_campo_solo_acepta_numeros == true && el_caracter_no_es_un_numero == true) {
                    return;
                }

                if (_textos_que_escribio_el_usuario[_campo_activo_del_formulario].size() < 25) {
                    _textos_que_escribio_el_usuario[_campo_activo_del_formulario] += caracter_escrito;
                }
            }
        }
    }
}

///=============================================================///
///   DIBUJAR
///=============================================================///
void UI_CreadorItems::dibujar(sf::RenderWindow& ventana_del_juego) {
    ventana_del_juego.clear(sf::Color(20, 20, 30));

    if (_estado_actual == EstadoDelCreadorDeItems::MOSTRANDO_LISTA) {

        sf::Text subtitulo("ITEMS EN BASE DE DATOS (.DAT)", _fuente, 35);
        subtitulo.setPosition(300.f, 30.f);
        subtitulo.setFillColor(sf::Color::Yellow);
        ventana_del_juego.draw(subtitulo);

        sf::Text cabecera("ID     NOMBRE               TIPO   EFECTO   PRECIO   RAREZA", _fuente, 22);
        cabecera.setPosition(150.f, 120.f);
        cabecera.setFillColor(sf::Color::Green);
        ventana_del_juego.draw(cabecera);

        float posicion_y_inicial = 170.f;

        for (int i = 0; i < _cantidad_de_items_en_la_lista; i++) {

            if (i > 15) break;

            const RegistroDeItem& item_de_esta_fila = _lista_de_items_para_mostrar[i];

            std::string linea_de_texto = std::to_string(item_de_esta_fila.id) + "      " +
                item_de_esta_fila.nombre + "              " +
                std::to_string(item_de_esta_fila.tipo_de_item) + "      " +
                std::to_string(item_de_esta_fila.valor_del_efecto) + "       " +
                std::to_string(item_de_esta_fila.precio) + "       " +
                std::to_string(item_de_esta_fila.rareza);

            sf::Text texto_de_la_linea(linea_de_texto, _fuente, 20);
            texto_de_la_linea.setPosition(150.f, posicion_y_inicial + (i * 30.f));
            texto_de_la_linea.setFillColor(sf::Color::Cyan);
            ventana_del_juego.draw(texto_de_la_linea);
        }

        sf::Text instrucciones_de_la_lista("Presiona ESCAPE para volver al Creador.", _fuente, 20);
        instrucciones_de_la_lista.setPosition(400.f, 650.f);
        instrucciones_de_la_lista.setFillColor(sf::Color(150, 150, 150));
        ventana_del_juego.draw(instrucciones_de_la_lista);
    }
    else {
        sf::Text titulo("CREADOR DE ITEMS", _fuente, 40);
        titulo.setPosition(480.f, 30.f);
        titulo.setFillColor(sf::Color::Yellow);
        ventana_del_juego.draw(titulo);

        ventana_del_juego.draw(_vista_previa_del_sprite);

        if (_estado_actual == EstadoDelCreadorDeItems::SELECCIONANDO_SPRITE) {
            sf::Text instrucciones("Usa Izq/Der para texturas  |  Presiona L para ver Lista\n(ID Actual: " + std::to_string(_id_de_la_textura_actual) + ")\nPresiona ENTER para elegir.", _fuente, 24);
            instrucciones.setPosition(320.f, 400.f);
            ventana_del_juego.draw(instrucciones);
        }
        else if (_estado_actual == EstadoDelCreadorDeItems::LLENANDO_FORMULARIO) {

            float posicion_y_inicial = 350.f;

            for (int i = 0; i < CANTIDAD_DE_CAMPOS_DEL_FORMULARIO; i++) {

                sf::Text etiqueta_del_campo(_nombres_de_los_campos[i], _fuente, 24);
                etiqueta_del_campo.setPosition(250.f, posicion_y_inicial + (i * 40.f));

                sf::Text texto_ingresado(_textos_que_escribio_el_usuario[i], _fuente, 24);
                texto_ingresado.setPosition(650.f, posicion_y_inicial + (i * 40.f));
                texto_ingresado.setFillColor(sf::Color::Cyan);

                if (i == _campo_activo_del_formulario) {
                    etiqueta_del_campo.setFillColor(sf::Color::Green);
                    texto_ingresado.setString(_textos_que_escribio_el_usuario[i] + "_");
                }

                ventana_del_juego.draw(etiqueta_del_campo);
                ventana_del_juego.draw(texto_ingresado);
            }

            sf::Text instrucciones_del_formulario("Flechas Arriba/Abajo para moverse. ESC para cambiar textura. ENTER para GUARDAR.", _fuente, 20);
            instrucciones_del_formulario.setPosition(200.f, 650.f);
            instrucciones_del_formulario.setFillColor(sf::Color(150, 150, 150));
            ventana_del_juego.draw(instrucciones_del_formulario);
        }
    }
}

///=============================================================///
///   GENERAR REGISTRO CON LOS DATOS INGRESADOS
///=============================================================///
RegistroDeItem UI_CreadorItems::generar_registro_con_los_datos_ingresados() const {
    RegistroDeItem registro_nuevo{};

    if (_textos_que_escribio_el_usuario[0] == "") {
        registro_nuevo.id = 0;
    }
    else {
        registro_nuevo.id = convertir_texto_a_numero_entero(_textos_que_escribio_el_usuario[0]);
    }

    std::string nombre_ingresado = _textos_que_escribio_el_usuario[1];
    int cantidad_de_letras_del_nombre = (int)nombre_ingresado.length();

    if (cantidad_de_letras_del_nombre > 29) {
        cantidad_de_letras_del_nombre = 29;
    }

    for (int i = 0; i < 30; i++) {
        registro_nuevo.nombre[i] = '\0';
    }
    for (int i = 0; i < cantidad_de_letras_del_nombre; i++) {
        registro_nuevo.nombre[i] = nombre_ingresado[i];
    }

    if (_textos_que_escribio_el_usuario[2] == "") {
        registro_nuevo.tipo_de_item = 0;
    }
    else {
        registro_nuevo.tipo_de_item = convertir_texto_a_numero_entero(_textos_que_escribio_el_usuario[2]);
    }

    if (_textos_que_escribio_el_usuario[3] == "") {
        registro_nuevo.valor_del_efecto = 0;
    }
    else {
        registro_nuevo.valor_del_efecto = convertir_texto_a_numero_entero(_textos_que_escribio_el_usuario[3]);
    }

    if (_textos_que_escribio_el_usuario[4] == "") {
        registro_nuevo.precio = 0;
    }
    else {
        registro_nuevo.precio = convertir_texto_a_numero_entero(_textos_que_escribio_el_usuario[4]);
    }

    if (_textos_que_escribio_el_usuario[5] == "") {
        registro_nuevo.rareza = 0;
    }
    else {
        registro_nuevo.rareza = convertir_texto_a_numero_entero(_textos_que_escribio_el_usuario[5]);
    }

    registro_nuevo.id_de_la_textura = _id_de_la_textura_actual;
    registro_nuevo.esta_activo = true;

    return registro_nuevo;
}

///=============================================================///
///   CONFIRMAR GUARDADO
///=============================================================///
void UI_CreadorItems::confirmar_guardado() {
    _se_solicito_guardar = false;
    _estado_actual = EstadoDelCreadorDeItems::SELECCIONANDO_SPRITE;
    for (int i = 0; i < CANTIDAD_DE_CAMPOS_DEL_FORMULARIO; i++) {
        _textos_que_escribio_el_usuario[i] = "";
    }
}