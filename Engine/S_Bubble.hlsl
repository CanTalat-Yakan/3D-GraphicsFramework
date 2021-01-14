struct DirectionalLight
{
    float3 direction;
    float4 diffuse;
    float4 ambient;
    float intensity;
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

float CalculateFallOff(float _radius, float3 _lightDir)
{
    float fallOff = max(0, _radius - length(_lightDir)); //calculating the fallOff acording to the radius of the light

    //When no radius is applied, then calculate without any radius
    if (_radius < 0)
        fallOff = 1;

    
    return fallOff;
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
    d2 = fallOff * pow(d2, 90);

    //Fresnel
    d3 = fallOff * saturate(1 - pow(d3, 0.5));

    
    return float4(2 * saturate(d * (max(d2, (d3 * 0.75)))) * _diffuse * _intensity);
}
float2 ReflectUV(float3 t3)
{
    float2 t2;
    t3 = normalize(t3) / sqrt(2.0);
    float3 q3 = abs(t3);
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

Texture2D SkyBoxTexture : register(t3);
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
    //normalize normal
    float3 normal = normalize(i.normal);
    
    float4 col = SkyBoxTexture.Sample(ObjSamplerState, ReflectUV(reflect(-i.worldPos - i.camPos, normalize(i.normal))));
	
     
    //calculating directionalLight
    float4 directionalLight =
        CalculateSpecular(
            normal,
            i.worldPos - i.camPos,
            dirLight.direction, dirLight.diffuse, dirLight.intensity);
    
    
    //calculate fresnel
    float3 viewDir = normalize(i.worldPos - i.camPos);
    float d = saturate(dot(normal, viewDir));
    float4 fresnel = saturate(1 - 5 * pow(d, 1.4));


    return (float4(fresnel.rgb, 0.6) + float4(dirLight.diffuse.rgb, 0.2)) * float4(col.rgb, 0.4) + directionalLight;
}