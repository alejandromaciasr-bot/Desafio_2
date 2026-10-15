#include "plataforma.h"
#include <fstream>
#include <limits>

// constructor
Plataforma::Plataforma()
{
    cantidadUsuarios = 0;
    capacidadUsuarios = 2;

    usuarios = new Usuario*[capacidadUsuarios];

    matrizAmistades =
        new bool[capacidadUsuarios * capacidadUsuarios]();
}

// destructor
Plataforma::~Plataforma()
{
    for (int i = 0; i < cantidadUsuarios; i++)
    {
        delete usuarios[i];
    }

    delete[] usuarios;
    delete[] matrizAmistades;
}

// ampliar los arreglos dinamicos
void Plataforma::ampliarCapacidad()
{
    int nuevaCapacidad = capacidadUsuarios * 2;

    Usuario** nuevoArreglo =
        new Usuario*[nuevaCapacidad];

    bool* nuevaMatriz =
        new bool[nuevaCapacidad * nuevaCapacidad]();

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        nuevoArreglo[i] = usuarios[i];
    }

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        for (int j = 0; j < cantidadUsuarios; j++)
        {
            nuevaMatriz[i * nuevaCapacidad + j] =
                matrizAmistades[i * capacidadUsuarios + j];
        }
    }

    delete[] usuarios;
    delete[] matrizAmistades;

    usuarios = nuevoArreglo;
    matrizAmistades = nuevaMatriz;
    capacidadUsuarios = nuevaCapacidad;
}

// buscar la posicion de un usuario
int Plataforma::buscarIndiceUsuario(const string& nombre) const
{
    for (int i = 0; i < cantidadUsuarios; i++)
    {
        if (usuarios[i]->getNombreUsuario() == nombre)
        {
            return i;
        }
    }

    return -1;
}

