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
    
     // 월드 행렬을 만든다. (로컬에서 월드 공간으로 변환시키기 위해)
    // 출력할 때는 vector4로 확장시켜서 해놨기 때문에 동차 좌표 w 값을 설정해줘야한다.
    // 이동을 위해서는 w 값을 0이 아닌 1로 해줘야 하므로 1.0f로 고정시켜둠
    // x, y, z에는 이동과 관련된 정보가 들어있다.
    float4 world = mul(float4(In.Pos, 1.0f), WorldMatrix);
    float4 view = mul(world, ViewMatrix);
    float4 proj = mul(view, ProjectionMatrix);

     // 최종 출력할 버텍스 버퍼의 위치를 투영행렬로 출력해준다.
    Out.Pos = proj;
    Out.Color = In.Color;
    Out.UV = In.UV;
    
    return Out;
}