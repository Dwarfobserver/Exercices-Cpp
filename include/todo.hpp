
#pragma once

#include <stdexcept>
#include <string>


// Erreur lancée dans les fonctions non implémentées.
class not_implemented_error : public std::logic_error {
public:
    inline not_implemented_error(std::string const& file, int line) : std::logic_error
        {"In file '" + file + "'\n"
         "Line " + std::to_string(line) + " : error due to an unimplemented function"}
    {}
};

// Macro pour faciliter la construction du message de l'erreur.
#define TODO() throw not_implemented_error{ __FILE__, __LINE__ }
