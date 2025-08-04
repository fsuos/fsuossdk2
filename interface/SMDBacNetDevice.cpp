#include "SMDBacNetDevice.hpp"
/* BACnet Stack API */
#include "bacnet/bacdcode.h"
#include "bacnet/bacint.h"
#include "bacnet/npdu.h"
#include "bacnet/apdu.h"
#include "bacnet/wp.h"
#include "boost/lambda/bind.hpp"
#include "boost/lambda/lambda.hpp"

SMDBacNetDevice::SMDBacNetDevice():socket_(io_context_, udp::endpoint(udp::v4(), 0)),deadline_(io_context_)
{
    port_ = 47808;
}

SMDBacNetDevice::~SMDBacNetDevice()
{
}

bool SMDBacNetDevice::Init(std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot)
{
    SMDSocketDevice::Init(controller, data_id, settingRoot);
    //app settings
    if(settingRoot["appSetting"] != Json::nullValue) {
        if(settingRoot["appSetting"].type() == Json::objectValue) {
            if(settingRoot["appSetting"]["device_id"] != Json::nullValue) {
                device_id_ = atoi(settingRoot["appSetting"]["device_id"].asString().c_str());
            }
        }
    }
    //判断IP，device_id_都在，才能启动udp socket
    std::cout<<"bacnet ip:"<<ip_<<" device_id:"<<device_id_<<std::endl;
    if(!ip_.empty() && device_id_)
    {
        remote_endpoint_.address(boost::asio::ip::address_v4::from_string(ip_));
        if(port_){
            remote_endpoint_.port(port_);
        }else{
            remote_endpoint_.port(47808);
        }
        printf("bacnet init ok\n");
    }else{
        printf("bacnet init failed\n");
    }
    return true;
}

int SMDBacNetDevice::Receive()
{
    boost::system::error_code ec;
    int received_bytes = 0;
    
//        boost::bind(&SMDBacNetDevice::handle_receive, this,
//                    boost::asio::placeholders::error,
//                    boost::asio::placeholders::bytes_transferred));
    timed_out_ = false;
    deadline_.expires_from_now(boost::posix_time::seconds(5));
    deadline_.async_wait(std::bind(&SMDBacNetDevice::DeadlineHandler, this,
                                 std::placeholders::_1));
    socket_.async_receive_from(
        boost::asio::buffer(npdu, sizeof(npdu)), peer_endpoint_, (boost::lambda::var(ec) = boost::lambda::_1,boost::lambda::var(received_bytes) = boost::lambda::_2));
    boost::posix_time::ptime now1 = boost::posix_time::second_clock::local_time();
           
    do {
        printf("SMDBacNetDevice::Receive in loop\n");
        io_context_.reset();
        io_context_.run_one();
    } while (timed_out_ == false && received_bytes == 0);
     boost::posix_time::time_duration diff = boost::posix_time::second_clock::local_time() - now1;
     std::cout<<"diff "<<diff.total_seconds()<<std::endl;
//ec == boost::system::errc::success && 
    std::cout<<"Socket Receive error  "<<ec.message()<<timed_out_<<" "<<received_bytes<<std::endl;
    if(timed_out_)
    {
        socket_.cancel();
        std::cout<<"Receive Timeout"<<std::endl;
        return 0;
    }else{
        deadline_.cancel();
        io_context_.reset();
        io_context_.run_one();
        std::cout<<"SMDBacNetDevice::handle_receive len:"<<received_bytes<<std::endl;
        if(received_bytes <= 0){
            return 0;
        }

        /* the signature of a BACnet/IPv packet */
        if (npdu[0] != BVLL_TYPE_BACNET_IP) {
            return 0;
        }
        /* Erase up to 16 bytes after the received bytes as safety margin to
         * ensure that the decoding functions will run into a 'safe field'
         * of zero, if for any reason they would overrun, when parsing the
         * message. */
        int max = 1506 - received_bytes;
        if (max > 0) {
            if (max > 16) {
                max = 16;
            }
            memset(&npdu[received_bytes], 0, max);
        }
        /* Data link layer addressing between B/IPv4 nodes consists of a 32-bit
           IPv4 address followed by a two-octet UDP port number (both of which
           shall be transmitted with the most significant octet first). This
           address shall be referred to as a B/IPv4 address.
        */

        /* pass the packet into the BBMD handler */
        
        int offset = bvlc_bbmd_enabled_handler(npdu, received_bytes);
        if (offset > 0) {
            int npdu_len = received_bytes - offset;
            if (npdu_len <= 1506) {
                /* shift the buffer to return a valid NPDU */
                for (int i = 0; i < npdu_len; i++) {
                    npdu[i] = npdu[offset + i];
                }
            } else {
                npdu_len = 0;
            }
        }
        int apdu_offset = 0;
        BACNET_ADDRESS dest = { 0 };
        BACNET_NPDU_DATA npdu_data = { 0 };
        int pdu_len = received_bytes - offset;
        apdu_offset = bacnet_npdu_decode(&npdu[0], pdu_len , &dest, &npdu_data);
        apdu_handler(&npdu[apdu_offset],
                        (uint16_t)(pdu_len - apdu_offset));
        return 1;
    }    
}

