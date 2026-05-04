#pragma once
#include <GL/glew.h>
#include <string>
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint id = 0;

    Shader(const char* vertSrc, const char* fragSrc) {
        GLuint vert = compile(GL_VERTEX_SHADER, vertSrc);
        GLuint frag = compile(GL_FRAGMENT_SHADER, fragSrc);
        id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        glLinkProgram(id);
        GLint ok; glGetProgramiv(id, GL_LINK_STATUS, &ok);
        if (!ok) {
            char log[1024]; glGetProgramInfoLog(id, 1024, nullptr, log);
            throw std::runtime_error(std::string("Shader link: ") + log);
        }
        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    ~Shader() { if (id) glDeleteProgram(id); }

    void use() const { glUseProgram(id); }

    void setMat4(const char* n, const float* v) const {
        glUniformMatrix4fv(glGetUniformLocation(id, n), 1, GL_FALSE, v);
    }
    void setInt(const char* n, int v) const {
        glUniform1i(glGetUniformLocation(id, n), v);
    }
    void setFloat(const char* n, float v) const {
        glUniform1f(glGetUniformLocation(id, n), v);
    }
    void setVec3(const char* n, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(id, n), x, y, z);
    }
    void setVec4(const char* n, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(id, n), x, y, z, w);
    }

private:
    GLuint compile(GLenum type, const char* src) {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);
        GLint ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[1024]; glGetShaderInfoLog(s, 1024, nullptr, log);
            throw std::runtime_error(std::string("Shader compile: ") + log);
        }
        return s;
    }
};

// ─── Chunk (world geometry) shaders ───────────────────────────────────────────

inline const char* CHUNK_VERT = R"glsl(
#version 330 core
layout(location = 0) in vec3  aPos;
layout(location = 1) in vec2  aUV;
layout(location = 2) in float aLight;

uniform mat4  uMVP;
uniform vec3  uCamPos;
uniform float uFogStart;
uniform float uFogEnd;

out vec2  vUV;
out float vLight;
out float vFog;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    vUV   = aUV;
    vLight = aLight;
    float dist = length(aPos - uCamPos);
    vFog = clamp((dist - uFogStart) / (uFogEnd - uFogStart), 0.0, 1.0);
}
)glsl";

inline const char* CHUNK_FRAG = R"glsl(
#version 330 core
in vec2  vUV;
in float vLight;
in float vFog;

uniform sampler2D uAtlas;
uniform vec3      uFogColor;
uniform float     uDaylight;

out vec4 FragColor;

void main() {
    vec4 tex = texture(uAtlas, vUV);
    if (tex.a < 0.1) discard;
    vec3 col = tex.rgb * vLight * uDaylight;
    col = mix(col, uFogColor, vFog);
    FragColor = vec4(col, tex.a);
}
)glsl";

// ─── 2-D overlay shaders (crosshair, HUD) ─────────────────────────────────────

inline const char* OVERLAY_VERT = R"glsl(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec4 uColor;
out vec4 vColor;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    vColor = uColor;
}
)glsl";

inline const char* OVERLAY_FRAG = R"glsl(
#version 330 core
in vec4 vColor;
out vec4 FragColor;
void main() { FragColor = vColor; }
)glsl";

// ─── Wireframe cube (block selection) ─────────────────────────────────────────

inline const char* WIRE_VERT = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 uMVP;
void main() { gl_Position = uMVP * vec4(aPos, 1.0); }
)glsl";

inline const char* WIRE_FRAG = R"glsl(
#version 330 core
out vec4 FragColor;
void main() { FragColor = vec4(0.0, 0.0, 0.0, 0.7); }
)glsl";
