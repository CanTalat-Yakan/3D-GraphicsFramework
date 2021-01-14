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
    float metallic;
    float opacity;
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
    float3 viewDir = normalize(_viewDir);
    float3 lightDir = normalize(_lightDir);

    float3 reflectedLightDir = normalize(reflect(lightDir, _normal));
    float3 halfVec = normalize(viewDir + lightDir); //the half Vector between the view Dir and the reflected light
    float fallOff = CalculateFallOff(_radius, _lightDir);

    float d = saturate(dot(lightDir, _normal) * fallOff); //calculating the dot product of the lightDir and the surface normal with fallOff
    float d2 = dot(-reflectedLightDir, viewDir); //calculating the area hit by the specular light
    float d3 = saturate(dot(_normal, viewDir)); //calculating the fresnel

    //Specular
    d2 = fallOff *
         (1 - params.roughness) * //calculating the specular according to roughness 1 => no specular
         (1 + params.metallic * 2) * //calculating the factor of the specular according to the metalic 1 => factor 9
         pow(d2, //calculating the power of the specular to make it the step smoother or more harsh
             params.metallic * 1000 + //makes the spec sharper with metalic
             90 - (70 * (params.roughness))) - //base 90 to -70 roughness 1 => power 20 (smoother)
             (params.metallic * 0.5); //makes the spec smaller with metalic

    //Fresnel
    d3 = fallOff *
         (1 - params.roughness) * //with roughness no fresnel
         saturate(1 - pow(d3,
                          0.5 + //Base fresnel Power
                          0.5 * params.metallic * (1 - d3))); //additional fresnel power with metallic

    
    return float4(saturate(d * (max(d2, (d3 * 0.75)))) * _diffuse * _intensity);
}
float4 CalculateAllPointLights(float3 _normal, float3 _worldPos, float3 _camPos)
{
    float4 col =
        CalculateDiffuse(
            _normal,
            _worldPos - pointLight.position,
            pointLight.diffuse, pointLight.intensity, pointLight.radius)
        + CalculateSpecular(
            _normal,
            _worldPos - _camPos,
            _worldPos - pointLight.position,
            pointLight.diffuse, pointLight.intensity, pointLight.radius)
        + CalculateDiffuse(
            _normal,
            _worldPos - pointLight2.position,
            pointLight2.diffuse, pointLight2.intensity, pointLight2.radius)
        + CalculateSpecular(
            _normal,
            _worldPos - _camPos,
            _worldPos - pointLight2.position,
            pointLight2.diffuse, pointLight2.intensity, pointLight2.radius)
        + CalculateDiffuse(
            _normal,
            _worldPos - pointLight3.position,
            pointLight3.diffuse, pointLight3.intensity, pointLight3.radius)
        + CalculateSpecular(
            _normal,
            _worldPos - _camPos,
            _worldPos - pointLight3.position,
            pointLight3.diffuse, pointLight3.intensity, pointLight3.radius)
        + CalculateDiffuse(
            _normal,
            _worldPos - pointLight4.position,
            pointLight4.diffuse, pointLight4.intensity, pointLight4.radius)
        + CalculateSpecular(
            _normal,
            _worldPos - _camPos,
            _worldPos - pointLight4.position,
            pointLight4.diffuse, pointLight4.intensity, pointLight4.radius);
    
    return col;
}
float2 ReflectUV(float3 t3)
{
    float2 t2;
    t3 = normalize(t3) / sqrt(2.0);
    //float3 q3 = abs(t3);
    float3 q3 = t3;
    if ((q3.x >= q3.y) && (q3.x >= q3.z))
    {
        t2.x = 0.5 - t3.z / t3.x;
        t2.y = 0.5 - t3.y / q3.x;
    }
    else if ((q3.y >= q3.x) && (q3.y >= q3.z))
    {
        t2.x = 0.5 + t3.x / q3.y;
        t2.y = 0.5 + t3.z / t3.y;
    }
    else
    {
        t2.x = 0.5 + t3.x / t3.z;
        t2.y = 0.5 - t3.y / q3.z;
    }
    return t2;
}
float4 CalculateReflection(float4 _col, float3 _normal, float3 _viewDir)
{
    _col *= saturate(pow(params.metallic, 5)) * 0.15f;
    float3 viewDir = normalize(_viewDir);
    float d = saturate(dot(_normal, viewDir));
    float4 fresnel = saturate(1 - 5 * pow(d, 1 + params.metallic)) * params.metallic;
    return _col * (1 + fresnel);
}

Texture2D ObjTexture : register(t0);
Texture2D ObjNormal : register(t1);
Texture2D ObjSkyBox : register(t3);
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

[maxvertexcount(3)]
void GS(triangle VS_OUTPUT i[3], inout TriangleStream<VS_OUTPUT> os)
{
    VS_OUTPUT t = (VS_OUTPUT) 0;


    for (int j = 0; j < 3; j++)
    {
        t.normal = i[j].normal;
        t.pos = i[j].pos;
        t.worldPos = i[j].worldPos;
        t.camPos = i[j].camPos;
        t.uv = i[j].uv;

        float3 tangent = i[1].worldPos.xyz - i[0].worldPos.xyz;
        float3 bitangent = i[2].worldPos.xyz - i[0].worldPos.xyz;
        //float3 bitangent = cross(tangent, t.normal);
        t.tbn = float3x3(normalize(tangent), normalize(bitangent), normalize(t.normal));
        os.Append(t);
    }
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float4 col = ObjTexture.Sample(ObjSamplerState, i.uv);
    float3 colNormal = ObjNormal.Sample(ObjSamplerState, float2(-i.uv.x, i.uv.y));
    
    //calculating normal
    float3 normal = normalize(i.normal);
    if (length(colNormal) > 0)
        normal = normalize(mul(normalize(colNormal * 2 - 1), i.tbn));
        //normal = normalize(mul(i.tbn, normalize(colNormal * 2 - 1)));

    float4 colReflect = ObjSkyBox.SampleLevel(ObjSamplerState, ReflectUV(reflect(-i.worldPos - i.camPos, normal)), params.roughness * 10);
    colReflect = CalculateReflection(colReflect, normal, i.worldPos - i.camPos);

    //calculating directionalLight
    float4 directionalLight =
        CalculateDiffuse(
            normal,
            dirLight.direction, dirLight.diffuse, dirLight.intensity)
        + CalculateSpecular(
            normal,
            i.worldPos - i.camPos,
            dirLight.direction, dirLight.diffuse, dirLight.intensity);

    float4 pointLights = CalculateAllPointLights(normal, i.worldPos, i.camPos);

    return
        float4(
            ((directionalLight + dirLight.ambient) * (col * params.diffuse + colReflect) + (pointLights * 0.382)).rgb, 
            1 - params.opacity);
    float4(normal.rgb, 1);
}