// *** CPI411 Lab#7 (BumpMap) 

float4x4 World;
float4x4 View;
float4x4 Projection;
float4x4 WorldInverseTranspose;

float3 CameraPosition;
float3 LightPosition;

// Light Uniforms
float4 AmbientColor;
float AmbientIntensity;

float4 DiffuseColor;
float DiffuseIntensity;

float4 SpecularColor;
float SpecularIntensity;
float Shininess;

bool useSelfShadowing = false;

float NormalMapRepeatU;
float NormalMapRepeatV;
int SelfShadow;
float BumpHeight;
int NormalizeTangentFrame;
int NormalizeNormalMap;
int MipMap;

float Reflectivity;
float Refractivity;
float3 ETARatio;

// Bump Mapping Uniforms
float height = 9.0f;
float2 UVScale;

texture normalMap;
texture environmentMap;

sampler tsampler1 = sampler_state
{
    texture = <normalMap>;
    magfilter = LINEAR; // None, POINT, LINEAR, Anisotropic
    minfilter = LINEAR;
    mipfilter = LINEAR;
    AddressU = Wrap; // Clamp, Mirror, MirrorOnce, Wrap, Border
    AddressV = Wrap;
};
sampler tsampler2 = sampler_state
{
    texture = <normalMap>;
    magfilter = None; // None, POINT, LINEAR, Anisotropic
    minfilter = None;
    mipfilter = None;
    AddressU = Wrap; // Clamp, Mirror, MirrorOnce, Wrap, Border
    AddressV = Wrap;
};
samplerCUBE SkyBoxSampler = sampler_state
{
    texture = <environmentMap>;
    magfilter = LINEAR;
    minfilter = LINEAR;
    mipfilter = LINEAR;
    AddressU = Mirror;
    AddressV = Mirror;
};

struct VertexShaderInput
{
    float4 Position : POSITION0;
    float4 Normal : NORMAL0;
    float4 Tangent : TANGENT0;
    float4 Binormal : BINORMAL0;
    float2 TexCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float3 Normal : TEXCOORD0;
    float3 Tangent : TEXCOORD1;
    float3 Binormal : TEXCOORD2;
    float2 TexCoord : TEXCOORD3;
    float3 Position3D : TEXCOORD4;
};

VertexShaderOutput TangentVertex(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

	// TODO: add your vertex shader code here.
    float3x3 objectToTangentSpace;
	objectToTangentSpace[0] = input.Tangent;
	objectToTangentSpace[1] = input.Binormal;
	objectToTangentSpace[2] = input.Normal;
	// [ Tx, Ty, Tz ] [ Objx ] = [ Tanx ]
	// [ Bx, By, Bz ] [ Objy ] = [ Tany ]
	// [ Nx, Ny, Nz ] [ Objz ] = [ Tanz ]

	// object -> tangent?
	output.Tangent = mul(objectToTangentSpace, input.Tangent);
	output.Binormal = mul(objectToTangentSpace, input.Binormal);
    output.Normal = mul(objectToTangentSpace, input.Normal);
	// World Space looks better IMO
    /*
    output.Normal = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
    output.Tangent = normalize(mul(input.Tangent, WorldInverseTranspose).xyz);
    output.Binormal = normalize(mul(input.Binormal, WorldInverseTranspose).xyz);
    */

    output.Position3D = worldPosition.xyz;
    output.TexCoord = input.TexCoord * UVScale; //* UVScale;
    return output;
}

