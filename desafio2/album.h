#ifndef ALBUM_H
#define ALBUM_H

#include "foto.h"
#include <string>
#include <iosfwd>

using namespace std;

class Album
{
private:
    string nombre;
    string rutaAlmacenamiento;
    string privacidad;

    Foto* fotos;
    int cantidadFotos;
    int capacidadFotos;

    void ampliarFotos();

public:
    Album();
    Album(const string& nombreAlbum, const string& rutaAlbum,
          const string& tipoPrivacidad);
    Album(const Album& otro);
    Album& operator=(const Album& otro);
    ~Album();

    static bool privacidadValida(const string& valor);

    const string& getNombre() const;
    const string& getRutaAlmacenamiento() const;
    const string& getPrivacidad() const;
    int getCantidadFotos() const;
    int getCapacidadFotos() const;
    const Foto* obtenerFoto(int posicion) const;

    // insertar fotos solo al preparar los datos iniciales
    bool agregarFoto(const Foto& foto);
    bool modificarLike(int posicion, bool darLike);

    size_t memoriaDinamicaEstimada() const;
    bool guardarEnArchivo(ostream& archivo) const;
    bool cargarDesdeArchivo(istream& archivo);
};

#endif
