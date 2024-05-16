#include "Collision.h"

//�~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outPositionB)
{
    //A�̑�����B�̓�����Ȃ瓖�����ĂȂ�
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    //A�̓���B�̑�����艺�Ȃ瓖�����ĂȂ�
    if(positionA.y+heightA<positionB.y)
    {
        return false;
    }

    DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
    DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);

    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);

    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    // Vec �̒������v�Z(XMVector2LengthSq �𗘗p���Ă��ǂ�)
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // radiusA �� radiusB �̒���
    float range = radiusA + radiusB;

    // radisuA + radisuB ���傫��������
    if (lengthSq > range)
    {
        // �������ĂȂ�
        return false;
    }

    // A �� B �������o���iB �͉��ɉ����o���Ă͂����Ȃ��j

    // �������Ă���ꍇ

    // Vec �����̒P�ʃx�N�g���iNormalize�j���擾
    Vec = DirectX::XMVector2Normalize(Vec);
    // ��L�̃x�N�g���� range ���X�P�[��
    Vec = DirectX::XMVectorScale(Vec, range);
    // ���̃x�N�g�����ʒu A �iPositionA�j����̑������ʒu�Ɉړ�
    Vec = DirectX::XMVectorAdd(PositionA, Vec);
    // �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, Vec);

    outPositionB.x = resultPos.x;
    outPositionB.y = positionB.y;
    outPositionB.z = resultPos.y;

    return true;


    ////A��B�̒P�ʃx�N�g�����Z�o
    //DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    //DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    //DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
    //DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
    //float lengthSq;
    //DirectX::XMStoreFloat(&lengthSq, LengthSq);

    ////��������
    //float range = radiusA + radiusB;
    //if (lengthSq > range)
    //{
    //    return false;
    //}

    ////A��B�������o��
    ////A��B�x�N�g���𐳋K��
    //Vec = DirectX::XMVector3Normalize(Vec);
    ////�x�N�g����range���X�P�[��
    //Vec = DirectX::XMVectorScale(Vec, range);
    ////
    //Vec = DirectX::XMVectorAdd(PositionA, Vec);

    //DirectX::XMStoreFloat3(&outPositionB, Vec);

}



bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& spherePosition,
    float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,
    float cylinderRadius,
    float cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition
)
{
    // ���̓�(.y + radius )���~���̑���(.y)��艺�Ȃ� false
    if (spherePosition.y + sphereRadius < cylinderPosition.y)
        return false;

    // ���̑���( .y - radius)���~���̓�(.y + Height)����Ȃ� false
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderRadius)
        return false;

    // XZ ���ʂł͈̔̓`�F�b�N
    // positionB - positionA �̋�����
    DirectX::XMFLOAT2 posA(spherePosition.x, spherePosition.z);
    DirectX::XMFLOAT2 posB(cylinderPosition.x, cylinderPosition.z);

    // posA �� XMVECTOR �ɕϊ�
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
    // posB �� XMVECTOR �ɕϊ�
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
    // posA ���� posB �ւ̃x�N�g�����v�Z(posB - posA)
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    // Vec �̒������v�Z(XMVector2LengthSq �𗘗p���Ă��ǂ�)
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // radiusA �� radiusB �̒���
    float range = sphereRadius + cylinderRadius;

    // radisuA + radisuB ���傫��������
    if (lengthSq > range)
    {
        // �������ĂȂ�
        return false;
    }

    // A �� B �������o���iB �͉��ɉ����o���Ă͂����Ȃ��j

    // �������Ă���ꍇ
    // Vec �����̒P�ʃx�N�g���iNormalize�j���擾
    Vec = DirectX::XMVector2Normalize(Vec);
    // ��L�̃x�N�g���� range ���X�P�[��
    Vec = DirectX::XMVectorScale(Vec, range);
    // ���̃x�N�g�����ʒu A �iPositionA�j����̑������ʒu�Ɉړ�
    Vec = DirectX::XMVectorAdd(PositionA, Vec);
    // �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, Vec);


    outCylinderPosition.x = spherePosition.x + resultPos.x;
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = spherePosition.z + resultPos.y;

    return true;
}

bool Collision::IntersectRayVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    const Model* model,
    HitResult& result)
{
    ////�ȑO�̏����������������悤�ɉ��̎���
    //if (end.y < 0.0f)
    //{
    //    result.position.x = end.x;
    //    result.position.y = 0.0f;
    //    result.position.z = end.z;
    //    result.normal.x = 0.0f;
    //    result.normal.y = 1.0f;
    //    result.normal.z = 0.0f;
    //    return true;
    //}
    //return false;


    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    //���[���h��Ԃ̃��C�̒���
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        //���b�V���m�[�h�擾
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

        //���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        //���C�̒���
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        //�O�p�`�i�ʁj�Ƃ̌�������
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector<UINT> indices = mesh.indices;

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                //�O�p�`�̒��_�𒊏o
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                //�O�p�`�̎O�Ӄx�N�g�����Z�o
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                //�O�p�`�̖@���x�N�g�����Z�o
                DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(AB, BC));

                //���ς̌��ʂ��v���X�Ȃ�Η�����
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
                float fDot;
                fDot = DirectX::XMVectorGetX(Dot);
                if (fDot >= 0.0f) continue;

                //���C�ƕ��ʂ̌�_���Z�o
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(SA, N), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
                if (x < .0f || x > neart) continue;     //��_�܂ł̋��������܂łɌv�Z�����ŋߋ������
                                                        //�傫�����̓X�L�b�v
                DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));

                //��_���O�p�`�̓����ɂ��邩����
                //�P��
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, DirectX::XMVectorSubtract(B, P));
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
                float fDot1;
                fDot1 = DirectX::XMVectorGetX(Dot1);
                if (fDot1 < 0.0f) continue;
                //�Q��
                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, DirectX::XMVectorSubtract(C, P));
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
                float fDot2;
                fDot2 = DirectX::XMVectorGetX(Dot2);
                if (fDot2 < 0.0f) continue;
                //�R��
                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, DirectX::XMVectorSubtract(A, P));
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
                float fDot3;
                fDot3 = DirectX::XMVectorGetX(Dot3);
                if (fDot3 < 0.0f) continue;

                //�ŋߋ������X�V
                neart = x;

                //��_�Ɩ@�����X�V
                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.materialIndex;
            }
        }
        if (materialIndex >= 0)
        {
            //���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                                                                                WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            //�q�b�g���ۑ�
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal,
                    WorldTransform);
                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }
    return hit;
}
