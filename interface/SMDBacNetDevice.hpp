#ifndef SMDBACNETDEVICE_HPP
#define SMDBACNETDEVICE_HPP
#include "SMDSocketDevice.hpp"


#include "bacnet/bacdef.h"
#include "bacnet/apdu.h"
#include "bacnet/rp.h"
#include "bacnet/datalink/bvlc.h"
#include "bacnet/basic/tsm/tsm.h"
#include "bacnet/bacapp.h"

#include <boost/asio/io_service.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "boost/asio/deadline_timer.hpp"


using boost::asio::ip::udp;
/*
 * 支持BacNet/IP协议读取，设置property，使用udp协议
 * 
 * 
 */
class SMDBacNetDevice : public SMDSocketDevice
{
public:
    SMDBacNetDevice();
    ~SMDBacNetDevice();
    bool Init(std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot);

protected:
    int Receive();
    void DeadlineHandler(boost::system::error_code ec) ;
    void handle_receive(const boost::system::error_code& error,
                                  std::size_t bytes_transferred);
                                  
    uint16_t apdu_decode_confirmed_service_request(
    uint8_t *apdu, /* APDU data */
    uint16_t apdu_len,
    BACNET_CONFIRMED_SERVICE_DATA *service_data,
    uint8_t *service_choice,
    uint8_t **service_request,
    uint16_t *service_request_len);
    
    bool apdu_confirmed_simple_ack_service(BACNET_CONFIRMED_SERVICE service_choice);
    
    int bvlc_bbmd_enabled_handler(
    uint8_t *mtu,
    uint16_t mtu_len);
    
    uint8_t Send_Read_Property_Request_Address(
    BACNET_OBJECT_TYPE object_type,
    uint32_t object_instance,
    BACNET_PROPERTY_ID object_property,
    uint32_t array_index = BACNET_ARRAY_ALL);
    
    void npdu_encode_npdu_data(
    BACNET_NPDU_DATA *npdu_data,
    bool data_expecting_reply,
    BACNET_MESSAGE_PRIORITY priority);
    
    void apdu_handler(
    uint8_t *apdu, /* APDU data */
    uint16_t apdu_len);
    
    int npdu_encode_pdu(
    uint8_t *npdu,
    const BACNET_NPDU_DATA *npdu_data);
    
    uint8_t encode_max_segs_max_apdu(int max_segs, int max_apdu);
    
    int rp_encode_apdu(
    uint8_t *apdu, uint8_t invoke_id, const BACNET_READ_PROPERTY_DATA *data);
    
    int bacnet_npdu_decode(
    const uint8_t *npdu,
    uint16_t pdu_len,
    BACNET_ADDRESS *dest,
    BACNET_NPDU_DATA *npdu_data);
    
    int bvlc_decode_header(
    const uint8_t *pdu,
    uint16_t pdu_len,
    uint8_t *message_type,
    uint16_t *message_length);
    

    
    
    int rp_ack_decode_service_request(
    uint8_t *apdu, int apdu_size, BACNET_READ_PROPERTY_DATA *data);
    
    int bvlc_encode_header(uint8_t *pdu, uint16_t pdu_size, uint8_t message_type, uint16_t length);
    
    int bvlc_encode_original_unicast(uint8_t *pdu, uint16_t pdu_size, const uint8_t *npdu, uint16_t npdu_len);
    
    int read_property_request_encode(uint8_t *apdu, const BACNET_READ_PROPERTY_DATA *data);
//    uint8_t Send_Read_Property_Multiple_Request(
//    uint8_t *pdu,
//    size_t max_pdu,
//    uint32_t device_id, /* destination device */
//    BACNET_READ_ACCESS_DATA *read_access_data);
    int bvlc_decode_original_unicast(
    const uint8_t *pdu,
    uint16_t pdu_len,
    uint8_t *npdu,
    uint16_t npdu_size,
    uint16_t *npdu_len);
    
    uint32_t device_id_ = BACNET_MAX_INSTANCE;
    uint32_t Target_Object_Instance = BACNET_MAX_INSTANCE;
    BACNET_OBJECT_TYPE Target_Object_Type = OBJECT_ANALOG_INPUT;
    BACNET_PROPERTY_ID Target_Object_Property = PROP_ACKED_TRANSITIONS;
    int32_t Target_Object_Index = BACNET_ARRAY_ALL;
    /* the invoke id is needed to filter incoming messages */
    uint8_t Request_Invoke_ID = 0;
    
    
    //tsm
    uint8_t Handler_Transmit_Buffer[MAX_PDU];
    
    BACNET_TSM_DATA TSM_List[MAX_TSM_TRANSACTIONS];

    /* invoke ID for incrementing between subsequent calls. */
    uint8_t Current_Invoke_ID = 1;

    //udp socket
    boost::asio::io_service io_context_;
    udp::socket socket_;
	
	udp::endpoint remote_endpoint_;
    udp::endpoint peer_endpoint_;
    
    uint8_t invoke_id = 1;
    
    /* buffer used for receive */
    uint8_t npdu[1506] = { 0 };
    
    bool timed_out_ = false;
    bool get_data_ = false;
    
    boost::asio::deadline_timer deadline_;
    
    BACNET_APPLICATION_DATA_VALUE appValue;
};

#endif // SMDBACNETDEVICE_HPP
