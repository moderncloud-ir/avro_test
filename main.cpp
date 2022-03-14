//#include "jwt/jwt.hpp"
//#include <algorithm>
//#include <array>
//#include <cstring>
//#include <functional>
//#include <iostream>
//#include <random>
//#include <string>

//template<typename T = std::mt19937>
//auto random_generator() -> T
//{
//	auto constexpr seed_bytes = sizeof(typename T::result_type) * T::state_size;
//	auto constexpr seed_len = seed_bytes / sizeof(std::seed_seq::result_type);
//	auto seed = std::array<std::seed_seq::result_type, seed_len>();
//	auto dev = std::random_device();
//	std::generate_n(begin(seed), seed_len, std::ref(dev));
//	auto seed_seq = std::seed_seq(begin(seed), end(seed));
//	return T{seed_seq};
//}
//auto generate_random_alphanumeric_string(std::size_t len) -> std::string
//{
//	static constexpr auto chars = "0123456789"
//								  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//								  "abcdefghijklmnopqrstuvwxyz";
//	thread_local auto rng = random_generator<>();
//	auto dist = std::uniform_int_distribution{{}, std::strlen(chars) - 1};
//	auto result = std::string(len, '\0');
//	std::generate_n(begin(result), len, [&]() { return chars[dist(rng)]; });
//	return result;
//}

//#include <random>
//void gen_random(char* s, const int len)
//{
//	static std::random_device rd;
//	static std::mt19937 mt(rd());
//	static std::uniform_int_distribution<int> dist(0, 25);
//	for (int i = 0; i < len; ++i)
//	{
//		s[i] = 'a' + dist(mt);
//	}
//	s[len] = 0;
//}

//class test
//{
//public:
//	test()
//	{}
//};

//int main()

//{
//	std::map<std::shared_ptr<test>, uint64_t> test_map;

//	std::shared_ptr<test> st(new test);

//	test_map[st] = 65;

//	auto stc = st;

//	test_map.erase(stc);

//	std::clog << test_map.size() << std::endl;
//	return 0;
//	char* s = new char[256];
//	gen_random(s, 256);

//	std::string key_str(s);
//	std::clog << key_str << std::endl;
//	using namespace jwt::params;

//	auto key = "secret"; //Secret to use for the algorithm
//	//Create JWT object
//	jwt::jwt_object obj{algorithm("HS256"), payload({{"some", "payload"}}), secret(key)};
//	obj.add_claim("iss", "empire.com")
//		.add_claim("sub", "oath")
//		.add_claim("id", "kingdom_id")
//		.add_claim("iat", 1513862371)
//		.add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{10});
//	//Get the encoded string/assertion
//	auto enc_str = obj.signature();
//	std::cout << enc_str << std::endl;

//	// Decode
//	auto dec_obj = jwt::decode(enc_str, algorithms({"HS256"}), secret(key));
//	std::cout << dec_obj.header() << std::endl;
//	std::cout << dec_obj.payload() << std::endl;

//	return 0;
//}

#include <complex>
#include <fstream>
#include <vector>

#include <avro/AvroTraits.hh>
#include <avro/Compiler.hh>
#include <avro/DataFile.hh>
#include <avro/Decoder.hh>
#include <avro/Encoder.hh>
#include <avro/Generic.hh>
#include <avro/GenericDatum.hh>
#include <avro/Parser.hh>
#include <avro/Reader.hh>
#include <avro/ResolverSchema.hh>
#include <avro/Schema.hh>
#include <avro/Specific.hh>
#include <avro/Writer.hh>

using namespace avro;

