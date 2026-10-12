
#include <iostream>
#include "usuario.h"
#include "plataforma.h"

using namespace std;

int main()
{
    cout << "===== Ude@Book =====" << endl;

    Plataforma plataforma;

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

    // registrar los primeros dos usuarios
    plataforma.agregarUsuario(usuario1);
    plataforma.agregarUsuario(usuario2);

    cout << endl;
    cout << "Usuarios registrados: "
         << plataforma.getCantidadUsuarios() << endl;

    cout << "Capacidad del arreglo: "
         << plataforma.getCapacidadUsuarios() << endl;

    // este usuario obliga a ampliar el arreglo
    plataforma.agregarUsuario(usuario3);

    cout << endl;
    cout << "Despues de agregar a Laura:" << endl;

    cout << "Usuarios registrados: "
         << plataforma.getCantidadUsuarios() << endl;

    cout << "Capacidad del arreglo: "
         << plataforma.getCapacidadUsuarios() << endl;

    // comprobar que no se admiten duplicados
    cout << endl;

    if (!plataforma.agregarUsuario(usuario1))
    {
        cout << "No se pudo registrar un usuario duplicado."
             << endl;
    }

    // buscar manualmente un usuario
    const Usuario* encontrado =
        plataforma.buscarUsuario("laura25");

    if (encontrado != nullptr)
    {
        cout << endl;
        cout << "Usuario encontrado: "
             << encontrado->getNombreUsuario() << endl;

        cout << "Correo: "
             << encontrado->getCorreo() << endl;
    }
    else
    {
        cout << "Usuario no encontrado." << endl;
    }

    return 0;
}
