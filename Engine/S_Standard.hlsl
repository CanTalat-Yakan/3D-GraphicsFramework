struct Light
{
    float3 direction;
    float intensity;
    float4 ambient;
    float4 diffuse;
};

cbuffer cbPerFrame
{
    Light light;
};

cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
    float3 WCP;
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
    float3 camPos : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4x4 tangent : TANGENT;
};

Texture2D ObjTexture : register(t0);
Texture2D ObjNormal : register(t1);
SamplerState ObjSamplerState;

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, float4(v.vertex, 1));
    o.normal = mul(World, float4(v.normal, 0));
    o.worldPos = mul(World, float4(v.vertex, 1));
    o.camPos = WCP;
    o.uv = v.uv;

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float4 col = ObjTexture.Sample(ObjSamplerState, i.uv);
    float4 colNormal = ObjNormal.Sample(ObjSamplerState, i.uv);

    //normalize normal
    i.normal = normalize(i.normal);

    //calculate diffuse 
    float d = saturate(dot(i.normal, light.direction)); //by calculating the angle of the normal and the light direction with the dot method
    float4 diffuse = d * light.diffuse * light.intensity; //then saturating the diffuse so the backsite does not get values below 1
	
    //calculate specular
    float3 viewDir = normalize(i.worldPos - i.camPos);
    float3 halfVec = viewDir + light.direction;
    float d2 = saturate(dot(normalize(halfVec), i.normal));
    d2 = pow(d2, 30);
    float d3 = saturate(dot(i.normal, viewDir));
    d3 = saturate(1 - pow(d3, 0.5));
    float4 specular = 2 * saturate(d) * (d2 + (d3 * 0.75)) * light.diffuse;
    
    //calulate normalMap
    float3 tangent = colNormal * 2 - 1;
    float d4 = dot(tangent, light.direction);
    float4 normal = (saturate(d) * d4) * light.diffuse;
    
    return (diffuse + specular + normal + light.ambient) * col;
}