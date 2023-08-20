#include "globals.hlsli"


// ����� �뵵�� ����ٰ� �׳� ���� ���ΰ��� (�Ϻη� �ּ�)
// ==================================
// ���α׷����� ����
//cbuffer Progressbar : register(b5)
//{
//    float p_currentHP;
//    float p_MaxHP;
//    float p_CurrentSP;
//    float p_MaxSP;

//    uint p_ProgressType;
//    uint p_TexType;
//    uint p_Padding[2];
    

//}

// ==================================
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
    float4 color = float4(0, 0, 0, 0); // ���� ������ ����ϱ� ���� �� �ʱ�ȭ�ϱ� 
    
    color = albedoTexture.Sample(pointSampler, In.UV);
        
    
    // ������ ������ ��ũ��Ʈ ���±� ������ �˾Ƽ� ���� �� ���̹Ƿ� �������ص� �� 
    
     // ���� HP ������ ����Ѵ�.  
    float BarRatio = p_CurrentSP / p_MaxSP;
    
    
    
    // ���� ���� (�����ʿ��� �������� ����)
    if (p_ProgressType == 0)
    {
    
    // bar �̹����� uv x ��ǥ�� HP ������ �°� �����Ѵ�.
    // ������-> ����
        if (In.UV.x > BarRatio)
        {
            discard;
        }
  
    }
    
    // ���ʿ��� ���������� ���� .. (�� ����) 
    else if (p_ProgressType == 1)
    {
        

    }
 

    return color;
}