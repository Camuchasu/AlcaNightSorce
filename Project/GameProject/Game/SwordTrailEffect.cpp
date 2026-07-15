#include "SwordTrailEffect.h"
#include "Player.h"
//コンストラクタ
SwordTrailEffect::SwordTrailEffect(float updateTime, float lifeTime)
	:ObjectBase(eEffect)
	, mp_player(nullptr)
	,m_trailVertices(1000)
	,m_elapsedTime(0.0f)
	,m_updateTime(updateTime)
	,m_lifeTime(lifeTime)
	,m_isTrace(false)
	,mp_texture(nullptr)
	
{
	mp_texture = CTexture::LoadTexture("Effect/tktk01/Texture/Flame01.png");
}
//デストラクタ
SwordTrailEffect::~SwordTrailEffect()
{
	//削除処理
	if (mp_texture != nullptr)
	{
		mp_texture->Release();
		delete mp_texture;
		mp_texture = nullptr;
	}
}
//剣の位置　トレース開始
void SwordTrailEffect::Start(std::shared_ptr<Player> player)
{
	//もうすでにトレースされている場合はリターンを返す
	if (m_isTrace) return;
	mp_player = player;
	m_isTrace = true;
	m_elapsedTime = 0.0f;
	m_trailPoints.clear();
}

//軌跡の始点と終点を算出
void SwordTrailEffect::CalcTrailPoint(CVector3D* outStartPoint, CVector3D* outEndPoint)
{
	const CMatrix& mtx = mp_player->GetWeaponMtx();
	CVector3D dir = mtx.GetUp();
	//始点＝武器の座標
	*outStartPoint = mtx.GetPosition();
	//終点＝視点＋上方向*剣の長さ
	*outEndPoint = *outStartPoint + dir * 1.0f;
}
//軌跡のポイントを追加
void SwordTrailEffect::AddTrailPoint()
{
	TrailPoint point;
	point.elapsedTime = 0.0f;
	//軌跡のポイントの始点と終点を求める
	CalcTrailPoint(&point.startPoint, &point.endPoint);

	//リストに軌跡のポイントを追加
	m_trailPoints.push_back(point);

}

//剣の位置　トレース終了
void SwordTrailEffect::End()
{
	//トレースをしていなけば、処理はしない
	if (!m_isTrace) return;
	m_isTrace = 0.0f;
}
//更新
void SwordTrailEffect::Update()
{
	auto itr = m_trailPoints.begin();
	while (itr != m_trailPoints.end())
	{
		TrailPoint& point = *itr;
		//経過時間を加算
		point.elapsedTime += CFPS::GetDeltaTime();
		//経過時間が生存時間を超えた場合
		if (point.elapsedTime >= m_lifeTime)
		{
			//その軌跡のポイントを削除する
			itr = m_trailPoints.erase(itr);
		}
		//超えていなければ、次のポイントに進める
		else
		{
			itr++;
		}
	}
	//トレース中であれば
	if (m_isTrace)
	{
		//経過時間が軌跡のポイント更新時間を終えたら
		if (m_elapsedTime >= m_updateTime)
		{
			//現在の剣の位置から軌跡のポイントを新しく追加
			AddTrailPoint();
			m_elapsedTime -= m_updateTime;
		}
		m_elapsedTime += CFPS::GetDeltaTime();
	}
}
//描画
void SwordTrailEffect::Render()
{
	if (CShadow::isDoShadow()) return;

	//軌跡のポイント数が０であれば、描画しない
	int size = m_trailPoints.size();
	if (size == 0) return;

	int index = 0; //頂点リストのインデックス値
	for (int i = 0; i < size; i++)
	{
		const TrailPoint& point = m_trailPoints[i];

		float starAlpha = (float)i / size;
		float endAlpha = (float)i / size;
		float colorAlph = 1.0f - (point.elapsedTime / m_lifeTime);
		//軌跡の視点の頂点データを設定
		m_trailVertices[index].vertex = point.startPoint;
		m_trailVertices[index].texcoord = CVector2D(0.0f, starAlpha);
		m_trailVertices[index].color = CVector4D(1.0f, 1.0f, 1.0f, colorAlph);
		//軌跡の終点の頂点データを設定
		m_trailVertices[index + 1].vertex = point.endPoint;
		m_trailVertices[index + 1].texcoord = CVector2D(1.0f, endAlpha);

		index += 2;
	}

	//シェーダーを設定
	CShader* shader = CShader::GetInstance("SwordTrail");
	shader->Enable();

	//テクスチャを有効化
	mp_texture->MapTexture();
	//シェーダーにテクスチャを設定
	GLint samplerId = glGetUniformLocation(shader->GetProgram(), "sampler");
	//ゼロ番を取ってくるそれを上の行に反映
	glUniform1i(samplerId, 0);
	//カメラの行列設定
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);

	//頂点配列を有効化（ON）
	glEnableVertexAttribArray(CShader::eVertexLocation);
	//テクスチャのUV配列を有効化（ON）
	glEnableVertexAttribArray(CShader::eTexcoordlLocation);
	//頂点カラ―配列を有効化
	glEnableVertexAttribArray(CShader::eVertexColorLocation);

	//軌跡の頂点座標の配列を設定
	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), &m_trailVertices.data()->vertex);
	//軌跡のテクスチャのUV座標の配列を設定
	glVertexAttribPointer(CShader::eTexcoordlLocation, 2,GL_FLOAT,GL_FALSE,sizeof(SVertex), &m_trailVertices.data()->texcoord);
	//頂点カラーの配列を設定（ON）
	glVertexAttribPointer(CShader::eVertexColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(SVertex), &m_trailVertices.data()->color);
	
	glDepthMask(false);//デプス値の書き込みをオフ
	glDisable(GL_CULL_FACE);//ポリゴンの裏面化リングをオフ
	//頂点のリストを描画する
	glDrawArrays(GL_QUAD_STRIP, 0, index);
	//オフにした設定を元に戻す
	glEnable(GL_CULL_FACE);
	glDepthMask(true);
	//各頂点配列を無効化（OFF）
	glDisableVertexAttribArray(CShader::eVertexLocation);
	glDisableVertexAttribArray(CShader::eTexcoordlLocation);
	glDisableVertexAttribArray(CShader::eVertexColorLocation);

	shader->Disable();
	//テクスチャの解除
	mp_texture->UnmapTexture();
}
