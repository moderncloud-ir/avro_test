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
#include <avro/Reader.hh>
#include <avro/ResolverSchema.hh>
#include <avro/Schema.hh>
#include <avro/Specific.hh>
#include <avro/Writer.hh>

using namespace avro;

void function_read_doubles();
void function_read_string();
void function_read_dynamic_schema();

int main()
{
    //    std::ifstream ifs("cpx.json");
    //    avro::ValidSchema cpxSchema;

    //    avro::compileJsonSchema(ifs, cpxSchema);

    //    std::unique_ptr<avro::OutputStream> out = avro::memoryOutputStream();
    //    avro::EncoderPtr e = avro::validatingEncoder(cpxSchema, avro::binaryEncoder());
    //    e->init(*out);
    //    std::string str{"hello"};
    //    std::string strW{"world"};

    //    avro::encode(*e, str);
    //    avro::encode(*e, strW);

    //    std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(*out);
    //    avro::DecoderPtr d = avro::validatingDecoder(cpxSchema, avro::binaryDecoder());
    //    d->init(*in);

    //    std::string strDecoded;
    //    std::string strWDecoded;
    //    avro::decode(*d, strDecoded);
    //    avro::decode(*d, strWDecoded);
    //    std::cout << strDecoded << std::endl;
    //    std::cout << strWDecoded << std::endl;

    function_read_doubles();
    function_read_string();
    function_read_dynamic_schema();

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
