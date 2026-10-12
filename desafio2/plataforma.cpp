#include "plataforma.h"

// constructor
Plataforma::Plataforma()
{
    cantidadUsuarios = 0;
    capacidadUsuarios = 2;

    usuarios = new Usuario*[capacidadUsuarios];
}

// destructor
Plataforma::~Plataforma()
{
    for (int i = 0; i < cantidadUsuarios; i++)
    {
        delete usuarios[i];
    }

    delete[] usuarios;
}

// ampliar arreglo dinamico
void Plataforma::ampliarCapacidad()
{
    int nuevaCapacidad = capacidadUsuarios * 2;

    Usuario** nuevoArreglo =
        new Usuario*[nuevaCapacidad];

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        nuevoArreglo[i] = usuarios[i];
    }

    delete[] usuarios;

    usuarios = nuevoArreglo;
    capacidadUsuarios = nuevaCapacidad;
}

// insertar usuario
bool Plataforma::agregarUsuario(const Usuario& usuario)
{
    if (usuario.getNombreUsuario().empty())
    {
        return false;
    }

    if (buscarUsuario(usuario.getNombreUsuario()) != nullptr)
    {
        return false;
    }

    if (cantidadUsuarios == capacidadUsuarios)
    {
        ampliarCapacidad();
    }

    usuarios[cantidadUsuarios] = new Usuario(usuario);

    cantidadUsuarios++;

    return true;
}

// buscar usuario manualmente
const Usuario* Plataforma::buscarUsuario(const string& nombre) const
{
    for (int i = 0; i < cantidadUsuarios; i++)
    {
        if (usuarios[i]->getNombreUsuario() == nombre)
        {
            return usuarios[i];
        }
    }

    return nullptr;
}

// consultar cantidad de usuarios
int Plataforma::getCantidadUsuarios() const
{
    return cantidadUsuarios;
}

// consultar capacidad del arreglo
int Plataforma::getCapacidadUsuarios() const
{
    return capacidadUsuarios;
}
