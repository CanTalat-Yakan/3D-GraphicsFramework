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
};
cbuffer cbParameter : register(b3)
{
    float time;
};
cbuffer cbParameter2 : register(b2)
{
    float time2;
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
    d2 = pow(d2, 30); //calculating power 30 to the specular
    float d3 = saturate(dot(_normal, viewDir)); // calculating the fresnel diffuse
    d3 = saturate(1 - pow(d3, 0.5)); //calculating power 0.5 to the fresnel

    
    return float4(2 * saturate(d) * (d2 + (d3 * 0.75)) * _diffuse * _intensity);
}

float rand(float2 n)
{
    return frac(sin(dot(n, float2(12.9898, 4.1414))) * 43758.5453);
}
float noise(float2 p)
{
    float2 ip = floor(p);
    float2 u = frac(p);
    u = u * u * (3.0 - 2.0 * u);
	
    float res = lerp(
		lerp(rand(ip), rand(ip + float2(1.0, 0.0)), u.x),
		lerp(rand(ip + float2(0.0, 1.0)), rand(ip + float2(1.0, 1.0)), u.x), u.y);
    return res * res;
}

float3 hash3(float2 p)
{
    float3 q = float3(dot(p, float2(127.1, 311.7)),
				   dot(p, float2(269.5, 183.3)),
				   dot(p, float2(419.2, 371.9)));
    return frac(sin(q) * 43758.5453);
}
float voronoise(in float2 x, float u, float v)
{
    float2 p = floor(x);
    float2 f = frac(x);
		
    float k = 1.0 + 63.0 * pow(1.0 - v, 4.0);
	
    float va = 0.0;
    float wt = 0.0;
    for (int j = -2; j <= 2; j++)
        for (int i = -2; i <= 2; i++)
        {
            float2 g = float2(float(i), float(j));
            float3 o = hash3(p + g) * float3(u, u, 1.0);
            float2 r = g - f + o.xy;
            float d = dot(r, r);
            float ww = pow(1.0 - smoothstep(0.0, 1.414, sqrt(d)), k);
            va += o.z * ww;
            wt += ww;
        }
	
    return va / wt;
}

Texture2D ObjTexture : register(t0);
Texture2D ObjNormal : register(t1);
SamplerState ObjSamplerState : register(s0);

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    v.vertex += -v.normal *
        pow(noise(float2((v.vertex.x * 0.2 + time), (v.vertex.z * 0.2 + time))) * 3, 1.3);

    o.pos = mul(WVP, float4(v.vertex, 1));
    o.normal = mul(World, float4(v.normal, 0));
    o.worldPos = mul(World, float4(v.vertex, 1));
    o.camPos = WCP;
    o.uv = float2(v.uv.x - time * 0.005, v.uv.y + time * 0.005);

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float tileSize = 14;
    float4 col = ObjTexture.Sample(ObjSamplerState, i.uv * tileSize);
    col *= 0.5;
    col += float4(0, 0, 0.4, 1);
    float3 colNormal = ObjNormal.Sample(ObjSamplerState, float2(-i.uv.x, i.uv.y));

    //calculating normal
    float3 normal = normalize(i.normal);
    
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
    float4 PointLight =
        CalculateDiffuse(
            normal, 
            i.worldPos - pointLight.position, 
            pointLight.diffuse, pointLight.intensity, 
            3.5)
        + CalculateSpecular(
            normal, 
            i.worldPos - i.camPos, 
            i.worldPos - pointLight.position, 
            pointLight.diffuse, pointLight.intensity,
            3.5);
    
    float4 foam = voronoise(float2((-i.uv.x * 100 + time2), (i.uv.y * 100 + time2)), 1, 1);
    
    return
        (directionalLight + PointLight + dirLight.ambient) * float4(col.rgb,0.5);
}