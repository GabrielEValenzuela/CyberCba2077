#ifndef CYBER_CBA_GAME_CONFIG_HPP
#define CYBER_CBA_GAME_CONFIG_HPP

/**
 * @file GameConfig.hpp
 * @brief Carga la configuración externa de ejecución de Cyber CBA.
 * @details Lee un archivo simple `clave=valor` para separar los parámetros operativos del binario.
 */
#include <string>

/** @invariant Las dimensiones y FPS almacenados son positivos; la duración de splash no es negativa. */
class GameConfig
{
public:
    /** @brief Crea una configuración segura por defecto. */
    GameConfig();
    /** @brief Carga valores desde un archivo. @param path Ruta del archivo `clave=valor`. @return `true` si se abrió y
     * procesó el archivo. */
    bool loadFromFile(const char* path);
    /** @brief Devuelve el título de la ventana. @return Referencia al título configurado. */
    const std::string& windowTitle() const;
    /** @brief Devuelve el ancho de la ventana. @return Ancho positivo en píxeles. */
    int screenWidth() const;
    /** @brief Devuelve el alto de la ventana. @return Alto positivo en píxeles. */
    int screenHeight() const;
    /** @brief Devuelve el límite de cuadros por segundo. @return FPS positivo. */
    int targetFps() const;
    /** @brief Devuelve la duración del splash. @return Segundos no negativos. */
    float splashSeconds() const;
    /** @brief Indica si se solicita telemetría de desarrollo. @return `true` únicamente para `ui_mode=development`. */
    bool isDevelopmentMode() const;
    const std::string& savePath() const;
    bool rainEnabled() const;
    int rainDensity() const;
    float rainIntensity() const;

private:
    std::string m_windowTitle;
    int m_screenWidth;
    int m_screenHeight;
    int m_targetFps;
    float m_splashSeconds;
    bool m_isDevelopmentMode;
    std::string m_savePath;
    bool m_rainEnabled;
    int m_rainDensity;
    float m_rainIntensity;
};

#endif // CYBER_CBA_GAME_CONFIG_HPP
