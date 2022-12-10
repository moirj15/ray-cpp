cbuffer Constants : register(b0) {
    float4x4 view_projection;
};

cbuffer ModelConstatns : register(b1) {
    float4x4 transform;
};

float4 VSMain(float3 position : POSITION) : SV_POSITION
{
    float4 transformed_pos = mul(mul(view_projection, transform), float4(position, 1.0));
    return transformed_pos;
}

float4 PSMain() : SV_TARGET {
    return float4(1.0, 1.0, 1.0, 1.0);
}
