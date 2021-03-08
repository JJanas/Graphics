float4x4 World;
float4x4 View;
float4x4 Projection;
float4x4 WorldInverseTranspose;

float3 LightPosition;
float3 CameraPosition;

float4 AmbientColor;
float AmbientIntensity;
float3 DiffuseLightDirection;
float4 DiffuseColor;
float DiffuseIntensity;
float4 SpecularColor;
float SpecularIntensity;
float Shininess;
float Reflectivity;
float3 ETARatio;
float FresnelBias;
float FresnelScale;
float FresnelPower;

Texture decalMap;
Texture environmentMap;

sampler tsampler1 = sampler_state
{
    texture = <decalMap>;
    magfilter = LINEAR;
    minfilter = LINEAR;
    mipfilter = LINEAR;
    AddressU = Wrap;
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

struct VertexInput
{
    float4 Position : POSITION;
    float4 Normal : NORMAL;
    float2 UV : TEXCOORD0;
};
struct GouraudVertexOutput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
	//float4 Normal : TEXCOORD0;
	//float4 WorldPosition : TEXCOORD1;
};
struct PhongVertexOutput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float4 Normal : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
};
struct PhongBlinnVertexOutput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float4 Normal : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
};
struct SchlickVertexOutput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float4 Normal : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
};
struct ToonVertexOutput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float4 Normal : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
};
struct HalfLifeVertexOutput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float4 Normal : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
};
struct ReflectionVertexOutput
{
    float4 Position : POSITION;
    float4 Normal : TEXCOORD0;
    float2 TextureCoordinate : TEXCOORD1;
    float4 WorldPosition : TEXCOORD2;
    float3 R : TEXCOORD3;
};
struct RefractionVertexOutput {
	float4 Position : POSITION;
	float4 Normal : TEXCOORD0;
	float2 TextureCoordinate : TEXCOORD1;
	float3 R : TEXCOORD2;
	float4 WorldPosition : TEXCOORD3;
};
struct DispersionVertexOutput
{
    float4 Position : POSITION;
    float4 Normal : TEXCOORD0;
    float2 TextureCoordinate : TEXCOORD1;
    float4 WorldPosition : TEXCOORD2;
};
struct FresnelVertexOutput
{
    float4 Position : POSITION;
    float4 Normal : TEXCOORD0;
    float4 WorldPosition : TEXCOORD1;
};
//VS
    GouraudVertexOutput GouraudVertex(VertexInput input)
    {
        GouraudVertexOutput output;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPosition = mul(worldPosition, View);
        output.Position = mul(viewPosition, Projection);
        float3 N = mul(input.Normal, WorldInverseTranspose).xyz;
        float3 V = normalize(CameraPosition - worldPosition.xyz);
        float3 L = normalize(LightPosition);
        float3 R = reflect(-L, N);
        float4 ambient = AmbientColor * AmbientIntensity;
        float4 diffuse = DiffuseIntensity * DiffuseColor * max(0, dot(N, L));
        float4 specular = pow(max(0, dot(V, R)), Shininess) * SpecularColor * SpecularIntensity;
        output.Color = saturate(ambient + diffuse + specular);
        return output;
    }

    PhongVertexOutput PhongVertex(VertexInput input)
    {
        PhongVertexOutput output;
        output.WorldPosition = mul(input.Position, World);
	//output.Position = input.Position;
	//output.Normal = input.Normal;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPos = mul(worldPosition, View);
        float4 projPosition = mul(viewPos, Projection);
        output.Position = projPosition;
        output.WorldPosition = worldPosition;
        output.Normal = mul(input.Normal, WorldInverseTranspose);

        return output;
    }

    PhongBlinnVertexOutput PhongBlinnVertex(VertexInput input)
    {
        PhongBlinnVertexOutput output;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPosition = mul(worldPosition, View);
        output.Position = mul(viewPosition, Projection);
        output.WorldPosition = worldPosition;
        output.Normal = mul(input.Normal, WorldInverseTranspose);
        return output;
    }

    SchlickVertexOutput SchlickVertex(VertexInput input)
    {
        SchlickVertexOutput output;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPosition = mul(worldPosition, View);
        output.Position = mul(viewPosition, Projection);
        output.Normal = mul(input.Normal, WorldInverseTranspose);
        output.WorldPosition = worldPosition;
        return output;
    }

    ToonVertexOutput ToonVertex(VertexInput input)
    {
        ToonVertexOutput output;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPosition = mul(worldPosition, View);
        output.Position = mul(viewPosition, Projection);
        output.Normal = mul(input.Normal, WorldInverseTranspose);
        output.WorldPosition = worldPosition;
        return output;
    }

    HalfLifeVertexOutput HalfLifeVertex(VertexInput input)
    {
        HalfLifeVertexOutput output;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPosition = mul(worldPosition, View);
        output.Position = mul(viewPosition, Projection);
        output.Normal = mul(input.Normal, WorldInverseTranspose);
        output.WorldPosition = worldPosition;
        return output;
    }

    ReflectionVertexOutput ReflectionVertex(VertexInput input)
    {
        ReflectionVertexOutput output;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPos = mul(worldPosition, View);
        float4 projPosition = mul(viewPos, Projection);
        output.Position = projPosition;
        output.Normal = input.Normal;

    /*
    float3 N = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
    float3 I = normalize(worldPosition.xyz - CameraPosition);
    output.R = reflect(I, N);
    */
        output.TextureCoordinate = input.UV;

        return output;
    }

    RefractionVertexOutput RefractionVertex(VertexInput input)
    {
        RefractionVertexOutput output;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPos = mul(worldPosition, View);
        float4 projPosition = mul(viewPos, Projection);
        output.Position = projPosition;
        output.Normal = input.Normal;
        output.TextureCoordinate = input.UV;
        output.WorldPosition = worldPosition;

        return output;
    }
    
    DispersionVertexOutput DispersionVertex(VertexInput input)
    {
        DispersionVertexOutput output;
        float4 worldPosition = mul(input.Position, World);
        float4 viewPos = mul(worldPosition, View);
        float4 projPosition = mul(viewPos, Projection);
        output.Position = projPosition;
        output.Normal = input.Normal;
        output.TextureCoordinate = input.UV;
        output.WorldPosition = worldPosition;

        return output;
    }

