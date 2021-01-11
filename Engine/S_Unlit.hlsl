struct Parameters
{
    float4 diffuse;
    float roughness;
};

cbuffer cbMatrix : register(b0)
{
    float4x4 WVP;
    float4x4 World;
};
cbuffer cbParameter : register(b2)
{
    float time;
    Parameters params;
};

struct appdata
{
    float3 vertex : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION;
};

Texture2D ObjTexture : register(t0);
SamplerState ObjSamplerState : register(s0);

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, float4(v.vertex, 1));
    o.worldPos = mul(World, float4(v.vertex, 1));

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    return params.diffuse;
}