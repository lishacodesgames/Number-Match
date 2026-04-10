#include <cstdint>
#include <string>
#include <array>

struct Storage {
   static uint32_t stage;
   static std::array<bool, 9> numbersCleared; /// if 'n' is cleared, [n-1] is true
   static uint32_t coins;
   static uint32_t bestScore;
   static uint32_t currentScore;

   static std::string formatCoins();
   static std::string formatBestScore();
   static std::string formatCurrentScore();
   
   static void load(); /// update score, numbersCleared and stage from file to storage
   static void save(); /// update file with score, numbersCleared and stage from storage
   static void save(uint32_t stage, std::array<bool, 9> numbersCleared, uint32_t currentScore); /// for saving specific info
private:
   static std::string format(uint32_t num); /// perfectly formats bestScore with commas
};