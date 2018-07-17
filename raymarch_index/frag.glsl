#version 330

smooth in vec4 theColor;
out vec4 outputColor;
in vec4 p;

void main()
{

    vec2 iResolution = vec2(640,480);
    vec2 uv = gl_FragCoord.xy/iResolution.xy;
    //Move Center to 0.5
    uv += -0.5;

    //
    uv.x *= iResolution.x/iResolution.y;

    float d = length(uv);

    float c = d;
    float r = .3;

    //Instead of this gives aliasing thing.
    //inorder to remove this we use smoothstep
    /*if (d < r) {
      c = 1.;
      } else {
      c = 0.;
      }*/
    //smoothstep(start,end,input)
    c = smoothstep(r,r-0.005,d);


    // Output to screen
    outputColor = vec4(vec3(c),1.0);
    //outputColor = p;
}
