
// �� 16����Ʈ ������ �������� 

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

// �浹ü ���� 
cbuffer Col : register(b3)
{
    
    int c_ColCount;
    int c_Padding[3];
}

// �ִϸ����� ����
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


// ���α׷����� ����
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

Texture2D albedoTexture : register(t0); // �÷��̾� HP��, �׸��� �ٸ� �̹������� �⺻������ ����ϴ� ����
Texture2D albedoTexture1 : register(t1); // �÷��̾� SP��
// ==== ���ͳ� ���� �뵵 
Texture2D albedoTexture2 : register(t2); // �Ϲ� ���� HP��
Texture2D albedoTexture3 : register(t3); // ���� HP�� 

Texture2D atlasTexture : register(t12); // �ִϸ��̼ǿ��� 12������ �ؽ�ó�� ���ε� ���� 

SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);