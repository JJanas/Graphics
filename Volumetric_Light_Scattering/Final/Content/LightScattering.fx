float4x4 World;
float4x4 View;
float4x4 Projection;
float4x4 WorldInverseTranspose;

float3 CameraPosition;
float3 LightPosition;
float4 FinalLightPosition;
// Light Uniforms
float4 AmbientColor;
float AmbientIntensity;

float4 DiffuseColor;
float DiffuseIntensity;

float4 SpecularColor;
float SpecularIntensity;
float Shininess;

float Weight;
float Decay;
float Density;
float Exposure;
texture lit;
texture prePass;

sampler tsampler1 = sampler_state
{
    texture = <lit>;
    magfilter = LINEAR; // None, POINT, LINEAR, Anisotropic
    minfilter = LINEAR;
    mipfilter = LINEAR;
    AddressU = Clamp; // Clamp, Mirror, MirrorOnce, Wrap, Border
    AddressV = Clamp;
};
sampler tsampler2 = sampler_state
{
    texture = <prePass>;
    magfilter = LINEAR; // None, POINT, LINEAR, Anisotropic
    minfilter = LINEAR;
    mipfilter = LINEAR;
    AddressU = Clamp; // Clamp, Mirror, MirrorOnce, Wrap, Border
    AddressV = Clamp;
};

struct VertexInput
{
    float4 Position : POSITION;
    float4 Normal : NORMAL;
};
struct PhongVertexOutput
{
    float4 Position : POSITION;
    float3 Normal : TEXCOORD0;
    float3 WorldPosition : TEXCOORD1;
};

PhongVertexOutput PhongVS(VertexInput input)
{
    PhongVertexOutput output;
    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);
    output.Normal = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
    output.WorldPosition = worldPosition.xyz;
    return output;
}
float4 PhongPS(PhongVertexOutput input) : COLOR0
{
    float3 L = normalize(LightPosition - input.WorldPosition);
    float3 V = normalize(CameraPosition - input.WorldPosition);
    float3 N = normalize(input.Normal);
    float3 R = reflect(-L, N);
    float4 diffuse = DiffuseColor * DiffuseIntensity * max(0, dot(N, L));
    diffuse.a = 1.0;
    float4 specular = pow(max(0, dot(V, R)), Shininess) * SpecularColor * SpecularIntensity;
    specular.a = 1.0;
    float4 ambient = AmbientColor * AmbientIntensity;
    return saturate(diffuse + specular + ambient);
}

/*
* Included SV_Position and Color as it is in the wrong position and doesn't display anything 
* From Textbook
*/
float4 PostProcessPS(float4 position : SV_Position, float4 theColor : COLOR0, float2 texCoordIn : TEXCOORD0) : COLOR0
{
    float2 texCoord = texCoordIn;
    float4 screenLightPos = FinalLightPosition;
    float NUM_SAMPLES = 100;
    // Calculate vector from pixel to light source in screen space.
    half2 deltaTexCoord = (texCoord - screenLightPos.xy);
    // Divide by number of samples and scale by control factor.
    deltaTexCoord *= 1.0f / NUM_SAMPLES * Density;
    // Store initial sample.
    half3 color = tex2D(tsampler2, texCoord);
    // Set up illumination decay factor.
    half illuminationDecay = 1.0f;
    // Evaluate summation from Equation 3 NUM_SAMPLES iterations.
    for (int i = 0; i < NUM_SAMPLES;  i++)
    {
    // Step sample location along ray.
        texCoord -= deltaTexCoord;
    // Retrieve sample at new location.
        half3 sample1 = tex2D(tsampler2, texCoord);
    // Apply sample attenuation scale/decay factors.
        sample1 *= illuminationDecay * Weight;
    // Accumulate combined color.
        color += sample1;
    // Update exponential decay factor.
        illuminationDecay *= Decay;
    }
    // Output final color with a further scale control factor.
    float4 outputcolor = float4(color * Exposure, 1);
    return saturate(outputcolor + tex2D(tsampler1, texCoordIn));
}

technique Phong
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 PhongVS();
        PixelShader = compile ps_4_0 PhongPS();
    }
}

technique PostProcessing
{
    pass Pass1
    {
        PixelShader = compile ps_4_0 PostProcessPS();
    }
}