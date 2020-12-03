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
    float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 camPos : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3x3 tbn : POSITION2;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
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
    
    //float3 t1 = cross(o.normal, float3(0, 0, 1));
    //float3 t2 = cross(o.normal, float3(0, 1, 0));
    
    //float3 appdata_tangent = (length(t1) > length(t2)) ? t1 : t2;
    //float3 tangent = mul(World, float4(appdata_tangent, 0));
    //float3 bitangent = cross(o.normal, tangent);
 
    //o.tbn[0] = normalize(tangent);
    //o.tbn[1] = normalize(bitangent);
    //o.tbn[2] = normalize(o.normal);
    
    //add the tangent and binormal
    //o.tangent = normalize(mul(float4(v.tangent, 0), World).xyz);
    //o.binormal = normalize(cross(o.normal, o.tangent));

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float4 col = ObjTexture.Sample(ObjSamplerState, i.uv);
    float3 colNormal = ObjNormal.Sample(ObjSamplerState, float2(-i.uv.x, i.uv.y));
 
    //calulate normal
    float3 normal = normalize(i.normal);
    //float3 normal = colNormal * 2 - 1;
    //normal = i.tbn[0] * colNormal.x + i.tbn[1] * colNormal.y + i.tbn[2] * colNormal.z;
    //normal = normalize(normal);
    
    //calculate diffuse 
    float d = saturate(dot(normal, light.direction)); //by calculating the angle of the normal and the light direction with the dot method
    float4 diffuse = d * light.diffuse * light.intensity; //then saturating the diffuse so the backsite does not get values below 1
	
    //calculate specular
    float3 viewDir = normalize(i.worldPos - i.camPos);
    float3 halfVec = viewDir + light.direction;
    float d2 = saturate(dot(normalize(halfVec), normal));
    d2 = pow(d2, 30);
    float d3 = saturate(dot(normal, viewDir));
    d3 = saturate(1 - pow(d3, 0.5));
    float4 specular = 2 * saturate(d) * (d2 + (d3 * 0.75)) * light.diffuse;

    
    return (diffuse + specular + light.ambient) * col;
}