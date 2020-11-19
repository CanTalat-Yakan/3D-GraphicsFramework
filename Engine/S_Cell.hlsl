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
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, v.vertex);
    o.normal = mul(World, v.normal);
    o.WorldPos = mul(World, v.vertex);
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
    float3 viewDir = normalize(WCP - i.WorldPos);
    float3 halfVec = viewDir + light.direction;
    float d2 = saturate(dot(normalize(halfVec), normal));
    d2 = pow(d2, 30);
    d2 /= 2.25;
    float4 specular = d * d2 * float4(0.8, 0.8, 0.8, 1);


    return (diffuse + light.ambient) * col;
}