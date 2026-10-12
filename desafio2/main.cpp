#include <iostream>
#include <fstream>
#include <string>

#include "usuario.h"
#include "plataforma.h"

using namespace std;

int main()
{
    cout << "===== Ude@Book =====" << endl;

    Plataforma plataforma;

    string ruta = "usuarios.txt";

    // verificar si ya existe el archivo
    ifstream comprobacion(ruta);

    bool archivoExiste = comprobacion.is_open();

    comprobacion.close();

    if (archivoExiste)
    {
        // cargar los usuarios existentes
        if (!plataforma.cargarUsuarios(ruta))
        {
            cout << "Error al cargar los usuarios." << endl;
            return 1;
        }

        cout << "Usuarios cargados desde el archivo." << endl;
    }
    else
    {
        // crear usuarios iniciales de prueba
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

        // guardar los usuarios iniciales
        if (!plataforma.guardarUsuarios(ruta))
        {
            cout << "Error al guardar los usuarios." << endl;
            return 1;
        }

        cout << "Archivo inicial creado correctamente." << endl;
    }

    cout << endl;
    cout << "===== Usuarios registrados =====" << endl;

    for (int i = 0; i < plataforma.getCantidadUsuarios(); i++)
    {
        const Usuario* actual = plataforma.obtenerUsuario(i);

        if (actual != nullptr)
        {
            cout << i << ". "
                 << actual->getNombreUsuario() << endl;
        }
    }

    cout << endl;
    cout << "Cantidad de usuarios: "
         << plataforma.getCantidadUsuarios() << endl;

    cout << "Capacidad del arreglo: "
         << plataforma.getCapacidadUsuarios() << endl;

    cout << endl;

    // registrar un nuevo usuario de prueba
    string nombreNuevo;

    cout << "Ingrese un nuevo nombre de usuario" << endl;
    cout << "(escriba 0 para salir): ";

    getline(cin, nombreNuevo);

    if (nombreNuevo != "0")
    {
        Usuario nuevoUsuario(nombreNuevo);

        if (plataforma.agregarUsuario(nuevoUsuario))
        {
            if (plataforma.guardarUsuarios(ruta))
            {
                cout << "Usuario registrado y guardado." << endl;
            }
            else
            {
                cout << "Error al guardar los cambios." << endl;
            }
        }
        else
        {
            cout << "Nombre vacio o usuario duplicado." << endl;
        }
    }

    cout << endl;
    cout << "Programa finalizado." << endl;

    return 0;
}
