
StructuredBuffer<matrix> WorldMatrixBuffer : register(t0);

AppendStructuredBuffer<uint> indexList : register(u0);




[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x;
    
    //‹‘äƒJƒŠƒ“ƒO‚Í‚±‚±‚Ås‚¤
    matrix mat = WorldMatrixBuffer[index];
    
    indexList.Append(index);
    
}