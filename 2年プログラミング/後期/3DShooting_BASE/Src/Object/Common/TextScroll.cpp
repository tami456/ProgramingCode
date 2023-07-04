#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Quaternion.h"
#include "../../Utility/AsoUtility.h"
#include "TextScroll.h"

TextScroll::TextScroll(void)
{
}

TextScroll::~TextScroll(void)
{
}

void TextScroll::Init(void)
{
    //アルファベットの画像の読み込み
    mResource = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::ALPHABET);

    //テキストロールの種別を初期設定
    mType = TYPE::TITLE;

    //1行当たりの情報
    //構造体が1文字
    std::vector<MsgInfo> infos;

    //文章の設定（１行ずつ）
    infos.emplace_back(MakeMsgInfo("Hajimemashite.",infos.size()));
    infos.emplace_back(MakeMsgInfo("Konnichiwa.",infos.size()));

    mTextMap.emplace(TYPE::TITLE, infos);

    //移動方向（演習ポイント①）
    
    Quaternion tmpQ = Quaternion::Euler(
        AsoUtility::Deg2RadF(-40.0f),
        0.0f, 0.0f
    );

    mVelocity = tmpQ.GetForward();

    //移動速度（移動量(方向×速度))
    mVelocity = VScale(mVelocity, SPEED_MOVE);

}

void TextScroll::Update(void)
{
    std::vector<MsgInfo>& infos = mTextMap[mType];
    for (auto& info : infos)
    {
        //文字の移動処理
        info.pos = VAdd(info.pos, mVelocity);
    }

}

void TextScroll::Draw(void)
{
    const std::vector<MsgInfo>& infos = mTextMap[mType];
    
    VECTOR pos;

    float plusX;

    //１行ずつ
    for (auto& info : infos)
    {

        plusX = 0.0f;

        //１文字ずつ
        for (auto& id : info.messages)
        {
            pos = info.pos;
            pos.x += plusX;
            //演習②横に座標をずらす

            DrawBillboard3D(pos, 0.5f, 0.5f, SIZE_3D, 0.0f,mResource.mHandleIds[id],true);

            plusX += SIZE_3D;
        }
    }
}

void TextScroll::Release(void)
{
}

TextScroll::MsgInfo TextScroll::MakeMsgInfo(const std::string& msg, size_t mapCount)
{
    MsgInfo ret;
    //文字を画像配列の添え字に変換
    std::vector<int> messages;

    int ascii;
    int idx;
    size_t len = msg.size();
    for (int i = 0; i < len; i++)
    {
        //演習③変換処理を完成させよ
        idx = 0;
        
        //１文字をASCIIコードに変換する
        ascii = msg.at(i);
       

        //(32だったらスペース(画像添え字52))
        if (ascii == 32)
        {
            idx = 52;
        }

        //ドット(.)
        if (ascii == 46)
        {
            idx = 53;
        }

        //カンマ(,)
        if (ascii == 44)
        {
            idx = 54;
        }

        //大文字
        if (ascii >= 65 && ascii <= 90)
        {
            idx = ascii - 65;
        }

        //小文字
        if (ascii >= 97 && ascii <= 122)
        {
            idx = ascii - 71;
        }

        messages.emplace_back(idx);
    }

    //行ごとの座標
    ret.pos =
    {
        -400.0f,
        START_Y - (mapCount * SIZE_3D),
        START_Z - (mapCount * SIZE_3D)
    };




    //座標と変換されたIDを代入する
    ret.messages = messages;

    return ret;
}
