#include <DxLib.h>
#include "../../../_debug/_debug/_DebugConOut.h"
#include "../../../_debug/_debug/_DebugDispOut.h"
#include "KeyInput.h"
#include "PadInput.h"
#include "Controller.h"

Controller::RingBuf* Controller::RingBuf::Create(int no)
{
    //新規の要素を作る
    //newを使う
    //初期化リスト…引数付きコンストラクタがなくても初期化できる
    //newの場合は=代入ができない
    //newじゃない場合はRingBuf() = {};
    auto beginBuf = new RingBuf{ 0,0.0,nullptr,nullptr };
    auto beforeBuf = beginBuf;
    auto nowBuf = beginBuf;

    //なぜ1なのかはもう上で要素をつくっているから
    for (int i = 1;i < no ;i++)
    {
        nowBuf = new RingBuf{ 0,0.0,beforeBuf,nullptr };
        //前のバフの次のバフは今のバフ
        beforeBuf->next = nowBuf;
        beforeBuf = nowBuf;
    }
    //最初のバフの前に今が来る
    beginBuf->before = nowBuf;
    nowBuf->next = beginBuf;

    return beginBuf;
    
}

void Controller::RingBuf::Destroy(RingBuf* buf)
{
    
    //自分を次に移動させて自分の前を消していく
    if (buf == nullptr || buf->before == nullptr)
    {
        return;
    }
    //自分の前を消すその時前の前のnullにして関係を断つ
    buf->before->next = nullptr;
    while(buf->next != nullptr)
    {
        buf = buf->next;
        delete buf->before;
    }

    delete buf;
}


//ringBufの更新を止めたらコマンドが二つ出る
std::string Controller::CheckCommand(DIR_LR dir, CommandMap& commandMap)
{

    int flag = true;
    auto buf = ringBuf_;
    //プレイヤーからもらったコマンドマップに対しての処理を書く
    //リングバッファの内容とマッチさせて一致していたら
    //一致していなかったら次のコマンドのマッチング処理

    //commandMapキーとバリューのペア
    //first…キー　second…バリュー
    for (const auto& command : commandMap)
    {
        flag = true;
        buf = ringBuf_;

        //arrayが取れてる 2U…左右
        //左右なのでdirで取り出す
        //list…コマンド内容
        //後から取り出していくのでrbegin(逆イテレーター)
        //最後まで回しきれたらコマンド成立
        //イテレーター前演算
        for (auto itr = command.second[static_cast<int>(dir)].rbegin();
            itr != command.second[static_cast<int>(dir)].rend();
            ++itr)
        {
            //この二つを満たしていたら次のコマンドチェック
            //0…キー　1…時間値
            if ((*itr)[0] != buf->value)
            {
                //この中でコマンド成立していなかった場合
                flag = false;
                break;
            }

            if ((*itr)[1] < buf->time)
            {
                //受付時間を超えている場合
                flag = false;
                break;
            }
            //後ろからチェックしていっているからbefore
            buf = buf->before;
        }
        
        if (flag)
        {
            //コマンド成立
            //成立したらリセットする

            /*ringBuf_ = ringBuf_->next;
            ringBuf_->value = static_cast<int>(InputID::NON);*/

            ringBuf_->time = 0.0;
            startBuf_ = ringBuf_;

            //firstにコマンド
            TRACE("コマンド成立:%s\n",command.first.c_str());
            return command.first;
        }
    }
    
    //どのコマンドも成立していなかった場合空を返す
    return std::string();
}



void Controller::WriteCmd(void)
{
    if (ringBuf_ == nullptr)
    {
        return;
    }

    //入力情報の取得と経過時間の取得
    //使用する命令　経過時間

    //現在時間の取得
    auto endTime = std::chrono::system_clock::now();
   
    //差分を求めることで経過時間が求まる
    //std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_);
    
    //キーの入力があったら保存
    //同じキーの入力があったら経過時間を継続
    //違うキーになった時に経過時間をリセット startTime_にendTimeを代入
    //ringbufとstartbuf　リングバフから見たnext
    //データの書き込みの際
    //WriteCmd
    //InputIDの
    //入力された情報に対してシフト演算してbit演算
    //valueに入れる
    int writeData = 0;

   for (auto id : InputID())
   {
       if (cntData_[id][static_cast<int>(Trg::Now)])
       {
           //1を重ね合わせてシフトする
           writeData |= 1 << static_cast<int>(id);
       }
       //コード量を減らす場合
       //writeData |= cntData_[id][static_cast<int>(Trg::Now)] << static_cast<int>(id);
   } 

   //読み込み優先
    //キーデータが変わっていたら
    //こうしないと一瞬でバッファを食いつぶす
   if (ringBuf_->value != writeData)
   {
       //先に移動させてキーと時間を入れる
       ringBuf_ = ringBuf_->next;
       //入力があった場合
       ringBuf_->value = writeData;
       //1個前がstartBuf_だったら
       if (startBuf_ != ringBuf_->before)
       {
           //キーの入力が変わった時に影響を受ける
           ringBuf_->time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
       }
       else
       {
           ringBuf_->time = 0;
       }
       
      
       
       startTime_ = endTime;
   }

   //入力があってプレイヤーが意図せずキーを押していた場合
    //countで数値をとって消す
    //writeDataの値が1以上で押した経過時間が本当に少しだったら押してないこととする(ニュートラル)
    //書き込みがなく、エンドからスタート引いた時間経過が短い時間だったら消す
    //ニュートラルとして受け取る時間を超えていたら入力開始
    //ニュートラルを経由するコマンドと経由しないコマンドを作る
   if (writeData == 0)
   {
       if (std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count() <= 300.0)
       {
       }
       else
       {
           ringBuf_->value = static_cast<int>(InputID::NON); 
       }

       //入力がなかった場合
       //countで数値をとれる
       if (std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count() >= 300.0)
       {
           //500を超えてたら
           startBuf_ = ringBuf_;
       }
   }
   /*if (writeData == 0)
   {
       
   }*/

   
   
    int no = 0;
    auto drawBuf = startBuf_;
    
    while (drawBuf != ringBuf_)
    {
        _dbgDrawFormatString(no * 128, 530, 0xffffff, "%lf", drawBuf->time);
        _dbgDrawFormatString(no * 128, 560, 0xffffff, "%d", drawBuf->value);
        drawBuf = drawBuf->next;
        no++;
    }

    if (startBuf_ != ringBuf_)
    {
        _dbgDrawFormatString(no * 128, 530, 0xffffff, "%lf", drawBuf->time);
        _dbgDrawFormatString(no * 128, 560, 0xffffff, "%d", drawBuf->value);
    }
}
