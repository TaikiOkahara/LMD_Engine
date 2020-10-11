struct WorldStruct
{
	//matrix g_WorldStruct;
    int int_sample;
};

struct CSInput
{
    uint3   groupThread  : SV_GroupThreadID;
    uint3   group        : SV_GroupID;
    uint    groupIndex   : SV_GroupIndex;
    uint3   dispatch     : SV_DispatchThreadID;
};


StructuredBuffer<WorldStruct>   BufIn0  : register(t0);       //!< “ü—Í—p.

RWStructuredBuffer<WorldStruct> BufOut  : register(u0);       //!< o—Í—p.

#define size_x      1
#define size_y      1
#define size_z      1


[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    int index = input.dispath.x;

    BufOut[index].int_sample = BufIn0[index].int_sample * 3;
}