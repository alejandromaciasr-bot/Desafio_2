
#include <iostream>
#include "usuario.h"

using namespace std;

int main()
{
    cout << "===== Ude@Book =====" << endl;

    // usuario con constructor sin parametro
    Usuario usuario1;
    usuario1.setNombreUsuario("juanito76");

    // usuario con constructor de un parametro
    Usuario usuario2("pedrito89");

    // usuario con todos los parametros
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

    cout << endl;
    cout << "Usuario 1: "
         << usuario1.getNombreUsuario() << endl;

    cout << "Usuario 2: "
         << usuario2.getNombreUsuario() << endl;

    cout << endl;
    cout << "===== Perfil de usuario 3 =====" << endl;

    cout << "Nombre: "
         << usuario3.getNombreUsuario() << endl;

    cout << "Nacimiento: "
         << usuario3.getFechaNacimiento() << endl;

    cout << "Correo: "
         << usuario3.getCorreo() << endl;

    cout << "Ciudad: "
         << usuario3.getCiudad() << endl;

    cout << "Pais: "
         << usuario3.getPais() << endl;

    cout << "Inscripcion: "
         << usuario3.getFechaInscripcion() << endl;

    cout << "Foto de perfil: "
         << usuario3.getFotoPerfil() << endl;

    cout << endl;

    if (usuario3.verificarContrasena("clave123"))
    {
        cout << "Contrasena correcta" << endl;
    }
    else
    {
        cout << "Contrasena incorrecta" << endl;
    }

    return 0;
}
