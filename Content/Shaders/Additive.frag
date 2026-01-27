#version 120

uniform sampler2D sourceTexture;
uniform sampler2D bloomTexture;

const float BLOOM_STRENGTH = 2.2;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    vec3 base  = texture2D(sourceTexture, uv).rgb;
    vec3 bloom = texture2D(bloomTexture, uv).rgb;

    // Linear space
    base  = pow(base, vec3(2.2));
    bloom = pow(bloom, vec3(2.2));

    vec3 result = base + bloom * BLOOM_STRENGTH;

    // Back to gamma
    result = pow(result, vec3(1.0 / 2.2));

    gl_FragColor = vec4(result, 1.0);
}
