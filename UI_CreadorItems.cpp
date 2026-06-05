#include "UI_CreadorItems.h"
#include <iostream>

UI_CreadorItems::UI_CreadorItems() {
    _estado = EstadoCreador::SELECCIONANDO_SPRITE;
    _idTexturaActual = 0;
    _campoActivo = 0;
    _guardarSolicitado = false;

    if (!_fuente.loadFromFile("assets/NorthEternal.otf")) {
        std::cout << "❌ ERROR FATAL: No se encontro la fuente!" << std::endl;
    }

    // Preparamos los textos del formulario
    _nombresCampos = {
        "1. ID Base de Datos:",
        "2. Nombre del Item:",
        "3. Tipo (1:Consumible, 2:Equipamiento, 3:Recurso, 4:Mueble):",
        "4. Efecto (Danio/Cura):",
        "5. Precio:",
        "6. Rareza (0 a 3):"
    };
    _inputsUsuario = { "", "", "", "", "", "" }; // Arrancan vacíos
}

void UI_CreadorItems::actualizarSprite(const sf::Texture& texturaMaestra) { // Calcula la posición del sprite dentro del mega-spritesheet
    const int TILE_SIZE = 32;
    const int COLUMNAS = 64;

    int col = _idTexturaActual % COLUMNAS;
    int fila = _idTexturaActual / COLUMNAS;

    _spritePreview.setTexture(texturaMaestra);
	_spritePreview.setTextureRect(sf::IntRect(col * TILE_SIZE, fila * TILE_SIZE, TILE_SIZE, TILE_SIZE)); // Solo muestra el tile seleccionado del mega-spritesheet

    // Lo hacemos x5 más grande y lo centramos (Asumiendo pantalla 1280x720)
    _spritePreview.setScale(5.f, 5.f);
    _spritePreview.setPosition(640.f - (TILE_SIZE * 2.5f), 150.f);
}

void UI_CreadorItems::procesarEventos(sf::Event& evento, const ItemManager& itemManager) {

    // ========================================================
    // MODO 1: ELIGIENDO EL DIBUJITO (Carrusel)
    // ========================================================
    if (_estado == EstadoCreador::SELECCIONANDO_SPRITE && evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Right) {
            _idTexturaActual++;
            if (_idTexturaActual >= MAX_TEXTURAS) _idTexturaActual = 0;
        }
        else if (evento.key.code == sf::Keyboard::Left) {
            _idTexturaActual--;
            if (_idTexturaActual < 0) _idTexturaActual = MAX_TEXTURAS - 1;
        }
        else if (evento.key.code == sf::Keyboard::Enter) {
            _estado = EstadoCreador::LLENANDO_FORMULARIO;
            _campoActivo = 0;
        }
        // 🌟 SI APRETA LA 'L', LLAMA A TU FUNCIÓN LEERTODOS()
        else if (evento.key.code == sf::Keyboard::L) {
            _estado = EstadoCreador::MOSTRANDO_LISTA;
            _listaItems = itemManager.leerTodos();
        }
    }
    // ========================================================
    // MODO 3: MOSTRANDO LA LISTA DE ITEMS (.DAT)
    // ========================================================
    else if (_estado == EstadoCreador::MOSTRANDO_LISTA && evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Escape) {
            _estado = EstadoCreador::SELECCIONANDO_SPRITE; // Volver al carrusel
        }
    }
    // ========================================================
    // MODO 2: LLENANDO EL FORMULARIO (Tipeando)
    // ========================================================
    else if (_estado == EstadoCreador::LLENANDO_FORMULARIO) {
        if (evento.type == sf::Event::KeyPressed) {
            if (evento.key.code == sf::Keyboard::Down) {
                _campoActivo = (_campoActivo + 1) % _nombresCampos.size();
            }
            else if (evento.key.code == sf::Keyboard::Up) {
                _campoActivo = (_campoActivo - 1 + _nombresCampos.size()) % _nombresCampos.size();
            }
            else if (evento.key.code == sf::Keyboard::Escape) {
                _estado = EstadoCreador::SELECCIONANDO_SPRITE;
            }
            else if (evento.key.code == sf::Keyboard::Enter) {
                _guardarSolicitado = true;
            }
        }

        if (evento.type == sf::Event::TextEntered) {
            sf::Uint32 codigoUnicode = evento.text.unicode;
            if (codigoUnicode == 8) {
                if (!_inputsUsuario[_campoActivo].empty()) {
                    _inputsUsuario[_campoActivo].pop_back();
                }
            }
            else if (codigoUnicode >= 32 && codigoUnicode <= 126) {
                char caracter = static_cast<char>(codigoUnicode);
                if (_campoActivo != 1 && (caracter < '0' || caracter > '9')) return;
                if (_inputsUsuario[_campoActivo].size() < 25) {
                    _inputsUsuario[_campoActivo] += caracter;
                }
            }
        }
    }
}