float4 TangentPixel(VertexShaderOutput input) : COLOR0
{

	// Vectors: L = Light; V = View; N = Normal; T = Tangent; B = Binormal; H = Halfway between L and V
    float3 L = normalize(LightPosition - input.Position3D);
    float3 V = normalize(CameraPosition - input.Position3D);
    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    float3 B = normalize(input.Binormal);
    float3 H = normalize(L + V);

    float3x3 tangentToWorld;
    // object -> tangent?
    tangentToWorld[0] = normalize(mul(input.Tangent, WorldInverseTranspose).xyz);
    tangentToWorld[1] = normalize(mul(input.Binormal, WorldInverseTranspose).xyz);
    tangentToWorld[2] = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
	// Calculate the normal, including the information in the bump map
    float3 normalTex;
    if (MipMap == 0)
    {
        normalTex = (tex2D(tsampler2, input.TexCoord).xyz);
    }
    if (MipMap == 1)
    {
        normalTex = (tex2D(tsampler1, input.TexCoord).xyz);
    }
    //float3 normalTex = (tex2D(tsampler1, input.TexCoord).xyz - float3(0.5, 0.5, 0.5)) * 2.0;
    normalTex = 1.0 * 2.0 * (normalTex - float3(0.5, 0.5, 0.5)); //expand(normalTex);

    normalTex.x *= (1 + .2f * (BumpHeight - 5));
    normalTex.y *= (1 + .2f * (BumpHeight - 5));
    normalTex.z *= (1 + .2f * (5 - BumpHeight));

	// *** Lab7 ********
	float3 bumpNormal = normalize(N + (normalTex.x * T + normalTex.y * B));

	// (Lab7 Option MonoGame3.4) 
	// If does not work, use the OPTION-A
	//float3 bumpNormal = normalize(N + (normalTex.x * float3(1, 0, 0) + normalTex.y * float3(0, 1, 0))); // OPTION A
	
	// *** for Assignment3 ***
    /*
    float3x3 TangentToWorld;

    TangentToWorld[0] = (input.Tangent);
    TangentToWorld[1] = (input.Binormal);
    TangentToWorld[2] = (input.Normal);
    */
    //float3 bumpNormal = normalize(mul(normalTex, tangentToWorld));
    //float3 bumpNormal = tangentToWorld[0] +
		//normalTex.x * tangentToWorld[2] +
		//normalTex.y * tangentToWorld[1];

    float4 ambient = AmbientColor * AmbientIntensity;

	//calculate Diffuse Term:
    float4 diffuse = DiffuseColor * DiffuseIntensity * max(0, (dot(bumpNormal, L)));
    //diffuse.a = 1.0;

	// calculate Specular Term (H,N):
    float4 specular = SpecularColor * SpecularIntensity * pow(saturate(dot(H, bumpNormal)), Shininess);
    //specular.a = 1.0;

	// Compute Final Color
    float4 finalColor = ambient + diffuse + specular; //ambient + diffuse + specular;
    return finalColor;
}
VertexShaderOutput TangentNormalVertex(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);
	// TODO: add your vertex shader code here.
    float3x3 objectToTangentSpace;
    objectToTangentSpace[0] = input.Tangent;
    objectToTangentSpace[1] = input.Binormal;
    objectToTangentSpace[2] = input.Normal;
	// [ Tx, Ty, Tz ] [ Objx ] = [ Tanx ]
	// [ Bx, By, Bz ] [ Objy ] = [ Tany ]
	// [ Nx, Ny, Nz ] [ Objz ] = [ Tanz ]

	// object -> tangent?
    output.Tangent = mul(objectToTangentSpace, input.Tangent);
    output.Binormal = mul(objectToTangentSpace, input.Binormal);
    output.Normal = mul(objectToTangentSpace, input.Normal);
    

    output.Position3D = worldPosition.xyz;
    output.TexCoord = input.TexCoord * UVScale; //* UVScale;
    return output;
}

