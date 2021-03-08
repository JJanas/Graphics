float4x4 World;
float4x4 View;
float4x4 Projection;
float4x4 InverseCamera; //Inverse Camera Matrix
float4x4 WorldInverseTranspose;
texture2D Texture;

float3 LightPosition;
float3 CameraPosition;

float4 AmbientColor;
float AmbientIntensity;
float4 DiffuseColor;
float DiffuseIntensity;
float4 SpecularColor;
float SpecularIntensity;
float Shininess;


sampler ParticleSampler :register(s0) = sampler_state {
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VertexShaderInput
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 ParticlePosition : POSITION1;
	float4 ParticleParamater : POSITION2; // x: Scale x/y: Color
};
struct VertexShaderOutput
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Color : COLOR0;
};
struct VertexInput
{
    float4 Position : POSITION;
    float4 ParticlePosition : POSITION1;
    float4 ParticleParameter : POSITION2;
    float4 Normal : NORMAL;
};
struct PhongVertexOutput
{
    float4 Position : POSITION;
    float4 Color : COLOR0;
    float4 Normal : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
};

VertexShaderOutput ParticleVertexShader(VertexShaderInput input)
{
	VertexShaderOutput output;
	float4 worldPosition = mul(input.Position, InverseCamera);
	worldPosition.xyz = worldPosition.xyz * sqrt(input.ParticleParamater.x);
	worldPosition += input.ParticlePosition;
	output.Position = mul(mul(mul(worldPosition, World), View), Projection);
	output.TexCoord = input.TexCoord;
	output.Color = 1 - input.ParticleParamater.x / input.ParticleParamater.y;
	return output;
}
float4 ParticlePixelShader(VertexShaderOutput input) : COLOR
{
	float4 color = tex2D(ParticleSampler, input.TexCoord);
	color *= input.Color;
	return color;
}

PhongVertexOutput PhongVertex(VertexInput input)
{
    PhongVertexOutput output;
    output.WorldPosition = mul(input.Position, InverseCamera);
	//output.Position = input.Position;
	//output.Normal = input.Normal;
    float4 worldPosition = mul(input.Position, World);
    float4 viewPos = mul(worldPosition, View);
    float4 projPosition = mul(viewPos, Projection);
    output.Position = mul(mul(mul(worldPosition, World), View), Projection);
    output.Normal = mul(input.Normal, WorldInverseTranspose);

    return output;
}

float4 PhongPixel(PhongVertexOutput input) : COLOR0
{
	//float4 viewPos = mul(input.WorldPosition, View);
	//float4 Position = mul(viewPos, Projection);
	//float3 N = mul(input.Normal, WorldInverseTranspose).xyz;
    float3 N = normalize(input.Normal.xyz);
    float3 V = normalize(CameraPosition - input.WorldPosition.xyz);
    float3 L = normalize(LightPosition);
    float3 R = reflect(-L, N);
    float4 ambient = AmbientColor * AmbientIntensity;
    float4 diffuse = DiffuseIntensity * DiffuseColor * max(0, dot(N, L));
    float4 specular = pow(max(0, dot(V, R)), Shininess) * SpecularColor * SpecularIntensity;
    float4 color = ambient + diffuse + specular;
    return color;
}

technique PhongParticle
{
    pass Pass0
    {
        VertexShader = compile vs_4_0 PhongVertex();
        PixelShader = compile ps_4_0 PhongPixel();
    }
}
technique Particle {
	pass Pass0
	{
		VertexShader = compile vs_4_0 ParticleVertexShader();
		PixelShader = compile ps_4_0 ParticlePixelShader();
	}
}