void SMDBacNetDevice::apdu_handler(
    uint8_t *apdu, /* APDU data */
    uint16_t apdu_len)
{
    BACNET_PDU_TYPE pdu_type;
    BACNET_CONFIRMED_SERVICE_DATA service_data = { 0 };
    uint8_t service_choice = 0;
    uint8_t *service_request = NULL;
    uint16_t service_request_len = 0;
    int len = 0; /* counts where we are in PDU */
    uint8_t invoke_id = 0;
    BACNET_CONFIRMED_SERVICE_ACK_DATA service_ack_data = { 0 };
    BACNET_ERROR_CODE error_code = ERROR_CODE_SUCCESS;
    BACNET_ERROR_CLASS error_class = ERROR_CLASS_SERVICES;
    uint8_t reason = 0;
    bool server = false;
    
    pdu_type = (BACNET_PDU_TYPE)(apdu[0] & 0xF0);
    switch (pdu_type) {
        case PDU_TYPE_CONFIRMED_SERVICE_REQUEST:
            break;
        case PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST:
            break;
#if !BACNET_SVC_SERVER
        case PDU_TYPE_SIMPLE_ACK:
            if (apdu_len < 3) {
                break;
            }
            invoke_id = apdu[1];
            service_choice = apdu[2];

            break;
        case PDU_TYPE_COMPLEX_ACK://zhang:重要
            if (apdu_len < 3) {
                break;
            }
            service_ack_data.segmented_message =
                (apdu[0] & BIT(3)) ? true : false;
            service_ack_data.more_follows = (apdu[0] & BIT(2)) ? true : false;
            invoke_id = service_ack_data.invoke_id = apdu[1];
            len = 2;
            if (service_ack_data.segmented_message) {
                if (apdu_len < 5) {
                    break;
                }
                service_ack_data.sequence_number = apdu[len++];
                service_ack_data.proposed_window_number = apdu[len++];
            }
            service_choice = apdu[len++];
            /* prepare the service request buffer and length */
            service_request_len = apdu_len - (uint16_t)len;
            service_request = &apdu[len];
            
            if (!apdu_confirmed_simple_ack_service((BACNET_CONFIRMED_SERVICE)service_choice)) {
                BACNET_READ_PROPERTY_DATA data;
                int len = rp_ack_decode_service_request(service_request, service_request_len, &data);
                if(len){
                    uint8_t *application_data = data.application_data;
                    int application_data_len  = data.application_data_len;
                    for (;;) {
                        BACNET_APPLICATION_DATA_VALUE value;
                        len = bacapp_decode_known_property(
                            application_data, (unsigned)application_data_len, &value,
                            data.object_type, data.object_property);

                        if (len < 0) {
                            break;
                        }
                        //std::cout<<"get value:"<<value.type.Real<<std::endl;
                        appValue = value;
                        if (len > 0) {
                            if (len < application_data_len) {
                                application_data += len;
                                application_data_len -= len;
                                /* there's more! */
                            } else {
                                break;
                            }
                        } else {
                            break;
                        }
                    }
                }
            }
            break;
        case PDU_TYPE_SEGMENT_ACK:
            /* FIXME: what about a denial of service attack here?
                we could check src to see if that matched the tsm */
            break;
        case PDU_TYPE_ERROR:
            if (apdu_len < 3) {
                break;
            }
            invoke_id = apdu[1];
            service_choice = apdu[2];
            /* prepare the service request buffer and length */
            service_request_len = apdu_len - 3;
            service_request = &apdu[3];
            break;
        case PDU_TYPE_REJECT:
            if (apdu_len < 3) {
                break;
            }
            invoke_id = apdu[1];
            reason = apdu[2];
            break;
        case PDU_TYPE_ABORT:
            if (apdu_len < 3) {
                break;
            }
            server = apdu[0] & 0x01;
            invoke_id = apdu[1];
            reason = apdu[2];
            break;
#endif
        default:
            break;
    }
}


