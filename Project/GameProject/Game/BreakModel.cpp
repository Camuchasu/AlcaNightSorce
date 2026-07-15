#include "BreakModel.h"

//コンストラクタ
BreakModelMesh::BreakModelMesh() :
	mp_vertex_array(nullptr),
	m_vertexNum(0),
	mp_normal(NULL),
	m_buffer(0), m_vao(0)
{
}
//デストラクタ
BreakModelMesh::~BreakModelMesh()
{
	if (mp_vertex_array) { delete[] mp_vertex_array; mp_vertex_array = nullptr; }
	if (mp_normal) { delete[] mp_normal; mp_normal = nullptr; }
	if (m_buffer) {
		glDeleteBuffers(1, &m_buffer);
		m_buffer = 0;
	}
}
//描画
void BreakModelMesh::Render() {


	if (!m_buffer) {
		//頂点バッファー未制作なら確保し
		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		//データを転送しておく
		glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex) * m_vertexNum, mp_vertex_array, GL_STATIC_DRAW);

	}
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	//頂点座標
	glEnableVertexAttribArray(CShader::eVertexLocation);
	//法線
	glEnableVertexAttribArray(CShader::eNormalLocation);
	//UV
	glEnableVertexAttribArray(CShader::eTexcoordlLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(BreakModelMesh::SVertex), 0);
	glVertexAttribPointer(CShader::eNormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(BreakModelMesh::SVertex), (void*)sizeof(CVector3D));
	glVertexAttribPointer(CShader::eTexcoordlLocation, 2, GL_FLOAT, GL_FALSE, sizeof(BreakModelMesh::SVertex), (void*)(sizeof(CVector3D) + sizeof(CVector3D)));
	
	glDrawArrays(GL_TRIANGLES, 0, m_vertexNum);

	glDisableVertexAttribArray(CShader::eVertexLocation);
	glDisableVertexAttribArray(CShader::eNormalLocation);
	glDisableVertexAttribArray(CShader::eTexcoordlLocation);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//重心からの相対座標に再構築
void BreakModelMesh::CalcVertex(const CVector3D& center)
{
	for (int i = 0; i < m_vertexNum; i++)
	{
		mp_vertex_array[i].v = mp_vertex_array[i].v - center;
	}
}
//球との判定
int BreakModelMesh::CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius)
{
	SVertex* v = mp_vertex_array;
	int cnt = 0;
	float dist;
	for (unsigned int i = 0; i < m_vertexNum; i += 3)
	{
		if (CCollision::CollisionTriangleSphere(v[i].v, v[i + 1].v, v[i + 2].v, mp_normal[i], center, radius, NULL, &dist))
		{

			out.push_back(CCollTriangle(mp_normal[i], v[i + 0].v, v[i + 1].v, v[i + 2].v, dist, 0));
			cnt++;
		}
	}
	return cnt;
}
//カプセルとの判定
int BreakModelMesh::CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap)
{
	SVertex* v = mp_vertex_array;
	int cnt = 0;
	float dist;
	for (unsigned int i = 0; i < m_vertexNum; i += 3)
	{
		if (CCollision::CollisionTriangleCapsule(v[i].v, v[i + 1].v, v[i + 2].v, mp_normal[i], cap, NULL, &dist))
		{

			out.push_back(CCollTriangle(mp_normal[i], v[i + 0].v, v[i + 1].v, v[i + 2].v, dist, 0));
			cnt++;
		}
	}
	return cnt;
}

//コンストラクタ
BreakModelGroup::BreakModelGroup() :
	m_minPos(FLT_MAX, FLT_MAX, FLT_MAX),
	m_maxPos(FLT_MIN, FLT_MIN, FLT_MIN),
	m_centerPos(0, 0, 0),
	m_alpha(1.0f)
{
	m_moveMat = CMatrix::MTranselate(Utility::Rand(-0.01f, 0.01f), Utility::Rand(-0.01f, 0.01f), Utility::Rand(-0.01f, 0.01f));
	m_rotMat = CMatrix::MRotation(Utility::Rand(-0.01f, 0.01f), Utility::Rand(-0.01f, 0.01f), Utility::Rand(-0.01f, 0.01f));
}

