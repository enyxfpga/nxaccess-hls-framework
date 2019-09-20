//--------------------------------------------------------------------------------
//--! Enyx Confidential
//--!
//--! Organization:          Enyx
//--! Project Identifier:    010 - Enyx nxAccess HLS Framework
//--! Author:                Raphael Charolois (raphael.charolois@enyx.com)
//--!
//--! © Copyright            Enyx 2019
//--! © Copyright Notice:    The source code for this program is not published or otherwise divested of its trade secrets, 
//--!                        irrespective of what has been deposited with the U.S. Copyright Office.
//--------------------------------------------------------------------------------

#include <cassert>
#include <iostream>

#include "configuration.hpp"
#include "messages.hpp"

#include "../include/enyx/oe/hwstrat/helpers.hpp"

namespace enyx {
namespace oe {
namespace nxaccess_hw_algo {


/// Print helper for cpu2fpga_header, will be moved out of there later
std::ostream& operator<<(std::ostream& os, const enyx::oe::hwstrat::cpu2fpga_header& header)
{
    os << "(version, dest, msg_type, ack_req, ts, len) = ("
       << int(header.version) << ", "
       << int(header.dest) << ", "
       << int(header.msg_type) << ", "
       << int(header.ack_request) << ", "
       << uint32_t(header.timestamp) << ", "
       << uint16_t(header.length) << ")";
    return os;
}


/// Converts software message structure to internal storage structure
void
convert(InstrumentConfiguration::instrument_configuration_data_item & internal_data,
                    const user_dma_update_instrument_configuration& msg) {
    internal_data.enabled = msg.enabled;
    internal_data.tick_to_cancel_collection_id = msg.tick_to_cancel_collection_id;
    internal_data.tick_to_cancel_threshold = msg.tick_to_cancel_threshold;
    internal_data.tick_to_trade_ask_collection_id = msg.tick_to_trade_ask_collection_id;
    internal_data.tick_to_trade_ask_price = msg.tick_to_trade_ask_price;
    internal_data.tick_to_trade_bid_collection_id = msg.tick_to_trade_bid_collection_id;
    internal_data.tick_to_trade_bid_price = msg.tick_to_trade_bid_price;

}

/// Converts data words from User DMA to software message structure
void
InstrumentConfiguration::read_word(user_dma_update_instrument_configuration_ack& ret, const enyx::hfp::dma_user_channel_data_in& word, int word_index) {
   #pragma HLS function_instantiate variable=word_index
    switch(word_index) {
    case 1: {
        enyx::oe::hwstrat::read_word(ret.header, word.data(127,64));
        ret.tick_to_cancel_threshold = word.data(63,0);
        break;
    }
    case 2: {
        ret.tick_to_trade_bid_price = word.data(127,64);
        ret.tick_to_trade_ask_price = word.data(63,0);
        break;
    }
    case 3: {
        ret.instrument_id = word.data(127, 96);
        ret.tick_to_trade_bid_collection_id = word.data(95, 80);
        ret.tick_to_cancel_collection_id = word.data(79, 64);
        ret.tick_to_trade_ask_collection_id = word.data(63, 48);
        ret.enabled = word.data(47, 40);
        break;
    }
    default:
        assert(false && "Handling only 4 words for user_dma_update_instrument_configuration decoding");
    }
}


/// Converts data words from User DMA to software message structure
void
InstrumentConfiguration::read_word(user_dma_update_instrument_configuration& ret, const enyx::hfp::dma_user_channel_data_in& word, int word_index) {
   #pragma HLS function_instantiate variable=word_index
    switch(word_index) {
    case 1: {
        enyx::oe::hwstrat::read_word(ret.header, word.data(127,64));
        ret.tick_to_cancel_threshold = word.data(63,0);
        break;
    }
    case 2: {
        ret.tick_to_trade_bid_price = word.data(127,64);
        ret.tick_to_trade_ask_price = word.data(63,0);
        break;
    }
    case 3: {
        ret.instrument_id = word.data(127, 96);
        ret.tick_to_trade_bid_collection_id = word.data(95, 80);
        ret.tick_to_cancel_collection_id = word.data(79, 64);
        ret.tick_to_trade_ask_collection_id = word.data(63, 48);
        ret.enabled = word.data(47, 40);
        break;
    }
    default:
        assert(false && "Handling only 4 words for user_dma_update_instrument_configuration decoding");
    }
}


/// Converts configuration message ack to software message structure to data words
void
InstrumentConfiguration::write_word(const user_dma_update_instrument_configuration_ack& in, enyx::hfp::dma_user_channel_data_out& out_word, int word_index) {
   #pragma HLS function_instantiate variable=word_index
    switch(word_index) {
    case 1: {
        out_word.data(127, 64) =  enyx::oe::hwstrat::get_word(in.header); //64
        out_word.data(63,0) = in.tick_to_cancel_threshold; // 64
        out_word.last = 0;
        break;
    }
    case 2: {
        out_word.data(127,64)=  in.tick_to_trade_bid_price;  // 64
        out_word.data(63,0) = in.tick_to_trade_ask_price; // 64
        out_word.last = 0 ;
        break;
    }
    case 3: {
        out_word.data(127,96) = in.instrument_id; //32
        out_word.data(95, 80) = in.tick_to_trade_bid_collection_id; //16
        out_word.data(79, 64) = in.tick_to_cancel_collection_id; //16
        out_word.data(63, 48) = in.tick_to_trade_ask_collection_id; //16
        out_word.data(47, 40) = in.enabled; //8
        out_word.last = 1;
        break;
    }
    default:
        assert(false && "Handling only 3 words for user_dma_update_instrument_configuration decoding");
    }
}

/// Converts configuration message ack to software message structure to data words
void
InstrumentConfiguration::write_word(const user_dma_update_instrument_configuration& in, enyx::hfp::dma_user_channel_data_out& out_word, int word_index) {
   #pragma HLS function_instantiate variable=word_index
    switch(word_index) {
    case 1: {
        out_word.data(127, 64) =  enyx::oe::hwstrat::get_word(in.header); //64
        out_word.data(63,0) = in.tick_to_cancel_threshold; // 64
        out_word.last = 0;
        break;
    }
    case 2: {
        out_word.data(127,64)=  in.tick_to_trade_bid_price;  // 64
        out_word.data(63,0) = in.tick_to_trade_ask_price; // 64
        out_word.last = 0;
        break;
    }
    case 3: {
        out_word.data(127,96) = in.instrument_id; //32
        out_word.data(95, 80) = in.tick_to_trade_bid_collection_id; //16
        out_word.data(79, 64) = in.tick_to_cancel_collection_id; //16
        out_word.data(63, 48) = in.tick_to_trade_ask_collection_id; //16
        out_word.data(47, 40) = in.enabled; //8
        out_word.last = 1;
        break;
    }
    default:
        assert(false && "Handling only 3 words for user_dma_update_instrument_configuration decoding");
    }
}

std::ostream& operator<<(std::ostream& os, const user_dma_update_instrument_configuration& conf)
{
    os << "header: " << conf.header << "\n"
       << "t2c threshold: " << conf.tick_to_cancel_threshold << "\n"
       << "t2t bid price: " << conf.tick_to_trade_bid_price << "\n"
       << "t2t ask price: " << conf.tick_to_trade_ask_price << "\n"
       << "instrument Id: " << conf.instrument_id << "\n"
       << "t2t bid colId: " << conf.tick_to_trade_bid_collection_id << "\n"
       << "t2c col Id   : " << conf.tick_to_cancel_collection_id << "\n"
       << "t2t ask colId: " << conf.tick_to_trade_ask_collection_id << "\n"
       << "instrument Id: " << conf.enabled;
    return os;
}

void
InstrumentConfiguration::p_handle_instrument_configuration(hls::stream<enyx::hfp::dma_user_channel_data_in> & conf_in,
                                                          hls::stream<InstrumentConfiguration::read_instrument_data_request> (& req_in)[2],
                                                          hls::stream<instrument_configuration_data_item> (& req_out)[2],
                                                          hls::stream<user_dma_update_instrument_configuration_ack> & conf_out) {

#pragma HLS INLINE recursive
#pragma HLS PIPELINE enable_flush

    static enum  { IDLE,  /// doing nothing
                   IGNORE_PACKET, /// ignore incoming packet
                   READ_CONF_WORD2, /// will process word 2 of DMA input
                   READ_CONF_WORD3 /// will process word 3 of DMA input
                 } current_state; /// current state in FSM
    #pragma HLS RESET variable=current_state

    static user_dma_update_instrument_configuration current_dma_message_read; /// DMA message being parsed message.
    static instrument_configuration_data_item write_data ;
    static InstrumentConfiguration::instrument_configuration_data_item values[InstrumentConfiguration::instrument_count];
#pragma HLS RESOURCE variable=values core=XPM_MEMORY uram 

    switch(current_state) {

    case IDLE:
    {
            if(!conf_in.empty()) {
                
                   enyx::hfp::dma_user_channel_data_in _read = conf_in.read();
                   read_word(current_dma_message_read, _read, 1); // convert word 1 into struct
                   if((current_dma_message_read.header.dest == enyx::oe::nxaccess_hw_algo::InstrumentDataConfiguration)
                           && (current_dma_message_read.header.msg_type == InstrumentConfiguration::UpdateInstrumentData)
                           && (current_dma_message_read.header.version == 1))
                   {
                       std::cout << "[CONF] Incoming configuration message : accepted. ack_request="
                                 << int(current_dma_message_read.header.ack_request)  << "\n";

                       current_state = READ_CONF_WORD2; // now process second word of packet
                   } else {
                       std::cout << "[WARNING][CONF] Incoming configuration message : message unknown, ignoring ! \n" ;
                       current_state = IGNORE_PACKET;
                   }

            } else {
                // process read requests from read request bus
                for(int i = 0; i != 2; ++i) {
                    if(!req_in[i].empty()) {
                        req_out[i].write(values[req_in[i].read()]);
                    }
                }

            }
            break;
    }
    case READ_CONF_WORD2: {
        if(!conf_in.empty()) {
            std::cout << "[CONF] processing word 2 of configuration message \n";
            enyx::hfp::dma_user_channel_data_in _read = conf_in.read();
            read_word(current_dma_message_read, _read, 2); // convert word 2 into struct
            current_state = READ_CONF_WORD3;
        }
        for(int i = 0; i != 2; ++i) {
            if(!req_in[i].empty()) {
                req_out[i].write(values[req_in[i].read()]);
            }
        }
        break;
    }
    case READ_CONF_WORD3:{
        if(!conf_in.empty()) {

            std::cout << "[CONF] processing word 3 of configuration message \n";
            enyx::hfp::dma_user_channel_data_in _read = conf_in.read();
            read_word(current_dma_message_read, _read, 3); // convert word 4 into struct
            // convert it to instrument_configuration_data_item
            convert(write_data, current_dma_message_read);
            values[current_dma_message_read.instrument_id] = write_data;

            std::cout << "[CONF] Configuration for instrument "
                      << std::hex << std::showbase << current_dma_message_read.instrument_id
                      << std::dec<< " updated."
                      << " tick_to_cancel_threshold=" << std::hex << current_dma_message_read.tick_to_cancel_threshold
                      << " tick_to_cancel_collid=" <<    std::hex << current_dma_message_read.tick_to_cancel_collection_id
                      << "\n";

            user_dma_update_instrument_configuration_ack ack;
            //header
            ack.header.error = 0;
            ack.header.version = 1;
            ack.header.source = enyx::oe::nxaccess_hw_algo::InstrumentDataConfiguration;
            ack.header.msg_type = InstrumentConfiguration::UpdateInstrumentData; // only notifications
            ack.header.length = sizeof(user_dma_update_instrument_configuration_ack); //unused here, shall be needed for retrocompat'. We put here a dump value.
            //applicative layer 
            ack.instrument_id = current_dma_message_read.instrument_id;
            ack.enabled = current_dma_message_read.enabled;
            ack.tick_to_cancel_collection_id = current_dma_message_read.tick_to_cancel_collection_id;
            ack.tick_to_cancel_threshold = current_dma_message_read.tick_to_cancel_threshold;
            ack.tick_to_trade_ask_collection_id = current_dma_message_read.tick_to_trade_ask_collection_id;
            ack.tick_to_trade_ask_price = current_dma_message_read.tick_to_trade_ask_price;
            ack.tick_to_trade_bid_collection_id = current_dma_message_read.tick_to_trade_bid_collection_id;
            ack.tick_to_trade_bid_price = current_dma_message_read.tick_to_trade_bid_price;

            conf_out.write(ack);

            current_state = IDLE;
        }
        break;
    }
    case IGNORE_PACKET: { /// goal of this step is to process an unknown packet and
                          /// let it through without parsing it
        if(!conf_in.empty()) {

            std::cout << "[CONF] Ignoring word from DMA. \n";
            enyx::hfp::dma_user_channel_data_in _read = conf_in.read();
            if(_read.last == 1) { /// it's the end of the packet, let's go back in IDLE state to process requests.
                current_state = IDLE;
            }
        }
        for(int i = 0; i != 2; ++i) {
            if(!req_in[i].empty()) {
                req_out[i].write(values[req_in[i].read()]);
            }
        }
        break;
    }
    }

}

}
}
}
