
// 꼭 16바이트 단위로 정렬하자 

cbuffer Transform : register(b0)
{
    //float4 Position;
    row_major matrix WorldMatrix;
    row_major matrix ViewMatrix;
    row_major matrix ProjectionMatrix;
}

cbuffer Grid : register(b2)
{
    float4 CameraPosition;
    float2 CameraScale;
    float2 Resolution;
}

// 충돌체 관련 
cbuffer Col : register(b3)
{
    
    int c_ColCount;
    int c_Padding[3];
}

// 애니메이터 관련
cbuffer Animator : register(b4)
{
    float2 SpriteLeftTop;
    float2 SpriteSize;
    float2 SpriteOffset;
    float2 AtlasSize;
    uint animationType;
    uint reverse;
    uint2 padding;
}


// 프로그래스바 관련
cbuffer Progressbar: register(b5)
{
    float p_currentHP;
    float p_MaxHP;
    float p_CurrentSP;
    float p_MaxSP;

    uint p_ProgressType;
    uint p_TexType;
    uint p_Padding[2];
    

}


cbuffer Hit : register(b6)
{
// 충돌 중인지 아닌지
    int h_IsHit;
    int h_Padding[3];   

}

cbuffer Material : register(b7)
{
    int cbiData1;
    int cbiData2;
    int cbiData3;
    int cbiData4;
    
    float cbfData1;
    float cbfData2;
    float cbfData3;
    float cbfData4;
    
    float2 cbxy1;
    float2 cbxy2;
    float2 cbxy3;
    float2 cbxy4;
    
    float3 cbxyz1;
    float cbxyzPadding1;
    float3 cbxyz2;
    float cbxyzPadding2;
    float3 cbxyz3;
    float cbxyzPadding3;
    float3 cbxyz4;
    float cbxyzPadding4;
    
    float4 cbxyzw1;
    float4 cbxyzw2;
    float4 cbxyzw3;
    float4 cbxyzw4;
    
    matrix cbmat1;
    matrix cbmat2;
    matrix cbmat3;
    matrix cbmat4;

}

Texture2D albedoTexture : register(t0); // 플레이어 HP바, 그리고 다른 이미지들이 기본적으로 사용하는 슬롯
Texture2D albedoTexture1 : register(t1); // 플레이어 SP바
// ==== 몬스터나 보스 용도 
Texture2D albedoTexture2 : register(t2); // 일반 몬스터 HP바
Texture2D albedoTexture3 : register(t3); // 보스 HP바 

Texture2D atlasTexture : register(t12); // 애니메이션에서 12번으로 텍스처를 바인딩 해줌 

SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);