#include "DxLib.h"
#include "WorldSprite.h"

void WorldSprite::Initialize(int textureGraph, int chipPixelSizeW, int chipPixelSizeH, int spriteNo)
{
    this->m_textureHandle = textureGraph;

    // 4頂点分のuvデータを設定する
   // u→横位置(x),v→縦位置(y)
    int texW, texH;
    GetGraphTextureSize(textureGraph, &texW, &texH);
    int chipNumW = texW / chipPixelSizeW;
    int chipNoX = spriteNo % chipNumW;
    int chipNumH = texH / chipPixelSizeH;
    int chipNoY = spriteNo / chipNumH;

    // テクスチャ全部を1.0とした時のcihp一個に対するuvのサイズ
    float oneChipUVRateW = 1.0f / (float)chipNumW;
    float oneChipUVRateH = 1.0f / (float)chipNumH;

    // uv座標を計算で出す
    // 左上
    m_vertex[0].u = (chipNoX + 0.0f) * oneChipUVRateW;
    m_vertex[0].v = (chipNoY + 0.0f) * oneChipUVRateH;
    // 右上
    m_vertex[1].u = (chipNoX + 1.0f) * oneChipUVRateW;
    m_vertex[1].v = (chipNoY + 0.0f) * oneChipUVRateH;
    // 左下
    m_vertex[2].u = (chipNoX + 0.0f) * oneChipUVRateW;
    m_vertex[2].v = (chipNoY + 1.0f) * oneChipUVRateH;
    // 右下
    m_vertex[3].u = (chipNoX + 1.0f) * oneChipUVRateW;
    m_vertex[3].v = (chipNoY + 1.0f) * oneChipUVRateH;

    // ディフューズ、スペキュラは初期化時に固定
    for (int i = 0; i < 4; i++)
    {
        m_vertex[i].dif = GetColorU8(255, 255, 255, 255);
        m_vertex[i].spc = GetColorU8(0, 0, 0, 0);
        m_vertex[i].norm = VGet(0.0f, 0.0f, -1.0f);   // 回転をサポートしないのでノーマルも固定

        // 補助テクスチャをサポートしないのでuv固定
        m_vertex[i].su = 0.0f;
        m_vertex[i].sv = 0.0f;
    }

    // 2ポリゴン分のインデックスデータをセット
    // データ量を減らすために頂点が重なっているところは同じ番号にする
    m_index[0] = 0;
    m_index[1] = 1;
    m_index[2] = 2;
    m_index[3] = 3;
    m_index[4] = 2;
    m_index[5] = 1;
}

void WorldSprite::SetTransform(const VECTOR& pos, float spriteSize)
{
    this->m_pos = pos;
    // ピボット中心で設定
    m_vertex[0].pos = VScale(VGet(-1.0f, 1.0f, 0.0f), spriteSize * 0.5f);
    m_vertex[1].pos = VScale(VGet(1.0f, 1.0f, 0.0f),  spriteSize * 0.5f);
    m_vertex[2].pos = VScale(VGet(-1.0, -1.0f, 0.0f), spriteSize * 0.5f);
    m_vertex[3].pos = VScale(VGet(1.0f, -1.0f, 0.0f), spriteSize * 0.5f);
    for (int i = 0; i < 4; i++)
    {
        m_vertex[i].pos = VAdd(m_vertex[i].pos, pos);
    }
}

void WorldSprite::Draw()
{
    // 2ポリゴンの描画
    DrawPolygonIndexed3D(m_vertex, 4, m_index, 2, m_textureHandle, true);
}
