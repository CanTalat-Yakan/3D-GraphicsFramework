cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
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
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

Texture2D ObjTexture : register(t0);
SamplerState ObjSamplerState : register(s0);

VS_OUTPUT VS(appdata v)
{
    VS_OUTPUT o;
    
    o.pos = mul(WVP, float4(v.vertex, 1));
    o.normal = v.normal;
    o.uv = float2(v.uv.x / 4.0035, v.uv.y / 3.0035);

    return o;
}

float4 PS(VS_OUTPUT i) : SV_TARGET
{
    float2 pos = float2(0.25, 0.3335);
    float2 offset = float2(2.0005, 1);

    float left = saturate(sign(i.normal.x));
    float right = saturate(sign(-i.normal.x));
    float front = saturate(sign(i.normal.z));
    float back = saturate(sign(-i.normal.z));
    float top = saturate(sign(i.normal.y));
    float base = saturate(sign(-i.normal.y));

    offset.x += -2 * left + -1 * front + 0 * right + 1 * back;
    offset.y += 1 * top + -1 * base;

    pos.x *= offset.x;
    pos.y *= offset.y;

    float4 col = ObjTexture.Sample(ObjSamplerState, -pos + -i.uv);


    return col;
}