int SMDBacNetDevice::rp_ack_decode_service_request(
    uint8_t *apdu, int apdu_size, BACNET_READ_PROPERTY_DATA *data)
{
    int apdu_len = 0; /* return value */
    int len = 0;
    uint32_t instance = 0;
    BACNET_OBJECT_TYPE type = OBJECT_NONE; /* for decoding */
    uint32_t property = 0; /* for decoding */
    BACNET_UNSIGNED_INTEGER unsigned_value = 0; /* for decoding */
    int data_len = 0;

    if (!apdu) {
        return -BACNET_STATUS_ERROR;
    }
    /* object-identifier [0] BACnetObjectIdentifier */
    len = bacnet_object_id_context_decode(
        &apdu[apdu_len], apdu_size - apdu_len, 0, &type, &instance);
    if (len > 0) {
        if (instance > BACNET_MAX_INSTANCE) {
            return BACNET_STATUS_ERROR;
        }
        apdu_len += len;
        if (data) {
            data->object_type = type;
            data->object_instance = instance;
        }
    } else {
        return BACNET_STATUS_ERROR;
    }
    /* property-identifier [1] BACnetPropertyIdentifier */
    len = bacnet_enumerated_context_decode(
        &apdu[apdu_len], apdu_size - apdu_len, 1, &property);
    if (len > 0) {
        apdu_len += len;
        if (data) {
            data->object_property = (BACNET_PROPERTY_ID)property;
        }
    } else {
        return BACNET_STATUS_ERROR;
    }
    /* property-array-index [2] Unsigned OPTIONAL */
    len = bacnet_unsigned_context_decode(
        &apdu[apdu_len], apdu_size - apdu_len, 2, &unsigned_value);
    if (len > 0) {
        apdu_len += len;
        if (data) {
            data->array_index = unsigned_value;
        }
    } else {
        /* wrong tag - skip apdu_len increment and go to next field */
        if (data) {
            data->array_index = BACNET_ARRAY_ALL;
        }
    }
    /* property-value [3] ABSTRACT-SYNTAX.&Type */
    if (!bacnet_is_opening_tag_number(
            &apdu[apdu_len], apdu_size - apdu_len, 3, &len)) {
        return BACNET_STATUS_ERROR;
    }
    /* determine the length of the data blob */
    data_len =
        bacnet_enclosed_data_length(&apdu[apdu_len], apdu_size - apdu_len);
    if (data_len == BACNET_STATUS_ERROR) {
        return BACNET_STATUS_ERROR;
    }
    /* count the opening tag number length */
    apdu_len += len;
    if (data_len > MAX_APDU) {
        /* not enough size in application_data to store the data chunk */
        return BACNET_STATUS_ERROR;
    } else if (data) {
        /* don't decode the application tag number or its data here */
        data->application_data = &apdu[apdu_len];
        data->application_data_len = data_len;
    }
    apdu_len += data_len;
    if (!bacnet_is_closing_tag_number(
            &apdu[apdu_len], apdu_size - apdu_len, 3, &len)) {
        return BACNET_STATUS_ERROR;
    }
    /* count the closing tag number length */
    apdu_len += len;

    return apdu_len;
}

bool SMDBacNetDevice::apdu_confirmed_simple_ack_service(BACNET_CONFIRMED_SERVICE service_choice)
{
    bool status = false;

    switch (service_choice) {
        case SERVICE_CONFIRMED_ACKNOWLEDGE_ALARM:
        case SERVICE_CONFIRMED_AUDIT_NOTIFICATION:
        case SERVICE_CONFIRMED_COV_NOTIFICATION:
        case SERVICE_CONFIRMED_COV_NOTIFICATION_MULTIPLE:
        case SERVICE_CONFIRMED_EVENT_NOTIFICATION:
        case SERVICE_CONFIRMED_SUBSCRIBE_COV:
        case SERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY:
        case SERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY_MULTIPLE:
        case SERVICE_CONFIRMED_LIFE_SAFETY_OPERATION:
            /* Object Access Services */
        case SERVICE_CONFIRMED_ADD_LIST_ELEMENT:
        case SERVICE_CONFIRMED_REMOVE_LIST_ELEMENT:
        case SERVICE_CONFIRMED_DELETE_OBJECT:
        case SERVICE_CONFIRMED_WRITE_PROPERTY:
        case SERVICE_CONFIRMED_WRITE_PROP_MULTIPLE:
            /* Remote Device Management Services */
        case SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL:
        case SERVICE_CONFIRMED_TEXT_MESSAGE:
        case SERVICE_CONFIRMED_REINITIALIZE_DEVICE:
            /* Virtual Terminal Services */
        case SERVICE_CONFIRMED_VT_CLOSE:
            /* Security Services */
        case SERVICE_CONFIRMED_REQUEST_KEY:
            status = true;
            break;
        default:
            break;
    }

    return status;
}

uint16_t SMDBacNetDevice::apdu_decode_confirmed_service_request(
    uint8_t *apdu, /* APDU data */
    uint16_t apdu_len,
    BACNET_CONFIRMED_SERVICE_DATA *service_data,
    uint8_t *service_choice,
    uint8_t **service_request,
    uint16_t *service_request_len)
{
    uint16_t len = 0; /* counts where we are in PDU */

    if (apdu_len >= 3) {
        service_data->segmented_message = (apdu[0] & BIT(3)) ? true : false;
        service_data->more_follows = (apdu[0] & BIT(2)) ? true : false;
        service_data->segmented_response_accepted =
            (apdu[0] & BIT(1)) ? true : false;
        service_data->max_segs = decode_max_segs(apdu[1]);
        service_data->max_resp = decode_max_apdu(apdu[1]);
        service_data->invoke_id = apdu[2];
        service_data->priority = apdu_network_priority();
        len = 3;
        if (service_data->segmented_message) {
            if (apdu_len >= (len + 2)) {
                service_data->sequence_number = apdu[len++];
                service_data->proposed_window_number = apdu[len++];
            } else {
                return 0;
            }
        }
        if (apdu_len > MAX_APDU) {
            return 0;
        } else if (apdu_len == (len + 1)) {
            /* no request data as seen with Inneasoft BACnet Explorer */
            *service_choice = apdu[len++];
            *service_request = NULL;
            *service_request_len = 0;
        } else if (apdu_len >= (len + 2)) {
            *service_choice = apdu[len++];
            *service_request = &apdu[len];
            *service_request_len = apdu_len - len;
        } else {
            return 0;
        }
    }

    return len;
}

