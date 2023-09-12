#include "globals.hlsli"

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


float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float4) 0.0f;
    
    // 1200 1032 // 120 130
    // 1080 -> 540
    // -540 + 1200 
    color = albedoTexture.Sample(pointSampler, In.UV);
    
    if (animationType == 1)
    {
        // flip��� Ȱ��ȭ���� ��� �Ʒ�ó�� ���������ش�. 
        if (reverse == 1)
            In.UV.x = 1 - In.UV.x;
        
        float2 diff = (AtlasSize - SpriteSize) / 2.0f;
        float2 UV = (SpriteLeftTop - diff - SpriteOffset)
                + (AtlasSize * In.UV);
    
        if (UV.x < SpriteLeftTop.x || UV.x > SpriteLeftTop.x + SpriteSize.x
            || UV.y < SpriteLeftTop.y || UV.y > SpriteLeftTop.y + SpriteSize.y)
            discard;
        
       
            color = atlasTexture.Sample(pointSampler, UV);
       
    }
    
    
    if (h_IsHit) // �ӽ������� true�� �����Ͽ� �׽�Ʈ
    {
        return color + float4(1, 0, 0, color.a); // ���� �̹��� �ȼ����ٰ� �������� �����δ�. (���� ���İ��� ����)
    }
    else
    {
        return color;
    }

   

}