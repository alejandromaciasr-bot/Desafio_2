#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
#include <iostream>

using namespace std;

class Mensaje
{
private:
    string emisor;
    string receptor;
    string texto;
    string fechaEnvio;
    bool leido;

public:
    Mensaje();
    Mensaje(const string& usuarioEmisor,
            const string& usuarioReceptor,
            const string& contenido,
            const string& fecha);

    const string& getEmisor() const;
    const string& getReceptor() const;
    const string& getTexto() const;
    const string& getFechaEnvio() const;
    bool estaLeido() const;
    void marcarLeido();

    static bool textoValido(const string& contenido);
    static bool fechaValida(const string& fecha);
    static bool fechaHoraValida(const string& fechaHora);

    bool guardarEnArchivo(ostream& archivo) const;
    bool cargarDesdeArchivo(istream& archivo);
};

#endif
