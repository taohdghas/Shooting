#include "Skybox.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
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
struct PointLight
{
    float4 color;//ライトの色
    float3 position;//ライトの位置
    float intensity;//輝度
    float radius;//ライトの届く最大距離
    float decay;//減衰率
};
struct SpotLight
{
    float4 color; //ライトの色
    float3 position; //ライトの位置
    float intensity; //輝度
    float3 direction; //スポットライトの方向
    float distance; //ライトの届く最大距離
    float decay; //減衰率
    float cosAngle; //スポットライトの余弦
    float cosFalloffStart;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<SpotLight> gSpotLight : register(b4);
TextureCube<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
   // output.color = gMaterial.color;
    float4 transformedUV = mul(float4(input.texcoord,0.0f,1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler,transformedUV.xy);
   
    if (gMaterial.enableLighting != 0)//Lightingする場合
    {
        
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);
        //拡散反射
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb *
        gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        //鏡面反射
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity *
        specularPow * float3(1.0f, 1.0f, 1.0f);
        
        //PointLight
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        float3 pointhalfVector = normalize(-pointLightDirection + toEye);
        float pointNdotH = dot(normalize(input.normal), pointhalfVector);
        float pointcos = pow(pointNdotH * 0.5f + 0.5f, 2.0f);
        float pointspecularPow = pow(saturate(pointNdotH), gMaterial.shininess);
        float distance = length(gPointLight.position - input.worldPosition);
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        
        //拡散反射
        float3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb *
        gPointLight.color.rgb * pointcos * gPointLight.intensity * factor;
        //鏡面反射
        float3 specularPointLight = gPointLight.color.rgb * gPointLight.intensity *
        pointspecularPow * factor * float3(1.0f, 1.0f, 1.0f);
        
        //SpotLight
        float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        float3 spothalfVector = normalize(-spotLightDirectionOnSurface + toEye);
        float spotNdotH = dot(normalize(input.normal), spothalfVector);
        float spotspecularPow = pow(saturate(spotNdotH), gMaterial.shininess);
        float spotdistance = length(gSpotLight.position - input.worldPosition);
        float attenuationFactor = pow(saturate(-spotdistance / gSpotLight.distance + 1.0), gSpotLight.decay);
        float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
        float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
        
        //拡散反射
        float3 diffuseSpotLight = gMaterial.color.rgb * textureColor.rgb *
        gSpotLight.color.rgb * gSpotLight.intensity * attenuationFactor * falloffFactor;
        //鏡面反射
        float3 specularSpotLight = gSpotLight.color.rgb * gSpotLight.intensity *
        spotspecularPow * attenuationFactor * falloffFactor;
        
        //拡散反射+鏡面反射
        output.color.rgb = diffuse + specular + diffusePointLight + specularPointLight
        + diffuseSpotLight + specularSpotLight;
        
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else//Lightingしない場合。前回と同じ演算
    {
        output.color = gMaterial.color * textureColor;
    }
    return output;
}
