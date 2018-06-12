#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <locale>
#include <chrono>

using namespace std::chrono_literals;
typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
boost::asio::io_service service;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("192.168.0.75"), 2323);

void client_handler(socket_ptr);

int main()
{
  setlocale(LC_ALL, "Russian");
  std::vector<std::thread *> v(32000);
  try
  {
    for (auto& i : v)
    {
      std::this_thread::sleep_for(4500us);
      i = new std::thread(client_handler, socket_ptr(new boost::asio::ip::tcp::socket(service)));
    }
  }
  catch (boost::system::system_error er)
  {
    std::cout << er.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "Error unknown!" << std::endl;
  }

  for (auto& i : v)
    i->join();

  system("pause");
  return 0;
}

void client_handler(socket_ptr sock)
{
  try
  {
    sock->connect(ep);
    std::string str(1024, 0);
    while (sock->is_open())
    {
      sock->write_some(boost::asio::buffer("Hello!"));
      size_t len = sock->read_some(boost::asio::buffer(str));
      std::cout << "Server has send: " << std::string(str.begin(), str.begin() + len) << std::endl;
    }
  }
  catch (boost::system::system_error er)
  {
    std::cout << er.what() << std::endl;
  }
}