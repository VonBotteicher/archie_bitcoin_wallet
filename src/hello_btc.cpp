/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <memory>
#include <bitcoin/client.hpp>

using namespace bc;
using namespace bc::client;
using namespace bc::protocol;

static const char * kLocalHostStr = "tcp://127.0.0.1:9091";

/**
 * A minimal example that connects to a server and fetches height.
 */
int main(int argc, char* argv[])
{
    if (argc != 1) //TODO switch to 2 to take an address
    {
        std::cerr << "usage: " << argv[0] << " <address>" << std::endl;
        return 1;
    }

    // Set up the server connection.
    zmq::context context;
    zmq::socket socket(context, zmq::socket::role::dealer);

    if (socket.connect({kLocalHostStr}) != error::success)
    {
        std::cerr << "Cannot connect to " << kLocalHostStr << std::endl;
        return 1;
    }

    const auto unknown_handler = [](const std::string& command)
    {
        std::cout << "unknown command: " << command << std::endl;
    };

    const auto error_handler = [](const code& code)
    {
        std::cout << "error: " << code.message() << std::endl;
    };

    const auto completion_handler = [](size_t height)
    {
        std::cout << "height: " << height << std::endl;
    };

    const auto history_handler = [](const chain::history::list& list){
        for (size_t i =0; i < list.size(); i++){

    //             /// If there is no output this is null_hash:max.
    // output_point output;
    // uint64_t output_height;

    // /// The satoshi value of the output.
    // uint64_t value;

    // /// If there is no spend this is null_hash:max.
    // input_point spend;

    // union
    // {
    //     /// The height of the spend or max if no spend.
    //     uint64_t spend_height;

    //     /// During expansion this value temporarily doubles as a checksum.
    //     uint64_t temporary_checksum;
    // };
            std::cout << " Output Point null: "<< (list[i].output.is_null() ? "true" : "false");
            std::cout << " Block: "<< list[i].output_height;
            std::cout << " Satoshis: " << list[i].value << std::endl;
            // if(list[i].output.validation.spent){
            //     std::cout << "spent";
            // }if(list[i].output.validation.coinbase){
            //     std::cout << "minted";
            // }
            // if(list[i].output.validation.confirmed){
            //     std::cout << "confirmed";
            // }
            // std::cout << ((list[i].spend.is_null())? "not a spend" : "spend");
            // std::cout << " index = " << list[i].spend.index();
            // std::cout << " index = " << list[i].spend.index();
            // std::cout << " on block "<< list[i].output_height << std::endl;
        }
        std::cout << "YeeHaww!" <<  std::endl;
    };

    const auto points_value_handler = [](const chain::points_value& value){
        std::cout<< "value: " << value.value() << std::endl;
    };

    socket_stream stream(socket);

    // Wait 2 seconds for the connection, with no failure retries.
    proxy proxy(stream, unknown_handler, 2000, 0);

    // Make the request.
    // proxy.blockchain_fetch_last_height(error_handler, completion_handler);

    // proxy.blockchain_fetch_unspent_outputs()
    wallet::payment_address address("12cbQLTFMXRnSzktFkuoG3eHoMeFtpTu3S");

    proxy.blockchain_fetch_history3(error_handler,history_handler,address);
    // proxy.blockchain_fetch_unspent_outputs(error_handler,points_value_handler,address,0,wallet::select_outputs::algorithm::individual);

    zmq::poller poller;
    poller.add(socket);

    // Wait 1 second for the response.
    if (poller.wait(1000).contains(socket.id()))
        stream.read(proxy);

    return 0;
}
