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
};

float CalculateFallOff(float _radius, float3 _lightDir)
{
    float fallOff = saturate(_radius - length(_lightDir)); //calculating the fallOff acording to the radius of the light

    //When no radius is applied, then calculate without any radius
    if (_radius < 0)
        fallOff = 1;

    
    return fallOff;
}
float4 CalculateDiffuse(float3 _normal, float3 _lightDir, float4 _diffuse, float _radius = -1)
{
    float fallOff = CalculateFallOff(_radius, _lightDir);
    
    float d = saturate(sign(dot(_normal, normalize(_lightDir))) * fallOff); //calculating the dot product of the lightDir and the surface normal with fallOff

    
    return float4(d * _diffuse);
}
float4 CalculateSpecular(float3 _normal, float3 _viewDir, float3 _lightDir, float4 _diffuse, float _radius = -1)
{
    float3 viewDir = normalize(_viewDir); //calculating the direction in which the camera targets
    float3 halfVec = viewDir + _lightDir; //the half Vector between the view Dir and the light
    float fallOff = CalculateFallOff(_radius, _lightDir);

    float d = saturate(sign(dot(_normal, normalize(_lightDir))) * fallOff); //calculating the dot product of the lightDir and the surface normal with fallOff

    halfVec = viewDir + _lightDir;
    float d2 = saturate(dot(normalize(halfVec), _normal));
    d2 = (d2 > 0.95) ? 1 * fallOff : 0;
    float4 pointLightSpecular = d * pointLight.diffuse;


    return float4(d * d2 * _diffuse);
}

Texture2D ObjTexture : register(t0);
SamplerState ObjSamplerState : register(s0);

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

    //calculating normal
    float3 normal = normalize(i.normal);


    //calculating directionalLight
    float4 directionalLight =
        CalculateDiffuse(
            normal,
            dirLight.direction,
            dirLight.diffuse)
        + CalculateSpecular(
            normal,
            i.worldPos - i.camPos,
            dirLight.direction,
            dirLight.diffuse);
    
    //calculating pointLight
    float4 PointLight =
        CalculateDiffuse(
            normal,
            i.worldPos - pointLight.position,
            pointLight.diffuse,
            3.5)
        + CalculateSpecular(
            normal,
            i.worldPos - i.camPos,
            i.worldPos - pointLight.position,
            pointLight.diffuse,
            3.5);

    //calculate outline
    float d = saturate(dot(normal, normalize(i.worldPos - i.camPos)));
    d = (d < 0.25) ? -8 : 0;
    float4 outline = d * dirLight.diffuse;


    return (directionalLight + PointLight + outline + dirLight.ambient) * col;
}