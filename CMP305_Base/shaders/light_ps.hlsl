// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 worldPos : TEXCOORD1;

};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour;
    float Zgrad = 1 - abs(input.normal.z);
    float Xgrad = 1 - abs(input.normal.x);
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = texture0.Sample(sampler0, input.tex);
	
	//CHECK IF SHE STEEP
	//CHECK IF SHE HIGH
	//MAKE GREEN
    
    if (input.worldPos.y  < 0)
    {
        textureColour = texture3.Sample(sampler0, input.tex);

    }
    else if (abs(input.normal.x) > 0.5 || abs(input.normal.z) > 0.5)
    {
        textureColour = texture1.Sample(sampler0, input.tex);

    }
    else if (input.worldPos.y >= 8)
    {
        textureColour = texture2.Sample(sampler0, input.tex);

    }
    else
    {
        textureColour = texture0.Sample(sampler0, input.tex);
    }
	
    lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
	
	return lightColour * textureColour;
}



