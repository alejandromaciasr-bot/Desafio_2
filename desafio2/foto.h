#ifndef FOTO_H
#define FOTO_H

#include <string>
#include <iosfwd>

using namespace std;

class Foto
{
private:
    string fechaCaptura;
    unsigned long tamanoBytes;
    string ruta;
    int likes;

public:
    Foto();
    Foto(const string& fecha, unsigned long tamano,
         const string& rutaArchivo, int cantidadLikes);

    const string& getFechaCaptura() const;
    unsigned long getTamanoBytes() const;
    const string& getRuta() const;
    int getLikes() const;

    // sobrecargas: aumentar o disminuir los likes
    Foto& operator++();
    Foto& operator--();

    size_t memoriaCadenasEstimada() const;
    bool guardarEnArchivo(ostream& archivo) const;
    bool cargarDesdeArchivo(istream& archivo);
};

#endif
