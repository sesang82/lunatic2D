#include "globals.hlsli"

//structedBuffer
struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};


VSOut main(VSIn In)
{
    VSOut Out = (VSOut)0.0f;
    
    In.Pos.x;
    In.Pos.y;
    
     // ���� ����� �����. (���ÿ��� ���� �������� ��ȯ��Ű�� ����)
    // ����� ���� vector4�� Ȯ����Ѽ� �س��� ������ ���� ��ǥ w ���� ����������Ѵ�.
    // �̵��� ���ؼ��� w ���� 0�� �ƴ� 1�� ����� �ϹǷ� 1.0f�� �������ѵ�
    // x, y, z���� �̵��� ���õ� ������ ����ִ�.
    float4 world = mul(float4(In.Pos, 1.0f), WorldMatrix);
    float4 view = mul(world, ViewMatrix);
    float4 proj = mul(view, ProjectionMatrix);

     // ���� ����� ���ؽ� ������ ��ġ�� ������ķ� ������ش�.
    Out.Pos = proj;
    Out.Color = In.Color;
    Out.UV = In.UV;
    
    return Out;
}