float4 TangentNormalPixel(VertexShaderOutput input) : COLOR0
{

	// Vectors: L = Light; V = View; N = Normal; T = Tangent; B = Binormal; H = Halfway between L and V
    float3 L = normalize(LightPosition - input.Position3D);
    float3 V = normalize(CameraPosition - input.Position3D);
    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    float3 B = normalize(input.Binormal);
    float3 H = normalize(L + V);

	// Calculate the normal, including the information in the bump map
    float4 normalTex;
    if (MipMap == 0)
    {
        normalTex = (tex2D(tsampler2, input.TexCoord) - float4(0.5, 0.5, 0.5, 0.0)) * 2.0;
    }
    if (MipMap == 1)
    {
        normalTex = (tex2D(tsampler1, input.TexCoord) - float4(0.5, 0.5, 0.5, 0.0)) * 2.0;
    }

    /*
    //float3 normalTex = (tex2D(tsampler1, input.TexCoord).xyz - float3(0.5, 0.5, 0.5)) * 2.0;
    normalTex = 1.0 * 2.0 * (normalTex - float3(0.5, 0.5, 0.5)); //expand(normalTex);

    normalTex.x *= (1 + .2f * (BumpHeight - 5));
    normalTex.y *= (1 + .2f * (BumpHeight - 5));
    normalTex.z *= (1 + .2f * (5 - BumpHeight));

    float4 ambient = AmbientColor * AmbientIntensity;

	//calculate Diffuse Term:
    float4 diffuse = DiffuseColor * DiffuseIntensity * max(0, (dot(bumpNormal, L)));
    //diffuse.a = 1.0;

	// calculate Specular Term (H,N):
    float4 specular = SpecularColor * SpecularIntensity * pow(saturate(dot(H, bumpNormal)), Shininess);
    //specular.a = 1.0;

	// Compute Final Color
    float4 finalColor = ambient + diffuse + specular; //ambient + diffuse + specular;
    return finalColor;
    */
    return normalTex;
}

VertexShaderOutput WorldNormalVertex(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);
	// World Space looks better IMO  
    output.Normal = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
    output.Tangent = normalize(mul(input.Tangent, WorldInverseTranspose).xyz);
    output.Binormal = normalize(mul(input.Binormal, WorldInverseTranspose).xyz);
    

    output.Position3D = worldPosition.xyz;
    output.TexCoord = input.TexCoord * UVScale; //* UVScale;
    return output;
}

float4 WorldNormalPixel(VertexShaderOutput input) : COLOR0
{

	// Vectors: L = Light; V = View; N = Normal; T = Tangent; B = Binormal; H = Halfway between L and V
    float3 L = normalize(LightPosition - input.Position3D);
    float3 V = normalize(CameraPosition - input.Position3D);
    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    float3 B = normalize(input.Binormal);
    float3 H = normalize(L + V);

	// Calculate the normal, including the information in the bump map
    float4 normalTex;
    if (MipMap == 0)
    {
        normalTex = tex2D(tsampler2, input.TexCoord);
    }
    if (MipMap == 1)
    {
        normalTex = tex2D(tsampler1, input.TexCoord);
    }
    /*
    //float3 normalTex = (tex2D(tsampler1, input.TexCoord).xyz - float3(0.5, 0.5, 0.5)) * 2.0;
    normalTex = 1.0 * 2.0 * (normalTex - float3(0.5, 0.5, 0.5)); //expand(normalTex);

    normalTex.x *= (1 + .2f * (BumpHeight - 5));
    normalTex.y *= (1 + .2f * (BumpHeight - 5));
    normalTex.z *= (1 + .2f * (5 - BumpHeight));

    float4 ambient = AmbientColor * AmbientIntensity;

	//calculate Diffuse Term:
    float4 diffuse = DiffuseColor * DiffuseIntensity * max(0, (dot(bumpNormal, L)));
    //diffuse.a = 1.0;

	// calculate Specular Term (H,N):
    float4 specular = SpecularColor * SpecularIntensity * pow(saturate(dot(H, bumpNormal)), Shininess);
    //specular.a = 1.0;

	// Compute Final Color
    float4 finalColor = ambient + diffuse + specular; //ambient + diffuse + specular;
    return finalColor;
    */
    return normalTex;
}
VertexShaderOutput ReflectionVertex(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

	// TODO: add your vertex shader code here.
    float3x3 objectToTangentSpace;
    objectToTangentSpace[0] = input.Tangent;
    objectToTangentSpace[1] = input.Binormal;
    objectToTangentSpace[2] = input.Normal;
	// [ Tx, Ty, Tz ] [ Objx ] = [ Tanx ]
	// [ Bx, By, Bz ] [ Objy ] = [ Tany ]
	// [ Nx, Ny, Nz ] [ Objz ] = [ Tanz ]

	// object -> tangent?
    output.Tangent = mul(objectToTangentSpace, input.Tangent);
    output.Binormal = mul(objectToTangentSpace, input.Binormal);
    output.Normal = mul(objectToTangentSpace, input.Normal);
	// World Space looks better IMO
    
    //output.Normal = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
    //output.Tangent = normalize(mul(input.Tangent, WorldInverseTranspose).xyz);
    //output.Binormal = normalize(mul(input.Binormal, WorldInverseTranspose).xyz);
    

    output.Position3D = worldPosition.xyz;
    output.TexCoord = input.TexCoord * UVScale; //* UVScale;
    return output;
}