int SMDBacNetDevice::bacnet_npdu_decode(
    const uint8_t *npdu,
    uint16_t pdu_len,
    BACNET_ADDRESS *dest,
    BACNET_NPDU_DATA *npdu_data)
{
    int len = 0; /* return value - number of octets loaded in this function */
    uint8_t i = 0; /* counter */
    uint16_t src_net = 0;
    uint16_t dest_net = 0;
    uint8_t slen = 0;
    uint8_t dlen = 0;
    uint8_t mac_octet = 0;

    if (npdu && npdu_data && (pdu_len >= 2)) {
        /* Protocol Version */
        npdu_data->protocol_version = npdu[0];
        /* control octet */
        /* Bit 7: 1 indicates that the NSDU conveys a network layer message. */
        /*          Message Type field is present. */
        /*        0 indicates that the NSDU contains a BACnet APDU. */
        /*          Message Type field is absent. */
        npdu_data->network_layer_message = (npdu[1] & BIT(7)) ? true : false;
        /*Bit 6: Reserved. Shall be zero. */
        /* Bit 4: Reserved. Shall be zero. */
        /* Bit 2: The value of this bit corresponds to data expecting reply */
        /* parameter in the N-UNITDATA primitives. */
        /* 1 indicates that a BACnet-Confirmed-Request-PDU, */
        /* a segment of a BACnet-ComplexACK-PDU, */
        /* or a network layer message expecting a reply is present. */
        /* 0 indicates that other than a BACnet-Confirmed-Request-PDU, */
        /* a segment of a BACnet-ComplexACK-PDU, */
        /* or a network layer message expecting a reply is present. */
        npdu_data->data_expecting_reply = (npdu[1] & BIT(2)) ? true : false;
        /* Bits 1,0: Network priority where: */
        /* B'11' = Life Safety message */
        /* B'10' = Critical Equipment message */
        /* B'01' = Urgent message */
        /* B'00' = Normal message */
        npdu_data->priority = (BACNET_MESSAGE_PRIORITY)(npdu[1] & 0x03);
        /* set the offset to where the optional stuff starts */
        len = 2;
        /*Bit 5: Destination specifier where: */
        /* 0 = DNET, DLEN, DADR, and Hop Count absent */
        /* 1 = DNET, DLEN, and Hop Count present */
        /* DLEN = 0 denotes broadcast MAC DADR and DADR field is absent */
        /* DLEN > 0 specifies length of DADR field */
        if (npdu[1] & BIT(5)) {
            if (pdu_len >= (len + 3)) {
                len += decode_unsigned16(&npdu[len], &dest_net);
                /* DLEN = 0 denotes broadcast MAC DADR and DADR field is absent
                 */
                /* DLEN > 0 specifies length of DADR field */
                dlen = npdu[len++];
                if (dest) {
                    dest->net = dest_net;
                    dest->len = dlen;
                }
                if (dlen) {
                    if ((dlen > MAX_MAC_LEN) || (pdu_len < (len + dlen))) {
                        /* address is too large could be a malformed message */
                        return -1;
                    }

                    for (i = 0; i < dlen; i++) {
                        mac_octet = npdu[len++];
                        if (dest) {
                            dest->adr[i] = mac_octet;
                        }
                    }
                }
            }
        }
        /* zero out the destination address */
        else if (dest) {
            dest->net = 0;
            dest->len = 0;
            for (i = 0; i < MAX_MAC_LEN; i++) {
                dest->adr[i] = 0;
            }
        }
        /* Bit 3: Source specifier where: */
        /* 0 =  SNET, SLEN, and SADR absent */
        /* 1 =  SNET, SLEN, and SADR present */
        if (npdu[1] & BIT(3)) {
            if (pdu_len >= (len + 3)) {
                len += decode_unsigned16(&npdu[len], &src_net);
                /* SLEN = 0 denotes broadcast MAC SADR and SADR field is absent
                 */
                /* SLEN > 0 specifies length of SADR field */
                slen = npdu[len++];
                if (slen) {
                    if ((slen > MAX_MAC_LEN) || (pdu_len < (len + slen))) {
                        /* address is too large could be a malformed message */
                        return -1;
                    }

                    for (i = 0; i < slen; i++) {
                        mac_octet = npdu[len++];
                    }
                }
            }
        } 
        /* The Hop Count field shall be present only if the message is */
        /* destined for a remote network, i.e., if DNET is present. */
        /* This is a one-octet field that is initialized to a value of 0xff. */
        if (dest_net) {
            if (pdu_len > len) {
                npdu_data->hop_count = npdu[len++];
            } else {
                npdu_data->hop_count = 0;
            }
        } else {
            npdu_data->hop_count = 0;
        }
        /* Indicates that the NSDU conveys a network layer message. */
        /* Message Type field is present. */
        if (npdu_data->network_layer_message) {
            if (pdu_len > len) {
                npdu_data->network_message_type =
                    (BACNET_NETWORK_MESSAGE_TYPE)npdu[len++];
                /* Message Type field contains a value in the range 0x80 - 0xFF,
                 */
                /* then a Vendor ID field shall be present */
                if (npdu_data->network_message_type >= 0x80) {
                    if (pdu_len >= (len + 2)) {
                        len += decode_unsigned16(
                            &npdu[len], &npdu_data->vendor_id);
                    }
                }
            }
        } else {
            /* Since npdu_data->network_layer_message is false,
             * it doesn't much matter what we set here; this is safe: */
            npdu_data->network_message_type = NETWORK_MESSAGE_INVALID;
        }
    }

    return len;
}
//return 4
int SMDBacNetDevice::bvlc_bbmd_enabled_handler(
    uint8_t *mtu,
    uint16_t mtu_len)
{
    uint16_t result_code = BVLC_RESULT_SUCCESSFUL_COMPLETION;
    uint8_t message_type = 0;
    uint16_t message_length = 0;
    int header_len = 0;
    int function_len = 0;
    uint8_t *pdu = NULL;
    uint16_t pdu_len = 0;
    uint8_t *npdu = NULL;
    uint16_t npdu_len = 0;
    bool send_result = false;
    uint16_t offset = 0;
    uint16_t ttl_seconds = 0;

    header_len =
        bvlc_decode_header(mtu, mtu_len, &message_type, &message_length);
    if (header_len != 4) {
        return 0;
    }
    pdu = &mtu[header_len];
    pdu_len = mtu_len - header_len;
    
    function_len =
    bvlc_decode_original_unicast(pdu, pdu_len, NULL, 0, &npdu_len);
    /* prepare the message for me! */
    offset = header_len + function_len - npdu_len;
    
    return offset;
}

