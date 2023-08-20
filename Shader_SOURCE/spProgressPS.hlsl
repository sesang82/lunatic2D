#include "globals.hlsli"


// 보라는 용도로 여기다가 그냥 갖다 붙인거임 (일부러 주석)
// ==================================
// 프로그래스바 관련
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
    float4 color = float4(0, 0, 0, 0); // 지역 변수는 사용하기 전에 꼭 초기화하기 
    
    color = albedoTexture.Sample(pointSampler, In.UV);
        
    
    // 각각의 정보는 스크립트 형태기 때문에 알아서 각자 들어갈 것이므로 걱정안해도 됨 
    
     // 현재 HP 비율을 계산한다.  
    float BarRatio = p_CurrentSP / p_MaxSP;
    
    
    
    // 감소 형태 (오른쪽에서 왼쪽으로 감소)
    if (p_ProgressType == 0)
    {
    
    // bar 이미지의 uv x 좌표를 HP 비율에 맞게 조절한다.
    // 오른쪽-> 왼쪽
        if (In.UV.x > BarRatio)
        {
            discard;
        }
  
    }
    
    // 왼쪽에서 오른쪽으로 증가 .. (힐 형태) 
    else if (p_ProgressType == 1)
    {
        

    }
 

    return color;
}