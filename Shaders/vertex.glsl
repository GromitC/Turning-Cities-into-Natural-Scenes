#version 150

// vertex shader
in vec3 vPos;
in vec3 vNorm;
in vec2 vTex;

// output values to fragment shader
out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 texCoord;
out float y;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main() {
    fN = vNorm;
    fE = vPos.xyz; 
    fL = LightPosition.xyz;

    texCoord = vTex;

    if( LightPosition.w != 0.0 ) { 
        fL = LightPosition.xyz - vPos.xyz; 
    }

    gl_Position = Projection*ModelView*vec4(vPos, 1.0); 
    y = vPos.y;
}
