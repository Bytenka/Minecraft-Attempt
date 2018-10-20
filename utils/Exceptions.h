#pragma once

#include <iostream>
#include <string>
#include <exception>

#define STOP_ON_ERROR(x)                                           \
  std::cout << x << "\n\nPress enter to continue..." << std::endl; \
  std::cin.get();                                                  \
  exit(EXIT_FAILURE)

/// Constructors can catch, but shall never throw!
/// Private functions that throw should handle the exception

namespace tk
{
class RuntimeException : public std::runtime_error
{
public:
  RuntimeException(std::string functionName, std::string whatHappend) noexcept : std::runtime_error("Unhandeled exception in function " + functionName)
  {
    // LOG EXCEPTION CREATED
    std::cout << "Stopping on exception...\n"
              << std::endl;
    m_context.append("Runtime exception caught in funtion \"" + functionName + "\": " + whatHappend + "\n");
  };

  ~RuntimeException() noexcept
  {
    std::cout << m_context << std::endl;
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.get();
    std::cout << "Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }

  void pushCurrentContext(std::string functionName, std::string containerName = "", std::string additionalInfo = "") noexcept // Additional info is the class name, the variable values at the moment etc...
  {
    m_context.append("\tCalled from function \"" + functionName + "\"");

    if (!containerName.empty())
      m_context.append(", in container \"" + containerName + "\"");

    if (!additionalInfo.empty())
      m_context.append(": " + additionalInfo);

    m_context.append("\n");
  }

private:
  std::string m_context;
};
}; // namespace tk