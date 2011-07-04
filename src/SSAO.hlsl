#define SSAO_SAMPLE_COUNT 16
#define BLUR_RADIUS 4

cbuffer cbSSAOProperties : register(cb0)
{
	float4x4 ViewProjection			: packoffset(c0.x);
	float4x4 InverseViewProjection	: packoffset(c4.x);
	float SampleRadius				: packoffset(c8.x);
	float BlurSigma					: packoffset(c8.y);
	float GaussianNumerator			: packoffset(c8.z);
	float CameraNearClip			: packoffset(c8.w);
	float CameraFarClip				: packoffset(c9.x);
	float SamplePower				: packoffset(c9.y);
	float2 Padding					: packoffset(c9.z);
}

cbuffer cbSSAOSampleDirections : register(cb1)
{	
	float4 SampleDirections[SSAO_SAMPLE_COUNT]	: packoffset(c0.x);
}

Texture2D Texture0 : register(t0);
Texture2D Texture1 : register(t1);
Texture2D Texture2 : register(t2);

SamplerState PointSampler  : register(s0);
SamplerState LinearSampler : register(s1);

struct PS_In_Quad
{
    float4 vPosition	: SV_POSITION;
    float2 vTexCoord	: TEXCOORD0;
	float2 vPosition2	: TEXCOORD1;
};

float4 GetPositionWS(float2 vPositionCS, float fDepth)
{
	float4 vPositionWS = mul(float4(vPositionCS.x, vPositionCS.y, fDepth, 1.0f), InverseViewProjection);
	vPositionWS.xyz = vPositionWS.xyz / vPositionWS.www;
	vPositionWS.w = 1.0f;

	return vPositionWS;
}

float GetLinearDepth(float nonLinearDepth, float nearClip, float farClip)
{
	float fPercFar = farClip / (farClip - nearClip);
	return ( -nearClip * fPercFar ) / ( nonLinearDepth - fPercFar);
}

float4 PS_SSAO(PS_In_Quad input) : SV_TARGET0
{
	// Texture0 = Normals
	// Texture1 = Depth
	// Texture2 = Random noise
	float3 vNormal = Texture0.Sample(PointSampler, input.vTexCoord).xyz;
	
	float fDepth = Texture1.Sample(PointSampler, input.vTexCoord).x;
	float4 vPositionWS = GetPositionWS(input.vPosition2, fDepth);

	// Sample the random texture so that this location will always yeild the same
	// random direction (so that there is no flickering)
	float3 vRandomDirection = Texture2.Sample(PointSampler, frac(input.vTexCoord * 111.111f)).xyz;
	
	float fAOSum = 0.0f;
	for (int i = 0; i < SSAO_SAMPLE_COUNT; i++)
	{
		// Create the ray
		float3 ray = reflect(SampleDirections[i].xyz, vRandomDirection) * SampleRadius;
		
		// Invert the ray if it points into the surface
		ray = ray * sign(dot(ray, vNormal));
		
		// Calculate the position to be sampled
		float4 vSamplePositionWS = float4(vPositionWS.xyz + ray, 1.0f);

		// Determine the screen space location of the sample
		float4 vSamplePositionCS = mul(vSamplePositionWS, ViewProjection);
		vSamplePositionCS.xyz = vSamplePositionCS.xyz / vSamplePositionCS.w;
		vSamplePositionCS.w = 1.0f;
		
		// Transform the camera space position to a texture coordinate
		float2 vSampleTexCoord = (0.5f * vSamplePositionCS.xy) + float2(0.5f, 0.5f);
		vSampleTexCoord.y = 1.0f - vSampleTexCoord.y;

		// Sample the depth of the new location
		float fSampleDepth = Texture1.Sample(PointSampler, vSampleTexCoord).x;
		
		float fSampleLinearDepth = GetLinearDepth(fSampleDepth, CameraNearClip, CameraFarClip);
		float fRayLinearDepth = GetLinearDepth(vSamplePositionCS.z, CameraNearClip, CameraFarClip);

		// Calculate the occlusion
		float fOcclusion = 1.0f;

		float fRaySampleDist = fRayLinearDepth - fSampleLinearDepth;
		if (fRaySampleDist < SampleRadius && fRaySampleDist > 0.0f && fSampleDepth < 1.0f)
		{
			fOcclusion = pow(fRaySampleDist / SampleRadius, SamplePower);
		}

		fAOSum += fOcclusion;
	}

	float fAO = fAOSum / SSAO_SAMPLE_COUNT;

	return float4(fAO, 0.0f, 0.0f, 1.0f);
}

float4 PS_SSAO_Composite(PS_In_Quad input) : SV_TARGET0
{
	float3 vSceneColor = Texture0.Sample(PointSampler, input.vTexCoord);
	float fAO = Texture1.Sample(LinearSampler, input.vTexCoord).x;

	return float4(fAO * vSceneColor, 1.0f);
}

float4 PS_Scale(PS_In_Quad input) : SV_TARGET0
{
	return Texture0.Sample(LinearSampler, input.vTexCoord);
}

// Calculates the gaussian blur weight for a given distance and sigmas
float CalcGaussianWeight(int sampleDist)
{
	return (GaussianNumerator * exp(-(sampleDist * sampleDist) / (2 * BlurSigma * BlurSigma)));
}

// Performs a gaussian blur in one direction
float Blur(float2 texCoord, int2 direction)
{
    float value = 0;
    for (int i = -BLUR_RADIUS; i < BLUR_RADIUS; i++)
    {
		float weight = CalcGaussianWeight(i);
		float sample = Texture0.Sample(PointSampler, texCoord, direction * i).x;
		value += sample * weight;
    }

    return value;
}

// Horizontal gaussian blur
float4 PS_BlurHorizontal(PS_In_Quad input) : SV_TARGET0
{
    return float4(Blur(input.vTexCoord, int2(2, 0)), 0.0f, 0.0f, 1.0f);
}

// Vertical gaussian blur
float4 PS_BlurVertical(PS_In_Quad input) : SV_TARGET0
{
	return float4(Blur(input.vTexCoord, int2(0, 2)), 0.0f, 0.0f, 1.0f);
}