#include <iostream>
#include <fstream>
#include <string>
#include "usuario.h"
#include "plataforma.h"

using namespace std;

// convertir a entero pequeno sin excepciones ni librerias adicionales
static bool convertirNumeroHasta20(const string& texto, int& numero)
{
    if (texto.empty())
    {
        return false;
    }

    numero = 0;

    for (int i = 0; i < static_cast<int>(texto.size()); i++)
    {
        if (texto[i] < '0' || texto[i] > '9')
        {
            return false;
        }

        numero = numero * 10 + (texto[i] - '0');

        if (numero > 20)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    const string rutaUsuarios = "usuarios.txt";
    const string rutaAmistades = "amistades.txt";
    const string rutaSolicitudes = "solicitudes.txt";
    const string rutaMensajes = "mensajes.txt";

    Plataforma plataforma;
    ifstream archivoUsuarios(rutaUsuarios);
    bool existenUsuarios = archivoUsuarios.is_open();
    archivoUsuarios.close();

    if (existenUsuarios)
    {
        if (!plataforma.cargarUsuarios(rutaUsuarios))
        {
            cout << "Error al cargar usuarios." << endl;
            return 1;
        }
    }
    else
    {
        plataforma.agregarUsuario(Usuario("juanito76"));
        plataforma.agregarUsuario(Usuario("pedrito89"));

        Usuario laura(
            "laura25",
            "2002-06-18",
            "laura@ejemplo.com",
            "/users/udeabook/laura25/albums/profile.png",
            "clave123",
            "Medellin",
            "Colombia",
            "2026-10-05"
            );
        plataforma.agregarUsuario(laura);

        if (!plataforma.guardarUsuarios(rutaUsuarios))
        {
            cout << "Error al crear usuarios.txt." << endl;
            return 1;
        }
    }

    ifstream archivoAmistades(rutaAmistades);
    bool existenAmistades = archivoAmistades.is_open();
    archivoAmistades.close();

    if (existenAmistades)
    {
        if (!plataforma.cargarAmistades(rutaAmistades))
        {
            cout << "Error al cargar amistades." << endl;
            return 1;
        }
    }
    else if (!plataforma.guardarAmistades(rutaAmistades))
    {
        cout << "Error al crear amistades.txt." << endl;
        return 1;
    }

    ifstream archivoSolicitudes(rutaSolicitudes);
    bool existenSolicitudes = archivoSolicitudes.is_open();
    archivoSolicitudes.close();

    if (existenSolicitudes)
    {
        if (!plataforma.cargarSolicitudes(rutaSolicitudes))
        {
            cout << "Error al cargar solicitudes." << endl;
            return 1;
        }
    }
    else if (!plataforma.guardarSolicitudes(rutaSolicitudes))
    {
        cout << "Error al crear solicitudes.txt." << endl;
        return 1;
    }

    // recuperar mensajes guardados, o crear archivo vacio
    ifstream archivoMensajes(rutaMensajes);
    bool existenMensajes = archivoMensajes.is_open();
    archivoMensajes.close();

    if (existenMensajes)
    {
        if (!plataforma.cargarMensajes(rutaMensajes))
        {
            cout << "Error al cargar mensajes." << endl;
            return 1;
        }
    }
    else if (!plataforma.guardarMensajes(rutaMensajes))
    {
        cout << "Error al crear mensajes.txt." << endl;
        return 1;
    }

    cout << "===== Ude@Book - Mensajes y sugerencias =====" << endl;
    cout << "Usuarios cargados: " << plataforma.getCantidadUsuarios() << endl;

    string opcion;
    do
    {
        cout << "\n1. Registrar usuario" << endl;
        cout << "2. Enviar solicitud" << endl;
        cout << "3. Ver solicitudes recibidas" << endl;
        cout << "4. Aceptar solicitud" << endl;
        cout << "5. Rechazar solicitud" << endl;
        cout << "6. Consultar amistad" << endl;
        cout << "7. Contar amigos" << endl;
        cout << "8. Listar usuarios" << endl;
        cout << "9. Enviar mensaje" << endl;
        cout << "10. Leer nuevos mensajes" << endl;
        cout << "11. Buscar mensajes recibidos por fechas" << endl;
        cout << "12. Ver mensajes enviados" << endl;
        cout << "13. Ver sugerencias de amistad" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        getline(cin, opcion);

        if (opcion == "1")
        {
            string nombre;
            cout << "Nombre de usuario: ";
            getline(cin, nombre);

            if (plataforma.agregarUsuario(Usuario(nombre)))
            {
                if (plataforma.guardarUsuarios(rutaUsuarios))
                {
                    cout << "Usuario registrado y guardado." << endl;
                }
                else
                {
                    cout << "Usuario creado en RAM, pero no se guardo." << endl;
                }
            }
            else
            {
                cout << "Nombre vacio o repetido." << endl;
            }
        }
        else if (opcion == "2")
        {
            string emisor, receptor;
            cout << "Quien envia: ";
            getline(cin, emisor);
            cout << "Quien recibe: ";
            getline(cin, receptor);

            if (plataforma.enviarSolicitud(emisor, receptor))
            {
                if (plataforma.guardarSolicitudes(rutaSolicitudes))
                {
                    cout << "Solicitud enviada y guardada." << endl;
                }
                else
                {
                    cout << "Solicitud creada en RAM, pero no se guardo." << endl;
                }
            }
            else
            {
                cout << "No se puede enviar: revisa usuarios, amistad o solicitudes pendientes." << endl;
            }
        }
        else if (opcion == "3")
        {
            string receptor;
            cout << "Usuario que consulta: ";
            getline(cin, receptor);
            int total = plataforma.contarSolicitudesRecibidas(receptor);

            if (total == -1)
            {
                cout << "Ese usuario no existe." << endl;
            }
            else if (total == 0)
            {
                cout << "No tiene solicitudes pendientes." << endl;
            }
            else
            {
                cout << "Solicitudes pendientes: " << total << endl;
                for (int i = 0; i < total; i++)
                {
                    const SolicitudAmistad* solicitud =
                        plataforma.obtenerSolicitudRecibida(receptor, i);
                    if (solicitud != nullptr)
                    {
                        cout << i << ". De " << solicitud->getEmisor() << endl;
                    }
                }
            }
        }
        else if (opcion == "4" || opcion == "5")
        {
            string receptor, emisor;
            cout << "Usuario que recibio la solicitud: ";
            getline(cin, receptor);
            cout << "Usuario que la envio: ";
            getline(cin, emisor);

            bool realizada = false;
            if (opcion == "4")
            {
                realizada = plataforma.aceptarSolicitud(receptor, emisor);
            }
            else
            {
                realizada = plataforma.rechazarSolicitud(receptor, emisor);
            }

            if (!realizada)
            {
                cout << "No existe esa solicitud pendiente." << endl;
            }
            else
            {
                bool guardada = plataforma.guardarSolicitudes(rutaSolicitudes);
                if (opcion == "4")
                {
                    guardada = plataforma.guardarAmistades(rutaAmistades) && guardada;
                }

                if (guardada)
                {
                    cout << (opcion == "4" ? "Solicitud aceptada. Ahora son amigos."
                                           : "Solicitud rechazada.") << endl;
                }
                else
                {
                    cout << "Cambio realizado en RAM, pero fallo al guardar." << endl;
                }
            }
        }
        else if (opcion == "6")
        {
            string primero, segundo;
            cout << "Primer usuario: ";
            getline(cin, primero);
            cout << "Segundo usuario: ";
            getline(cin, segundo);

            if (plataforma.buscarUsuario(primero) == nullptr ||
                plataforma.buscarUsuario(segundo) == nullptr)
            {
                cout << "Uno de los usuarios no existe." << endl;
            }
            else
            {
                cout << (plataforma.sonAmigos(primero, segundo) ?
                             "Si son amigos." : "No son amigos.") << endl;
            }
        }
        else if (opcion == "7")
        {
            string nombre;
            cout << "Usuario: ";
            getline(cin, nombre);
            int total = plataforma.contarAmigos(nombre);
            if (total == -1)
            {
                cout << "Ese usuario no existe." << endl;
            }
            else
            {
                cout << "Cantidad de amigos: " << total << endl;
            }
        }
        else if (opcion == "8")
        {
            cout << "Usuarios registrados:" << endl;
            for (int i = 0; i < plataforma.getCantidadUsuarios(); i++)
            {
                const Usuario* usuario = plataforma.obtenerUsuario(i);
                cout << i << ". " << usuario->getNombreUsuario() << endl;
            }
        }
        else if (opcion == "9")
        {
            string emisor, receptor, texto;
            cout << "Quien envia: ";
            getline(cin, emisor);
            cout << "Quien recibe: ";
            getline(cin, receptor);
            cout << "Mensaje (hasta 512 caracteres): ";
            getline(cin, texto);

            if (plataforma.enviarMensaje(emisor, receptor, texto))
            {
                if (plataforma.guardarMensajes(rutaMensajes))
                {
                    cout << "Mensaje enviado y guardado." << endl;
                }
                else
                {
                    cout << "Mensaje creado en RAM, pero fallo al guardar." << endl;
                }
            }
            else
            {
                cout << "No se pudo enviar: deben ser amigos y el texto no puede"
                     << " estar vacio ni superar 512 caracteres." << endl;
            }
        }
        else if (opcion == "10")
        {
            string receptor;
            cout << "Usuario que consulta: ";
            getline(cin, receptor);

            if (plataforma.buscarUsuario(receptor) == nullptr)
            {
                cout << "Ese usuario no existe." << endl;
            }
            else
            {
                int total = 0;
                for (int i = 0; i < plataforma.getCantidadMensajes(); i++)
                {
                    const Mensaje* mensaje = plataforma.obtenerMensaje(i);
                    if (mensaje->getReceptor() == receptor &&
                        !mensaje->estaLeido())
                    {
                        cout << "De: " << mensaje->getEmisor() << endl;
                        cout << "Fecha: " << mensaje->getFechaEnvio() << endl;
                        cout << "Texto: " << mensaje->getTexto() << endl;
                        cout << "--------------------" << endl;
                        total++;
                    }
                }

                if (total == 0)
                {
                    cout << "No hay mensajes nuevos." << endl;
                }
                else
                {
                    plataforma.marcarMensajesLeidos(receptor);
                    if (plataforma.guardarMensajes(rutaMensajes))
                    {
                        cout << "Mensajes leidos y guardados: " << total << endl;
                    }
                    else
                    {
                        cout << "Se marcaron como leidos en RAM, pero fallo"
                             << " el guardado." << endl;
                    }
                }
            }
        }
        else if (opcion == "11")
        {
            string receptor, inicio, fin;
            cout << "Usuario que consulta: ";
            getline(cin, receptor);
            cout << "Fecha inicio (AAAA-MM-DD): ";
            getline(cin, inicio);
            cout << "Fecha fin (AAAA-MM-DD): ";
            getline(cin, fin);

            if (plataforma.buscarUsuario(receptor) == nullptr)
            {
                cout << "Ese usuario no existe." << endl;
            }
            else if (!Mensaje::fechaValida(inicio) ||
                     !Mensaje::fechaValida(fin) || inicio > fin)
            {
                cout << "Rango de fechas no valido." << endl;
            }
            else
            {
                int total = 0;
                for (int i = 0; i < plataforma.getCantidadMensajes(); i++)
                {
                    const Mensaje* mensaje = plataforma.obtenerMensaje(i);
                    string fecha = mensaje->getFechaEnvio().substr(0, 10);

                    if (mensaje->getReceptor() == receptor &&
                        fecha >= inicio && fecha <= fin)
                    {
                        cout << "De: " << mensaje->getEmisor() << endl;
                        cout << "Fecha: " << mensaje->getFechaEnvio() << endl;
                        cout << "Texto: " << mensaje->getTexto() << endl;
                        cout << "Estado: "
                             << (mensaje->estaLeido() ? "Leido" : "No leido")
                             << endl;
                        cout << "--------------------" << endl;
                        total++;
                    }
                }
                cout << "Mensajes encontrados: " << total << endl;
            }
        }
        else if (opcion == "12")
        {
            string emisor;
            cout << "Usuario que consulta: ";
            getline(cin, emisor);
            if (plataforma.buscarUsuario(emisor) == nullptr)
            {
                cout << "Ese usuario no existe." << endl;
            }
            else
            {
                int total = 0;
                for (int i = 0; i < plataforma.getCantidadMensajes(); i++)
                {
                    const Mensaje* mensaje = plataforma.obtenerMensaje(i);
                    if (mensaje->getEmisor() == emisor)
                    {
                        cout << "Para: " << mensaje->getReceptor() << endl;
                        cout << "Fecha: " << mensaje->getFechaEnvio() << endl;
                        cout << "Texto: " << mensaje->getTexto() << endl;
                        cout << "Estado: "
                             << (mensaje->estaLeido() ? "Leido" : "No leido")
                             << endl;
                        cout << "--------------------" << endl;
                        total++;
                    }
                }
                cout << "Mensajes enviados: " << total << endl;
            }
        }
        else if (opcion == "13")
        {
            string nombre;
            string entradaK;
            int k = 0;

            cout << "Usuario que consulta: ";
            getline(cin, nombre);
            cout << "Cuantas sugerencias desea (1 a 20): ";
            getline(cin, entradaK);

            if (!convertirNumeroHasta20(entradaK, k) || k < 1)
            {
                cout << "K debe ser un entero de 1 a 20." << endl;
            }
            else if (plataforma.buscarUsuario(nombre) == nullptr)
            {
                cout << "Ese usuario no existe." << endl;
            }
            else
            {
                // dos arreglos dinamicos para resultados y coincidencias
                int* indices = new int[k];
                int* coincidencias = new int[k];

                int total = plataforma.generarSugerencias(
                    nombre, k, indices, coincidencias);

                if (total <= 0)
                {
                    cout << "No hay sugerencias disponibles." << endl;
                }
                else
                {
                    cout << "Sugerencias encontradas: " << total << endl;

                    for (int i = 0; i < total; i++)
                    {
                        const Usuario* sugerido =
                            plataforma.obtenerUsuario(indices[i]);

                        cout << i << ". " << sugerido->getNombreUsuario()
                             << " - amigos en comun: "
                             << coincidencias[i] << endl;
                    }

                    string respuesta;
                    cout << "Desea enviar solicitud a un sugerido (s/n): ";
                    getline(cin, respuesta);

                    if (respuesta == "s" || respuesta == "S")
                    {
                        string entradaPosicion;
                        int posicion = 0;

                        cout << "Numero del sugerido: ";
                        getline(cin, entradaPosicion);

                        if (!convertirNumeroHasta20(entradaPosicion, posicion) ||
                            posicion >= total)
                        {
                            cout << "Posicion no valida." << endl;
                        }
                        else
                        {
                            const Usuario* sugerido =
                                plataforma.obtenerUsuario(indices[posicion]);

                            if (plataforma.enviarSolicitud(
                                    nombre, sugerido->getNombreUsuario()))
                            {
                                if (plataforma.guardarSolicitudes(rutaSolicitudes))
                                {
                                    cout << "Solicitud enviada y guardada." << endl;
                                }
                                else
                                {
                                    cout << "Solicitud creada en RAM, pero no se guardo."
                                         << endl;
                                }
                            }
                            else
                            {
                                cout << "No se pudo enviar la solicitud." << endl;
                            }
                        }
                    }
                }

                delete[] indices;
                delete[] coincidencias;
            }
        }
        else if (opcion != "0")
        {
            cout << "Opcion no valida." << endl;
        }
    }
    while (opcion != "0" && cin);

    return 0;
}
