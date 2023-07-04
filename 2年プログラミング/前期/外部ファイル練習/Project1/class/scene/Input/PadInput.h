#pragma once
#include "Controller.h"
class PadInput :
    public Controller
{
public:
    PadInput();
    ~PadInput();
private:
    //CntType GetType(void) override { return CntType::Pad; };
    void Update(void)override;
    bool Init(void);
    std::array<char, 256> padData_;
    std::map<InputID, int>inputTbl_;
};

