#include "Object3d.hlsli"

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
struct PointLight
{
    float4 color;//ライトの色
    float3 position;//ライトの位置
    float intensity;//輝度
};
struct Camera
{
    float3 worldPosition;
};
struct PointLight
{
    float4 color;//ライトの色
    float3 position; //ライトの位置
    float intensity;//輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
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
    }//
    //output.colorのα値が0のときにPixelを棄却
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    if (gMaterial.enableLighting != 0)//Lightingする場合
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        float RdotE = dot(reflectLight, toEye);
        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        float NDotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess); //反射強度
        //拡散反射
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb *
        gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        //鏡面反射
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity *
        specularPow * float3(1.0f, 1.0f, 1.0f);
         //pointLight
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        float3 pointhalfVector = normalize(-pointLightDirection + toEye);
        float pointNdotL = dot(normalize(input.normal), pointhalfVector);
        float pointcos = pow(pointNdotL * 0.5f + 0.5f, 2.0f);
        float3 pointreflectLight = reflect(pointLightDirection, normalize(input.normal));
        float pointRdotE = dot(pointreflectLight, toEye);
        float pointspecularPow = pow(saturate(pointRdotE), gMaterial.shininess);
        //拡散反射
        float3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb *
        gPointLight.color.rgb * cos * gPointLight.intensity;
        //鏡面反射
        float3 specularPointLight = gPointLight.color.rgb * gPointLight.intensity *
        pointspecularPow * float3(1.0f, 1.0f, 1.0f);
        //拡散反射 + 鏡面反射
       // output.color.rgb = diffuse + specular;
        output.color.rgb = diffuse + specular +
        diffusePointLight * specularPointLight;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else//Lightingしない場合。前回と同じ演算
    {
        output.color = gMaterial.color * textureColor;
    }
    return output;
}
