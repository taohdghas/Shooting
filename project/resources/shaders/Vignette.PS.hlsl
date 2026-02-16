#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_Target0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    //周囲を0に、中心になるほど明るくなるように計算で調整
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    
    float vignette = correct.x * correct.y * 32.0f;
    vignette = saturate(pow(vignette, 0.8f));
    
    //赤っぽいビネット色
    float3 vignetteColor = float3(0.6f, 0.0f, 0.0f);

    // 周辺ほど赤く、中心ほど元の色
    output.color.rgb = lerp(vignetteColor, output.color.rgb, vignette);
    
    return output;
}