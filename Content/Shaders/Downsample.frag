#version 120

uniform sampler2D sourceTexture;
uniform vec2 texelSize;

const float THRESHOLD = 0.4;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    vec3 color = texture2D(sourceTexture, uv).rgb;

    // Convert to linear space (approx)
    color = pow(color, vec3(2.2));

    // Luminance
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));

    // Threshold
    color *= max(brightness - THRESHOLD, 0.0);

    gl_FragColor = vec4(color, 1.0);
}
