uniform sampler2D texture;
uniform float offset;

void main()
{
	vec2 offx = vec2(offset, 0.0);
	vec2 offy = vec2(0.0, offset);

	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy)               * 1 +
                 texture2D(texture, gl_TexCoord[0].xy - offx)        * 2 +
                 texture2D(texture, gl_TexCoord[0].xy + offx)        * 2 +
                 texture2D(texture, gl_TexCoord[0].xy - offy)        * 2 +
                 texture2D(texture, gl_TexCoord[0].xy + offy)        * 2 +
                 texture2D(texture, gl_TexCoord[0].xy - offx - offy) * 1 +
                 texture2D(texture, gl_TexCoord[0].xy - offx + offy) * 1 +
                 texture2D(texture, gl_TexCoord[0].xy + offx - offy) * 1 +
                 texture2D(texture, gl_TexCoord[0].xy + offx + offy) * 1;

	gl_FragColor =  gl_Color * (pixel / 13.0);
}
