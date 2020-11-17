struct Light
{
    float3 direction;
    float intensity;
    float4 ambient;
    float4 diffuse;
};

cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
    float3 WCP;
};

cbuffer cbPerFrame
{
    Light light;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION;
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);

    output.normal = mul(normal, World);

    output.UV = inTexCoord;

    output.WorldPos = mul(inPos, World);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    input.normal = normalize(input.normal);

    float d = dot(input.normal, light.direction);
    
    float4 col = ObjTexture.Sample(ObjSamplerState, input.UV);
    float4 diffuse = saturate(d * light.diffuse);
	
    float3 viewDirection = normalize(WCP - input.WorldPos);
    float3 halfVec = viewDirection + light.direction;
    float d2 = saturate(dot(halfVec, input.normal));
    //float4 specular = saturate(dot(viewDirection, input.normal));
    float4 specular =  d2;

    return (diffuse + specular) * col;
}