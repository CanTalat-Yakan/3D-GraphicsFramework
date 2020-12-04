struct DirectionalLight
{
    float3 direction;
    float intensity;
    float4 diffuse;
    float4 ambient;
};

struct PointLight
{
    float3 position;
    float intensity;
    float4 diffuse;
    float radius;
};

cbuffer cbPerFrame
{
    DirectionalLight dirLight;
    PointLight pointLight;
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

float3 calculateTangent(float3 n)
{
    float3 v = float3(1.0, 0.0, 0.0);
    float d = dot(v, n);
    if (abs(d) < 1.0e-3)
    {
        v = float3(0.0, 1.0, 0.0);
        d = dot(v, n);
    }
    return normalize(v - d * n);
}

Texture2D ObjTexture : register(t0);
Texture2D ObjNormal : register(t1);
SamplerState ObjSamplerState : register(s0);

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, float4(v.vertex, 1));
    o.normal = normalize(mul(World, float4(v.normal, 0)));
    o.worldPos = mul(World, float4(v.vertex, 1));
    o.camPos = WCP;
    o.uv = v.uv;

    float3 t1 = cross(o.normal, float3(0, 0, 1));
    float3 t2 = cross(o.normal, float3(0, 1, 0));
    v.tangent = (length(t1) > length(t2)) ? normalize(t1) : normalize(t2);
    //v.tangent = calculateTangent(o.normal);

    o.tangent = normalize(mul(World, float4(v.tangent, 1)));
    o.binormal = normalize(cross(o.normal, o.tangent));
    o.tbn = float3x3(o.tangent, o.binormal, o.normal);

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float4 col = ObjTexture.Sample(ObjSamplerState, i.uv);
    float3 colNormal = ObjNormal.Sample(ObjSamplerState, float2(-i.uv.x, i.uv.y));
    
    //calulate normal
    float3 normal = i.normal;
    //i.tbn = float3x3(i.tangent, i.binormal, i.normal);
    //normal = mul(colNormal * 2 - 1, i.tbn);
    //normal = normalize(normal);
    

    //calculate dirLight diffuse 
    float d = saturate(dot(normal, dirLight.direction)); //by calculating the angle of the normal and the light direction with the dot method
    float4 diffuse = d * dirLight.diffuse * dirLight.intensity; //then saturating the diffuse so the backsite does not get values below 1
	
    //calculate dirLight specular
    float3 viewDir = normalize(i.worldPos - i.camPos);
    float3 halfVec = viewDir + dirLight.direction;
    float d2 = saturate(dot(normalize(halfVec), normal));
    d2 = pow(d2, 30);
    float d3 = saturate(dot(normal, viewDir));
    d3 = saturate(1 - pow(d3, 0.5));
    float4 specular = 2 * saturate(d) * (d2 + (d3 * 0.75)) * dirLight.diffuse;

    //calculate pointLight diffuse
    float3 pointDir = (i.worldPos - pointLight.position);
    float fallOff = saturate(3.5 - length(pointDir));
    float d4 = saturate(dot(normal, normalize(pointDir)) * fallOff);
    float4 pointLightDiffuse = d4 * pointLight.diffuse * pointLight.intensity;
    
    //calculate pointLight specular
    halfVec = viewDir + pointDir;
    float d5 = saturate(dot(normalize(halfVec), normal));
    d5 = pow(d5, 30);
    float d6 = saturate(dot(normal, viewDir));
    d6 = saturate(1 - pow(d6, 0.5));
    float4 pointLightSpecular = 2 * saturate(d4) * (d5 + (d6 * 0.75)) * pointLight.diffuse;


    return 
        (diffuse + specular + pointLightDiffuse + pointLightSpecular + dirLight.ambient) * col;
        float4(normal, 1);
}