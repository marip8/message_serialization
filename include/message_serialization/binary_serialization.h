#ifndef MESSAGE_SERIALIZATION_BINARY_SERIALIZATION_H
#define MESSAGE_SERIALIZATION_BINARY_SERIALIZATION_H

#include <fstream>
#include <ros/serialization.h>
#include <ros/console.h>

namespace message_serialization
{

template<typename T>
inline bool serializeToBinary(const std::string& file,
                              const T& message)
{
  uint32_t serial_size = ros::serialization::serializationLength(message);
  boost::shared_array<uint8_t> buffer(new uint8_t[serial_size]);
  ros::serialization::OStream stream(buffer.get(), serial_size);
  ros::serialization::serialize(stream, message);

  std::ofstream ofs(file, std::ios::out|std::ios::binary);
  if(ofs)
  {
    ofs.write((char*) buffer.get(), serial_size);
    return ofs.good();
  }

  return false;
}

template<typename T>
inline bool deserializeFromBinary(const std::string& file,
                                  T& message)
{
  std::ifstream ifs(file, std::ios::in|std::ios::binary);
  if(!ifs)
  {
    return false;
  }

  ifs.seekg(0, std::ios::end);
  std::streampos end = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  std::streampos begin = ifs.tellg();
  uint32_t file_size = end - begin;

  boost::shared_array<uint8_t> ibuffer(new uint8_t[file_size]);
  ifs.read((char*) ibuffer.get(), file_size);

  bool success = true;
  try
  {
    ros::serialization::IStream istream(ibuffer.get(), file_size);
    ros::serialization::deserialize(istream, message);
  }
  catch (const std::exception &ex)
  {
    ROS_ERROR_STREAM("Deserialization error: '" << ex.what() << "'");
    success = false;
  }

  ifs.close();
  return success;
}

} // namespace amsted_message_serialization

#endif // AMSTED_MESSAGE_SERIALIZATION_BINARY_SERIALIZATION_H
