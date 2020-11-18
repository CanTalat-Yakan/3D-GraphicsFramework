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
    float4 pos : SV_POSITION;
    float3 WorldPos : POSITION;
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OUTPUT VS(float4 _vertex : POSITION, float2 _uv : TEXCOORD, float3 _normal : NORMAL)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, _vertex);
    o.normal = mul(World, _normal);
    o.WorldPos = mul(World, _vertex);
    o.UV = _uv;

    return o;
}

float4 PS(VS_OUTPUT _i) : SV_TARGET
{
    float4 col = ObjTexture.Sample(ObjSamplerState, _i.UV);

    //normalize normal
    float3 normal = normalize(_i.normal);

    //calculate diffuse 
    float d = dot(normal, light.direction); //by calculating the angle of the normal and the light direction with the dot method
    float4 diffuse = saturate(d * light.diffuse); //then saturating the diffuse so the backsite does not get values below 1
	
    return (light.ambient + diffuse) * col;
}