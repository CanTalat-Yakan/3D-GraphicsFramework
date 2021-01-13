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
};
cbuffer cbParameter : register(b3)
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
    float3 vPos : POSITION2;
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


    d2 = //calculating power 30 to the specular
        (1 - params.roughness) * //calculating the specular according to roughness 1 => no specular
        (1 + params.metallic * 2) * //calculating the factor of the specular according to the metalic 1 => factor 9
        pow(d2, //calculating the power of the specular to make it the step smoother or more harsh
            params.metallic * 800 + //makes the spec sharper with metalic
            90 - (70 * (params.metallic))) - //base 90 to -70 roughness 1 => power 20 (smoother)
            (params.metallic * 0.5); //makes the spec smaller with metalic

    float d3 = saturate(dot(_normal, normalize(viewDir + _lightDir))); // calculating the fresnel diffuse
    d3 = (1 - params.roughness) * saturate(1 - pow(d3, 0.5 + 0.5 * params.metallic)); //calculating power 0.5 to the fresnel

    
    return float4(2 * saturate(d * (d2 + d3)) * _diffuse * _intensity);
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
SamplerState ObjSamplerState : register(s0);

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;

    v.vertex += float3(
        sin((cos(v.vertex.x * 0.2 + time) + v.vertex.x * 0.2) + time), 
        pow(noise(float2((v.vertex.x * 0.2 + time), (v.vertex.z * 0.2 + time))) * 3, 1) + sin((v.vertex.x * 0.2 + v.vertex.z * 0.2) + time),
        sin((cos(v.vertex.z * 0.2 - time) + v.vertex.z * 0.2) - time));

    o.pos = mul(WVP, float4(v.vertex, 1));
    o.normal = mul(World, float4(v.normal, 0));
    o.worldPos = mul(World, float4(v.vertex, 1));
    o.camPos = WCP;
    o.uv = float2(v.uv.x - time * 0.005, v.uv.y + time * 0.005);
    o.vPos = v.vertex;
    
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
        t.vPos = i[j].vPos;
        os.Append(t);
    }
    
    os.RestartStrip();
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float tiling = 14;
    float4 col = ObjTexture.Sample(ObjSamplerState, tiling * i.uv);
    
    
    //float3 base = float3(0.02, 0.12, 0.17); //(5, 30, 42)
    float3 base = float3(0.22, 0.32, 0.37); //(5, 30, 42)
    float3 light = float3(0.38, 0.56, 0.56); //(96, 142, 142);

    col *= float4(0.58, 0.76, 0.76, 1);

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
    
    //float4 foam = voronoise(float2((-i.uv.x * 100 + time2), (i.uv.y * 100 + time2)), 1, 1);
    float range = (pow(i.vPos.y, 0.25) - 1);
    //float4 foam = float4(float3(1, 1, 1) * range, 0.6);
    float4 detail = float4(base + saturate((light * range)).rgb, 0.2 + 2 * saturate(range));
    //float4 deep = float4(float3(0.2, 0.2, 0.5) * pow(saturate(1 - range), 1.8), 1);
    float4 deep = float4(base, 0.1);
    //float4 deep = float4(0.2 + base * pow(saturate(1 - range), 1.8), 0.1);

    //return float4((directionalLight + dirLight.ambient).rgb * col.rgb + (foam + deep).rgb, 1);
    return (directionalLight + dirLight.ambient) * float4(col.rgb, 0.1) + detail;
    //return float4((directionalLight + dirLight.ambient).rgb * base + (range + light), 1);
}