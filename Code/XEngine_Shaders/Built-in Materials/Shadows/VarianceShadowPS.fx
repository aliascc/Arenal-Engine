
#include "Structures.fx"
#include "ShadowFunctions.fx"

float2 main(VSOutputPosPosDepth input) : SV_Target0
{
	float depth = input.PosDepth.x / input.PosDepth.y;

	float2 moments = ComputeMoments(depth);

	return moments;
}
