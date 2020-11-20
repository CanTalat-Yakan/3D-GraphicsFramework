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

struct appdata
{
    float4 vertex : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 camPos : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, v.vertex);
    o.normal = mul(World, v.normal);
    o.worldPos = mul(World, v.vertex);
    o.camPos = mul(World, WCP);
    o.uv = v.uv;

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float3 normal = normalize(i.normal);

    float4 col = ObjTexture.Sample(ObjSamplerState, i.uv);
	
    float3 viewDir = normalize(i.worldPos - i.camPos);
    float d = saturate(dot(normal, viewDir));
    float4 fresnel = pow(d, 10);


    return (fresnel) * col;
}