FROM ubuntu:22.04 AS ubuntu_setup

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

WORKDIR /workspace

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    make \
    cmake=3.22.1-1ubuntu1.22.04.2 \
    git \
    mingw-w64 \
    libglu1-mesa-dev \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Create minimal GL headers for Windows compilation
RUN mkdir -p /usr/x86_64-w64-mingw32/include/GL && \
    cat > /usr/x86_64-w64-mingw32/include/GL/gl.h << 'EOF'
#ifndef __gl_h_
#define __gl_h_

#define GL_VERSION_1_1 1

#include <KHR/khrplatform.h>

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

/* OpenGL 1.1 functions */
typedef void (__stdcall *GLPROC)();
void __stdcall glAccum(GLenum op, GLfloat value);
void __stdcall glAlphaFunc(GLenum func, GLclampf ref);
/* Add more function declarations as needed */

#endif
EOF

RUN cat > /usr/x86_64-w64-mingw32/include/GL/glu.h << 'EOF'
#ifndef __glu_h_
#define __glu_h_

#include <GL/gl.h>

#define GLU_VERSION_1_1 1

typedef struct GLUnurbs GLUnurbs;
typedef struct GLUquadric GLUquadric;
typedef struct GLUtesselator GLUtesselator;

/* GLU functions */
void __stdcall gluBeginCurve(GLUnurbs* nurb);
void __stdcall gluBeginPolygon(GLUtesselator* tess);
void __stdcall gluBeginSurface(GLUnurbs* nurb);
void __stdcall gluBeginTrim(GLUnurbs* nurb);
/* Add more GLU function declarations as needed */

#endif
EOF

# Create KHR platform header
RUN mkdir -p /usr/x86_64-w64-mingw32/include/KHR && \
    cat > /usr/x86_64-w64-mingw32/include/KHR/khrplatform.h << 'EOF'
#ifndef __khrplatform_h_
#define __khrplatform_h_

typedef int khronos_int32_t;
typedef unsigned int khronos_uint32_t;
typedef signed char khronos_int8_t;
typedef unsigned char khronos_uint8_t;

#define KHRONOS_APIENTRY __stdcall

#endif
EOF