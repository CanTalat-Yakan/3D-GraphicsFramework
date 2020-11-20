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
    float3 WorldPos : POSITION;
    float3 CamPos : POSITION1;
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, v.vertex);
    o.normal = mul(World, v.normal);
    o.WorldPos = mul(World, v.vertex);
    o.CamPos = mul(World, WCP);
    o.UV = v.uv;

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float4 col = ObjTexture.Sample(ObjSamplerState, i.UV);

    //normalize normal
    float3 normal = normalize(i.normal);

    //calculate diffuse 
    float d = dot(normal, light.direction); //by calculating the angle of the normal and the light direction with the dot method
    float4 diffuse = saturate(normalize(d) * light.diffuse * 0.9f); //then saturating the diffuse so the backsite does not get values below 1
	
    //calculate specular
    float3 viewDir = normalize(i.WorldPos - i.CamPos);
    float3 halfVec = viewDir + light.direction;
    float d2 = saturate(dot(normalize(halfVec), normal));
    float4 specular = 50 * d * pow(d2, 70) * (light.diffuse * 0.1f);


    return (diffuse + specular + light.ambient) * col;
}