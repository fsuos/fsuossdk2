#ifndef Sxyw_H
#define Sxyw_H
#include "SPModbus.h"
#include "Sxyw_interface.h"
#include "UniDataDevice.h"


/**
 * @file 测试.pdf
 * @brief
 */
#define RT_Sxyw 5156
class Sxyw: public UniDataDevice<Sxyw_Data_t, SPModbus, RT_Sxyw>
{
public:
    Sxyw();
    ~Sxyw();
    bool InitSetting(const Json::Value&  settingRoot);
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
private:
    enum Sxyw_Status {
        Sxyw_IDLE = 10,
	Sxyw_R3_LEVEL,
        Sxyw_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(Sxyw, SMDSPDevice);
#endif
