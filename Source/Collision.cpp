#include "Collision.h"
//���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionB)
{
    //A->B�̒P�ʃx�N�g���Z�o
	// positionA �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	// positionB �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	// positionA ���� positionB �ւ̃x�N�g�����v�Z(positionB - positionA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec �̒������v�Z(XMVector3LengthSq �𗘗p���Ă��ǂ�)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//����
	// radiusA �� radiusB �̒���
	float range = radiusA + radiusB;
	// lengthSq �� range ���傫���Ȃ� true �ł������Ă��Ȃ� 
	if (lengthSq > range)
	{
		// �������Ă��Ȃ�
		return false;
	}

	// �������Ă���ꍇ
	// Vec �����̒P�ʃx�N�g���iNormalize�j���擾
	Vec = DirectX::XMVector3Normalize(Vec);
	// ��L�̃x�N�g���� range ���X�P�[��
	Vec = DirectX::XMVectorScale(Vec, range);
	// ���̃x�N�g�����ʒu A �iPositionA�j����̑������ʒu�Ɉړ�
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
	DirectX::XMStoreFloat3(&outPositionB, Vec);

	return true;

}

// �~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
{
	// A �̑���(positionA.y) �� B �̓��ipositionB.y + height�j����Ȃ�
	if (positionA.y > positionB.y + heightB)
	{
		// �������ĂȂ�
		return false;
	}
	// A �̓��� B �̑�����艺�Ȃ�
	if (positionA.y + heightA < positionB.y)
	{
		// �������ĂȂ�
		return false;
	}
	// XZ ���ʂł͈̔̓`�F�b�N
	// positionB - positionA �̋�����
	DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
	DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

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
}

//���Ɖ~���̌�������
bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
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

bool Collision::IntersectRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
{
#if 0
	if (end.y < 0.0f)
	{
		result.position.x = end.x;
		result.position.y = 0.0f;
		result.position.z - end.z;
		result.normal.x = 0.0f;
		result.normal.y = 1.0f;
		result.normal.z = 0.0f;
		return true;
	}
	return false;
#endif
#if 1
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);//�n�_
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);//�I�_
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);//end��start�̍�
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);//3D�x�N�g���̒������o��

	//���[���h��Ԃ̃��C�̒���
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{

		//���b�V���m�[�h�擾
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		//���C�����[���h��Ԃ��烍�[�J����Ԃւ̕ϊ�
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);//XMfloat4x4�̍s��(node.worldTransform)��XMLoadFloat4x4��XMMATRIX�ɕύX��WorldTransform�ɑ��
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform); //WorldTransform�̍s��t�֐���InverseWorldTransform�ɑ��

		//���C�̎n�_
		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		//���C�̏I�_
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		//�n�_����I�_�̒���
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);//E��S�̍�
		
		//���C�̕����x�N�g��
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);//SE�̐��K��
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);//3D�x�N�g��SE�̒�����Ԃ�

		//���C�̒���
		float neart;
		DirectX::XMStoreFloat(&neart, Length);//Length��ϊ�����neart�ɓ����

		//�O�p�`(��)�Ƃ̌�������
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

				//�@�O�p�`�̒��_���o
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index+1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index+2));
				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//�A�O�p�`�̎O�Ӄx�N�g�����Z�o
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);


				//�B�O�p�`�̖@���x�N�g�����Z�o
				DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(AB, DirectX::XMVectorScale(CA,-1)));

				//�C���ς̌��ʂ��v���X�Ȃ�Η�����
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
				float fDot;
				DirectX::XMStoreFloat(&fDot, Dot);
				if (fDot >= 0.0f)continue;

				//�D���C�ƕ��ʂ̌�_�Z�o
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
				DirectX::XMVECTOR d1 = DirectX::XMVector3Dot(SA, N);
				DirectX::XMVECTOR d2 = Dot;

				DirectX::XMVECTOR X = DirectX::XMVectorDivide(d1, d2);
				float x;
				DirectX::XMStoreFloat(&x, X);
				if (x<.0f || x>neart)continue;//��_�܂ł̋��������܂łɌv�Z�����ۋߋ������傫���Ƃ��̓X�L�b�v
				DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));

				DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
				DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
				DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);

				//�E��_���O�p�`�̓����ɂ��邩����
				//1��
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(PA, PB));
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f)continue;
				//2��
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(PB, PC));
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f)continue;
				//3��
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(PC, PA));
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f)continue;

				neart = x;
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}

			if (materialIndex >= 0)
			{
				//���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
				DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);

				DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
				DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
				float distance;
				DirectX::XMStoreFloat(&distance, WorldCrossLength);

				//�q�b�g���ۑ�
				if (result.distance > distance)
				{
					DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);
					result.distance = distance;
					result.materialIndex = materialIndex;
					DirectX::XMStoreFloat3(&result.position, WorldPosition);
					DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
					hit = true;
				}
			}
		}
	}

	return hit;
#endif
}
