#include "Object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
    int useBlinnPhong;
};
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};
struct DirectionalLight
{
    float4 color;//ライトの色
    float3 direction;//ライトの向き
    float intensity;//輝度
};
struct Camera
{
    float3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
   // output.color = gMaterial.color;
    float4 transformedUV = mul(float4(input.texcoord,0.0f,1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler,transformedUV.xy);
 
     //textureのα値が0.5以下の時にPixelを棄却
    if (textureColor.a <= 0.5)
    {
        discard;
    }
    //textureのα値が0の時にPixelを棄却
    if (textureColor.a == 0.0)
    {
        discard;
    }
    //output.colorのα値が0のときにPixelを棄却
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

        float specularPow;
        if (gMaterial.useBlinnPhong != 0)
        { // Blinn-Phong
            float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
            float NDotH = dot(normalize(input.normal), halfVector);
            specularPow = pow(saturate(NDotH), gMaterial.shininess);
        }
        else
        { // Phong
            float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
            float RdotE = dot(reflectLight, toEye);
            specularPow = pow(saturate(RdotE), gMaterial.shininess);
        }

        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);

        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    return output;
}
