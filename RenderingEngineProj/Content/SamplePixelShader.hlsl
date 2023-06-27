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
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    // Assuming these are your variables
    float3 lightPosition = float3(-50, -50, -50);

    // Calculate light direction
    float3 lightDir = normalize(lightPosition - input.x.xyz);

    // Calculate view direction
    float3 viewDir = normalize(input.camera_pos - input.x.xyz);

    // Get normalized normal vector
    float3 normal = normalize(input.normal);

    // Set constant colors for ambient and diffuse light
    float3 lightColor = float3(0.8, 0.8, 0.8);
    float3 ambientColor = float3(0.6, 0.6, 0.6);

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

    return float4(finalColor, 1.0f);
}
