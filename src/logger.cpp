#include "logger.hpp"
#include <cstdlib>

// Définition des flux globaux
std::ofstream access_log("tools/logs/access.log", std::ios::trunc); // Mode truncate
std::ofstream error_log("tools/logs/error.log", std::ios::trunc);  // Mode truncate

// Optionnel : Vérifiez si les fichiers sont bien ouverts
struct LoggerInitializer {
    LoggerInitializer() {
        if (!access_log.is_open()) {
            std::cerr << "Failed to open access.log" << std::endl;
            exit(1);
        }
        if (!error_log.is_open()) {
            std::cerr << "Failed to open error.log" << std::endl;
            exit(1);
        }
    }
};

// Initialisation automatique
LoggerInitializer logger_initializer;