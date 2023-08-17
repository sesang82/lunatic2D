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
    
    // 기본적으로 초록색으로
    float4 color = float4(0.0f, 1.0f, 0.0f, 1.0f); // 초록
    
    // 충돌에 걸릴 때만
    if (c_ColCount > 0)
    {
         // 빨간색
        color = float4(1.0f, 0.0f, 0.0f, 1.0f); // 빨강
    }
   
    
    return color;
}