struct GlobalConstants
{
    float4x4 view_projection;
};

ConstantBuffer<GlobalConstants> global_constants : register(b0);

// TODO: something better later
struct ObjectConstants
{
    float4x4 transform;
};

ConstantBuffer<ObjectConstants> object_constants: register(b1);

float4 VSMain(float3 position : POSITION) : SV_POSITION
{
    float4 transformed_pos = mul(global_constants.view_projection * object_constants.transform, float4(position, 1.0));
    return transformed_pos;
}

float4 PSMain() : SV_TARGET {
    return float4(1.0, 1.0, 1.0, 1.0);
}
