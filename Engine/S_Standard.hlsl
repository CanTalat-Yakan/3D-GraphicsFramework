struct DirectionalLight
{
    float3 direction;
    float4 diffuse;
    float4 ambient;
    float intensity;
};
struct PointLight
{
    float3 position;
    float4 diffuse;
    float intensity;
    float radius;
};
struct Parameters
{
    float4 diffuse;
    float roughness;
};

cbuffer cbMatrix : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float3 WCP;
};
cbuffer cbLighting : register(b1)
{
    DirectionalLight dirLight;
    PointLight pointLight;
    PointLight pointLight2;
    PointLight pointLight3;
    PointLight pointLight4;
};
cbuffer cbParameter : register(b2)
{
    float time;
    Parameters params;
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

float3 calculateTangent(float3 _normal)
{
    float3 t1 = cross(_normal, float3(0, 0, 1));
    float3 t2 = cross(_normal, float3(0, 1, 0));


    return (length(t1) > length(t2)) ? normalize(t1) : normalize(t2);
}
float3 calculateTangent2(float3 _normal)
{
    float3 vec = float3(1.0, 0.0, 0.0);
    float d = dot(vec, _normal);

    if (abs(d) < 1.0e-3)
    {
        vec = float3(0.0, 1.0, 0.0);
        d = dot(vec, _normal);
    }

    
    return normalize(vec - d * _normal);
}
float CalculateFallOff(float _radius, float3 _lightDir)
{
    float fallOff = max(0, _radius - length(_lightDir)); //calculating the fallOff acording to the radius of the light

    //When no radius is applied, then calculate without any radius
    if (_radius < 0)
        fallOff = 1;

    
    return fallOff;
}
float4 CalculateDiffuse(float3 _normal, float3 _lightDir, float4 _diffuse, float _intensity, float _radius = -1)
{
    float fallOff = CalculateFallOff(_radius, _lightDir);

    float d = saturate(dot(_normal, normalize(_lightDir)) * fallOff); //calculating the dot product of the lightDir and the surface normal with fallOff


    return float4(d * _diffuse * _intensity);
}
float4 CalculateSpecular(float3 _normal, float3 _viewDir, float3 _lightDir, float4 _diffuse, float _intensity, float _radius = -1)
{
    float3 viewDir = normalize(_viewDir); //calculating the direction in which the camera targets
    float3 halfVec = viewDir + _lightDir; //the half Vector between the view Dir and the light
    float fallOff = CalculateFallOff(_radius, _lightDir);
    
    float d = saturate(dot(_normal, normalize(_lightDir)) * fallOff); //calculating the dot product of the lightDir and the surface normal with fallOff

    float d2 = saturate(dot(normalize(halfVec), _normal)); //calculating the area hit by the specular light
    d2 = (1 - params.roughness) * pow(d2, 30 + (20 * (1 - 2 * params.roughness))); //calculating power 30 to the specular
    float d3 = saturate(dot(_normal, viewDir)); // calculating the fresnel diffuse
    d3 = saturate(1 - 2 * pow(d3, 0.5)); //calculating power 0.5 to the fresnel

    
    return float4(2 * saturate(d) * (d2 + (d3 * 0.75)) * _diffuse * _intensity);
}
float4 CalculateAllPointLights(float3 _normal, float3 _worldPos, float3 _camPos)
{
    float4 col =
        CalculateDiffuse(
            _normal,
            _worldPos - pointLight.position,
            pointLight.diffuse, pointLight.intensity,
            pointLight.radius)
        + CalculateSpecular(
            _normal,
            _worldPos - _camPos,
            _worldPos - pointLight.position,
            pointLight.diffuse, pointLight.intensity,
            pointLight.radius)
        + CalculateDiffuse(
            _normal,
            _worldPos - pointLight2.position,
            pointLight2.diffuse, pointLight2.intensity,
            pointLight2.radius)
        + CalculateSpecular(
            _normal,
            _worldPos - _camPos,
            _worldPos - pointLight2.position,
            pointLight2.diffuse, pointLight2.intensity,
            pointLight2.radius)
        + CalculateDiffuse(
            _normal,
            _worldPos - pointLight3.position,
            pointLight3.diffuse, pointLight3.intensity,
            pointLight3.radius)
        + CalculateSpecular(
            _normal,
            _worldPos - _camPos,
            _worldPos - pointLight3.position,
            pointLight3.diffuse, pointLight3.intensity,
            pointLight3.radius)
        + CalculateDiffuse(
            _normal,
            _worldPos - pointLight4.position,
            pointLight4.diffuse, pointLight4.intensity,
            pointLight4.radius)
        + CalculateSpecular(
            _normal,
            _worldPos - _camPos,
            _worldPos - pointLight4.position,
            pointLight4.diffuse, pointLight4.intensity,
            pointLight4.radius);
    
    return col;
}

Texture2D ObjTexture : register(t0);
Texture2D ObjNormal : register(t1);
SamplerState ObjSamplerState : register(s0);

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    o.pos = mul(WVP, float4(v.vertex, 1));
    o.normal = mul(World, float4(v.normal, 0));
    o.worldPos = mul(World, float4(v.vertex, 1));
    o.camPos = WCP;
    o.uv = v.uv;

    v.tangent = calculateTangent(normalize(o.normal));

    o.tangent = mul(float4(v.tangent, 0), World);
    o.binormal = cross(normalize(o.normal), normalize(o.tangent));
    o.tbn = float3x3(normalize(o.tangent), normalize(o.binormal), normalize(o.normal));

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float4 col = ObjTexture.Sample(ObjSamplerState, i.uv);
    float3 colNormal = ObjNormal.Sample(ObjSamplerState, float2(-i.uv.x, i.uv.y));
    
    //calculating normal
    float3 normal = normalize(i.normal);
    if (length(colNormal) > 0)
    {
        i.tbn = float3x3(i.tangent, i.binormal, i.normal);
        normal = mul(colNormal * 2 - 1, i.tbn);
        normal = normalize(normal);
    }

    
    //calculating directionalLight
    float4 directionalLight =
        CalculateDiffuse(
            normal,
            dirLight.direction,
            dirLight.diffuse, dirLight.intensity)
        + CalculateSpecular(
            normal,
            i.worldPos - i.camPos,
            dirLight.direction,
            dirLight.diffuse, dirLight.intensity);
    
    //calculating pointLight
    float4 PointLights = CalculateAllPointLights(normal, i.worldPos, i.camPos);


    return
        (directionalLight + PointLights + dirLight.ambient) * float4(col.rgb, 1);
}