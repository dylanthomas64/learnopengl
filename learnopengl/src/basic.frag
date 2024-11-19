#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float blend;

const float offset = 1.0 / 300.0;  

vec3 rgb2hsv(vec3 c);



void main()
{
    
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];

    // texture1 kernel
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture1, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
        col += sampleTex[i] * kernel[i];
    }

    //texture2 kernel
     for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture2, TexCoord.st + offsets[i]));
    }
    vec3 sorted_col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
        sorted_col += sampleTex[i] * kernel[i];
    }



    //vec3 original_col = vec3(texture(texture1, TexCoord.st));
    gl_FragColor = mix(vec4(col, 1.0), vec4(sorted_col, 1.0), blend);
}  