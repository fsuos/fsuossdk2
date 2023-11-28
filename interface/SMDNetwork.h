#ifndef SMDNETWORK_H
#define SMDNETWORK_H
#include <memory>
#include <vector>
class SMDNetwork
{
public:
    SMDNetwork();
    ~SMDNetwork();

    virtual bool send_buffer(uint32_t data_id, uint8_t* data, int data_len) { return true; }
    virtual void send_msg(std::shared_ptr<std::vector<uint8_t>> &msg) {}
};

#endif // SMDNETWORK_H
