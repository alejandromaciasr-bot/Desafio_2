#include "plataforma.h"
#include <fstream>
#include <limits>

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

    Usuario** nuevoArreglo = new Usuario*[nuevaCapacidad];

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

// obtener usuario por posicion
const Usuario* Plataforma::obtenerUsuario(int posicion) const
{
    if (posicion < 0 || posicion >= cantidadUsuarios)
    {
        return nullptr;
    }

    return usuarios[posicion];
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

// guardar todos los usuarios
bool Plataforma::guardarUsuarios(const string& ruta) const
{
    ofstream archivo(ruta);

    if (!archivo.is_open())
    {
        return false;
    }

    archivo << "UDEABOOK_USUARIOS_V1" << '\n';
    archivo << cantidadUsuarios << '\n';

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        if (!usuarios[i]->guardarEnArchivo(archivo))
        {
            return false;
        }
    }

    archivo.close();

    return archivo.good();
}

// cargar todos los usuarios
bool Plataforma::cargarUsuarios(const string& ruta)
{
    ifstream archivo(ruta);

    if (!archivo.is_open())
    {
        return false;
    }

    string encabezado;

    if (!getline(archivo, encabezado))
    {
        return false;
    }

    if (encabezado != "UDEABOOK_USUARIOS_V1")
    {
        return false;
    }

    int totalUsuarios;

    if (!(archivo >> totalUsuarios) || totalUsuarios < 0)
    {
        return false;
    }

    archivo.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!archivo)
    {
        return false;
    }

    // cargar primero en una plataforma temporal
    Plataforma temporal;

    for (int i = 0; i < totalUsuarios; i++)
    {
        Usuario nuevoUsuario;

        if (!nuevoUsuario.cargarDesdeArchivo(archivo))
        {
            return false;
        }

        if (!temporal.agregarUsuario(nuevoUsuario))
        {
            return false;
        }
    }

    // intercambiar los arreglos y sus contadores
    Usuario** arregloAnterior = usuarios;
    int cantidadAnterior = cantidadUsuarios;
    int capacidadAnterior = capacidadUsuarios;

    usuarios = temporal.usuarios;
    cantidadUsuarios = temporal.cantidadUsuarios;
    capacidadUsuarios = temporal.capacidadUsuarios;

    temporal.usuarios = arregloAnterior;
    temporal.cantidadUsuarios = cantidadAnterior;
    temporal.capacidadUsuarios = capacidadAnterior;

    return true;
}
