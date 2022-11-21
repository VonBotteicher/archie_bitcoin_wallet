#include <cstdio>
#include <bitcoin/explorer/utility.hpp>
#include <bitcoin/explorer/config/ec_private.hpp>
#include <bitcoin/system/math/checksum.hpp>
#include <bitcoin/system/math/hash.hpp>
#include <bitcoin/system/utility/endian.hpp>
#include <bitcoin/system/utility/data.hpp>
#include <bitcoin/system/wallet/payment_address.hpp>

using libbitcoin::ripemd160_hash;
using libbitcoin::sha256_hash;

// Prefix for mainnet = 0x00
static constexpr uint8_t kMainNetPrefix = 0x00;

// This is a magic constant from BIP32.
static constexpr char kBip32Magick[] = "Bitcoin seed";

int main(int argc, char* argv[]){
      
	if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <address>" << std::endl;
        return 1;
    }
    std::string str_to_match(argv[1]);
    std::cout << "string to match: " << str_to_match << std::endl;
    std::cout << "size: " << str_to_match.size() << std::endl;

    bool match_found = false;
    while(!match_found){

    // generate a new seed, the length of the seed is given by the first/only argument
	const std::vector<uint8_t> seed = libbitcoin::explorer::new_seed(192u);

    // create private key, a.k.a secret
    const std::array<uint8_t,32> secret = libbitcoin::explorer::new_key(seed);
    
    // Multiply the "generator point" on the elliptic curve by our secret.
    // The resultant point, also on the elliptic curve, is our public key.
    libbitcoin::ec_compressed point;
    libbitcoin::secret_to_public(point, secret);
    libbitcoin::wallet::ec_public ec_pub(point);

    // Convert public key to address
    libbitcoin::wallet::payment_address my_address(point);


    // Now repeat the process starting with the same seed, using more low level functions

    // BIP 32 defined magic salt
    static const auto magic = libbitcoin::to_chunk(kBip32Magick);

    // Use half of the HMAC SHA256 hash to generate private key from seed
    // Really more necessary for HD wallets, if wallet is JBOK could probably just use the first
    // 256 bits of the seed as the private key itself, or feed any sized seed into sha-256
    const auto intermediate = libbitcoin::split(libbitcoin::hmac_sha512_hash(seed, magic));
    const std::array<uint8_t,32> my_2nd_secret = intermediate.left;

    // Multiply the "generator point" on the elliptic curve by our secret.
    // The resultant point, also on the elliptic curve, is our public key.
    libbitcoin::ec_compressed my_2nd_point;
    libbitcoin::ec_uncompressed my_2nd_point_uncomp;
    libbitcoin::secret_to_public(my_2nd_point, my_2nd_secret);
    libbitcoin::secret_to_public(my_2nd_point_uncomp, my_2nd_secret);

    // Pubkeyhash: ripemd160(sha256(public key))
    std::array<uint8_t,20> public_key_hash = ripemd160_hash(sha256_hash(my_2nd_point));


    std::vector<uint8_t> my_2nd_address(25,0);
    std::fill(my_2nd_address.begin(),my_2nd_address.end(),0x00);
    my_2nd_address[0] = kMainNetPrefix;
    std::copy_n(public_key_hash.begin(),20 ,my_2nd_address.begin()+1);
    std::vector<uint8_t> my_2nd_address_prefix(21,0);
    std::copy_n(my_2nd_address.begin(),21 ,my_2nd_address_prefix.begin());


    // Hash address and prefix
    std::array<uint8_t,32> addr_hash = sha256_hash(sha256_hash(my_2nd_address));
    
    // Append the first 4 bytes of the address hash to the end of the address
    unsigned int addr_checksum;
    std:memcpy(&addr_checksum,&addr_hash[0],4);
    std::array<uint8_t,4> addr_checksum_le = libbitcoin::to_little_endian(addr_checksum);
    std::copy_n(addr_checksum_le.begin(), 4 ,my_2nd_address.begin()+21);


    std::array<uint8_t,25> ba_address = my_address.to_payment();
    std::cout << "seed: "<<libbitcoin::config::base16(seed) << std::endl;
    std::cout << "private key: " << libbitcoin::config::base16(secret) << std::endl;
    std::cout << "public key: " << ec_pub << std::endl;
    std::cout << "address        : " << libbitcoin::config::base58(libbitcoin::to_chunk(ba_address)) << std::endl;
    // std::cout << "address 0x     : " << libbitcoin::config::base16(ba_address) << std::endl;
    std::cout << "2nd address    : " << libbitcoin::config::base58(my_2nd_address) << std::endl;
    // std::cout << "2nd address 0x : " << libbitcoin::config::base16(my_2nd_address) << std::endl;
    // std::cout << "2nd address hash : " << libbitcoin::config::base16(addr_hash) << std::endl;
    
    std::stringstream my_address_str_str;
    std::string my_address_str;
    my_address_str_str << my_address;
    my_address_str_str >> my_address_str;

    match_found = (my_address_str.substr(0,str_to_match.size()) == str_to_match);

    std::cout << "strings equal: " << (match_found ? "yes" : "no") << std::endl;
    }
	return 0;
}	