FresnelVertexOutput FresnelVertex(VertexInput input)
{
    FresnelVertexOutput output;
    float4 worldPosition = mul(input.Position, World);
    float4 viewPos = mul(worldPosition, View);
    float4 projPosition = mul(viewPos, Projection);
    output.Position = projPosition;
    output.Normal = input.Normal;
    output.WorldPosition = worldPosition;

    return output;
}

	//PS

    float4 GouraudPixel(GouraudVertexOutput input) : COLOR0
    {
        return input.Color;
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

    float4 PhongBlinnPixel(PhongBlinnVertexOutput input) : COLOR0
    {
        float3 N = normalize(input.Normal.xyz);
        float3 V = normalize(CameraPosition - input.WorldPosition.xyz);
        float3 L = normalize(LightPosition);
        float3 H = (L + V) / abs(L + V);
        float4 ambient = AmbientColor * AmbientIntensity;
        float4 diffuse = DiffuseIntensity * DiffuseColor * max(0, dot(N, L));
        float4 specular = pow(max(dot(H, N), 0), Shininess) * SpecularColor * SpecularIntensity;

        float4 color = ambient + diffuse + specular;
        return color;
    }

    float4 SchlickPixel(SchlickVertexOutput input) : COLOR0
    {
        float3 N = normalize(input.Normal.xyz);
        float3 V = normalize(CameraPosition - input.WorldPosition.xyz);
        float3 L = normalize(LightPosition);
        float3 R = reflect(-L, N);
        float4 T = dot(V, R);
        float4 ambient = AmbientColor * AmbientIntensity;
        float4 diffuse = DiffuseIntensity * DiffuseColor * max(0, dot(N, L));
        float4 specular = SpecularColor * SpecularIntensity * T / (Shininess - T * Shininess + T);

        float4 color = saturate(ambient + diffuse + specular);
        return color;
    }

    float4 ToonPixel(ToonVertexOutput input) : COLOR0
    {
        float3 N = normalize(input.Normal.xyz);
        float3 V = normalize(CameraPosition - input.WorldPosition.xyz);
        float3 L = normalize(LightPosition);
        float3 R = reflect(-L, N);
        float D = dot(V, R);
        if (D < -0.7)
        {
            return float4(0, 0, 0, 1);
        }
        else if (D < 0.2)
        {
            return float4(0.25, 0.25, 0.25, 1);
        }
        else if (D < 0.97)
        {
            return float4(0.5, 0.5, 0.5, 1);
        }
        else
        {
            return float4(1, 1, 1, 1);
        }
    }

    float4 HalfLifePixel(HalfLifeVertexOutput input) : COLOR0
    {
        float3 N = mul(input.Normal, WorldInverseTranspose).xyz;
        float3 V = normalize(CameraPosition - input.WorldPosition.xyz);
        float3 L = normalize(LightPosition);
        float3 H = normalize(L + V);
        float4 ambient = AmbientColor * AmbientIntensity;
        float4 diffuse = pow((0.5 * (dot(L, N) + 1)), 2) * DiffuseColor * DiffuseIntensity;
        float4 specular = pow(max(dot(H, N), 0), Shininess) * SpecularColor * SpecularIntensity;

        float4 color = saturate(ambient + diffuse + specular);
        return color;
    }

    float4 ReflectionPixel(ReflectionVertexOutput input) : COLOR0
    {
        float3 N = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
        float3 I = normalize(input.WorldPosition.xyz - CameraPosition);
        input.R = reflect(I, N);
        float4 reflectedColor = texCUBE(SkyBoxSampler, input.R);
        float4 decalColor = tex2D(tsampler1, input.TextureCoordinate);

        return lerp(decalColor, reflectedColor, Reflectivity);
    }

    float4 RefractionPixel(RefractionVertexOutput input) : COLOR0
    {
        float3 N = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
        float3 I = normalize(input.WorldPosition.xyz - CameraPosition);
        float cosI = dot(-I, N);
        float cosT2 = 1.0f - ETARatio * ETARatio * (1.0f - cosI * cosI);
        float3 T = ETARatio * I + ((ETARatio * cosI - sqrt(abs(cosT2))) * N);
        T = T * (float3) (cosT2 > 0);
        float4 decalColor = tex2D(tsampler1, input.TextureCoordinate);
        float4 refractedColor = texCUBE(SkyBoxSampler, T);
        return lerp(decalColor, refractedColor, Reflectivity);
    }

    float4 DispersionPixel(DispersionVertexOutput input) : COLOR0
    {
        float3 N = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
        float3 I = normalize(input.WorldPosition.xyz - CameraPosition);
        float cosI = dot(-I, N);
        float cosT2 = 1.0f - ETARatio * ETARatio * (1.0f - cosI * cosI);
        float3 T = ETARatio * I + ((ETARatio * cosI - sqrt(abs(cosT2))) * N);
        T = T * (float3) (cosT2 > 0);
        float3 R = reflect(I, N);
        float3 TRed = refract(I, N, ETARatio.x);
        float3 TGreen = refract(I, N, ETARatio.y);
        float3 TBlue = refract(I, N, ETARatio.z);
        float reflectionFactor = FresnelBias + FresnelScale * pow(1 + dot(I, N), FresnelPower);
        float4 refractedColor = texCUBE(SkyBoxSampler, T);
        float4 reflectedColor = texCUBE(SkyBoxSampler, R);

        return lerp(refractedColor, reflectedColor, reflectionFactor);

    }

float4 FresnelPixel(FresnelVertexOutput input) : COLOR0
{
    float3 N = normalize(mul(input.Normal, WorldInverseTranspose).xyz);
    float3 I = normalize(input.WorldPosition.xyz - CameraPosition);
    float3 R = reflect(I, N);
    float3 TRed = refract(I, N, ETARatio.x);
    float3 TGreen = refract(I, N, ETARatio.y);
    float3 TBlue = refract(I, N, ETARatio.z);
    float reflectionFactor = FresnelBias + FresnelScale * pow(1 + dot(I, N), FresnelPower);
    float4 reflectedColor = texCUBE(SkyBoxSampler, R);
    float4 refractedColor;
    refractedColor.r = texCUBE(SkyBoxSampler, TRed).r;
    refractedColor.g = texCUBE(SkyBoxSampler, TGreen).g;
    refractedColor.b = texCUBE(SkyBoxSampler, TBlue).b;
    refractedColor.a = 1;

    return lerp(refractedColor, reflectedColor, reflectionFactor);
}
//Tech
technique Gouraud
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 GouraudVertex();
        PixelShader = compile ps_4_0 GouraudPixel();
    }
	
}
technique Phong
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 PhongVertex();
        PixelShader = compile ps_4_0 PhongPixel();
    }

}
technique PhongBlinn
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 PhongBlinnVertex();
        PixelShader = compile ps_4_0 PhongBlinnPixel();
    }

}
technique Schlick
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 SchlickVertex();
        PixelShader = compile ps_4_0 SchlickPixel();
    }

}
technique Toon
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 ToonVertex();
        PixelShader = compile ps_4_0 ToonPixel();
    }

}
technique HalfLife
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 HalfLifeVertex();
        PixelShader = compile ps_4_0 HalfLifePixel();
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
technique Dispersion
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 DispersionVertex();
        PixelShader = compile ps_4_0 DispersionPixel();
    }
}
technique Fresnel
{
    pass Pass1
    {
        VertexShader = compile vs_4_0 FresnelVertex();
        PixelShader = compile ps_4_0 FresnelPixel();
    }
}