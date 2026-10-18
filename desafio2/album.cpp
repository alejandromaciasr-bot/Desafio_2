#include "medidorrecursos.h"
#include "album.h"
#include <iostream>
#include <limits>

Album::Album()
{
    nombre = "";
    rutaAlmacenamiento = "";
    privacidad = "publico";
    fotos = nullptr;
    cantidadFotos = 0;
    capacidadFotos = 0;
}

Album::Album(const string& nombreAlbum, const string& rutaAlbum,
             const string& tipoPrivacidad) : Album()
{
    nombre = nombreAlbum;
    rutaAlmacenamiento = rutaAlbum;
    privacidad = tipoPrivacidad;
}

// copia profunda para que cada album sea dueno de sus fotos
Album::Album(const Album& otro) : Album()
{
    *this = otro;
}

Album& Album::operator=(const Album& otro)
{
    if (this == &otro)
    {
        return *this;
    }

    Foto* nuevasFotos = nullptr;
    if (otro.capacidadFotos > 0)
    {
        nuevasFotos = new Foto[otro.capacidadFotos];
        for (
            int i = 0;
            i < otro.cantidadFotos;
            i++
            )
        {
            MedidorRecursos::contarIteracion();
            nuevasFotos[i] = otro.fotos[i];
        }
    }

    delete[] fotos;
    fotos = nuevasFotos;
    cantidadFotos = otro.cantidadFotos;
    capacidadFotos = otro.capacidadFotos;
    nombre = otro.nombre;
    rutaAlmacenamiento = otro.rutaAlmacenamiento;
    privacidad = otro.privacidad;
    return *this;
}

Album::~Album()
{
    delete[] fotos;
}

void Album::ampliarFotos()
{
    int nuevaCapacidad = capacidadFotos == 0 ? 2 : capacidadFotos * 2;
    Foto* nuevoArreglo = new Foto[nuevaCapacidad];

    for (
        int i = 0;
        i < cantidadFotos;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        nuevoArreglo[i] = fotos[i];
    }

    delete[] fotos;
    fotos = nuevoArreglo;
    capacidadFotos = nuevaCapacidad;
}

bool Album::privacidadValida(const string& valor)
{
    return valor == "publico" || valor == "amigos" || valor == "privado";
}

const string& Album::getNombre() const
{
    return nombre;
}

const string& Album::getRutaAlmacenamiento() const
{
    return rutaAlmacenamiento;
}

const string& Album::getPrivacidad() const
{
    return privacidad;
}

int Album::getCantidadFotos() const
{
    return cantidadFotos;
}

int Album::getCapacidadFotos() const
{
    return capacidadFotos;
}

const Foto* Album::obtenerFoto(int posicion) const
{
    if (posicion < 0 || posicion >= cantidadFotos)
    {
        return nullptr;
    }
    return &fotos[posicion];
}

bool Album::agregarFoto(const Foto& foto)
{
    if (foto.getRuta().empty())
    {
        return false;
    }
    if (cantidadFotos == capacidadFotos)
    {
        ampliarFotos();
    }
    fotos[cantidadFotos] = foto;
    cantidadFotos++;
    return true;
}

bool Album::modificarLike(int posicion, bool darLike)
{
    if (posicion < 0 || posicion >= cantidadFotos)
    {
        return false;
    }
    if (darLike)
    {
        ++fotos[posicion];
    }
    else
    {
        if (fotos[posicion].getLikes() == 0)
        {
            return false;
        }
        --fotos[posicion];
    }
    return true;
}

size_t Album::memoriaDinamicaEstimada() const
{
    size_t bytes = static_cast<size_t>(capacidadFotos) * sizeof(Foto);
    bytes = bytes + MedidorRecursos::memoriaCadena(nombre);
    bytes = bytes + MedidorRecursos::memoriaCadena(rutaAlmacenamiento);
    bytes = bytes + MedidorRecursos::memoriaCadena(privacidad);

    for (
        int i = 0;
        i < cantidadFotos;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        bytes = bytes + fotos[i].memoriaCadenasEstimada();
    }
    return bytes;
}

bool Album::guardarEnArchivo(ostream& archivo) const
{
    archivo << nombre << '\n';
    archivo << rutaAlmacenamiento << '\n';
    archivo << privacidad << '\n';
    archivo << cantidadFotos << '\n';

    for (
        int i = 0;
        i < cantidadFotos;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        if (!fotos[i].guardarEnArchivo(archivo))
        {
            return false;
        }
    }
    return archivo.good();
}

bool Album::cargarDesdeArchivo(istream& archivo)
{
    string nuevoNombre;
    string nuevaRuta;
    string nuevaPrivacidad;
    int totalFotos;

    if (!getline(archivo, nuevoNombre) ||
        !getline(archivo, nuevaRuta) ||
        !getline(archivo, nuevaPrivacidad) ||
        !(archivo >> totalFotos) || totalFotos < 0)
    {
        return false;
    }

    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!archivo || nuevoNombre.empty() || nuevaRuta.empty() ||
        !privacidadValida(nuevaPrivacidad))
    {
        return false;
    }

    Album temporal(nuevoNombre, nuevaRuta, nuevaPrivacidad);
    for (
        int i = 0;
        i < totalFotos;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        Foto nuevaFoto;
        if (!nuevaFoto.cargarDesdeArchivo(archivo) ||
            !temporal.agregarFoto(nuevaFoto))
        {
            return false;
        }
    }

    *this = temporal;
    return true;
}
