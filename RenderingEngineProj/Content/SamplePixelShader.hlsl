/*struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return float4(input.color, 1.0f);
}*/



struct PixelShaderInput
{

    float4 x : TEXCOORD0;
    float3 color : COLOR;
    float3 normal : NORMAL;
    float3 camera_pos : EYE;
    float2 tex : TEXCOORD1;

};

float4 main(PixelShaderInput input) : SV_TARGET
{
    // Assuming these are your variables
    float3 lightPosition = float3(-100.0f, -100.0f, -100.0f);

    // Calculate light direction from light source to origin
    //float3 lightDir = normalize(input.x.xyz - lightPosition);
    float3 lightDir = normalize(float3(0.0f, 0.0f, 0.0f) - lightPosition);

    // Calculate view direction
    float3 viewDir = normalize(input.camera_pos - input.x.xyz);

    // Get normalized normal vector
    float3 normal = normalize(input.normal);

    // Set constant colors for ambient and diffuse light
    float3 lightColor = float3(0.9f, 0.9f, 0.9f);
    float3 ambientColor = float3(0.3, 0.3, 0.3);

    // Calculate ambient light
    float3 ambient = input.color.rgb * ambientColor;

    // Calculate diffuse light
    float3 diffuse = input.color.rgb * lightColor * max(0, dot(normal, lightDir));

    // Calculate reflection vector
    float3 reflection = reflect(-lightDir, normal);

    // Set constant for specular light intensity
    float specularIntensity = 32.0f;

    // Calculate specular light
    float3 specular = lightColor * pow(max(0, dot(reflection, viewDir)), specularIntensity);

    // Combine all three light components
    float3 finalColor = ambient + diffuse + specular;
    
    float4 finalColor1 = float4(finalColor, 1.0f);
    //
    //
    //Texture
    //Texture2D gridTexture;
    //SamplerState gridSampler;
    // Sample from the grid texture
    //float4 textureColor = gridTexture.Sample(gridSampler, input.tex);

    // Combine the vertex color with the texture color
    //float4 finalColor2 = finalColor1 * textureColor;

    return float4(finalColor, 1.0f);
    //return finalColor2;
}
