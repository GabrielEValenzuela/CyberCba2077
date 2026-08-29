#ifndef CYBERPUNK_GAME_MODEL_HPP
#define CYBERPUNK_GAME_MODEL_HPP

#include <cstdint>
#include <string>
namespace cyberpunk
{

    class GameModel
    {
        public:
           GameModel() = default;

          // Example state: "credits" (placeholder)
           void addCredits(std::int64_t delta);
           [[nodiscard]] std::int64_t credits() const;

         bool fueDetectado() const  //CLASES DE SIMULACION!!
         {
           return false;    //Suponemos que por defecto nunca esta detectado

         }

         int resultadoObtenido() const
         {
           return 80;      //suponemos que en una prueba de habilidad obtiene 80
         }

         std::string ultimaMision()const // suponemos que se termino esta mision identificada con un string
         {
          std::string mision= "Asistir al ultimo concierto de samurai";
           return mision;

         }




        private:
            std::int64_t mCredits {0};



    };

} // namespace cyberpunk

#endif // CYBERPUNK_GAME_MODEL_HPP