int SMDBacNetDevice::bvlc_decode_original_unicast(
    const uint8_t *pdu,
    uint16_t pdu_len,
    uint8_t *npdu,
    uint16_t npdu_size,
    uint16_t *npdu_len)
{
    int bytes_consumed = 0;
    uint16_t i = 0;

    if (pdu && npdu && (pdu_len > 0) && (pdu_len <= npdu_size)) {
        for (i = 0; i < pdu_len; i++) {
            npdu[i] = pdu[i];
        }
    }
    if (npdu_len) {
        *npdu_len = pdu_len;
    }
    bytes_consumed = (int)pdu_len;

    return bytes_consumed;
}

void SMDBacNetDevice::handle_receive(const boost::system::error_code& error,
                                  std::size_t bytes_transferred)
{
    if (!error || error == boost::asio::error::message_size) {
        std::cout<<"SMDBacNetDevice::handle_receive len:"<<bytes_transferred<<std::endl;
    }
    
}

void SMDBacNetDevice::DeadlineHandler(boost::system::error_code ec) {

  if (ec == boost::asio::error::operation_aborted) {
    return;
  }
  std::cout<<"timeout called"<<std::endl;
  timed_out_ = true;
}



uint8_t SMDBacNetDevice::Send_Write_Property_Request_Data(
    BACNET_OBJECT_TYPE object_type,
    uint32_t object_instance,
    BACNET_PROPERTY_ID object_property,
    const uint8_t *application_data,
    int application_data_len,
    uint8_t priority,
    uint32_t array_index)
{
    BACNET_ADDRESS dest;
    BACNET_ADDRESS my_address;
    unsigned max_apdu = 0;
    uint8_t invoke_id = 0;
    bool status = false;
    int len = 0;
    int pdu_len = 0;
    int bytes_sent = 0;
    BACNET_WRITE_PROPERTY_DATA data;
    BACNET_NPDU_DATA npdu_data;

    /* encode the NPDU portion of the packet */
    npdu_encode_npdu_data(&npdu_data, true, MESSAGE_PRIORITY_NORMAL);
    pdu_len = npdu_encode_pdu(&Handler_Transmit_Buffer[0], &npdu_data);
    /* encode the APDU portion of the packet */
    data.object_type = object_type;
    data.object_instance = object_instance;
    data.object_property = object_property;
    data.array_index = array_index;
    data.application_data_len = application_data_len;
    memcpy(
        &data.application_data[0], &application_data[0],
        application_data_len);
    data.priority = priority;
    len =
        wp_encode_apdu(&Handler_Transmit_Buffer[pdu_len], invoke_id, &data);
    pdu_len += len;
        
    #define BIP_HEADER_MAX (1 + 1 + 2)
    #define BIP_MPDU_MAX (BIP_HEADER_MAX + MAX_PDU)

    uint8_t mtu[BIP_MPDU_MAX] = { 0 };
    uint16_t mtu_len = 0;

    mtu_len = bvlc_encode_original_unicast(mtu, sizeof(mtu), &Handler_Transmit_Buffer[0], pdu_len);
    boost::system::error_code ec;
    socket_.async_send_to(boost::asio::buffer(mtu, mtu_len), remote_endpoint_,boost::lambda::var(ec) = boost::lambda::_1);
    do {
        io_context_.reset();
        io_context_.run_one();
    } while (ec == boost::asio::error::would_block);
            
    if (ec) {
        std::cout<<"Socket connect error  "<<ec.message()<<std::endl;
        return 0;
    }
    invoke_id++;
    if(invoke_id == 0)
    {
        invoke_id++;
    }
    return invoke_id;
}

