struct VOut {
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD0;
    float4 color    : COLOR;
};

VOut VShader(float4 position : POSITION,float2 texcoord : TEXCOORD0, float4 color : COLOR) {
    VOut output;

    output.position = position;
    output.color = color;
    output.tex = texcoord;
    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET {
    return color;
}