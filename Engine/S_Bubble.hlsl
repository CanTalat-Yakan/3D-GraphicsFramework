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
    float3 viewDir = normalize(_viewDir); //calculating the direction in which the camera targets
    float3 halfVec = viewDir + _lightDir; //the half Vector between the view Dir and the light
    float fallOff = CalculateFallOff(_radius, _lightDir);
    
    float d = saturate(dot(_normal, normalize(_lightDir)) * fallOff); //calculating the dot product of the lightDir and the surface normal with fallOff

    float d2 = saturate(dot(normalize(halfVec), _normal)); //calculating the area hit by the specular light
    d2 = (1 - 0) * pow(d2, 30); //calculating power 30 to the specular
    float d3 = saturate(dot(_normal, viewDir)); // calculating the fresnel diffuse
    d3 = saturate(1 - 2 * pow(d3, 0.5)); //calculating power 0.5 to the fresnel

    
    return float4(2 * saturate(d) * (d2 + (d3 * 0.75)) * _diffuse * _intensity);
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
    
    //float4 col = ObjTexture.Sample(ObjSamplerState, i.uv);
    //float4 col = ObjTexture.SampleLevel(ObjSamplerState, mycubemap(reflect(-i.worldPos - i.camPos, normal)), 2);
    float4 col = SkyBoxTexture.Sample(ObjSamplerState, ReflectUV(reflect(-i.worldPos - i.camPos, normal)));
    //float4 col = texCUBElod(ObjTexture, float4(i.uv, 0));
    //float4 col2 = SAMPLE_TEXTURECUBE_LOD(ObjTexture, ObjSamplerState, reflect(-i.worldPos - i.camPos, normal), 0);
	
     
    //calculating directionalLight
    float4 directionalLight =
        CalculateSpecular(
            normal,
            i.worldPos - i.camPos,
            dirLight.direction,
            dirLight.diffuse, dirLight.intensity);
    
    
    //calculate fresnel
    float3 viewDir = normalize(i.worldPos - i.camPos);
    float d = saturate(dot(normal, viewDir));
    float4 fresnel = 1 - 5 * pow(d, 1.4);


    return (saturate(float4(fresnel.rgb, 0.1)) + directionalLight) * float4(dirLight.diffuse.rgb, 0.2) + float4(col.rgb, 0.4);
}