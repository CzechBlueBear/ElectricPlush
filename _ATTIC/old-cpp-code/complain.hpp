#ifndef COMPLAIN_HPP
#define COMPLAIN_HPP

#include <string>

#define COMPLAIN(msg) do { doComplain(__PRETTY_FUNCTION__, msg); } while(0)

void doComplain(const char *where, const std::string &msg);

#define ON_GL_ERROR_COMPLAIN() do { doOnGLErrorComplain(__PRETTY_FUNCTION__); } while(0)

void doOnGLErrorComplain(const char *where);

#endif