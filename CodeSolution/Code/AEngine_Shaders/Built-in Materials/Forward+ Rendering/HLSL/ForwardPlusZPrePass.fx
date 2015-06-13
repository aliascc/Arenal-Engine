
#include <Structures.fx>

cbuffer _AE_CB_World_View_Proj : register(b0)
{
	matrix _AE_World		: packoffset(c0);
	matrix _AE_View			: packoffset(c4);
	matrix _AE_PROJection	: packoffset(c8);
};

VSOuputPos main(VSInputPos input)
{
	VSOuputPos output = (VSOuputPos)0;

	matrix wvp = mul(_AE_PROJection, mul(_AE_View, _AE_World));

	output.Position = mul(wvp, float4(input.Position, 1.0f));

	return output;
}