void function_read_doubles();
void function_read_string();
void function_read_array()
{
	std::ifstream ifs("cpx_array.json");
	avro::ValidSchema cpxSchema;

	avro::compileJsonSchema(ifs, cpxSchema);

	std::unique_ptr<avro::OutputStream> out = avro::memoryOutputStream();
	avro::EncoderPtr e = avro::validatingEncoder(cpxSchema, avro::binaryEncoder());
	e->init(*out);

	cpxSchema.toJson(std::clog);

	std::vector<std::string> string_vector{{"hello"}, {"world"}};
	avro::encode(*e, string_vector);

	std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(*out);
	avro::DecoderPtr d = avro::validatingDecoder(cpxSchema, avro::binaryDecoder());
	d->init(*in);

	std::vector<std::string> vd;

	avro::decode(*d, vd);
	std::clog << "array size " << vd.size() << std::endl;

	for (const auto &item : vd)
	{
		std::clog << item << std::endl;
	}
}
void function_read_dynamic_schema();

int main()
{

	function_read_doubles();
	function_read_string();
	function_read_dynamic_schema();
	function_read_array();

	return 0;
}

void function_read_dynamic_schema()
{

	// First construct our complex data type:
	avro::RecordSchema myRecord("complex");

	// Now populate my record with fields (each field is another schema):
	myRecord.addField("real", avro::DoubleSchema());
	myRecord.addField("imaginary", avro::DoubleSchema());
	myRecord.addField("data", avro::IntSchema());
	avro::ArraySchema complexArray(myRecord);

	// The complex record is the same as used above, let's make a schema
	// for an array of these record

	avro::ArraySchema allArray(complexArray);

	// The above code created our schema, but at this point it is possible that a schema is not valid
	// (a record may not have any fields, or some field names may not be unique, etc.) In order to use the schema, you need to convert it to the ValidSchema object:

	// this will throw if the schema is invalid!
	avro::ValidSchema validComplexArray(allArray);

	// now that I have my schema, what does it look like in JSON?
	// print it to the screen
	validComplexArray.toJson(std::cout);
}

// schema string

//{
//    "type": "record",
//             "name": "cpx",
//                      "fields" : [
//                          {"name": "re", "type": "string"},
//                          {"name": "im", "type" : "string"}
//                      ]
//}
void function_read_string()
{
	std::ifstream ifs("cpx_strings.json");
	avro::ValidSchema cpxSchema;

	avro::compileJsonSchema(ifs, cpxSchema);

	std::unique_ptr<avro::OutputStream> out = avro::memoryOutputStream();
	avro::EncoderPtr e = avro::validatingEncoder(cpxSchema, avro::binaryEncoder());
	e->init(*out);
	std::string str{"hello"};
	std::string strW{"world"};

	avro::encode(*e, str);
	avro::encode(*e, strW);

	std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(*out);
	avro::DecoderPtr d = avro::validatingDecoder(cpxSchema, avro::binaryDecoder());
	d->init(*in);

	std::string strDecoded;
	std::string strWDecoded;
	avro::decode(*d, strDecoded);
	avro::decode(*d, strWDecoded);

	std::cout << strDecoded << std::endl;
	std::cout << strWDecoded << std::endl;
}

//______________schema double
//{
//    "type" : "record",
//             "name" : "cpx",
//                      "fields"
//        : [{"name": "re", "type": "double"}, {"name": "im", "type": "double"}]
//}

void function_read_doubles()
{
	std::ifstream ifs("cpx_doubles.json");
	avro::ValidSchema cpxSchema;
	avro::compileJsonSchema(ifs, cpxSchema);

	std::unique_ptr<avro::OutputStream> out = avro::memoryOutputStream();
	avro::EncoderPtr e = avro::validatingEncoder(cpxSchema, avro::binaryEncoder());
	e->init(*out);
	std::complex<double> c1(1.0, 2.0);
	avro::encode(*e, std::real(c1));
	avro::encode(*e, std::imag(c1));

	std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(*out);
	avro::DecoderPtr d = avro::validatingDecoder(cpxSchema, avro::binaryDecoder());
	d->init(*in);

	std::complex<double> c2;

	double re, im;
	avro::decode(*d, re);
	avro::decode(*d, im);
	c2 = std::complex<double>(re, im);

	//    avro::decode(*d, c2);
	std::cout << '(' << std::real(c2) << ", " << std::imag(c2) << ')' << std::endl;
}
