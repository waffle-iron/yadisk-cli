#include <yadisk/client.hpp>
using namespace yadisk;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {

    po::options_description desc("Yandex Disk CLI");

    desc.add_options()
    ("help", "produce help message")
    ("token", "oAuth token");

    po::positional_options_description p;
        
    p.add("command", 1).add("resource", 2);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if(vm.count("command")) {
        auto command = vm["command"].as<std::string>();
        if (command == "help") {
            std::cout << desc << std::endl;   
        }
        else if (command == "version") {
            std::cout << YDCLI_VERSION_STRING << std::endl;
        }
        else {
            std::string token;
            if (vm.count("token")) {
                token = vm["token"].as<std::string>();
            } 
            else {
                token = std::getenv("YAD_TOKEN");
            }
    
            Client client{ token }; 

            if (command == "ls") {
                auto info = client.info();
                std::cout << info.dump(4) << std::endl;
            }
            else if (command =="status") {
                std::cout << "server is " << (client.ping() ? "available" : "not available") << std::endl;
            }
            else if (command == "info") {
                if (vm.count("resource")) {
                    auto info = client.info();
                    std::cout << info.dump(4) << std::endl;
                }
                else {
                    auto resource = vm["resource"].as<std::string>();
                    auto info = client.info(resource);
                    std::cout << info.dump(4) << std::endl;
                }
            }       
        }
    }
    else {
        std::cout << desc << std::endl; 
    }
}
