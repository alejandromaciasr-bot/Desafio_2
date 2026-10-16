#ifndef SOLICITUDAMISTAD_H
#define SOLICITUDAMISTAD_H

#include <string>

using namespace std;

class SolicitudAmistad
{
private:
    string emisor;
    string receptor;

public:
    SolicitudAmistad();
    SolicitudAmistad(const string& usuarioEmisor,
                     const string& usuarioReceptor);

    const string& getEmisor() const;
    const string& getReceptor() const;
};

#endif
