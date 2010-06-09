#pragma once

#include "common.h"

#include "sha.h"
#include "hex.h"
#include "hmac.h"
#include "randpool.h"
#include "osrng.h"

static const int random_sequnce_length = 16 * 8;
static const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace crypto
			{			
				inline int _base36(char ch)
				{                           
					if (ch >= '0' && ch <= '9')
						return (ch - '0');
					else if (ch >= 'A' && ch <= 'I')
						return (ch - 'A' + 10);
					else if (ch >= 'J' && ch <= 'R')
						return (ch - 'J' + 19);
					else if (ch >= 'S' && ch <= 'Z')
						return (ch - 'S' + 28);
					else if (ch >= 'a' && ch <= 'i')
						return (ch - 'a' + 10);
					else if (ch >= 'j' && ch <= 'r')
						return (ch - 'j' + 19);
					else if (ch >= 's' && ch <= 'z')
						return (ch - 's' + 28);
					else
						return 0;
				}  
				
				inline string base36(int64_t integer)
				{
					if(integer < 0)
					{
						return "";
					}
					if(integer == 0)
					{
						return "0";
					}
					
					string ret ="";
					
					int m = 0;
					
					while(integer / 36 > 0)
					{
						m = integer % 36;
						integer /= 36;
						
						ret = alphabet[m] + ret;
					}
					
					if(integer > 0)
					{
						ret = alphabet[integer] + ret;
					}
					
					return ret;					
				}
				
				inline int64_t base36(const string& base36)
				{
					int64_t ret = 0;
					
					int i = base36.length();
					int exp = 1;
					
					foreach(char s, base36)
					{
						--i;						
						exp = 1;
						
						for(int j = 0; j < i; j++)
						{
							exp *= 36;
						}
						
						ret += exp * _base36(s);						
					}
					
					return ret;
				}
				
				inline string sha1(const string& msg)
				{
					string output = "";
					
					CryptoPP::SHA1 sha;
					CryptoPP::HexEncoder encoder;					
					
					byte digest[CryptoPP::SHA1::DIGESTSIZE];
					
					sha.CalculateDigest(digest, (const byte*)msg.c_str(), strlen(msg.c_str()));
					
					//encode message
					encoder.Attach(new CryptoPP::StringSink(output));
					encoder.Put(digest, sizeof(digest));
					encoder.MessageEnd();
					
					return output;
				}
				
				inline string hmac(const string& msg, const string& secret_key)
				{
					string output = "";
					
					byte hmac[CryptoPP::HMAC<CryptoPP::SHA1>::DIGESTSIZE];
					
					typedef CryptoPP::HMAC<CryptoPP::SHA1> HMAC;
					CryptoPP::HexEncoder encoder;
					
					
					HMAC((const byte*)secret_key.c_str(), strlen(secret_key.c_str()))
						.CalculateDigest(hmac, (const byte*)msg.c_str(), strlen(msg.c_str()));
					
					encoder.Attach(new CryptoPP::StringSink(output));
					encoder.Put(hmac, sizeof(hmac));
					encoder.MessageEnd();					
						
					return output;
				}
				
				inline int hmac_size()
				{
					return CryptoPP::HMAC<CryptoPP::SHA1>::DIGESTSIZE * 2;
				}
				
				inline bool hmac_verify(const string& mac, const string& msg, const string& secret_key)
				{
					string h_mac = hmac(msg, secret_key);
					
					return (h_mac == mac);					
				}
				
				template <int SIZE>
				inline string random()
				{
					string output = "";
					
					byte block[SIZE];
					
					CryptoPP::AutoSeededRandomPool random;
					CryptoPP::HexEncoder encoder;
					
					random.GenerateBlock(block, SIZE);					
					
					encoder.Attach(new CryptoPP::StringSink(output));
					encoder.Put(block, sizeof(block));
					encoder.MessageEnd();
					
					return output;
				}		
				
				inline string random()
				{
					return random<random_sequnce_length>();
				}
			}
		}
	}
}
