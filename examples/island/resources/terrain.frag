varying vec3 normal;
uniform float lightFactor;

void main()
{
    vec3 lightPosition = vec3(-1.0, 1.0, 1.0);
    vec3 eyePosition = vec3(0.0, 0.0, 1.0);
    vec3 halfVector = normalize(lightPosition + eyePosition);
    float intensity = lightFactor + (1.0 - lightFactor) * dot(normalize(normal), normalize(halfVector));
    gl_FragColor = gl_Color * vec4(intensity, intensity, intensity, 1.0);
}
