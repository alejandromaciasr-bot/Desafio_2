#include "medidorrecursos.h"
#include "foto.h"
#include <iostream>
#include <limits>

Foto::Foto()
{
    fechaCaptura = "";
    tamanoBytes = 0;
    ruta = "";
    likes = 0;
}

Foto::Foto(const string& fecha, unsigned long tamano,
           const string& rutaArchivo, int cantidadLikes)
{
    fechaCaptura = fecha;
    tamanoBytes = tamano;
    ruta = rutaArchivo;
    likes = cantidadLikes >= 0 ? cantidadLikes : 0;
}

const string& Foto::getFechaCaptura() const
{
    return fechaCaptura;
}

unsigned long Foto::getTamanoBytes() const
{
    return tamanoBytes;
}

const string& Foto::getRuta() const
{
    return ruta;
}

int Foto::getLikes() const
{
    return likes;
}

Foto& Foto::operator++()
{
    likes++;
    return *this;
}

Foto& Foto::operator--()
{
    if (likes > 0)
    {
        likes--;
    }
    return *this;
}

size_t Foto::memoriaCadenasEstimada() const
{
    return MedidorRecursos::memoriaCadena(fechaCaptura)
    + MedidorRecursos::memoriaCadena(ruta);
}

bool Foto::guardarEnArchivo(ostream& archivo) const
{
    archivo << fechaCaptura << '\n';
    archivo << tamanoBytes << '\n';
    archivo << ruta << '\n';
    archivo << likes << '\n';
    return archivo.good();
}

bool Foto::cargarDesdeArchivo(istream& archivo)
{
    string fecha;
    unsigned long tamano;
    string rutaArchivo;
    int cantidadLikes;

    if (!getline(archivo, fecha) || !(archivo >> tamano))
    {
        return false;
    }
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!archivo || !getline(archivo, rutaArchivo) ||
        !(archivo >> cantidadLikes) || cantidadLikes < 0)
    {
        return false;
    }
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!archivo || fecha.empty() || rutaArchivo.empty())
    {
        return false;
    }

    fechaCaptura = fecha;
    tamanoBytes = tamano;
    ruta = rutaArchivo;
    likes = cantidadLikes;
    return true;
}
