#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times); 
std::string ProgressBar(float percent); 
};                                    // namespace Format

#endif