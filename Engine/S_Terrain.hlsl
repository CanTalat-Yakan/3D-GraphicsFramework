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
struct GS_OUTPUT
{
    float3 normal : NORMAL;
};

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
    d2 = 0.1 * pow(d2, 30); //calculating power 30 to the specular
    float d3 = saturate(dot(_normal, viewDir)); // calculating the fresnel diffuse
    d3 = saturate(1 - pow(d3, 0.5)); //calculating power 0.5 to the fresnel

    
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
Texture2D ObjHeight : register(t2);
SamplerState ObjSamplerState : register(s0);

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;
    
    float displacement = ObjHeight.SampleLevel(ObjSamplerState, v.uv, 0).r;
    v.vertex += -v.normal * displacement * 25;
    
    o.normal = mul(World, float4(v.normal, 0));
    o.pos = mul(WVP, float4(v.vertex, 1));
    o.worldPos = mul(World, float4(v.vertex, 1));
    o.camPos = WCP;
    o.uv = v.uv;

    return o;
}

[maxvertexcount(3)]
void GS(triangle VS_OUTPUT i[3], inout TriangleStream<VS_OUTPUT> os)
{
    VS_OUTPUT t = (VS_OUTPUT) 0;
    
    float3 normal = normalize(cross(
        i[2].worldPos.xyz - i[0].worldPos.xyz,
        i[1].worldPos.xyz - i[0].worldPos.xyz));

    for (int j = 0; j < 3; j++)
    {
        t.normal = normal;
        t.pos = i[j].pos;
        t.worldPos = i[j].worldPos;
        t.camPos = i[j].camPos;
        t.uv = i[j].uv;
        os.Append(t);
    }
    
    os.RestartStrip();
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    //calculating normal
    float3 normal = normalize(i.normal);
    
    //calculating directionalLight
    float4 Lighting =
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
    Lighting += CalculateAllPointLights(
            normal,
            i.worldPos, i.camPos);

    float tileSize = 3;
    float accuracy = 0.025;
    
    float4 colXZ = ObjTexture.Sample(ObjSamplerState, fmod(i.worldPos.xz, tileSize) / tileSize);
    float4 colYZ = ObjTexture.Sample(ObjSamplerState, fmod(i.worldPos.yz, tileSize) / tileSize);
    float4 colXY = ObjTexture.Sample(ObjSamplerState, fmod(i.worldPos.xy, tileSize) / tileSize);

    normal = pow(abs(i.normal), accuracy);
    normal /= dot(normal, 1);


    return (colXZ * normal.y + colYZ * normal.x + colXY * normal.z) * (float4(Lighting.rgb, 1) + dirLight.ambient) * params.diffuse;
}