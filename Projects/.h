#ifndef PD195ZE31_H
#define PD195ZE31_H
#include "SPModbus.h"
#include "pd195ze31_interface.h"
#include "UniDataDevice.h"


/**
 * @class PD195ZE31
 * @author marship
 * @date 11/09/2023
 * @file PD195Z-E31用户手册.pdf
 * @brief
 */
#define RT_PD195ZE31 5130
class PD195ZE31: public UniDataDevice<PD195ZE31_Data_t, SPModbus, RT_PD195ZE31>
{
public:
    PD195ZE31();
    ~PD195ZE31();
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
private:
    enum PD195ZE31_Status {
        PD195ZE31_IDLE = 0,
        READ_FLOAT1
    };
};

PLUMA_INHERIT_PROVIDER(PD195ZE31, SMDSPDevice);
#endif
