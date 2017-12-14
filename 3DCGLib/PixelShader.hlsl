struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color;
    color.r = 1.0f;
    color.g = 0.0f;
    color.b = 0.0f;
    color.a = 1.0f;
    return color;
}