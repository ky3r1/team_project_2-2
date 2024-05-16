#include "Collision.h"
//球と球の交差判定
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionB)
{
    //A->Bの単位ベクトル算出
	// positionA を XMVECTOR に変換
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	// positionB を XMVECTOR に変換
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	// positionA から positionB へのベクトルを計算(positionB - positionA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec の長さを計算(XMVector3LengthSq を利用しても良い)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//距離
	// radiusA と radiusB の長さ
	float range = radiusA + radiusB;
	// lengthSq が range より大きいなら true であたっていない 
	if (lengthSq > range)
	{
		// あたっていない
		return false;
	}

	// あたっている場合
	// Vec 方向の単位ベクトル（Normalize）を取得
	Vec = DirectX::XMVector3Normalize(Vec);
	// 上記のベクトルを range 分スケール
	Vec = DirectX::XMVectorScale(Vec, range);
	// そのベクトルを位置 A （PositionA）からの足した位置に移動
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// 出力用の位置（outPositionB）に代入する
	DirectX::XMStoreFloat3(&outPositionB, Vec);

	return true;

}

// 円柱と円柱の交差判定
bool Collision::IntersectCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
{
	// A の足元(positionA.y) が B の頭（positionB.y + height）より上なら
	if (positionA.y > positionB.y + heightB)
	{
		// 当たってない
		return false;
	}
	// A の頭が B の足もより下なら
	if (positionA.y + heightA < positionB.y)
	{
		// 当たってない
		return false;
	}
	// XZ 平面での範囲チェック
	// positionB - positionA の距離が
	DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
	DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

	// posA を XMVECTOR に変換
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
	// posB を XMVECTOR に変換
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
	// posA から posB へのベクトルを計算(posB - posA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec の長さを計算(XMVector2LengthSq を利用しても良い)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// radiusA と radiusB の長さ
	float range = radiusA + radiusB;

	// radisuA + radisuB より大きかったら
	if (lengthSq > range)
	{
		// 当たってない
		return false;
	}

	// A が B を押し出す（B は下に押し出してはいけない）
	// あたっている場合
	// Vec 方向の単位ベクトル（Normalize）を取得
	Vec = DirectX::XMVector2Normalize(Vec);
	// 上記のベクトルを range 分スケール
	Vec = DirectX::XMVectorScale(Vec, range);
	// そのベクトルを位置 A （PositionA）からの足した位置に移動
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// 出力用の位置（outPositionB）に代入する
	DirectX::XMFLOAT2 resultPos;
	DirectX::XMStoreFloat2(&resultPos, Vec);

	outPositionB.x = resultPos.x;
	outPositionB.y = positionB.y;
	outPositionB.z = resultPos.y;

	return true;
}

//球と円柱の交差判定
bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	// 球の頭(.y + radius )が円柱の足元(.y)より下なら false
	if (spherePosition.y + sphereRadius < cylinderPosition.y)
		return false;

	// 球の足元( .y - radius)が円柱の頭(.y + Height)より上なら false
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderRadius)
		return false;

	// XZ 平面での範囲チェック
	// positionB - positionA の距離が
	DirectX::XMFLOAT2 posA(spherePosition.x, spherePosition.z);
	DirectX::XMFLOAT2 posB(cylinderPosition.x, cylinderPosition.z);

	// posA を XMVECTOR に変換
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
	// posB を XMVECTOR に変換
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
	// posA から posB へのベクトルを計算(posB - posA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec の長さを計算(XMVector2LengthSq を利用しても良い)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// radiusA と radiusB の長さ
	float range = sphereRadius + cylinderRadius;

	// radisuA + radisuB より大きかったら
	if (lengthSq > range)
	{
		// 当たってない
		return false;
	}

	// A が B を押し出す（B は下に押し出してはいけない）

	// あたっている場合
	// Vec 方向の単位ベクトル（Normalize）を取得
	Vec = DirectX::XMVector2Normalize(Vec);
	// 上記のベクトルを range 分スケール
	Vec = DirectX::XMVectorScale(Vec, range);
	// そのベクトルを位置 A （PositionA）からの足した位置に移動
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// 出力用の位置（outPositionB）に代入する
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
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);//始点
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);//終点
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);//endとstartの差
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);//3Dベクトルの長さを出す

	//ワールド空間のレイの長さ
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{

		//メッシュノード取得
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		//レイをワールド空間からローカル空間への変換
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);//XMfloat4x4の行列(node.worldTransform)をXMLoadFloat4x4でXMMATRIXに変更しWorldTransformに代入
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform); //WorldTransformの行列逆関数をInverseWorldTransformに代入

		//レイの始点
		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		//レイの終点
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		//始点から終点の長さ
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);//EとSの差
		
		//レイの方向ベクトル
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);//SEの正規化
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);//3DベクトルSEの長さを返す

		//レイの長さ
		float neart;
		DirectX::XMStoreFloat(&neart, Length);//Lengthを変換してneartに入れる

		//三角形(面)との交差判定
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

				//①三角形の頂点抽出
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index+1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index+2));
				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//②三角形の三辺ベクトルを算出
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);


				//③三角形の法線ベクトルを算出
				DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(AB, DirectX::XMVectorScale(CA,-1)));

				//④内積の結果がプラスならば裏向き
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
				float fDot;
				DirectX::XMStoreFloat(&fDot, Dot);
				if (fDot >= 0.0f)continue;

				//⑤レイと平面の交点算出
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
				DirectX::XMVECTOR d1 = DirectX::XMVector3Dot(SA, N);
				DirectX::XMVECTOR d2 = Dot;

				DirectX::XMVECTOR X = DirectX::XMVectorDivide(d1, d2);
				float x;
				DirectX::XMStoreFloat(&x, X);
				if (x<.0f || x>neart)continue;//交点までの距離が今までに計算した際近距離より大きいときはスキップ
				DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));

				DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
				DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
				DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);

				//⑥交点が三角形の内側にあるか判定
				//1つ目
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(PA, PB));
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f)continue;
				//2つ目
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(PB, PC));
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f)continue;
				//3つ目
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
				//ローカル空間からワールド空間へ変換
				DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);

				DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
				DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
				float distance;
				DirectX::XMStoreFloat(&distance, WorldCrossLength);

				//ヒット情報保存
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
