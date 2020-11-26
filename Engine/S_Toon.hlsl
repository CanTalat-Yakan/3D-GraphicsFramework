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
};

Texture2D ObjTexture;
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

    //normalize normal
    float3 normal = normalize(i.normal);

    //calculate diffuse 
    float d = saturate(sign(dot(normal, light.direction))); //by calculating the angle of the normal and the light direction with the dot method
    float4 diffuse = d * light.diffuse * 0.9f; //then saturating the diffuse so the backsite does not get values below 1
	
    //calculate specular
    float3 viewDir = normalize(i.worldPos - i.camPos);
    float3 halfVec = viewDir + light.direction;
    float d2 = dot(normalize(halfVec), normal);
    d2 = (d2 > 0.95) ? 1 : 0;
    float4 specular = d * d2 * light.diffuse;

    //calculate outline
    float d3 = saturate(dot(normal, viewDir));
    d3 = (d3 < 0.25) ? -2 : 0;
    float4 outline = d3 * light.diffuse;

    
    return (diffuse + specular + outline + light.ambient) * col;
}