float4 ReflectionPixel(VertexShaderOutput input) : COLOR0
{

	// Vectors: L = Light; V = View; N = Normal; T = Tangent; B = Binormal; H = Halfway between L and V
    float3 L = normalize(LightPosition - input.Position3D);
    float3 V = normalize(CameraPosition - input.Position3D);
    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    float3 B = normalize(input.Binormal);
    float3 H = normalize(L + V);

    //float3x3 tangentToWorld;
    // object -> tangent?
    //tangentToWorld[0] = normalize(mul(input.Tangent, WorldInverseTranspose).xyz);
    //tangentToWorld[1] = normalize(mul(input.Binormal, WorldInverseTranspose).xyz);
    //tangentToWorld[2] = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
	// Calculate the normal, including the information in the bump map
    float3 normalTex;
    float4 decalColor;
    if (MipMap == 0)
    {
        normalTex = (tex2D(tsampler2, input.TexCoord).xyz);
        decalColor = tex2D(tsampler2, input.TexCoord);
    }
    if (MipMap == 1)
    {
        normalTex = (tex2D(tsampler1, input.TexCoord).xyz);
        decalColor = tex2D(tsampler1, input.TexCoord);
    }
    //float3 normalTex = (tex2D(tsampler1, input.TexCoord).xyz - float3(0.5, 0.5, 0.5)) * 2.0;
    normalTex = 1.0 * 2.0 * (normalTex - float3(0.5, 0.5, 0.5)); //expand(normalTex);

    normalTex.x *= (1 + .2f * (BumpHeight - 5));
    normalTex.y *= (1 + .2f * (BumpHeight - 5));
    normalTex.z *= (1 + .2f * (5 - BumpHeight));

	// *** Lab7 ********
    float3 bumpNormal = normalize(N + (normalTex.x * T + normalTex.y * B));

	// (Lab7 Option MonoGame3.4) 
	// If does not work, use the OPTION-A
	//float3 bumpNormal = normalize(N + (normalTex.x * float3(1, 0, 0) + normalTex.y * float3(0, 1, 0))); // OPTION A
	
	// *** for Assignment3 ***
    /*
    float3x3 TangentToWorld;

    TangentToWorld[0] = (input.Tangent);
    TangentToWorld[1] = (input.Binormal);
    TangentToWorld[2] = (input.Normal);
    */
    //float3 bumpNormal = normalize(mul(normalTex, tangentToWorld));
    //float3 bumpNormal = tangentToWorld[0] +
		//normalTex.x * tangentToWorld[2] +
		//normalTex.y * tangentToWorld[1];

    float3 I = normalize(input.Position3D - CameraPosition);
    float3 R = normalize(reflect(I, bumpNormal));
    float4 reflectedColor = texCUBE(SkyBoxSampler, R);
    
    //float4 ambient = AmbientColor * AmbientIntensity;

	//calculate Diffuse Term:
    //float4 diffuse = DiffuseColor * DiffuseIntensity * max(0, (dot(bumpNormal, L)));
    //diffuse.a = 1.0;

	// calculate Specular Term (H,N):
    //float4 specular = SpecularColor * SpecularIntensity * pow(saturate(dot(H, bumpNormal)), Shininess);
    //specular.a = 1.0;

	// Compute Final Color
    //float4 finalColor = ambient + diffuse + specular; //ambient + diffuse + specular;
    //return finalColor;
    return lerp(decalColor, reflectedColor, Reflectivity);
    //return reflectedColor;
}
VertexShaderOutput RefractionVertex(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);
	// World Space looks better IMO  
    output.Normal = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
    output.Tangent = normalize(mul(input.Tangent, WorldInverseTranspose).xyz);
    output.Binormal = normalize(mul(input.Binormal, WorldInverseTranspose).xyz);
    

    output.Position3D = worldPosition.xyz;
    output.TexCoord = input.TexCoord * UVScale; //* UVScale;
    return output;
}