// insertar un usuario sin duplicar nombres
bool Plataforma::agregarUsuario(const Usuario& usuario)
{
    if (usuario.getNombreUsuario().empty())
    {
        return false;
    }

    if (buscarIndiceUsuario(usuario.getNombreUsuario()) != -1)
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

// buscar un usuario por su nombre
const Usuario* Plataforma::buscarUsuario(const string& nombre) const
{
    int indice = buscarIndiceUsuario(nombre);

    if (indice == -1)
    {
        return nullptr;
    }

    return usuarios[indice];
}

// consultar un usuario segun su posicion
const Usuario* Plataforma::obtenerUsuario(int posicion) const
{
    if (posicion < 0 || posicion >= cantidadUsuarios)
    {
        return nullptr;
    }

    return usuarios[posicion];
}

int Plataforma::getCantidadUsuarios() const
{
    return cantidadUsuarios;
}

int Plataforma::getCapacidadUsuarios() const
{
    return capacidadUsuarios;
}

// crear una amistad reciproca
// por ahora solo para pruebas
bool Plataforma::establecerAmistad(const string& usuario1,
                                   const string& usuario2)
{
    int i = buscarIndiceUsuario(usuario1);
    int j = buscarIndiceUsuario(usuario2);

    if (i == -1 || j == -1 || i == j)
    {
        return false;
    }

    if (matrizAmistades[i * capacidadUsuarios + j])
    {
        return false;
    }

    matrizAmistades[i * capacidadUsuarios + j] = true;
    matrizAmistades[j * capacidadUsuarios + i] = true;

    return true;
}

// consultar si dos usuarios son amigos
bool Plataforma::sonAmigos(const string& usuario1,
                           const string& usuario2) const
{
    int i = buscarIndiceUsuario(usuario1);
    int j = buscarIndiceUsuario(usuario2);

    if (i == -1 || j == -1)
    {
        return false;
    }

    return matrizAmistades[i * capacidadUsuarios + j];
}

// contar amigos de un usuario
int Plataforma::contarAmigos(const string& nombre) const
{
    int indice = buscarIndiceUsuario(nombre);

    if (indice == -1)
    {
        return -1;
    }

    int total = 0;

    for (int j = 0; j < cantidadUsuarios; j++)
    {
        if (matrizAmistades[indice * capacidadUsuarios + j])
        {
            total++;
        }
    }

    return total;
}

// guardar los datos basicos de los usuarios
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

// cargar los usuarios antes de cargar las amistades
bool Plataforma::cargarUsuarios(const string& ruta)
{
    ifstream archivo(ruta);

    if (!archivo.is_open())
    {
        return false;
    }

    string encabezado;

    if (!getline(archivo, encabezado) ||
        encabezado != "UDEABOOK_USUARIOS_V1")
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

    Plataforma temporal;

    for (int i = 0; i < totalUsuarios; i++)
    {
        Usuario nuevoUsuario;

        if (!nuevoUsuario.cargarDesdeArchivo(archivo) ||
            !temporal.agregarUsuario(nuevoUsuario))
        {
            return false;
        }
    }

    // intercambiar los arreglos y sus matrices
    Usuario** usuariosAnteriores = usuarios;
    bool* matrizAnterior = matrizAmistades;

    int cantidadAnterior = cantidadUsuarios;
    int capacidadAnterior = capacidadUsuarios;

    usuarios = temporal.usuarios;
    matrizAmistades = temporal.matrizAmistades;
    cantidadUsuarios = temporal.cantidadUsuarios;
    capacidadUsuarios = temporal.capacidadUsuarios;

    temporal.usuarios = usuariosAnteriores;
    temporal.matrizAmistades = matrizAnterior;
    temporal.cantidadUsuarios = cantidadAnterior;
    temporal.capacidadUsuarios = capacidadAnterior;

    return true;
}

// guardar cada amistad una sola vez
bool Plataforma::guardarAmistades(const string& ruta) const
{
    ofstream archivo(ruta);

    if (!archivo.is_open())
    {
        return false;
    }

    int totalRelaciones = 0;

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        for (int j = i + 1; j < cantidadUsuarios; j++)
        {
            if (matrizAmistades[i * capacidadUsuarios + j])
            {
                totalRelaciones++;
            }
        }
    }

    archivo << "UDEABOOK_AMISTADES_V1" << '\n';
    archivo << totalRelaciones << '\n';

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        for (int j = i + 1; j < cantidadUsuarios; j++)
        {
            if (matrizAmistades[i * capacidadUsuarios + j])
            {
                archivo << usuarios[i]->getNombreUsuario() << '\n';
                archivo << usuarios[j]->getNombreUsuario() << '\n';
            }
        }
    }

    archivo.close();

    return archivo.good();
}

// cargar todas las amistades desde el archivo
bool Plataforma::cargarAmistades(const string& ruta)
{
    ifstream archivo(ruta);

    if (!archivo.is_open())
    {
        return false;
    }

    string encabezado;

    if (!getline(archivo, encabezado) ||
        encabezado != "UDEABOOK_AMISTADES_V1")
    {
        return false;
    }

    int totalRelaciones;

    if (!(archivo >> totalRelaciones) || totalRelaciones < 0)
    {
        return false;
    }

    archivo.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!archivo)
    {
        return false;
    }

    bool* nuevaMatriz =
        new bool[capacidadUsuarios * capacidadUsuarios]();

    for (int k = 0; k < totalRelaciones; k++)
    {
        string nombre1;
        string nombre2;

        if (!getline(archivo, nombre1) ||
            !getline(archivo, nombre2))
        {
            delete[] nuevaMatriz;
            return false;
        }

        int i = buscarIndiceUsuario(nombre1);
        int j = buscarIndiceUsuario(nombre2);

        if (i == -1 || j == -1 || i == j ||
            nuevaMatriz[i * capacidadUsuarios + j])
        {
            delete[] nuevaMatriz;
            return false;
        }

        nuevaMatriz[i * capacidadUsuarios + j] = true;
        nuevaMatriz[j * capacidadUsuarios + i] = true;
    }

    delete[] matrizAmistades;
    matrizAmistades = nuevaMatriz;

    return true;
}
