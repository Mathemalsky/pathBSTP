#pragma once

#include <stdexcept>
#include <string>

class Exception : public std::exception {
public:
  explicit Exception(const char* message) : pMessage(message) {}
  explicit Exception(const std::string& message) : pMessage(message) {}

  virtual ~Exception() noexcept {}
  virtual const char* what() const noexcept { return pMessage.c_str(); }

protected:
  std::string pMessage;
};

class InvalidArgument : public Exception {
public:
  InvalidArgument(const std::string& msg) : Exception(msg) {}
};