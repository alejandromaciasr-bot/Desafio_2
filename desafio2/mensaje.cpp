#include "medidorrecursos.h"
#include "mensaje.h"

// convierte dos o cuatro digitos decimales a un entero
static int numeroDesdeTexto(const string& texto, int inicio, int cantidad)
{
    int numero = 0;
    for (int i = inicio; i < inicio + cantidad; i++)
    {
        MedidorRecursos::contarIteracion();
        if (texto[i] < '0' || texto[i] > '9')
        {
            return -1;
        }
        numero = numero * 10 + (texto[i] - '0');
    }
    return numero;
}

Mensaje::Mensaje()
{
    emisor = "";
    receptor = "";
    texto = "";
    fechaEnvio = "";
    leido = false;
}

Mensaje::Mensaje(const string& usuarioEmisor,
                 const string& usuarioReceptor,
                 const string& contenido,
                 const string& fecha) : Mensaje()
{
    emisor = usuarioEmisor;
    receptor = usuarioReceptor;
    texto = contenido;
    fechaEnvio = fecha;
}

const string& Mensaje::getEmisor() const
{
    return emisor;
}

const string& Mensaje::getReceptor() const
{
    return receptor;
}

const string& Mensaje::getTexto() const
{
    return texto;
}

const string& Mensaje::getFechaEnvio() const
{
    return fechaEnvio;
}

bool Mensaje::estaLeido() const
{
    return leido;
}

void Mensaje::marcarLeido()
{
    leido = true;
}

// cuenta caracteres sin depender de algoritmos de la stl
// se admite una sola linea de texto por mensaje
bool Mensaje::textoValido(const string& contenido)
{
    int cantidad = 0;
    bool hayContenido = false;

    for (int i = 0; i < static_cast<int>(contenido.size()); i++)
    {
        MedidorRecursos::contarIteracion();
        unsigned char caracter = static_cast<unsigned char>(contenido[i]);

        if (caracter == '\n' || caracter == '\r')
        {
            return false;
        }

        // en utf-8 los bytes de continuacion no son caracteres nuevos
        if ((caracter & 0xC0) != 0x80)
        {
            cantidad++;
        }

        if (caracter != ' ' && caracter != '\t')
        {
            hayContenido = true;
        }
    }

    return hayContenido && cantidad <= 512;
}

// comprueba el formato aaaa-mm-dd y dias reales del calendario
bool Mensaje::fechaValida(const string& fecha)
{
    if (fecha.size() != 10 || fecha[4] != '-' || fecha[7] != '-')
    {
        return false;
    }

    int anio = numeroDesdeTexto(fecha, 0, 4);
    int mes = numeroDesdeTexto(fecha, 5, 2);
    int dia = numeroDesdeTexto(fecha, 8, 2);

    if (anio < 1 || mes < 1 || mes > 12 || dia < 1)
    {
        return false;
    }

    int dias = 31;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
    {
        dias = 30;
    }
    else if (mes == 2)
    {
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) ||
                        (anio % 400 == 0);
        dias = bisiesto ? 29 : 28;
    }

    return dia <= dias;
}

bool Mensaje::fechaHoraValida(const string& fechaHora)
{
    if (fechaHora.size() != 19 || fechaHora[10] != ' ' ||
        fechaHora[13] != ':' || fechaHora[16] != ':')
    {
        return false;
    }

    if (!fechaValida(fechaHora.substr(0, 10)))
    {
        return false;
    }

    int hora = numeroDesdeTexto(fechaHora, 11, 2);
    int minuto = numeroDesdeTexto(fechaHora, 14, 2);
    int segundo = numeroDesdeTexto(fechaHora, 17, 2);

    return hora >= 0 && hora <= 23 &&
           minuto >= 0 && minuto <= 59 &&
           segundo >= 0 && segundo <= 59;
}

bool Mensaje::guardarEnArchivo(ostream& archivo) const
{
    archivo << emisor << '\n';
    archivo << receptor << '\n';
    archivo << fechaEnvio << '\n';
    archivo << (leido ? 1 : 0) << '\n';
    archivo << texto << '\n';
    return archivo.good();
}

bool Mensaje::cargarDesdeArchivo(istream& archivo)
{
    string nuevoEmisor;
    string nuevoReceptor;
    string nuevaFecha;
    string nuevoEstado;
    string nuevoTexto;

    if (!getline(archivo, nuevoEmisor) ||
        !getline(archivo, nuevoReceptor) ||
        !getline(archivo, nuevaFecha) ||
        !getline(archivo, nuevoEstado) ||
        !getline(archivo, nuevoTexto))
    {
        return false;
    }

    if (nuevoEmisor.empty() || nuevoReceptor.empty() ||
        !fechaHoraValida(nuevaFecha) || !textoValido(nuevoTexto) ||
        (nuevoEstado != "0" && nuevoEstado != "1"))
    {
        return false;
    }

    emisor = nuevoEmisor;
    receptor = nuevoReceptor;
    fechaEnvio = nuevaFecha;
    texto = nuevoTexto;
    leido = (nuevoEstado == "1");
    return true;
}