void BreakModelGroup::Release()
{
	for (auto& m : m_meshList) {
		delete m.second;
	}
}
//更新
void BreakModelGroup::Update(bool isCrush, const std::map<std::string, CMaterial*>& material)
{
	//破壊フラグが立っていれば
	if (isCrush)
	{
		auto it = material.begin();
		while (it != material.end())
		{
			//透明度を設定
			it->second->m_diffuse = CVector4D(1.0f, 1.0f, 1.0f, m_alpha);
			it++;
		}

		//透明度を減少
		m_alpha -= 0.008f;

		if (m_matrix.GetPosition().y >= 0)
		{
			m_matrix = m_moveMat * m_matrix * m_rotMat;
			m_moveMat = CMatrix::MTranselate(0, -0.01f, 0) * m_moveMat;
		}
	}
}
//描画
void BreakModelGroup::Render(const char* mtlName)
{
	//自分自身のメッシュリストに、引数で受け取った要素が存在しなければ関数を抜ける
	if (!m_meshList[mtlName]) return;

	//描画
	m_meshList[mtlName]->Render();

}
//座標補正計算関数
void BreakModelGroup::CalcVertex()
{
	m_centerPos = (m_minPos * 0.5f) + (m_maxPos * 0.5f);

	m_matrix = CMatrix::MTranselate(m_centerPos);

	//最小地点・最大地点の更新
	m_minPos = m_minPos - m_centerPos;
	m_maxPos = m_maxPos - m_centerPos;

	auto it = m_meshList.begin();
	while (it != m_meshList.end())
	{
		it->second->CalcVertex(m_centerPos);
		it++;
	}
}
//球との判定
int BreakModelGroup::CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius)
{
	//グループ内での接触したポリゴンの数
	int cnt_group = 0;
	//自身の逆行列を取得
	CMatrix inv = m_matrix.GetInverse();
	//「自分自身(それぞれのグループ)の逆行列」と「受け取った補正済みの座標の行列」をかけ合わせて、「受け取った座標」を「グループの中心点からの相対的な座標」に変換
	CVector3D c = inv * CVector4D(center, 1);

	//敵のモデルの最小地点・最大地点を算出
	CVector3D min = c + CVector3D(1, 1, 1) * -radius;
	CVector3D max = c + CVector3D(1, 1, 1) * radius;

	//もし、自分自身(グループ)の大まかな方形と、受け取ったモデルの大まかな方形が重なっていなければ当たり判定をせずに繰り返す
	if (!CCollision::CollisionAABB(m_minPos, m_maxPos, min, max)) return false;

	std::vector<CCollTriangle> ret;

	auto it = m_meshList.begin();
	while (it != m_meshList.end())
	{
		//現在指している要素が存在しなければ
		if (!it->second)
		{
			//要素を次に進めて、繰り返す
			it++;
			continue;
		}

		//メッシュリストに登録されているポリゴンとの当たり判定を行う
		int cnt = it->second->CollisionSphere(ret, c, radius);

		cnt_group += cnt;

		it++;
	}
	for (auto& o : ret)
	{
		o.Transform(m_matrix);
	}
	std::copy(ret.begin(), ret.end(), std::back_inserter(out));

	return cnt_group;
}
//カプセルとの判定
int BreakModelGroup::CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap)
{
	//グループ内での接触したポリゴンの数
	int cnt_group = 0;
	//自身の逆行列を取得
	CMatrix inv = m_matrix.GetInverse();
	//「自分自身(それぞれのグループ)の逆行列」と「受け取った補正済みの座標の行列」をかけ合わせて、「受け取った座標」を「グループの中心点からの相対的な座標」に変換

	CVector3D t = inv * CVector4D(cap.GetStartPoint(), 1);
	CVector3D b = inv * CVector4D(cap.GetEndPoint(), 1);
	CCapsule c = CCapsule(t, b, cap.GetRadius());

	CVector3D mn(min(cap.GetStartPoint().x, cap.GetEndPoint().x) - cap.GetRadius(),
		min(cap.GetStartPoint().y, cap.GetEndPoint().y) - cap.GetRadius(),
		min(cap.GetStartPoint().z, cap.GetEndPoint().z) - cap.GetRadius());

	CVector3D mx(max(cap.GetStartPoint().x, cap.GetEndPoint().x) + cap.GetRadius(),
		max(cap.GetStartPoint().y, cap.GetEndPoint().y) + cap.GetRadius(),
		max(cap.GetStartPoint().z, cap.GetEndPoint().z) + cap.GetRadius());

	//もし、自分自身(グループ)の大まかな方形と、受け取ったモデルの大まかな方形が重なっていなければ当たり判定をせずに繰り返す
	if (!CCollision::CollisionAABB(m_minPos, m_maxPos, mn, mx)) return false;

	std::vector<CCollTriangle> ret;

	auto it = m_meshList.begin();
	while (it != m_meshList.end())
	{
		//現在指している要素が存在しなければ
		if (!it->second)
		{
			//要素を次に進めて、繰り返す
			it++;
			continue;
		}

		//メッシュリストに登録されているポリゴンとの当たり判定を行う
		int cnt = it->second->CollisionCapsule(ret, c);

		cnt_group += cnt;

		it++;
	}
	for (auto& o : ret)
	{
		o.Transform(m_matrix);
	}
	std::copy(ret.begin(), ret.end(), std::back_inserter(out));

	return cnt_group;
}
//コンストラクタ
BreakModel::BreakModel() : m_isCrush(false)
{
}

