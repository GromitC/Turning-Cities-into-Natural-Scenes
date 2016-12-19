#version 150

// fragment shader 
out vec4 fColor; 
// per-fragment interpolated values from the 
// vertex shader 
in vec3 fN; 
in vec3 fL; 
in vec3 fE; 
in vec2 texCoord;

in float y; // for texture interpolation

uniform sampler2D text1;
uniform sampler2D text2;
uniform sampler2D text3;
uniform sampler2D text4;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

uniform int lighting;

void main() {
    // Normalize the input lighting vectors 
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    vec3 H = normalize( L + E ); 
    vec4 ambient = AmbientProduct; 

    float Kd = max(dot(L, N), 0.0); 
    vec4 diffuse = Kd*DiffuseProduct; 

    float Ks = pow(max(dot(N, H), 0.0), Shininess); 
    vec4 specular = Ks*SpecularProduct;
    //discard the specular highlight if the 
    //vertex is not facing light 
    if( dot(L, N) < 0.0 )
        specular = vec4(0.0, 0.0, 0.0, 1.0); 

    vec4 shadeLight = ambient + diffuse + specular; 
    shadeLight.a = 1.0;

    vec4 text1Tex = vec4(texture(text1,texCoord).rgb, 1.0);
    vec4 text2Tex = vec4(texture(text2,texCoord).rgb, 1.0);
    vec4 text3Tex = vec4(texture(text3,texCoord).rgb, 1.0);
    vec4 text4Tex = vec4(texture(text4,texCoord).rgb, 1.0);

    float newY = y * 1000;

    vec4 texInterp;
    //if (newY<=5){
    //texInterp = vec4(clamp(1.0f - abs(newY - 0)/100.0f, 0, 0.5), clamp(1.0f - abs(newY - 50)/150.0f, 0, 1), clamp(1.0f - abs(newY - 200)/150.0f, 0, 0.5), clamp(1.0f - abs(newY - 500)/150.0f, 0, 1));
    //}
    //else if(newY>=100){
    //    texInterp = vec4(clamp(1.0f - abs(newY - 0)/100.0f, 0, 0.5), clamp(1.0f - abs(newY - 50)/150.0f, 0, 1), clamp(1.0f - abs(newY - 100)/150.0f, 0, 0.5), 2);
    //}
    //else{
    texInterp = vec4(clamp(1.0f - abs(newY - 0)/100.0f, 0, 1), clamp(1.0f - abs(newY - 50)/150.0f, 0, 1), clamp(1.0f - abs(newY - 100)/150.0f, 0, 0.5), clamp(1.0f - abs(newY - 150)/150.0f, 0.3, 1));
    //}

    texInterp = texInterp / (texInterp.x + texInterp.y + texInterp.z + texInterp.w);
    vec4 shadeTex = (texInterp.x * text1Tex) + (texInterp.y * text2Tex) + (texInterp.z * text3Tex) + (texInterp.w * text4Tex);

    fColor = shadeLight*shadeTex;
    //fColor = shadeLight;./
}
