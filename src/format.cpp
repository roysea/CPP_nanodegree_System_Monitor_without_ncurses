#include <string>

#include "../include/format.h"

using std::string;

/**
 * This function formats time 
 * @param time as seconds
 * @return formated time as string: HH:MM:SS 
*/
string Format::ElapsedTime(long seconds) { 
    
    long hh = seconds / 3600;
    long rem = seconds % 3600;
    int mm = rem / 60;
    int ss = rem % 60;
    
    return (hh < 10 ? "0" : "") + std::to_string(hh) + ":" +
           (mm < 10 ? "0" : "") + std::to_string(mm) + ":" +
           (ss < 10 ? "0" : "") + std::to_string(ss); 
    }

string Format::ProgressBar(float percent) {
  std::string result{"0%"};
  int size{100};
  float bars{(percent * size)};

  for (int i{0}; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  string display{std::to_string(percent).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + std::to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}