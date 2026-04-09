#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <array>

class Storage {
   static inline int bestScore = 0;
public:
   static inline int stage = 1;
   static inline std::array<bool, 9> numbersCleared = {0, 0, 0, 0, 0, 0, 0, 0, 0}; /// if 'n' is cleared, [n-1] is true

   static inline std::string getBestScore() { /// perfectly formats bestScore with commas
      if(bestScore >= 1000) {
         std::ostringstream oss;
         if(bestScore >= 1000000) {
            oss << (bestScore / 1000000) % 1000 << ","; // millions
            oss << std::setfill('0') << std::setw(3) << (bestScore / 1000) % 1000 << ","; // thousands
         } else
            oss << bestScore / 1000 << ","; // thousands (if less than a million, don't show millions place)

         oss << std::setfill('0') << std::setw(3) << bestScore % 1000; // hundreds
         return oss.str();
      } else
         return std::to_string(bestScore);
   }

   static inline void load() { /// update score, numbersCleared and stage from file to storage
      std::ifstream file("assets/storage/storage.txt");
      if(!file.is_open()) return; // if file doesn't exist, just return with default values

      file >> bestScore >> stage;
      for(int i = 0; i < 9; i++)
         file >> numbersCleared[i];
      
      file.close();
   }

   static inline void save() { /// update file with score, numbersCleared and stage from storage
      std::ofstream file("assets/storage/storage.txt");
      if(!file.is_open()) return;

      file << bestScore << " " << stage << "\n";
      for(int i = 0; i < 9; i++)
         file << numbersCleared[i] << " ";

      file.close();
   }
};