/** Sends a Write Property request.
 * @ingroup DSWP
 *
 * @param device_id [in] ID of the destination device
 * @param object_type [in]  Type of the object whose property is to be written.
 * @param object_instance [in] Instance # of the object to be written.
 * @param object_property [in] Property to be written.
 * @param object_value [in] The value to be written to the property.
 * @param priority [in] Write priority of 1 (highest) to 16 (lowest)
 * @param array_index [in] Optional: if the Property is an array,
 *   - 0 for the array size
 *   - 1 to n for individual array members
 *   - BACNET_ARRAY_ALL (~0) for the array value to be ignored (not sent)
 * @return invoke id of outgoing message, or 0 on failure.
 */
uint8_t SMDBacNetDevice::Send_Write_Property_Request(
    BACNET_OBJECT_TYPE object_type,
    uint32_t object_instance,
    BACNET_PROPERTY_ID object_property,
    const BACNET_APPLICATION_DATA_VALUE *object_value,
    uint8_t priority,
    uint32_t array_index)
{
    uint8_t application_data[MAX_APDU] = { 0 };
    int apdu_len = 0, len = 0;

    while (object_value) {
#if PRINT_ENABLED_DEBUG
        fprintf(
            stderr,
            "WriteProperty service: "
            "%s tag=%d\n",
            (object_value->context_specific ? "context" : "application"),
            (int)(object_value->context_specific ? object_value->context_tag
                                                 : object_value->tag));
#endif
        len = bacapp_encode_data(&application_data[apdu_len], object_value);
        if ((len + apdu_len) < MAX_APDU) {
            apdu_len += len;
        } else {
            return 0;
        }
        object_value = object_value->next;
    }

    return Send_Write_Property_Request_Data(
        object_type, object_instance, object_property,
        &application_data[0], apdu_len, priority, array_index);
}

uint8_t SMDBacNetDevice::Send_Read_Property_Request_Address(
    BACNET_OBJECT_TYPE object_type,
    uint32_t object_instance,
    BACNET_PROPERTY_ID object_property,
    uint32_t array_index)
{  
    int len = 0;
    int pdu_len = 0;
    int bytes_sent = 0;
    BACNET_READ_PROPERTY_DATA data;
    BACNET_NPDU_DATA npdu_data;
    
    /* is there a tsm available? */

    /* encode the NPDU portion of the packet */
    npdu_encode_npdu_data(&npdu_data, true, MESSAGE_PRIORITY_NORMAL);
    pdu_len = npdu_encode_pdu(
        &Handler_Transmit_Buffer[0], &npdu_data);
    /* encode the APDU portion of the packet */
    data.object_type = object_type;
    data.object_instance = object_instance;
    data.object_property = object_property;
    data.array_index = array_index;
    len =  rp_encode_apdu(&Handler_Transmit_Buffer[pdu_len], invoke_id, &data);
    pdu_len += len;
       
    #define BIP_HEADER_MAX (1 + 1 + 2)
    #define BIP_MPDU_MAX (BIP_HEADER_MAX + MAX_PDU)

    uint8_t mtu[BIP_MPDU_MAX] = { 0 };
    uint16_t mtu_len = 0;

    mtu_len = bvlc_encode_original_unicast(mtu, sizeof(mtu), &Handler_Transmit_Buffer[0], pdu_len);
    boost::system::error_code ec;
    socket_.async_send_to(boost::asio::buffer(mtu, mtu_len), remote_endpoint_,boost::lambda::var(ec) = boost::lambda::_1);
    do {
        io_context_.reset();
        io_context_.run_one();
    } while (ec == boost::asio::error::would_block);
            
    if (ec) {
        std::cout<<"Socket connect error  "<<ec.message()<<std::endl;
        return 0;
    }
    invoke_id++;
    if(invoke_id == 0)
    {
        invoke_id++;
    }
    return invoke_id;
}

