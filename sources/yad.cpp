#include <yadisk/client.hpp>
using namespace yadisk;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {

    po::options_description desc("Yandex Disk CLI");

    desc.add_options()
    ("help", "produce help message")
    ("token", po::value<std::string>(), "oAuth token")
    ("force,f", po::value<bool>(), "")
    ("command", po::value<std::string>(), "command")
    ("resource", po::value<std::string>(), "resource")
    ("location", po::value<std::string>(), "location");
    

    po::positional_options_description p;
        
    p.add("command", 1).add("resource", 1).add("location", 1);

    po::variables_map vm;
    auto parsed = po::command_line_parser(argc, argv).style(po::command_line_style::short_allow_adjacent).
              options(desc).positional(p).run();
    po::store(parsed, vm);
    po::notify(vm);

    bool force = vm.count("force");

    try {

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
                
                json answer;
                Client client{ token }; 

                if (command == "ls") {
                    url::path resource = url::path::separator;
                    answer = client.info(resource);
                }
                else if (command =="status") {
                    std::cout << "server is " << (client.ping() ? "available" : "not available") << std::endl;
                }
                else if (command == "info") {
                    if (vm.count("resource")) {
                        url::path resource = vm["resource"].as<std::string>();
                        answer = client.info(resource);
                    }
                    else {
                        answer = client.info();
                    }
                }       
                else if (command == "mkdir") {
                    if (vm.count("resource")) {
                        auto path = vm["resource"].as<std::string>();
                        answer = client.mkdir(path);
                    }
                    else throw std::invalid_argument("resource");
                }
                else if (command == "remove") {
                    if (vm.count("resource")) {
                        auto path = vm["resource"].as<std::string>();
                        answer = client.remove(path, force);
                    }
                    else throw std::invalid_argument("resource");
                }
                else if (command == "publish") {
                    if (vm.count("resource")) {
                        auto path = vm["resource"].as<std::string>();
                        answer = client.publish(path);
                    }
                    else throw std::invalid_argument("resource");
                }
                else if (command == "unpublish") {
                    if (vm.count("resource")) {
                        auto path = vm["resource"].as<std::string>();
                        answer = client.unpublish(path);
                    }
                    else throw std::invalid_argument("resource");
                }
                else if (command == "copy") {
                    if (vm.count("resource")) {
                        if (vm.count("location")) {
                            auto from = vm["resource"].as<std::string>();
                            auto to = vm["location"].as<std::string>();
                            client.copy(from, to, force);
                        }
                        else throw std::invalid_argument("location");
                    }
                    else throw std::invalid_argument("resource");
                }

                else if (command == "move") {
                    if (vm.count("resource")) {
                        if (vm.count("location")) {
                            auto from = vm["resource"].as<std::string>();
                            auto to = vm["location"].as<std::string>();
                            client.move(from, to, force);
                        }
                        else throw std::invalid_argument("location");
                    }
                    else throw std::invalid_argument("resource");
                }
                if (! answer.empty()) {
                    std::cout << answer.dump(4) << std::endl;
                }
            }
        }
        else {
            std::cout << desc << std::endl; 
        }
    }
    catch(std::exception& exc) {
        std::cout << desc << std::endl; 
    }
}
