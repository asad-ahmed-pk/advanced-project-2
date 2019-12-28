//
// ConfigParser.hpp
// Parses the config XML file and provides an interface to required attributes
//


#include "config/ConfigParser.hpp"
#include "config/config.hpp"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace Config
{
    // Instance setup
    ConfigParser& ConfigParser::GetInstance()
    {
        static ConfigParser instance;

        // copy sample config and create folder if not exists
        CheckAndCreateDefaultConfig();

        // read xml file into memory if not read yet
        if (instance.m_PropertyTree == nullptr) {
            instance.m_PropertyTree = std::unique_ptr<boost::property_tree::ptree>(new boost::property_tree::ptree);
            boost::property_tree::read_xml(CONFIG_FILE_PATH, *instance.m_PropertyTree);
        }

        return instance;
    }

    // Check if the config file does not exist and create it if it does not
    void ConfigParser::CheckAndCreateDefaultConfig()
    {
        // check dir
        if (!boost::filesystem::exists(CONFIG_DIR))
        {
            boost::filesystem::create_directories(CONFIG_DIR);

            // copy sample config into this dir if it does not exist
            if (!boost::filesystem::exists(CONFIG_FILE_PATH)) {
                boost::filesystem::copy_file(SAMPLE_CONFIG_PATH, CONFIG_FILE_PATH);
            }
        }
    }
}