void SMDBacNetDevice::npdu_encode_npdu_data(
    BACNET_NPDU_DATA *npdu_data,
    bool data_expecting_reply,
    BACNET_MESSAGE_PRIORITY priority)
{
    if (npdu_data) {
        npdu_data->data_expecting_reply = data_expecting_reply;
        npdu_data->protocol_version = BACNET_PROTOCOL_VERSION;
        npdu_data->network_layer_message = false; /* false if APDU */
        npdu_data->network_message_type =
            NETWORK_MESSAGE_INVALID; /* optional */
        npdu_data->vendor_id = 0; /* optional, if net message type is > 0x80 */
        npdu_data->priority = priority;
        npdu_data->hop_count = HOP_COUNT_DEFAULT;
    }
}
uint8_t SMDBacNetDevice::encode_max_segs_max_apdu(int max_segs, int max_apdu)
{
    uint8_t octet = 0;

    if (max_segs < 2) {
        octet = 0;
    } else if (max_segs < 4) {
        octet = 0x10;
    } else if (max_segs < 8) {
        octet = 0x20;
    } else if (max_segs < 16) {
        octet = 0x30;
    } else if (max_segs < 32) {
        octet = 0x40;
    } else if (max_segs < 64) {
        octet = 0x50;
    } else if (max_segs == 64) {
        octet = 0x60;
    } else {
        octet = 0x70;
    }

    /* max_apdu must be 50 octets minimum */
    if (max_apdu <= 50) {
        octet |= 0x00;
    } else if (max_apdu <= 128) {
        octet |= 0x01;
        /*fits in a LonTalk frame */
    } else if (max_apdu <= 206) {
        octet |= 0x02;
        /*fits in an ARCNET or MS/TP frame */
    } else if (max_apdu <= 480) {
        octet |= 0x03;
    } else if (max_apdu <= 1024) {
        octet |= 0x04;
        /* fits in an ISO 8802-3 frame */
    } else if (max_apdu <= 1476) {
        octet |= 0x05;
    }

    return octet;
}

int SMDBacNetDevice::rp_encode_apdu(
    uint8_t *apdu, uint8_t invoke_id, const BACNET_READ_PROPERTY_DATA *data)
{
    int len = 0; /* length of each encoding */
    int apdu_len = 0; /* total length of the apdu, return value */

    if (apdu) {
        apdu[0] = PDU_TYPE_CONFIRMED_SERVICE_REQUEST;
        apdu[1] = encode_max_segs_max_apdu(0, MAX_APDU);
        apdu[2] = invoke_id;
        apdu[3] = SERVICE_CONFIRMED_READ_PROPERTY; /* service choice */
    }
    len = 4;
    apdu_len += len;
    if (apdu) {
        apdu += len;
    }
    len = read_property_request_encode(apdu, data);
    apdu_len += len;

    return apdu_len;
}



int SMDBacNetDevice::read_property_request_encode(
    uint8_t *apdu, const BACNET_READ_PROPERTY_DATA *data)
{
    int len = 0; /* length of each encoding */
    int apdu_len = 0; /* total length of the apdu, return value */

    if (!data) {
        return 0;
    }
    /* object-identifier [0] BACnetObjectIdentifier */
    if (data->object_type <= BACNET_MAX_OBJECT) {
        len = encode_context_object_id(
            apdu, 0, data->object_type, data->object_instance);
        apdu_len += len;
        if (apdu) {
            apdu += len;
        }
    }
    /* property-identifier [1] BACnetPropertyIdentifier */
    if (data->object_property <= MAX_BACNET_PROPERTY_ID) {
        len = encode_context_enumerated(apdu, 1, data->object_property);
        apdu_len += len;
        if (apdu) {
            apdu += len;
        }
    }
    /* property-array-index [2] Unsigned OPTIONAL */
    if (data->array_index != BACNET_ARRAY_ALL) {
        len = encode_context_unsigned(apdu, 2, data->array_index);
        apdu_len += len;
    }

    return apdu_len;
}

int SMDBacNetDevice::npdu_encode_pdu(
    uint8_t *npdu,
    const BACNET_NPDU_DATA *npdu_data)
{
    int len = 0; /* return value - number of octets loaded in this function */
    uint8_t i = 0; /* counter  */

    if (npdu_data) {
        /* protocol version */
        if (npdu) {
            npdu[0] = npdu_data->protocol_version;
        }
        /* initialize the control octet */
        if (npdu) {
            npdu[1] = 0;
            /* Bit 7: 1 indicates that the NSDU conveys a network layer message.
             */
            /*          Message Type field is present. */
            /*        0 indicates that the NSDU contains a BACnet APDU. */
            /*          Message Type field is absent. */
            if (npdu_data->network_layer_message) {
                npdu[1] |= BIT(7);
            }
            /*: The value of this bit corresponds to the */
            /* data_expecting_reply parameter in the N-UNITDATA primitives. */
            /* 1 indicates that a BACnet-Confirmed-Request-PDU, */
            /* a segment of a BACnet-ComplexACK-PDU, */
            /* or a network layer message expecting a reply is present. */
            /* 0 indicates that other than a BACnet-Confirmed-Request-PDU, */
            /* a segment of a BACnet-ComplexACK-PDU, */
            /* or a network layer message expecting a reply is present. */
            if (npdu_data->data_expecting_reply) {
                npdu[1] |= BIT(2);
            }
            /* Bits 1,0: Network priority where: */
            /* B'11' = Life Safety message */
            /* B'10' = Critical Equipment message */
            /* B'01' = Urgent message */
            /* B'00' = Normal message */
            npdu[1] |= (npdu_data->priority & 0x03);
        }
        len = 2;
        
        
        if (npdu_data->network_layer_message) {
            if (npdu) {
                npdu[len] = npdu_data->network_message_type;
            }
            len++;
            /* Message Type field contains a value in the range 0x80 - 0xFF, */
            /* then a Vendor ID field shall be present */
            if (npdu_data->network_message_type >= 0x80) {
                if (npdu) {
                    encode_unsigned16(&npdu[len], npdu_data->vendor_id);
                }
                len += 2;
            }
        }
    }

    return len;
}

