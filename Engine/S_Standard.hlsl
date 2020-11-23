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
    float4x4 Transl;
    float3 WCP;
};

cbuffer cbPerFrame
{
    Light light;
};

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

Texture2D ObjTexture;
SamplerState ObjSamplerState;

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, v.vertex);
    o.normal = mul(World, v.normal);
    o.worldPos = mul(World, v.vertex);
    o.camPos = mul(Transl, WCP);
    o.uv = v.uv;

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float4 col = ObjTexture.Sample(ObjSamplerState, i.uv);

    //normalize normal
    float3 normal = normalize(i.normal);

    //calculate diffuse 
    float d = saturate(dot(normal, light.direction)); //by calculating the angle of the normal and the light direction with the dot method
    float4 diffuse = d * light.diffuse * light.intensity; //then saturating the diffuse so the backsite does not get values below 1
	
    //calculate specular
    float3 viewDir = normalize(i.worldPos - i.camPos);
    float3 halfVec = viewDir + light.direction;
    float d2 = saturate(dot(normalize(halfVec), normal));
    float4 specular = 2 * saturate(d) * pow(d2, 30) * light.diffuse;
	
    return (diffuse + specular + light.ambient) * col;
}