BreakModel::BreakModel(const BreakModel& m) : m_isCrush(false)
{
	*this = m;
}

BreakModel::~BreakModel()
{
	//それぞれが別に持つグループの削除
	auto it = m_breakGroupList.begin();
	while (it != m_breakGroupList.end())
	{
		delete m_breakGroupList[it->first];
		it++;
	}

	//それぞれが別に持つマテリアルデータの削除
	auto it2 = m_materialList.begin();
	while (it2 != m_materialList.end())
	{
		delete m_materialList[it2->first];
		it2++;
	}
}

void BreakModel::operator=(const BreakModel& m)
{
	memcpy(this, &m, sizeof(CModel));
	//それぞれが別に持つグループの生成
	auto it = m.m_breakGroupList.begin();
	while (it != m.m_breakGroupList.end()) {
		m_breakGroupList[it->first] = new BreakModelGroup();
		*m_breakGroupList[it->first] = *it->second;
		it++;
	}

	//それぞれが別に持つマテリアルデータの生成
	auto it2 = m.m_materialList.begin();
	while (it2 != m.m_materialList.end())
	{
		m_materialList[it2->first] = new CMaterial();
		*m_materialList[it2->first] = *it2->second;
		it2++;
	}
}

bool BreakModel::Load(const char* file)
{
	//モデルファイルまでのパスを記録
	MakePath(file);

	//OBJファイル読み込み用ポインタ
	FILE* fp = nullptr;

	//受け取ったファイルパスを使用して、「読み込みモード」でファイルを開く
	fopen_s(&fp, file, "r");
	//もしファイルの読み込みに失敗していれば
	if (!fp)
	{
		//コンソールにエラーメッセージを表示
		printf("%sを開けません", file);
		//「false」を返す
		return false;
	}

	/*読み込んだファイルの中身を一行ずつ読み込み、必要情報の数を調べる*/
	//頂点座標データ数
	int VertexCnt = 0;
	//法線データ数
	int NormalCnt = 0;
	//テクスチャコード数
	int UVCnt = 0;
	//面数
	int FCnt = 0;

	//読み込み回数を減らすため、先に一行分の文字列を格納するための領域
	char buf[256] = "";
	//キーワード格納用の領域
	char key[128] = "";

	//表記補助用ポインタ
	BreakModelMesh* mesh = nullptr;
	BreakModelGroup* group = nullptr;

	/*
	*「渡したアドレス」がファイルの末尾(NULL)の場合のみ「true」を返す「feof関数」の反転を使用し、
	*ファイルの末尾(fp == NULL)でなければ繰り返す。
	*/
	while (!feof(fp))
	{
		//keyをクリア(最初の要素に「NULL」を格納)
		key[0] = NULL;
		//一行ずつ、bufの中に格納
		fgets(buf, 256, fp);
		//bufの中から一単語(スペース)区切りで文字列を取得し、keyの中に格納する
		sscanf_s(buf, "%s", key, 32);
		//keyの中身を確認し、一致すれば(差分が0なら)Cntを増加させて上から（頂点・法線・UV・面）を数えている
		if (!strcmp(key, "v")) VertexCnt++;
		else if (!strcmp(key, "vn")) NormalCnt++;
		else if (!strcmp(key, "vt")) UVCnt++;
		else if (!strcmp(key, "f")) FCnt++;
		//もしg BarrelBobyの「g」を発見すればBarrelBobyの破片グループを作成
		else if (strcmp(key, "g") == 0)
		{
			char name[NAME_STR_SIZE];
			//使用するグループの名前を取得
			sscanf_s(buf, "%s %s", key, 32, name, NAME_STR_SIZE);
			//もしグループ配列の中に、特定のグループ名を持つ要素が存在していなければ生成
			if (!m_breakGroupList[name]) m_breakGroupList[name] = new BreakModelGroup();
			//使用する表記補助用ポインタの指す要素を切り替える
			group = m_breakGroupList[name];
		}
		//もし「usemtl」を発見すれば
		else if (strcmp(key, "usemtl") == 0)
		{
			char name[NAME_STR_SIZE];
			//使用するメッシュの名前を取得
			sscanf_s(buf, "%s %s", key, 32, name, NAME_STR_SIZE);
			if (!group) printf("[%s]グループを設定して出力してください\n", file);
			//もし、現在指しているグループのメッシュ配列の中に特定のメッシュ名を持つ要素が存在していなければ生成する
			if (!group->m_meshList[name]) group->m_meshList[name] = new BreakModelMesh();
			//使用する表記補助用ポインタの指す要素を切り替える
			mesh = group->m_meshList[name];
		}
		//もし「mtllib」を発見すれば
		else if (strcmp(key, "mtllib") == 0)
		{
			char name[PATH_SIZE];
			strcpy_s(name, PATH_SIZE, m_filePath);
			int l = (int)strlen(name);
			//使用するマテリアルファイルのパスを取得
			sscanf_s(buf, "%s %s", key, 32, &name[l], PATH_SIZE - l);
			//マテリアルデータを読み込み
			LoadMaterial(name);
		}
	}

#ifdef CHECK
	printf(" v = %d\n n = %d\n uv = %d\n f = %d\n", VertexCnt, NormalCnt, UVCnt, FCnt);
#endif // !CHECK

	//表示用データの配列を確保
	CVector3D* pVertex = new CVector3D[VertexCnt];
	CVector3D* pNormal = new CVector3D[NormalCnt];
	CVector2D* pUV = new CVector2D[UVCnt];

	//連想配列の先頭を取得(リストと同様)
	auto it = m_breakGroupList.begin();
	//連想配列の最後まで繰り返す
	while (it != m_breakGroupList.end())
	{
		group = it->second;

		auto it2 = group->m_meshList.begin();
		while (it2 != group->m_meshList.end())
		{
			//表記補助用ポインタに、現在の変数itが指している連想配列要素の第二引数の要素を渡す
			mesh = it2->second;
			//描画用頂点配列（三角形を保存する箱）を作成
			mesh->mp_vertex_array = new BreakModelMesh::SVertex[3 * FCnt];
			mesh->mp_normal = new CVector3D[3 * FCnt];
			//要素を次へ
			it2++;
		}
		//要素を次へ
		it++;
	}
	//各データ数カウントの初期化
	VertexCnt = 0;
	NormalCnt = 0;
	UVCnt = 0;

	//読み書きの位置をファイルの先頭に戻す
	fseek(fp, 0, SEEK_SET);

	//ファイルの末尾でなければ繰り返す
	while (!feof(fp))
	{
		//keyをクリア
		key[0] = 0;
		//一行ずつ、bufの中に格納
		fgets(buf, 256, fp);
		//bufの中から一単語(スペース)区切りで文字列を取得し、keyの中に格納する
		sscanf_s(buf, "%s", key, 32);
		/*「v」を読み込んだら、座標データを読み込む*/
		if (strcmp(key, "v") == 0)
		{
			//現在のカウントが指す配列の要素へ、それぞれの値を書き込む
			sscanf_s(buf, "%s %f %f %f", key, 32, &pVertex[VertexCnt].x, &pVertex[VertexCnt].y, &pVertex[VertexCnt].z);

			//読込先を次のデータへ
			VertexCnt++;
		}
		else if (strcmp(key, "vn") == 0) /*「vn」を読み込んだら、法線データを読み込む*/
		{
			//現在のカウントが指す配列の要素へ、それぞれの値を書き込む
			sscanf_s(buf, "%s %f %f %f", key, 32, &pNormal[NormalCnt].x, &pNormal[NormalCnt].y, &pNormal[NormalCnt].z);
			//読込先を次のデータへ
			NormalCnt++;
		}
		else if (strcmp(key, "vt") == 0) /*「vt」を読み込んだら、テクスチャーコードデータを読み込む*/
		{
			//現在のカウントが指す配列の要素へ、それぞれの値を書き込む
			sscanf_s(buf, "%s %f %f", key, 32, &pUV[UVCnt].x, &pUV[UVCnt].y);
			pUV[UVCnt].y = 1.0f - pUV[UVCnt].y;
			//読込先を次のデータへ
			UVCnt++;
		}
		else if (!strcmp(key, "f")) /*「f」を読み込んだら、面データを読み込む*/
		{
			//仮に一行分のデータを格納するための領域を確保
			CVector3D face[3];
			sscanf_s(buf, "%s %f/%f/%f %f/%f/%f %f/%f/%f", key, 32, &face[0].x, &face[0].y, &face[0].z, &face[1].x, &face[1].y, &face[1].z, &face[2].x, &face[2].y, &face[2].z);

#ifdef CHECK
			//デバッグ用
			printf("%f/%f/%f %f/%f/%f %f/%f/%f \n", face[0].x, face[0].y, face[0].z, face[1].x, face[1].y, face[1].z, face[2].x, face[2].y, face[2].z);
#endif
			BreakModelMesh::SVertex* v = &mesh->mp_vertex_array[mesh->m_vertexNum];
			int idx = mesh->m_vertexNum;
			//一行に含まれる3つのデータを、自分自身の持つデータの各場所に格納
			for (int i = 0; i < 3; i++, mesh->m_vertexNum++)
			{
				mesh->mp_vertex_array[mesh->m_vertexNum].v = pVertex[(int)face[i].x - 1];
				mesh->mp_vertex_array[mesh->m_vertexNum].t = pUV[(int)face[i].y - 1];
				mesh->mp_vertex_array[mesh->m_vertexNum].n = pNormal[(int)face[i].z - 1];
				/*現在指し示しているグループの最小座標のそれぞれの要素を更新*/
				group->m_minPos.x = mesh->mp_vertex_array[mesh->m_vertexNum].v.x < group->m_minPos.x ? mesh->mp_vertex_array[mesh->m_vertexNum].v.x : group->m_minPos.x;
				group->m_minPos.y = mesh->mp_vertex_array[mesh->m_vertexNum].v.y < group->m_minPos.y ? mesh->mp_vertex_array[mesh->m_vertexNum].v.y : group->m_minPos.y;
				group->m_minPos.z = mesh->mp_vertex_array[mesh->m_vertexNum].v.z < group->m_minPos.z ? mesh->mp_vertex_array[mesh->m_vertexNum].v.z : group->m_minPos.z;
				/*現在指し示しているグループの最大座標のそれぞれの要素を更新*/
				group->m_maxPos.x = mesh->mp_vertex_array[mesh->m_vertexNum].v.x > group->m_maxPos.x ? mesh->mp_vertex_array[mesh->m_vertexNum].v.x : group->m_maxPos.x;
				group->m_maxPos.y = mesh->mp_vertex_array[mesh->m_vertexNum].v.y > group->m_maxPos.y ? mesh->mp_vertex_array[mesh->m_vertexNum].v.y : group->m_maxPos.y;
				group->m_maxPos.z = mesh->mp_vertex_array[mesh->m_vertexNum].v.z > group->m_maxPos.z ? mesh->mp_vertex_array[mesh->m_vertexNum].v.z : group->m_maxPos.z;
			}
			//当たり判定用法線
			CVector3D e1 = v[1].v - v[0].v;
			CVector3D e2 = v[2].v - v[0].v;
			CVector3D n = CVector3D::Cross(e1, e2);
			if (n.LengthSq() > 0) n.Normalize();
			for (int i = 0; i < 3; i++) {
				mesh->mp_normal[idx + i] = n;
			}
		}
		else if (strcmp(key, "g") == 0) /*[g]を読み込んだら、補助用ポインタの読み込んでいるメッシュデータを、特定の名称のものに切り替える*/
		{
			//メッシュ名保存用変数
			char name[NAME_STR_SIZE];
			//発見したメッシュ名を上で用意した変数に書き込み
			sscanf_s(buf, "%s %s", key, 32, name, NAME_STR_SIZE);
			//連想配列を使用して作成した領域(使用メッシュ)を、補助用ポインタに渡す
			group = m_breakGroupList[name];
		}
		else if (strcmp(key, "usemtl") == 0) /*「usemtl」を読み込んだら、補助用ポインタの読み込んでいるメッシュデータを、特定の名称のものに切り替える*/
		{
			//メッシュ名保存用変数
			char name[NAME_STR_SIZE];
			//発見したメッシュ名を上で用意した変数に書き込み
			sscanf_s(buf, "%s %s", key, 32, name, NAME_STR_SIZE);
			//連想配列を使用して作成した領域(使用メッシュ)を、補助用ポインタに渡す
			mesh = group->m_meshList[name];
		}
	}

	it = m_breakGroupList.begin();
	while (it != m_breakGroupList.end())
	{
		BreakModelGroup* group = it->second;
		//現在指し示しているグループのモデル座標を、大体の中心からの相対的な座標に変換する
		group->CalcVertex();
		it++;
	}
	//開いていたファイルを閉じる
	fclose(fp);
	return true;
}
//マテリアルファイルの読み込み
bool BreakModel::LoadMaterial(const char* file)
{
	//マテリアルの読み込み
	FILE* fp = nullptr;

	fopen_s(&fp, file, "r");

	if (!fp)
	{
		printf("%sを開けません", file);
		return false;
	}

	CMaterial* material = nullptr;

	//データの最後なら「真」を返すfeof関数の否定なので、データの最後でなければ「真」を返して繰り返し、データの最後まで中身を確認していく。
	while (!feof(fp))
	{
		//データを格納するための領域(バッファ)を確保する。
		char buf[256];
		//一行ずつbufの中に格納していく。
		fgets(buf, 256, fp);
		//文字列を取得してそれを格納する配列を宣言し、その中を「""」(空の中身)で初期化する。
		char key[32] = "";
		//bufの中身をスペース区切りで最初の一文字を取得し、keyの中に格納する。
		sscanf_s(buf, "%s", key, 32);
		//以下、keyの中身とmtlデータの行ごとの名前を比べて、その行が一致すればその行のデータを一致する項目のデータに書き込む(データを発見して書き込む処理)
		if (!strcmp(key, "newmtl"))
		{
			char name[NAME_STR_SIZE];
			sscanf_s(buf, "%s %s", key, 32, name, NAME_STR_SIZE);

			m_materialList[name] = new CMaterial();
			material = m_materialList[name];
			material->m_shader_name = "StaticMesh";
			strcpy_s(material->m_name, NAME_STR_SIZE, name);
		}
		//ambient
		if (!strcmp(key, "Ka"))		sscanf_s(buf, "%s %f %f %f", key, 32, &material->m_ambient.x, &material->m_ambient.y, &material->m_ambient.z);
		//diffuse
		if (!strcmp(key, "Kd"))		sscanf_s(buf, "%s %f %f %f", key, 32, &material->m_diffuse.x, &material->m_diffuse.y, &material->m_diffuse.z);
		//specular
		if (!strcmp(key, "Ks"))		sscanf_s(buf, "%s %f %f %f", key, 32, &material->m_specular.x, &material->m_specular.y, &material->m_specular.z);
		//emissive
		if (!strcmp(key, "Ke"))		sscanf_s(buf, "%s %f %f %f", key, 32, &material->m_emissive.x, &material->m_emissive.y, &material->m_emissive.z);
		//shininess
		if (!strcmp(key, "Ns"))		sscanf_s(buf, "%s %f", key, 32, &material->m_shininess);
		//alpha(透明度)
		if (!strcmp(key, "d")) 		sscanf_s(buf, "%s %f", key, 32, &material->m_diffuse.w);
		//使用ファイルの名前
		if (!strcmp(key, "map_Kd"))
		{
			material->mp_texture = new CTexture();
			char name[32];
			strcpy_s(material->m_texture_name, NAME_STR_SIZE, strchr(buf, ' ') + 1);
			//末尾の改行消し
			if (char* n = strrchr(material->m_texture_name, '\n')) *n = '\0';
			char str[PATH_SIZE];
			strcpy_s(str, PATH_SIZE, m_filePath);
			strcat_s(str, PATH_SIZE, material->m_texture_name);
			material->mp_texture->Load(str);
		}
	}
	return true;
}
//マテリアルの取得
CMaterial* BreakModel::GetMaterial(int no)const
{
	int cnt = 0;
	//no番目のマテリアルを返却
	for (auto& m : m_materialList) {
		if (cnt == no) return m.second;
		cnt++;
	}
	return nullptr;
}
//更新処理
void BreakModel::Update()
{
	//動きの更新
	for (auto& g : m_breakGroupList)
	{
		g.second->Update(m_isCrush, m_materialList);
	}
}
//行列を指定して描画
void BreakModel::Render(const CMatrix& m)
{
	for (auto& mat : m_materialList)
	{
		CMaterial* material = mat.second;

		//シェーダー設定
		CShader* s = material->mp_shader = CShader::GetInstance(material->m_shader_name);
		s->Enable();
		//パラメーターの送信
		glUniform1f(glGetUniformLocation(s->GetProgram(), "shadow_bias"), m_shadow_bias);
		glUniform1i(glGetUniformLocation(s->GetProgram(), "toon"), m_toon ? 1 : 0);

		//マテリアル転送
		material->Map();

		//深度バッファへの書き込みOFF
		if (m_isCrush) glDepthMask(GL_FALSE);

		//各グループのメッシュを描画
		for (auto& mgl : m_breakGroupList) {
			BreakModelGroup* mg = mgl.second;
			//グループ毎に位置を変更する
			SendShaderParam(s, m * mg->m_matrix, CCamera::GetCurrent()->GetViewMatrix() * m * mg->m_matrix, CCamera::GetCurrent()->GetProjectionMatrix());

			mg->Render(material->m_name);
		}

		if (m_isCrush) glDepthMask(GL_TRUE);

		material->Unmap();

		s->Disable();
	}
}
//描画処理
void BreakModel::Render()
{
	UpdateMatrix();
	Render(m_matrix);
}
//モデルを破棄する
void BreakModel::Release()
{
	for (auto& m : m_materialList) {
		m.second->Release();
		delete m.second;
	}
	for (auto& g : m_breakGroupList) {
		g.second->Release();
		delete g.second;
	}
	m_materialList.clear();
	m_breakGroupList.clear();
}
//球との判定
std::vector<CCollTriangle>  BreakModel::CollisionSphere(const CVector3D& center, float radius) const
{
	std::vector<CCollTriangle> out;
	//表示時の拡縮値変更を考慮して、現在の大まかな拡縮値を取得
	float scale = CVector3D(m_matrix.m00, m_matrix.m10, m_matrix.m20).Length();
	//面データの数
	int cnt = 0;
	//面データ情報
	//「自分自身の逆行列」と「受け取った判定点の行列」をかけ合わせて、「受け取った判定点の座標」を「自分自身の中心点からの相対的な座標」に変換
	CVector3D p = m_matrix.GetInverse() * CVector4D(center, 1);
	//取得した拡縮値を使用して、判定対象の半径を補正
	float r = radius / scale;

	auto it = m_breakGroupList.begin();
	while (it != m_breakGroupList.end()) {
		int c = it->second->CollisionSphere(out, p, r);
		it++;
	}

	for (auto& o : out)
	{
		//拡縮補正後の判定では、実際の距離との相違が発生するので、最後にスケールをかけて正しい値に戻す
		o.m_dist *= scale;
		//自身の行列をワールド行列に変換
		o.Transform(m_matrix);
	}

	return out;
}
//カプセルとの判定
std::vector<CCollTriangle> BreakModel::CollisionCapsule(const CCapsule& cap) const
{
	std::vector<CCollTriangle> out;
	//表示時の拡縮値変更を考慮して、現在の大まかな拡縮値を取得
	float scale = CVector3D(m_matrix.m00, m_matrix.m10, m_matrix.m20).Length();
	//面データの数
	int cnt = 0;
	//面データ情報
	//「自分自身の逆行列」と「受け取った判定点の行列」をかけ合わせて、「受け取った判定点の座標」を「自分自身の中心点からの相対的な座標」に変換
	CMatrix inv = m_matrix.GetInverse();
	CVector3D t = inv * CVector4D(cap.GetStartPoint(), 1);
	CVector3D b = inv * CVector4D(cap.GetEndPoint(), 1);
	float r = cap.GetRadius() / scale;
	CCapsule cap2 = CCapsule(t, b, r);

	auto it = m_breakGroupList.begin();
	while (it != m_breakGroupList.end()) {
		int c = it->second->CollisionCapsule(out, cap2);
		it++;
	}

	for (auto& o : out)
	{
		//拡縮補正後の判定では、実際の距離との相違が発生するので、最後にスケールをかけて正しい値に戻す
		o.m_dist *= scale;
		//自身の行列をワールド行列に変換
		o.Transform(m_matrix);
	}
	return out;
}
//モデルの読み込みと生成
BreakModel* BreakModel::CreateModel(const char* path)
{
	//生成と読み込み
	BreakModel* model = new BreakModel();
	model->Load(path);

	return model;
}