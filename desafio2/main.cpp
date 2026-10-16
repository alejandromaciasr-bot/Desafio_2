#include <iostream>
#include <fstream>
#include <string>
#include "usuario.h"
#include "plataforma.h"

using namespace std;

int main()
{
    const string rutaUsuarios = "usuarios.txt";
    const string rutaAmistades = "amistades.txt";
    const string rutaSolicitudes = "solicitudes.txt";

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

    cout << "===== Ude@Book - Solicitudes de amistad =====" << endl;
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
        else if (opcion != "0")
        {
            cout << "Opcion no valida." << endl;
        }
    }
    while (opcion != "0" && cin);

    return 0;
}
