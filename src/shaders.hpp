const char* const g_vertexSource = R"(
#version 330
precision highp float;

layout(location = 0) in vec2 pos;

void main() {
    gl_Position = vec4(pos, 0, 1);
}
)";

const char* const g_fragmentSource = R"(
#version 330
precision highp float;

uniform vec4 color;
out vec4 outColor;

void main() {
    outColor = color;
}
)";
