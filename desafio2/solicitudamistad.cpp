#include "solicitudamistad.h"

SolicitudAmistad::SolicitudAmistad()
{
    emisor = "";
    receptor = "";
}

SolicitudAmistad::SolicitudAmistad(const string& usuarioEmisor,
                                   const string& usuarioReceptor)
{
    emisor = usuarioEmisor;
    receptor = usuarioReceptor;
}

const string& SolicitudAmistad::getEmisor() const
{
    return emisor;
}

const string& SolicitudAmistad::getReceptor() const
{
    return receptor;
}