void UI_CreadorItems::dibujar(sf::RenderWindow& ventana) {
    ventana.clear(sf::Color(20, 20, 30));

    // Si estamos mostrando la lista, cambia por completo lo que se dibuja
    if (_estado == EstadoCreador::MOSTRANDO_LISTA) {
        sf::Text subTitulo("ITEMS EN BASE DE DATOS (.DAT)", _fuente, 35);
        subTitulo.setPosition(300.f, 30.f);
        subTitulo.setFillColor(sf::Color::Yellow);
        ventana.draw(subTitulo);

        // Cabecera tipo tabla alineada
        sf::Text cabecera("ID     NOMBRE               TIPO   EFECTO   PRECIO   RAREZA", _fuente, 22);
        cabecera.setPosition(150.f, 120.f);
        cabecera.setFillColor(sf::Color::Green);
        ventana.draw(cabecera);

        float startY = 170.f;
        // Listamos los ítems guardados en el vector cache
        for (size_t i = 0; i < _listaItems.size(); ++i) {
            const auto& item = _listaItems[i];

            // Limitamos a 10 ítems por pantalla para que no se rebase el alto (después podés meter scroll)
            if (i > 15) break;

            // Construimos la línea de texto formateada de forma compacta
            std::string linea = std::to_string(item.id) + "      " +
                item.nombre + "              " +
                std::to_string(item.tipoItem) + "      " +
                std::to_string(item.valorEfecto) + "       " +
                std::to_string(item.precio) + "       " +
                std::to_string(item.rareza);

            sf::Text textoLinea(linea, _fuente, 20);
            textoLinea.setPosition(150.f, startY + (i * 30.f));
            textoLinea.setFillColor(sf::Color::Cyan);
            ventana.draw(textoLinea);
        }

        sf::Text instLista("Presiona ESCAPE para volver al Creador.", _fuente, 20);
        instLista.setPosition(400.f, 650.f);
        instLista.setFillColor(sf::Color(150, 150, 150));
        ventana.draw(instLista);
    }
    // Si no está en modo lista, dibuja el carrusel o el formulario de siempre
    else {
        sf::Text titulo("CREADOR DE ITEMS", _fuente, 40);
        titulo.setPosition(480.f, 30.f);
        titulo.setFillColor(sf::Color::Yellow);
        ventana.draw(titulo);

        ventana.draw(_spritePreview);

        if (_estado == EstadoCreador::SELECCIONANDO_SPRITE) {
            // 🌟 Agregamos la instrucción de la 'L' en la UI
            sf::Text inst("Usa Izq/Der para texturas  |  Presiona L para ver Lista\n(ID Actual: " + std::to_string(_idTexturaActual) + ")\nPresiona ENTER para elegir.", _fuente, 24);
            inst.setPosition(320.f, 400.f);
            ventana.draw(inst);
        }
        else if (_estado == EstadoCreador::LLENANDO_FORMULARIO) {
            float startY = 350.f;
            for (size_t i = 0; i < _nombresCampos.size(); ++i) {
                sf::Text etiqueta(_nombresCampos[i], _fuente, 24);
                etiqueta.setPosition(250.f, startY + (i * 40.f));

                sf::Text inputTexto(_inputsUsuario[i], _fuente, 24);
                inputTexto.setPosition(650.f, startY + (i * 40.f));
                inputTexto.setFillColor(sf::Color::Cyan);

                if (i == _campoActivo) {
                    etiqueta.setFillColor(sf::Color::Green);
                    inputTexto.setString(_inputsUsuario[i] + "_");
                }
                ventana.draw(etiqueta);
                ventana.draw(inputTexto);
            }

            sf::Text instForm("Flechas Arriba/Abajo para moverse. ESC para cambiar textura. ENTER para GUARDAR.", _fuente, 20);
            instForm.setPosition(200.f, 650.f);
            instForm.setFillColor(sf::Color(150, 150, 150));
            ventana.draw(instForm);
        }
    }
}

ItemReg UI_CreadorItems::generarRegistro() const {
    ItemReg nuevo{};
    // stoi convierte el string (ej: "15") a un int (15)
    nuevo.id = _inputsUsuario[0].empty() ? 0 : std::stoi(_inputsUsuario[0]);

    std::string nombreIngresado = _inputsUsuario[1];
    // Copiamos el string al array de chars del struct de forma segura
    strncpy_s(nuevo.nombre, sizeof(nuevo.nombre), nombreIngresado.c_str(), _TRUNCATE);

    nuevo.tipoItem = _inputsUsuario[2].empty() ? 0 : std::stoi(_inputsUsuario[2]);
    nuevo.valorEfecto = _inputsUsuario[3].empty() ? 0 : std::stoi(_inputsUsuario[3]);
    nuevo.precio = _inputsUsuario[4].empty() ? 0 : std::stoi(_inputsUsuario[4]);
    nuevo.rareza = _inputsUsuario[5].empty() ? 0 : std::stoi(_inputsUsuario[5]);

    nuevo.idTextura = _idTexturaActual;
    nuevo.activo = true;

    return nuevo;
}

void UI_CreadorItems::confirmarGuardado() {
    _guardarSolicitado = false;
    _estado = EstadoCreador::SELECCIONANDO_SPRITE;
    _inputsUsuario = { "", "", "", "", "", "" }; // Vaciamos para el próximo ítem
}