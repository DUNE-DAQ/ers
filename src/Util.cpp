#include <stdio.h>

#include <ers/internal/Util.hpp>
#include <ers/internal/macro.hpp>

void
ers::tokenize(const std::string& text, const std::string& separators, std::vector<std::string>& result)
{
  std::string::size_type start_p, end_p;
  start_p = 0;
  do {
    end_p = text.find_first_of(separators, start_p);
    if (end_p == std::string::npos) {
      end_p = text.length();
    }
    result.push_back(text.substr(start_p, end_p - start_p));
    start_p = text.find_first_not_of(separators, end_p);
  } while (start_p != std::string::npos);
}

int
ers::read_from_environment(const char* name, int default_value)
{
  int value = default_value;
  const char* env = ::getenv(name);
  if (env) {
    if (sscanf(env, "%d", &value) != 1) {
      ERS_INTERNAL_ERROR("Wrong value \"" << env << "\" is given for the \"" << name << "\" environment")
    }
  }
  return value;
}

const char*
ers::read_from_environment(const char* name, const char* default_value)
{
  const char* env = ::getenv(name);
  return (env ? env : default_value);
}
