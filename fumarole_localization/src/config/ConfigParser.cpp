//
// ConfigParser.hpp
// Parses the config XML file and provides an interface to required attributes
//


#include "config/ConfigParser.hpp"
#include "config/config.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace Config
{
    // Instance setup
    ConfigParser& ConfigParser::GetInstance()
    {
        static ConfigParser instance;

        // read xml file into memory if not read yet
        if (instance.m_PropertyTree == nullptr) {
            instance.m_PropertyTree = std::unique_ptr<boost::property_tree::ptree>(new boost::property_tree::ptree);
            boost::property_tree::read_xml(CONFIG_FILE_PATH, *instance.m_PropertyTree);
        }

        return instance;
    }
}

