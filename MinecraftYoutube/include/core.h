#ifndef MINECRAFT_CLONE_CORE_H
#define MINECRAFT_CLONE_CORE_H

// Glad and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library stuff
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <filesystem>

// GLM stuff
#define GLM_EXT_INCLUDED
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

// Use this for hash map and hash sets instead of the crappy std lib
#include <robin_hood.h>

// My libraries
#include <cppUtils/cppUtils.hpp>

// Stb libraries
#include <stb_image.h>

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

#endif 