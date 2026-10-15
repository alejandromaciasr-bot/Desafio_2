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

    Plataforma plataforma;

    ifstream comprobacionUsuarios(rutaUsuarios);
    bool existenUsuarios = comprobacionUsuarios.is_open();
    comprobacionUsuarios.close();

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
        Usuario usuario1("juanito76");
        Usuario usuario2("pedrito89");

        Usuario usuario3(
            "laura25",
            "2002-06-18",
            "laura@ejemplo.com",
            "/users/udeabook/laura25/albums/profile.png",
            "clave123",
            "Medellin",
            "Colombia",
            "2026-10-05"
            );

        plataforma.agregarUsuario(usuario1);
        plataforma.agregarUsuario(usuario2);
        plataforma.agregarUsuario(usuario3);

        if (!plataforma.guardarUsuarios(rutaUsuarios))
        {
            cout << "Error al crear usuarios.txt." << endl;
            return 1;
        }
    }

    ifstream comprobacionAmistades(rutaAmistades);
    bool existenAmistades = comprobacionAmistades.is_open();
    comprobacionAmistades.close();

    if (existenAmistades &&
        !plataforma.cargarAmistades(rutaAmistades))
    {
        cout << "Error al cargar amistades.txt." << endl;
        return 1;
    }

    cout << "===== Ude@Book - Prueba de amistades ====="
         << endl;

    cout << "Usuarios cargados: "
         << plataforma.getCantidadUsuarios() << endl;

    string opcion;

    do
    {
        cout << "\n1. Registrar usuario" << endl;
        cout << "2. Crear amistad (solo prueba tecnica)" << endl;
        cout << "3. Consultar amistad" << endl;
        cout << "4. Contar amigos" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";

        getline(cin, opcion);

        if (opcion == "1")
        {
            string nombre;

            cout << "Nombre de usuario: ";
            getline(cin, nombre);

            Usuario nuevo(nombre);

            if (plataforma.agregarUsuario(nuevo))
            {
                if (plataforma.guardarUsuarios(rutaUsuarios))
                {
                    cout << "Usuario registrado y guardado."
                         << endl;
                }
                else
                {
                    cout << "Usuario en RAM, pero fallo al guardar."
                         << endl;
                }
            }
            else
            {
                cout << "Nombre vacio o repetido." << endl;
            }
        }
        else if (opcion == "2")
        {
            string primero;
            string segundo;

            cout << "Primer usuario: ";
            getline(cin, primero);

            cout << "Segundo usuario: ";
            getline(cin, segundo);

            if (plataforma.establecerAmistad(primero, segundo))
            {
                if (plataforma.guardarAmistades(rutaAmistades))
                {
                    cout << "Amistad mutua creada y guardada."
                         << endl;
                }
                else
                {
                    cout << "Amistad en RAM, pero fallo al guardar."
                         << endl;
                }
            }
            else
            {
                cout << "No se puede crear esa amistad."
                     << endl;
            }
        }
        else if (opcion == "3")
        {
            string primero;
            string segundo;

            cout << "Primer usuario: ";
            getline(cin, primero);

            cout << "Segundo usuario: ";
            getline(cin, segundo);

            if (plataforma.buscarUsuario(primero) == nullptr ||
                plataforma.buscarUsuario(segundo) == nullptr)
            {
                cout << "Uno de los usuarios no existe."
                     << endl;
            }
            else if (plataforma.sonAmigos(primero, segundo))
            {
                cout << "Si son amigos." << endl;
            }
            else
            {
                cout << "No son amigos." << endl;
            }
        }
        else if (opcion == "4")
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
                cout << "Cantidad de amigos: "
                     << total << endl;
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
