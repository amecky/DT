uniform extern float4x4 gWVP;
uniform extern texture gTex;

sampler TexS = sampler_state {
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

struct OutputVS {
	float4 posH   : POSITION0;
	float2 tex0   : TEXCOORD0;
	float4 color0 : COLOR0;
};

OutputVS BasicVS(float3 posL : POSITION0,float2 tex0 : TEXCOORD0 , float4 color : COLOR0) {
	OutputVS outVS = (OutputVS)0;	
	outVS.posH = mul(float4(posL, 1.0f), gWVP);		
	outVS.tex0 = tex0;
	outVS.color0 = color;
	return outVS;
}

float4 BasicPS(OutputVS input) : COLOR {
	float4 clr = tex2D(TexS, input.tex0);
	//return clr * input.color0;
	return float4(1,0,0,1);
}

technique PTCTech {
	pass P0 {
		vertexShader = compile vs_2_0 BasicVS();
		pixelShader  = compile ps_2_0 BasicPS();
	}
};