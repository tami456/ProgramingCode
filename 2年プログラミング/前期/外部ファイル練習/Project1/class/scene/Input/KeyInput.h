#pragma once
#include "Controller.h"

class KeyInput :
    public Controller
{
public:
    KeyInput();
    ~KeyInput();

private:
   // CntType GetType(void) override { return CntType::Key; };
    void Update(void)override;
    std::array<char, 256> keyData_;
    //ç\ë¢Çäiî[Ç∑ÇÈÇΩÇﬂÇÃÇ‡ÇÃ
    std::map<InputID, int>inputTbl_;
};