float4 RefractionPixel(VertexShaderOutput input) : COLOR0
{

	// Vectors: L = Light; V = View; N = Normal; T = Tangent; B = Binormal; H = Halfway between L and V
    float3 L = normalize(LightPosition - input.Position3D);
    float3 V = normalize(CameraPosition - input.Position3D);
    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    float3 B = normalize(input.Binormal);
    float3 H = normalize(L + V);

	// Calculate the normal, including the information in the bump map
    float3 normalTex;
    float4 decalColor;
    if (MipMap == 0)
    {
        normalTex = tex2D(tsampler2, input.TexCoord).xyz;
        decalColor = tex2D(tsampler2, input.TexCoord);
    }
    if (MipMap == 1)
    {
        normalTex = tex2D(tsampler1, input.TexCoord).xyz;
        decalColor = tex2D(tsampler2, input.TexCoord);
    }
    
    //float3 normalTex = (tex2D(tsampler1, input.TexCoord).xyz - float3(0.5, 0.5, 0.5)) * 2.0;
    normalTex = 1.0 * 2.0 * (normalTex - float3(0.5, 0.5, 0.5)); //expand(normalTex);

    normalTex.x *= (1 + .2f * (BumpHeight - 5));
    normalTex.y *= (1 + .2f * (BumpHeight - 5));
    normalTex.z *= (1 + .2f * (5 - BumpHeight));

    float3 bumpNormal = normalize(N + (normalTex.x * T + normalTex.y * B));

    float3 I = normalize(input.Position3D - CameraPosition);
    float3 R = refract(I, bumpNormal, ETARatio.r);
    float4 refractedColor = texCUBE(SkyBoxSampler, R);
    
    float4 ambient = AmbientColor * AmbientIntensity;

	//calculate Diffuse Term:
    float4 diffuse = DiffuseColor * DiffuseIntensity * max(0, (dot(bumpNormal, L)));
    //diffuse.a = 1.0;

	// calculate Specular Term (H,N):
    float4 specular = SpecularColor * SpecularIntensity * pow(saturate(dot(H, bumpNormal)), Shininess);
    //specular.a = 1.0;

	// Compute Final Color
    float4 finalColor = ambient + diffuse + specular; //ambient + diffuse + specular;
    /*
    return finalColor;
    return normalTex;
    */
    return lerp(finalColor, refractedColor, Refractivity);
}
technique Normal
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 TangentNormalVertex();
        PixelShader = compile ps_4_0 TangentNormalPixel();
    }
}

technique NormalRGB
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 WorldNormalVertex();
        PixelShader = compile ps_4_0 WorldNormalPixel();
    }
}

technique Tangent
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 TangentVertex();
        PixelShader = compile ps_4_0 TangentPixel();
    }
}

technique Reflection
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 ReflectionVertex();
        PixelShader = compile ps_4_0 ReflectionPixel();
    }
}

technique Refraction
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 RefractionVertex();
        PixelShader = compile ps_4_0 RefractionPixel();
    }
}