int SMDBacNetDevice::bvlc_decode_header(
    const uint8_t *pdu,
    uint16_t pdu_len,
    uint8_t *message_type,
    uint16_t *message_length)
{
    int bytes_consumed = 0;

    if (pdu && (pdu_len >= 4)) {
        if (pdu[0] == BVLL_TYPE_BACNET_IP) {
            if (message_type) {
                *message_type = pdu[1];
            }
            if (message_length) {
                decode_unsigned16(&pdu[2], message_length);
            }
            bytes_consumed = 4;
        }
    }

    return bytes_consumed;
}

int SMDBacNetDevice::bvlc_encode_header(
    uint8_t *pdu, uint16_t pdu_size, uint8_t message_type, uint16_t length)
{
    int bytes_encoded = 0;

    if (pdu && (pdu_size >= 2)) {
        pdu[0] = BVLL_TYPE_BACNET_IP;
        pdu[1] = message_type;
        /* The 2-octet BVLC Length field is the length, in octets,
           of the entire BVLL message, including the two octets of the
           length field itself, most significant octet first. */
        encode_unsigned16(&pdu[2], length);
        bytes_encoded = 4;
    }

    return bytes_encoded;
}

int SMDBacNetDevice::bvlc_encode_original_unicast(
    uint8_t *pdu, uint16_t pdu_size, const uint8_t *npdu, uint16_t npdu_len)
{
    int bytes_encoded = 0;
    uint16_t length = 4;
    uint16_t i = 0;

    length += npdu_len;
    if (pdu && (pdu_size >= length)) {
        bytes_encoded = bvlc_encode_header(
            pdu, pdu_size, BVLC_ORIGINAL_UNICAST_NPDU, length);
        if (bytes_encoded == 4) {
            if (npdu && (npdu_len > 0)) {
                for (i = 0; i < npdu_len; i++) {
                    pdu[4 + i] = npdu[i];
                }
                bytes_encoded = (int)length;
            }
        }
    }
    return bytes_encoded;
}
//
//
//uint8_t SMDBacNetDevice::Send_Read_Property_Multiple_Request(
//    uint8_t *pdu,
//    size_t max_pdu,
//    uint32_t device_id, /* destination device */
//    BACNET_READ_ACCESS_DATA *read_access_data)
//{
//    BACNET_ADDRESS dest;
//    BACNET_ADDRESS my_address;
//    unsigned max_apdu = 0;
//    uint8_t invoke_id = 0;
//    bool status = false;
//    int len = 0;
//    int pdu_len = 0;
//    BACNET_NPDU_DATA npdu_data;
//
//    /* is the device bound? */
//    invoke_id = tsm_next_free_invokeID();
//    if (invoke_id) {
//        /* encode the NPDU portion of the packet */
//        npdu_encode_npdu_data(&npdu_data, true, MESSAGE_PRIORITY_NORMAL);
//        pdu_len = npdu_encode_pdu(&pdu[0], &dest, &my_address, &npdu_data);
//        /* encode the APDU portion of the packet */
//        len = rpm_encode_apdu(
//            &pdu[pdu_len], max_pdu - pdu_len, invoke_id, read_access_data);
//        if (len <= 0) {
//            return 0;
//        }
//        pdu_len += len;
//        /* is it small enough for the destination to receive?
//           note: if there is a bottleneck router in between
//           us and the destination, we won't know unless
//           we have a way to check for that and update the
//           max_apdu in the address binding table. */
//        if ((unsigned)pdu_len < max_apdu) {
//            tsm_set_confirmed_unsegmented_transaction(
//                invoke_id, &dest, &npdu_data, &pdu[0], (uint16_t)pdu_len);
//                //datalink_send_pdu(&dest, &npdu_data, &pdu[0], pdu_len);
//
//        } else {
//            tsm_free_invoke_id(invoke_id);
//            invoke_id = 0;
//        }
//    }
//
//    return invoke_id;
//}